#include "Font.h"
#include "OS.h"
#include "pioneer/text/TextSupport.h"

const U32 MAX_FAST_GLYPHS          = 256;
const U32 QUESTION_MARK_CHARCODE   = 63;

struct FontVertex {
	vec3     pos;
	Color4ub color;
	vec2     uv;
};

Font::Font(const std::string &filename, Video::Device *video)
: m_video(video)
{
	m_glyphsFast.resize(MAX_FAST_GLYPHS);

	RefCountedPtr<FileSystem::FileData> fontDef = FileSystem::gameDataFiles.ReadFile(filename);
	if (!fontDef.Valid())
		OS::Error("Missing font %s", filename.c_str());
	StringRange data = fontDef->AsStringRange();

	bool doingCharacters = false;

	while (!data.Empty()) {
		const StringRange line = data.ReadLine();
		if (line.Empty()) continue;

		if (!doingCharacters) {
			if (!starts_with(line.begin, "textures"))
				OS::Error("Malformed font definition");
			ParseTexture(line);
			data.ReadLine(); //skip font name
			doingCharacters = true;
			continue;
		} else {
			ParseGlyph(line);
		}
	}

	m_height = 8.f;
	for (auto it = m_glyphs.begin(); it != m_glyphs.end(); ++it)
		m_height = std::max(it->second.geomSize.y, m_height);

	//Since this buffer is filled every frame, it could be
	//shared with all fonts
	Video::VertexBufferDesc	vbdesc = {};
	vbdesc.attribs[0].semantic = ATTRIB_POSITION;
	vbdesc.attribs[0].format   = ATTRIB_FORMAT_FLOAT3;
	vbdesc.attribs[1].semantic = ATTRIB_COLOR;
	vbdesc.attribs[1].format   = ATTRIB_FORMAT_UBYTE4;
	vbdesc.attribs[2].semantic = ATTRIB_UV;
	vbdesc.attribs[2].format   = ATTRIB_FORMAT_FLOAT2;
	vbdesc.primitiveType = PRIMITIVE_TRIANGLES;
	vbdesc.usage         = BUFFER_DYNAMIC;
	vbdesc.numVertices   = 256;
	m_vtxBuffer.reset(video->CreateVertexBuffer(vbdesc));

	//init shader
	m_shader = video->CreateShader("font");
	m_cbuffer.reset(m_shader->CreateConstBuffer());
	c_ModelViewMatrix = m_shader->GetConstHandle("ModelViewMatrix");
	c_ProjMatrix      = m_shader->GetConstHandle("ProjMatrix");

	Video::RenderStateDesc rsdesc;
	rsdesc.depthWriteEnabled = false;
	rsdesc.blendMode = BLEND_ALPHA;
	m_renderState = video->CreateRenderState(rsdesc);

	m_video = video;
}

void Font::RenderString(const char *str, float x, float y, const Color &color)
{
	UpdateGeometry(str, x, y, color);

	m_video->SetRenderState(m_renderState);
	m_video->SetTexture(0, m_texture.Get());
	m_video->SetShader(m_shader);
	m_video->SetVertexBuffer(m_vtxBuffer.get());

	//set constants
	mat4 modelView(1.f);
	m_cbuffer->Set(c_ModelViewMatrix, modelView);
	m_cbuffer->Set(c_ProjMatrix, m_video->GetProjectionMatrix());
	m_shader->ApplyConstants(m_cbuffer.get());

	m_video->Draw(0, m_vtxCount);
}

const Font::Glyph &Font::GetGlyph(U32 ch) const
{
	if (ch < MAX_FAST_GLYPHS)
		return m_glyphsFast[ch];
	auto it = m_glyphs.find(ch);
	if (it != m_glyphs.end())
		return it->second;
	return m_glyphsFast[QUESTION_MARK_CHARCODE];
}

void Font::ParseTexture(const StringRange &line)
{
	const char *space = line.FindSpace();
	if (space == line.end) OS::Error("Malformed font definition");

	m_textureName = StringRange(space + 1, line.end).StripSpace().ToString();
	m_texture.Reset(m_video->CreateTexture(FileSystem::JoinPath("fonts", m_textureName)));
}

void Font::ParseGlyph(const StringRange &line)
{
	const vec2 texSize(m_texture->GetDesc().width, m_texture->GetDesc().height);

	std::stringstream ss(line.ToString());
	Glyph g;
	U32 charcode;
	ss >> charcode;
	ss >> g.uvOffs.x;
	ss >> g.uvOffs.y;
	ss >> g.geomSize.x;
	ss >> g.geomSize.y;
	ss >> g.geomOffset.x;
	ss >> g.geomOffset.y;

	//scale to [0.0-1.0]
	g.uvOffs /= texSize;
	g.uvSize = g.geomSize / texSize;

	m_glyphs[charcode] = g;
	if (charcode < MAX_FAST_GLYPHS)
		m_glyphsFast[charcode] = g;
}

void Font::UpdateGeometry(const char *str, float x, float y, const Color &color)
{
	FontVertex *vts = m_vtxBuffer->Map<FontVertex>();
	const Video::VertexBufferDesc &vbdesc = m_vtxBuffer->GetDesc();
	const U32 vtxSize    = vbdesc.VertexSize();
	SDL_assert(sizeof(FontVertex) == vtxSize);
	const U32 posOffset  = vbdesc.Offset(ATTRIB_POSITION);
	const U32 uvOffset   = vbdesc.Offset(ATTRIB_UV);
	const U32 colOffset  = vbdesc.Offset(ATTRIB_COLOR);
	U32 numVertices = 0;
	const Color4ub ubCol(color);
	float px = x;
	float py = y;
	U32 i = 0;
	while (str[i]) {
		if (str[i] == '\n') {
			px = x;
			py += GetHeight();
			i++;
		} else {
			U32 chr;
			int n = Text::utf8_decode_char(&chr, &str[i]);
			SDL_assert(n);
			i += n;

			const Glyph &glyph = GetGlyph(chr);
			//AddGlyphGeometry
			{
				const float offx = px + glyph.geomOffset.x;
				const float offy = py + glyph.geomOffset.y;
				const float offU = glyph.uvOffs.x;
				const float offV = glyph.uvOffs.y;

				const vec3 p0(offx,                    offy,                  0.f);
				const vec3 p1(offx,                    offy+glyph.geomSize.y, 0.f);
				const vec3 p2(offx + glyph.geomSize.x, offy,                  0.f);
				const vec3 p3(offx+glyph.geomSize.x,   offy+glyph.geomSize.y, 0.f);

				const vec2 t0(offU,                offV);
				const vec2 t1(offU,                offV+glyph.uvSize.y);
				const vec2 t2(offU+glyph.uvSize.x, offV);
				const vec2 t3(offU+glyph.uvSize.x, offV+glyph.uvSize.y);

#define ADDVTX(_pos, _col, _uv) \
				vts->pos     = _pos; \
				vts->color   = _col; \
				vts->uv      = _uv;  \
				vts++;

				ADDVTX(p0, ubCol, t0)
				ADDVTX(p1, ubCol, t1)
				ADDVTX(p2, ubCol, t2)

				ADDVTX(p2, ubCol, t2)
				ADDVTX(p1, ubCol, t1)
				ADDVTX(p3, ubCol, t3)

				numVertices += 6;
#undef ADDVTX
			}
			//add kerning
			px += glyph.geomSize.x + glyph.geomOffset.x;
		}
	}
	m_vtxBuffer->Unmap();

	m_vtxCount = numVertices;
}
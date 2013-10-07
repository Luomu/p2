#pragma once
/*
 * Texture atlas font
 */
#include "Common.h"
#include "Texture.h"
#include "VideoDevice.h"

class Font {
public:
	struct Glyph {
		vec2 uvOffs;
		vec2 uvSize;
		vec2 geomSize;
		vec2 geomOffset;

		Glyph()
		: uvOffs(0.f) , uvSize(0.f)
		, geomSize(0.f), geomOffset(0.f)
		{}
	};

	Font(const std::string &fileName, Video::Device*);
	void RenderString(const char*, float x, float y, const Color &color = Color::WHITE);
	const Glyph &GetGlyph(U32 ch) const;
	float GetHeight() const { return m_height; }

private:
	void ParseTexture(const StringRange&);
	void ParseGlyph(const StringRange&);
	void UpdateGeometry(const char*, float x, float y, const Color&);

	Video::TexturePtr m_texture;

	Video::Device *m_video;

	std::vector<Glyph>   m_glyphsFast;
	std::map<U32, Glyph> m_glyphs;
	std::string          m_textureName;
	float                m_height;
	U32                  m_vtxCount;

	std::unique_ptr<Video::VertexBuffer> m_vtxBuffer;
	Video::RenderState *m_renderState;

	//shouldn't be unique
	Video::Shader* m_shader;
	std::unique_ptr<Video::ShaderConstBuffer> m_cbuffer;
	Video::ShaderConstHandle *c_ModelViewMatrix;
	Video::ShaderConstHandle *c_ProjMatrix;
};

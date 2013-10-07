#include "engine/TextureManager.h"
#include "engine/VideoDevice.h"

using namespace Video;

static TextureManager *s_instance = nullptr;

TextureManager::TextureManager(Device *d)
: m_device(d)
{
}

TexturePtr TextureManager::GetTexture(const std::string &name)
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
		return it->second;

	TexturePtr newtex(m_device->CreateTexture(name));
	m_textures[name] = newtex;
	return newtex;
}

void TextureManager::Init(Device *d)
{
	SDL_assert(s_instance == nullptr);
	s_instance = new TextureManager(d);
}

void TextureManager::Destroy()
{
	delete s_instance;
	s_instance = nullptr;
}

TextureManager *TextureManager::Instance()
{
	SDL_assert(s_instance);
	return s_instance;
}
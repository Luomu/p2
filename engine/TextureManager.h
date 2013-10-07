#pragma once
#include "Common.h"
#include "Texture.h"

namespace Video {
	class Device;
	class TextureManager {
	public:
		TextureManager(Device*);

		/// get cached texture, or load and store in cache
		TexturePtr GetTexture(const std::string&);

		static void Init(Device*);
		static void Destroy();
		static TextureManager *Instance();

	private:
		Device *m_device;
		std::unordered_map<std::string, TexturePtr> m_textures;
	};
}
#pragma once
#include "engine/Shader.h"

namespace Video {
	class GLProgram;

	class GLConstHandle : public ShaderConstHandle {
	public:
		friend class Program;

		GLConstHandle(const std::string&);

		void  Reset(SHADER_CONST_TYPE type, GLint loc, U32 offset);

		GLint GetLocation() const { return m_location; }

	private:
		GLint m_location;
	};

	class GLConstBuffer : public ShaderConstBuffer {
	public:
		GLConstBuffer(size_t);

	private:
		Program *m_program;
	};

	class GLProgram : public Shader {
	public:
		struct ShaderException { };

		GLProgram();
		/// create program from filename.glsl
		GLProgram(const std::string &fileName, const std::vector<ShaderMacro> *macros);
		~GLProgram();

		/// reload shader file from disk
		virtual void Reload();

		/// bind and unbind program
		void Use();
		void Unuse();

		virtual ShaderConstBuffer *CreateConstBuffer();
		virtual ShaderConstHandle *GetConstHandle(const std::string &name);
		virtual void               ApplyConstants(ShaderConstBuffer*);

		GLuint  GetProgram() const { return m_program; }

	private:
		/// load, compile and link
		void LoadShaders(const std::string &name, const std::string &defines);
		void InitUniforms();
		/// load or reload shader, if not, try loading fallback shader
		void TryLoad();

		GLuint m_program;
		typedef std::unordered_map<std::string, GLConstHandle*> HandleMap;
		HandleMap m_handles;
		U32 m_constBufferSize;
	};
}
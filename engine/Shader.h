#pragma once
#include "Common.h"
#include "VideoResource.h"

namespace Video {
	struct ShaderMacro {
		ShaderMacro(const std::string &name_, const std::string &value);
		std::string name;
		std::string value;
	};

	/*
	 * Handle to one uniform
	 * A handle is valid if it is actually used by a shader.
	 * Modifying & reloading a shader will update validity
	 * of existing handles (and may also change constant types).
	 */
	class ShaderConstHandle {
	public:
		ShaderConstHandle(const std::string&);

		const std::string &GetName()   const { return m_name;   }
		SHADER_CONST_TYPE  GetType()   const { return m_type;   }
		U32                GetOffset() const { return m_offset; }
		bool               IsValid()   const { return m_valid;  }
		void               SetValid(bool b)  { m_valid = b;     }

	protected:
		std::string       m_name;
		SHADER_CONST_TYPE m_type;
		U32               m_offset;
		bool              m_valid;
	};

	/*
	 * Uniform buffer for shaders
	 */
	class ShaderConstBuffer {
	public:
		ShaderConstBuffer(size_t);
		virtual ~ShaderConstBuffer();

		U8 *GetData() const { return m_data; }

		/*
		 * Setters for each supported constant type.
		 * Using an invalid handle does nothing.
		 */
		virtual void Set(ShaderConstHandle*, vec2);
		virtual void Set(ShaderConstHandle*, vec3);
		virtual void Set(ShaderConstHandle*, vec4);
		virtual void Set(ShaderConstHandle*, const mat4&);

	protected:
		/// Simple memcopying of the value to the buffer
		template<typename T>
		void CopyData(ShaderConstHandle *handle, const T &value);

		U8 *m_data;
	};

	class Shader : public VideoResource {
	public:
		virtual ~Shader();

		virtual ShaderConstBuffer *CreateConstBuffer()                     = 0;
		virtual ShaderConstHandle *GetConstHandle(const std::string &name) = 0;
		virtual void               ApplyConstants(ShaderConstBuffer*)      = 0;

	protected:
		std::vector<ShaderMacro> m_macros;
	};
}
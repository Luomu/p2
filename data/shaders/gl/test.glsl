#ifdef VERTEX_SHADER
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_uv0;

out vec2 v_uv0;

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;

void main(void) {
	gl_Position = ModelViewMatrix * a_position;
	gl_Position = ProjMatrix * gl_Position;
	v_uv0 = a_uv0;
}
#else
uniform sampler2D Texture0;

layout (location = 0) out vec4 color;
layout (location = 1) in  vec2 v_uv0;

void main(void) {
	color = texture2D(Texture0, v_uv0);
}
#endif

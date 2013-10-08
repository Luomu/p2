#ifdef VERTEX_SHADER
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_uv0;

out vec2 v_uv0;
out vec4 v_color;

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;

void main(void) {
	gl_Position = ModelViewMatrix * a_position;
	gl_Position = ProjMatrix * gl_Position;
	v_uv0   = a_uv0;
	v_color = a_color;
}
#else
uniform sampler2D Texture0;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv0;
in vec4 v_color;

void main(void) {
	fragColor = texture2D(Texture0, v_uv0) * v_color;
}
#endif

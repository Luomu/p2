uniform sampler2D Texture0;
uniform mat4 UvMatrix0;
uniform vec2 LensCenter;
uniform vec2 ScreenCenter;
uniform vec2 Scale;
uniform vec2 ScaleIn;
uniform vec4 HmdWarpParam;

#ifdef VERTEX_SHADER
layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_uv0;

out vec2 v_uv0;

void main(void) {
	gl_Position = a_position;
	v_uv0       = vec2(UvMatrix0 * vec4(a_uv0,0,1));
	v_uv0.x = v_uv0.x;
	v_uv0.y = 1.0 - v_uv0.y;
}
#else
layout (location = 0) out vec4 color;
layout (location = 1) in  vec2 v_uv0;

vec2 HmdWarp(in vec2 in01) {
	vec2 theta   = (in01 - LensCenter) * ScaleIn; //scales to [-1, 1]
	float rSq    = theta.x * theta.x + theta.y * theta.y;
	vec2 rvector = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq +
		HmdWarpParam.z * rSq * rSq +
		HmdWarpParam.w * rSq * rSq * rSq);
	return LensCenter + Scale * rvector;
}

vec4 frag() {
	vec2 tc = HmdWarp(v_uv0);
	if (!all(equal(clamp(tc, ScreenCenter-vec2(0.25,0.5), ScreenCenter+vec2(0.25,0.5)), tc)))
		return vec4(0.0);

	return texture2D(Texture0, tc);
}

void main(void) {
	color = frag();
}
#endif

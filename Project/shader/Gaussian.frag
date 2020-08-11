//=============================================================================
//!
//!	@file	Gaussian.frag
//!	@brief	ブラー
//!
//!	@author	YKCH
//!
//=============================================================================

#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

struct Input
{
	vec2	_uv;	//!< UV
};

struct Output
{
	vec3 _color;
};

layout(location = 10) uniform sampler2D gTexture;

layout(location = 0) in		 Input	input;
layout(location = 0) out	 Output	output;

layout(location = 11) uniform float width;		//!< 画面横サイズ
layout(location = 12) uniform float height;		//!< 画面縦サイズ


void main()
{
	vec2 invTextureSize = vec2(width,height);
	vec4 color;
	vec2 uv = input._uv;

	color  = texture2D(gTexture, uv + vec2(-0.5, -0.5)*invTextureSize) * (1.0/4.0);
	color += texture2D(gTexture, uv + vec2(+0.5, -0.5)*invTextureSize) * (1.0/4.0);
	color += texture2D(gTexture, uv + vec2(-0.5, +0.5)*invTextureSize) * (1.0/4.0);
	color += texture2D(gTexture, uv + vec2(+0.5, +0.5)*invTextureSize) * (1.0/4.0);

    output._color = color.rgb;
}




//=============================================================================
//!
//!	@file	GBuffer.frag
//!	@brief	Gバッファ作成
//!
//!	@author	YKCH
//!
//=============================================================================
#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

struct Input
{
	vec3	_worldPosition;
	vec3	_worldNormal;
	vec4	_color;
	vec2	_uv;
};

//	バッファ
struct Output
{
	vec4 _colorPosition;
	vec4 _colorNormal;
	vec4 _colorAlbedo;
	vec4 _colorEmissive;
};

layout(location = 11) uniform vec4 matreialColor;	//!< 材質色

layout(location = 12) uniform float	roughness;				//!< ラフネス
layout(location = 13) uniform float	metalness;				//!< メタルネス
layout(location = 14) uniform vec3	emissiveColor;			//!< 自己発光色
layout(location = 15) uniform sampler2D materialTexture;	//!< テクスチャ

//! オブジェクトサブルーチン
subroutine void ObjectModeType();				
subroutine uniform ObjectModeType ObjectMode;


layout(location = 0) in  Input	input;
layout(location = 0) out Output output;

int FALSE = 0;
int TRUE  = 1;

//!	マテリアル
subroutine( ObjectModeType )
void Material()
{
	vec3 position = input._worldPosition;
	vec3 normal   = input._worldNormal;
	vec4 albedo   = input._color;
	vec2 uv		  = input._uv;

	normal = normalize(normal) * 0.5 + 0.5;

	albedo = matreialColor;

	output._colorPosition = vec4(position, roughness);
	output._colorNormal   = vec4(normal  , metalness);
	output._colorAlbedo   = vec4(albedo);
	output._colorEmissive = vec4(emissiveColor,1.0f);
}

//!	頂点色
subroutine( ObjectModeType )
void Vertex()
{
	vec3 position = input._worldPosition;
	vec3 normal   = input._worldNormal;
	vec4 albedo   = input._color;
	vec2 uv		  = input._uv;

	normal = normalize(normal) * 0.5 + 0.5;

	output._colorPosition = vec4(position, roughness);
	output._colorNormal   = vec4(normal  , metalness);
	output._colorAlbedo   = vec4(albedo);
	output._colorEmissive = vec4(emissiveColor,1.0f);
}

//!	テクスチャ色
subroutine( ObjectModeType )
void Texture()
{
	vec3 position = input._worldPosition;
	vec3 normal   = input._worldNormal;
	vec4 albedo   = input._color;
	vec2 uv		  = input._uv;

	normal = normalize(normal) * 0.5 + 0.5;

	albedo *= texture2D(materialTexture,uv);

	output._colorPosition = vec4(position, roughness);
	output._colorNormal   = vec4(normal  , metalness);
	output._colorAlbedo   = vec4(albedo);
	output._colorEmissive = vec4(emissiveColor,1.0f);
}

void main()
{
	ObjectMode();
}



//=============================================================================
//!
//!	@file	GBuffer.vert
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
	vec3	_position;
	vec3	_normal;
	vec4	_color;
	vec2	_uv;
};

struct Output
{
	vec3	_worldPosition;
	vec3	_worldNormal;
	vec4	_color;
	vec2	_uv;
};

layout(location = 0) uniform mat4	matProj;	//!< プロジェクション行列
layout(location = 1) uniform mat4	matView;	//!< ビュー行列
layout(location = 2) uniform mat4	matWorld;	//!< モデルワールド行列


layout(location = 0) in  Input		input;
layout(location = 0) out Output		output;


int FALSE = 0;
int TRUE  = 1;

void main()
{
	vec4 worldPosition = vec4(input._position,1.0);
	vec3 worldNormal   = input._normal;
	vec4 color         = input._color;
	vec2 uv            = input._uv;

	worldPosition = matWorld * worldPosition;
	worldNormal = mat3x3(matWorld) * worldNormal;


	output._worldPosition = worldPosition.xyz;
	output._worldNormal	  = worldNormal;
	output._color         = color;
	output._uv            = uv;
	gl_Position = matProj * matView * worldPosition;
}





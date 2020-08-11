//=============================================================================
//!
//!	@file	Lighting.frag
//!	@brief	ライティング
//!
//!	@author	YKCH
//!
//=============================================================================
#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

struct Input
{
	vec2 _uv;
};

struct Output
{
	vec3 _diffuse;
	vec3 _specular;
};

layout(location = 0) in  Input  input;
layout(location = 0) out Output output;


//	Gバッファ

layout(location = 10) uniform sampler2D	texPosition;	//!< 座標XYZ  , roughness	
layout(location = 11) uniform sampler2D	texNormal;		//!< 法線XYZ  , metalness
layout(location = 12) uniform sampler2D	texAlbedo;		//!< 反射率RGB, lightingFlag

layout(location = 13) uniform vec3		lightPosition;	//!< 座標
layout(location = 14) uniform vec3		lightColor;		//!< 色
layout(location = 15) uniform float		lightPower;		//!< パワー
layout(location = 16) uniform int		lightMode;		//!< モード
layout(location = 17) uniform vec3		eyePosition;	//!< カメラ座標

subroutine void LightingModeType();
subroutine uniform LightingModeType Lighting;

const int FALSE = 0;
const int TRUE  = 1;


//	pow近似式
float	fastPow(float x, float  y) { return exp( log(x) * y ); }
vec2	fastPow(vec2  x, vec2   y) { return exp( log(x) * y ); }
vec3	fastPow(vec3  x, vec3   y) { return exp( log(x) * y ); }
vec4	fastPow(vec4  x, vec4   y) { return exp( log(x) * y ); }

vec2	fastPow(vec2  x, float  y) { return exp( log(x) * y ); }
vec3	fastPow(vec3  x, float  y) { return exp( log(x) * y ); }
vec4	fastPow(vec4  x, float  y) { return exp( log(x) * y ); }

#define pow	fastPow

const float PI = 3.14159265;

float saturate(float param)
{
	return clamp(param, 0.0, 1.0);
}
vec2 saturate(vec2 param)
{
	return clamp(param, vec2(0.0, 0.0), vec2(1.0, 1.0));
}
vec3 saturate(vec3 param)
{
	return clamp(param, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
}
//-----------------------------------------------------------------------------
//	フレネル反射
//!	@param	[in]	NdotV	内積結果 (cosΘ)
//!	@param	[in]	f0		基本反射率 (フレネルバイアス)
//-----------------------------------------------------------------------------
float	fresnel(float NdotV, float f0)
{
	// return f0 + (1.0 - f0) * pow(1.0 - NdotV, 5.0);
	// ↓[高速化近似] 	
	return f0 + (1.0 - f0) * exp(-8.35 * NdotV);
}

vec3	RGBtoYUV(vec3 rgb)
{
	vec3	result;
	result.x =  0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;	// Y
	result.y = -0.169 * rgb.r - 0.331 * rgb.g + 0.500 * rgb.b;	// U
	result.z =  0.500 * rgb.r - 0.419 * rgb.g - 0.081 * rgb.b;	// V

	return result;
}

vec3	YUVtoRGB(vec3 yuv)
{
	vec3	result;

	result.r = 1.000 * yuv.x                 + 1.402 * yuv.z;	// R
	result.g = 1.000 * yuv.x - 0.344 * yuv.y - 0.714 * yuv.z;	// G
	result.b = 1.000 * yuv.x + 1.772 * yuv.y                ;	// B

	return result;
}

float sqr(float x) { return x*x; }


//-----------------------------------------------------------------------------
//! 0.0～1.0の乱数を取得
//!	@param	[in]	seed	ピクセル単位で変化する任意の値 (uv値などを推奨)
//!	@return	乱数値
//-----------------------------------------------------------------------------
float rand(vec2 seed)
{
	return fract( sin( dot( seed.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

//-----------------------------------------------------------------------------
//!	-1.0～+1.0の乱数を取得
//!	@param	[in]	seed	ピクセル単位で変化する任意の値 (uv値などを推奨)
//!	@return	乱数値
//-----------------------------------------------------------------------------
float signed_rand(vec2 seed)
{
	return rand(seed) * 2.0 - 1.0;
}

//=============================================================================
//	ディフューズ
//=============================================================================
//-----------------------------------------------------------------------------
//!	正規化ランバート
//-----------------------------------------------------------------------------
float DiffuseLambert(vec3 N, vec3 L)
{
	const float p = 1.0 / PI;
	return max(0, dot(L, N))*p;
}
//-----------------------------------------------------------------------------
//!	オーレン・ネイヤー
//-----------------------------------------------------------------------------
float DiffuseOrenNayar(float roughness,vec3 N,vec3 V,vec3 L)
{
	// calculate intermediary values
	float NdotL = dot(N, L);
	float NdotV = dot(N, V);

	float angleVN = acos(NdotV);
	float angleLN = acos(NdotL);

	float alpha = max(angleVN, angleLN);
	float beta = min(angleVN, angleLN);
	float gamma = dot(V - N * NdotV, L - N * NdotL);

	float roughnessSquared = roughness * roughness;

	// calculate A and B
	float A = 1.0 - 0.5 * (roughnessSquared / (roughnessSquared + 0.57));

	float B = 0.45 * (roughnessSquared / (roughnessSquared + 0.09));

	float C = sin(alpha) * tan(beta);

	// put it all together
	float L1 = max(0.0, NdotL) * (A + B * max(0.0, gamma) * C);

	// get the final color 
	return  L1;
}


//=============================================================================
//	スペキュラー
//=============================================================================
//-----------------------------------------------------------------------------
//	BlinnPhong
//-----------------------------------------------------------------------------
float SpecularBlinnPhong(float roughness, float NdotH)
{
	float	specularPower = 2 / saturate(roughness * roughness + 0.005) - 2;
	float	factor = (specularPower + 2.0) * (1.0 / (2.0 * PI));
	return pow(max(0, NdotH), specularPower) * factor;
}
//-----------------------------------------------------------------------------
//	GGX(Generalized - Trowbridge - Reitz)
//-----------------------------------------------------------------------------
float SpecularGGX(float roughness, float NdotH)
{
	float CosSquared = NdotH*NdotH;
	float TanSquared = (1 - CosSquared) / CosSquared;
	return (1.0 / PI) * sqr(roughness / (CosSquared * (roughness*roughness + TanSquared)));
}

float G1(vec3 N, vec3 V, float roughness)
{
	float NdotV = dot(N, V);
	float k = roughness * sqrt(2.0 / PI);
	return NdotV / (NdotV * (1 - k) + k);
}
float specularOcclusion(float NdotV, float ao)
{
	float c = NdotV + ao;
	return saturate(c * c - 1.0 + ao);
}

//-----------------------------------------------------------------------------
//!	CookTorrance反射
//-----------------------------------------------------------------------------
float SpecularCookTorrance(float roughness, float metalness, vec3 N, vec3 L, vec3 V)
{
	vec3 H = normalize(V + L);
	float VdotH = dot(V, H);
	float NdotH = dot(N, H);
	float NdotL = dot(N, L);
	float NdotV = saturate(dot(N, V));
	float F		= fresnel(NdotV, saturate(metalness));	//	フレネル項
	float D		= SpecularGGX(roughness + 0.004, dot(N, H));	//	反射光

#if 0
	//	一般的なジオメトリ項
	//	CookTorrance
	float G1 = NdotH * NdotV / VdotH;
	float G2	= 2.0 * NdotH * NdotL / VdotH;
	float G		= saturate(min(G1, G2));
#else
	//	Smithの方法
	float G		= G1(L, H, roughness)*G1(V, H, roughness);
#endif
//	return (F * (1.0 / 4.0)) * (D * G) /  NdotV;
	return (F * D * G) / (4.0 * NdotV);
}


	#define DIFFUSE_LIGHT DiffuseLambert(N, L)					//	Lambert
//	#define DIFFUSE_LIGHT DiffuseOrenNayar(roughness, N, V, L)	//	OrenNayer
//	#define DIFFUSE_LIGHT 0	//	なし

//	#define SPECULAR_LIGHT SpecularBlinnPhong(roughness, dot(N, H))		//	BlinnPhong
	#define SPECULAR_LIGHT SpecularGGX(roughness+0.004, dot(N, H))		//	GGX
//	#define SPECULAR_LIGHT SpecularCookTorrance(roughness, metalness, N, L, V) * specularOcclusion(dot(N, V), ao)	//	CookTorrance
//	#define SPECULAR_LIGHT 0	//	なし

//=============================================================================
//	光源種類
//=============================================================================
//-----------------------------------------------------------------------------
//! 環境光
//-----------------------------------------------------------------------------
subroutine( LightingModeType )
void AmbientLight()
{
	vec2 uv = input._uv;

	vec3  worldPosition	= texture2D(texPosition,uv).xyz;
	vec3  worldNormal	= texture2D(texNormal,uv).xyz;
	vec3  albedo		= texture2D(texAlbedo,uv).rgb;
	float roughness		= texture2D(texPosition,uv).w;
	float metalness		= texture2D(texNormal,uv).w;

	output._diffuse  = albedo * lightColor * lightPower;
	output._specular = vec3(0.0,0.0,0.0);
}

//-----------------------------------------------------------------------------
//!	平行光源
//-----------------------------------------------------------------------------
subroutine( LightingModeType )
void DirectionalLight()
{
	vec2 uv = input._uv;

	vec3  worldPosition	= texture2D(texPosition,uv).xyz;
	vec3  worldNormal	= texture2D(texNormal,uv).xyz;
	vec3  albedo		= texture2D(texAlbedo,uv).rgb;
	float roughness		= texture2D(texPosition,uv).w;
	float metalness		= texture2D(texNormal,uv).w;

	vec3 lightDir = lightPosition;
	
	vec3 N = normalize(worldNormal * 2.0 - 1.0);		//	法線
	vec3 V = normalize(eyePosition - worldPosition);	//	座標
	vec3 L = normalize(lightDir);						//	ライト
	vec3 H = normalize(V + L);							//	ハーフベクトル

	float ao	   = 1.0;
	vec3  diffuse  = lightColor * DIFFUSE_LIGHT  * lightPower;
	vec3  specular = lightColor * SPECULAR_LIGHT * lightPower;

	vec3	supecularColor	= mix(vec3(0.04, 0.04, 0.04), albedo.rgb, metalness);
	float	NdotV			= saturate(dot(N, V));
	float	fresnelFactor	= fresnel(NdotV, saturate(metalness));
	float	diffuseFactor	= 1.0 - fresnelFactor;
	float	specularFactor	= 1.0 - fresnelFactor;

	output._diffuse  = albedo.rgb	  * vec3(diffuse)  * diffuseFactor;
	output._specular = supecularColor * vec3(specular) * specularFactor;
};

//-----------------------------------------------------------------------------
//!	点光源
//-----------------------------------------------------------------------------
subroutine( LightingModeType )
void PointLight()
{
	vec2 uv = input._uv;

	vec3  worldPosition	= texture2D(texPosition,uv).xyz;
	vec3  worldNormal	= texture2D(texNormal,uv).xyz;
	vec3  albedo		= texture2D(texAlbedo,uv).rgb;
	float roughness		= texture2D(texPosition,uv).w;
	float metalness		= texture2D(texNormal,uv).w;

	vec3 lightDir = lightPosition - worldPosition;

	float dist = length(lightDir);

	//	値は適当	
	float constantAttenuation = 1.0;
	float linearAttenuation = 0.0001;
	float quadraticAttenuation = 0.0;

    //	減衰係数
	float attenuation =	1.0 / (constantAttenuation +
						linearAttenuation * dist +
						quadraticAttenuation * dist * dist);
	
	vec3 N = normalize(worldNormal * 2.0 - 1.0);		//	法線
	vec3 V = normalize(eyePosition - worldPosition);	//	座標
	vec3 L = normalize(lightDir);						//	ライト
	vec3 H = normalize(V + L);							//	ハーフベクトル

	float ao	   = 1.0;
	vec3  diffuse  = lightColor * DIFFUSE_LIGHT  * attenuation * lightPower;
	vec3  specular = lightColor * SPECULAR_LIGHT * lightPower;

	vec3	supecularColor	= mix(vec3(0.04, 0.04, 0.04), albedo.rgb, metalness);
	float	NdotV			= saturate(dot(N, V));
	float	fresnelFactor	= fresnel(NdotV, saturate(metalness));
	float	diffuseFactor	= 1.0 - fresnelFactor;
	float	specularFactor	= 1.0 - fresnelFactor;

	output._diffuse  = albedo.rgb	  * vec3(diffuse)  * diffuseFactor;
	output._specular = supecularColor * vec3(specular) * specularFactor;
};
//-----------------------------------------------------------------------------
//!	スポットライト
//-----------------------------------------------------------------------------
subroutine( LightingModeType )
void SpotLight()
{
	//	未実装
	
	vec2 uv = input._uv;

	vec3  worldPosition	= texture2D(texPosition,uv).xyz;
	vec3  worldNormal	= texture2D(texNormal,uv).xyz;
	vec3  albedo		= texture2D(texAlbedo,uv).rgb;
	float roughness		= texture2D(texPosition,uv).w;
	float metalness		= texture2D(texNormal,uv).w;
};


vec3 NormalDecode(vec2 normal)
{
	vec4 enc = vec4(normal,0.0,0.0);
	vec4 nn = enc * vec4(2,2,0,0) + vec4(-1,-1,1,-1);
	float l = dot(nn.xyz, - nn.xyw);
	nn.z = l;
	nn.xy *= sqrt(l);
	return nn.xyz * 2 + vec3(0,0,-1);
}

void main()
{
	Lighting();
}

/*
//	メモ
void LaplacianFilter()
{
	output._diffuse = vec3(1,1,1);

	vec4 c = texture(texAlbedo, input._uv) * 8.0;

	c += textureOffset(texAlbedo, input._uv, ivec2(-1, -1)) * -1.0;
	c += textureOffset(texAlbedo, input._uv, ivec2( 0, -1)) * -1.0;
	c += textureOffset(texAlbedo, input._uv, ivec2(+1, -1)) * -1.0;

	c += textureOffset(texAlbedo, input._uv, ivec2(-1,  0)) * -1.0;
	c += textureOffset(texAlbedo, input._uv, ivec2(+1,  0)) * -1.0;

	c += textureOffset(texAlbedo, input._uv, ivec2(-1, +1)) * -1.0;
	c += textureOffset(texAlbedo, input._uv, ivec2( 0, +1)) * -1.0;
	c += textureOffset(texAlbedo, input._uv, ivec2(+1, +1)) * -1.0;


	if( c.r > 0.05 ) output._diffuse = vec3(0,0,0);
	if( c.g > 0.05 ) output._diffuse = vec3(0,0,0);
	if( c.b > 0.05 ) output._diffuse = vec3(0,0,0);
}
*/

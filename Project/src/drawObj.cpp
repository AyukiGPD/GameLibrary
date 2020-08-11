//=============================================================================
//!
//!	@file	drawObj.cpp
//!	@brief	�}�`�`��֐��Q
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace Math;

//-----------------------------------------------------------------------------
//!	�@���쐬
//!	@param	[in]	A	���_A
//!	@param	[in]	B	���_B
//!	@param	[in]	C	���_C
//!	@return	�@���x�N�g��
//-----------------------------------------------------------------------------
Vector3 CreateTriangleNormal(const Vector3& A, const Vector3& B, const Vector3& C)
{
	Vector3 dir1(B - A);
	Vector3 dir2(C - B);
	Vector3 result = Vector3::Normalize(Vector3::Cross(dir1, dir2));	//AB BC�̊O��
	return result;
}

//!	@todo
//!	GL�̊֐���}�N����������GL�p�I�u�W�F�N�g�����


static RefPointer<BufferObject::Vao>	gpSprite2dVao;		//!< 2DVao	�Ƃ肠���������ɍ��
static ShaderHundler					gTexture2dShader;

//	�Ƃ肠�����̎���
bool Load2DSprite()
{
	if(!gpSprite2dVao)
	{
		struct Vertex2D
		{
			Vector3 _position;
			Vector2 _uv;
			Color _color;
		};
		//	2D�`��pVao
		const float size = 1.0f;
		const float offset = 1.0f - size;
		const Vertex2D vertices[] =
		{
			{ Vector3(-size - offset, +size + offset, 0.0f), Vector2( 0, +1), Color(255, 255, 255) },
			{ Vector3(+size - offset, +size + offset, 0.0f), Vector2(+1, +1), Color(255, 255, 255) },
			{ Vector3(-size - offset, -size + offset, 0.0f), Vector2( 0,  0), Color(255, 255, 255) },
			{ Vector3(+size - offset, -size + offset, 0.0f), Vector2(+1,  0), Color(255, 255, 255) },
		};
		const u32 indices[] =
		{
			0, 1, 2, 3
		};

		//	�o�b�t�@���
		BufferObject::Element elements[] =
		{
			{ 0, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex2D, _position) },
			{ 1, 2, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex2D, _uv)		 },
			{ 2, 4, BufferObject::ELEMENT_TYPE::U8 , true , offsetof(Vertex2D, _color)	 },
		};

		const int vCount = ARRAY_COUNT(vertices);
		const int iCount = ARRAY_COUNT(indices);
		const int eCount = ARRAY_COUNT(elements);
		BufferObject::Vao::Desc desc;
		desc.SetVertexData(sizeof(Vertex2D), vCount, &vertices[0]);
		desc.SetIndexData(sizeof(u32), iCount, &indices[0]);
		desc.SetElementList(elements, eCount);

		gpSprite2dVao.reset(BufferObject::Vao::Create(
			desc,
			BufferObject::PRIMITIVE_TYPE::TRIANGLE_STRIP,
			BufferObject::INDEX_TYPE::TYPE_32
			));
		if(!gpSprite2dVao) return false;
	}

	if (gTexture2dShader.IsEmpty())
	{
		ShaderManager* pShader = Manager::Shader();
		ShaderHundler shader = pShader->Load("Standard2D", PROGRAM_TYPE::VS_FS);
		if (shader.IsEmpty()) return false;
		gTexture2dShader = shader;
	}
	return true;
}

namespace Render2D
{

//-----------------------------------------------------------------------------
//!	�l�p�`��
//!	@param	[in]	x		���WX
//!	@param	[in]	y		���WY
//!	@param	[in]	w		�T�C�YW
//!	@param	[in]	h		�T�C�YH
//!	@param	[in]	color	�F
//-----------------------------------------------------------------------------
void Rect(f32 x, f32 y, f32 w, f32 h,const Color& color)
{
	f32 x1 = x + w, y1 = y + h;
	glBegin(GL_QUADS);
	SetColor(color);
	glVertex2f(x, y);
	glVertex2f(x1, y);
	glVertex2f(x1, y1);
	glVertex2f(x, y1);
	glEnd();
}
//-----------------------------------------------------------------------------
//!	�l�p�`��
//!	@param	[in]	position	���W
//!	@param	[in]	size		�T�C�Y
//!	@param	[in]	color		�F
//-----------------------------------------------------------------------------
void Rect(const Vector2& position, const Vector2& size, const Color& color)
{
	Rect(position.x, position.y, size.x, size.y, color);
}


//-----------------------------------------------------------------------------
//!	�e�N�X�`���`��
//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^
//-----------------------------------------------------------------------------
void RenderBuffer(Texture* pTexture)
{
	if(!gpSprite2dVao)
	{
		if(!Load2DSprite())
		{
			assert(false);
		}
	}
	if (pTexture)
	{
		ShaderManager* pShader = Manager::Shader();
		pShader->Begin(gTexture2dShader);
		Texture::BindTexture(pTexture, 0);
		gpSprite2dVao->Render();
		Texture::UnBindTexture(0);
		pShader->End();
	}
	else
	{
		gpSprite2dVao->Render();
	}
}

};	//	namespace Render2D

namespace Render3D
{

//-----------------------------------------------------------------------------
//!	�_�`��
//!	@param	[in]	position	���W
//!	@param	[in]	color		�F		default:��
//!	@param	[in]	size		�T�C�Y	default:1.0f
//-----------------------------------------------------------------------------
void Point(const Vector3& position, const Color& color, f32 size)
{
	glPointSize(size);

	static u32 indices[1] = { 0 };
	Vector3 positionArray[1] = { position };
	Color colorArray[1] = { color };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Color), colorArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);
}

//-----------------------------------------------------------------------------
//!	���`��
//!	@param	[in]	from	�n�_
//!	@param	[in]	to		�I�_
//!	@param	[in]	color	�F		default:��
//-----------------------------------------------------------------------------
void Line(const Vector3& from, const Vector3& to, const Color& color)
{
	Manager::Shader()->SetSubroutine(SHADER_TYPE::FS, "Material");
	Manager::Shader()->SetUniform(11, color);
	static u32 indices[2] = { 0, 1 };
	Vector3 positionArray[2] = { from, to };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glEnableVertexAttribArray(0);

	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(0);

}

//-----------------------------------------------------------------------------
//!	���`��
//!	@param	[in]	from		�n�_
//!	@param	[in]	to			�I�_
//!	@param	[in]	fromColor	�n�_�F
//!	@param	[in]	toColor		�I�_�F
//-----------------------------------------------------------------------------
void Line(const Vector3& from, const Vector3& to, const Color& fromColor, const Color& toColor)
{
	static u32 indices[2] = { 0, 1 };
	Vector3 positionArray[2] = { from, to };
	Color colorArray[2] = { fromColor, toColor };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE , sizeof(Color), colorArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);
}

//-----------------------------------------------------------------------------
//!	�l�p�`��
//!	@param	[in]	position	���W
//!	@param	[in]	size		�T�C�Y
//!	@param	[in]	color		�F		default:��
//-----------------------------------------------------------------------------
void Rect(const Vector3& position, const Vector3& size, const Color& color)
{
	static u32 indices[6] = { 0, 1, 3, 1, 2, 3 };
	Vector3 positionArray[4] =
	{
		position,
		position + Vector3(size.x, 0.0f, 0.0f),
		position + Vector3(0.0f, size.y, 0.0f),
		position + size,
	};
	Color colorArray[4] = { color, color, color,color };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Color), colorArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);
}

//-----------------------------------------------------------------------------
//!	�l�p�`��
//!	@param	[in]	p1		���_�f�[�^
//!	@param	[in]	p2		���_�f�[�^
//!	@param	[in]	p3		���_�f�[�^
//!	@param	[in]	p4		���_�f�[�^
//!	@param	[in]	color	�F		default:��
//-----------------------------------------------------------------------------
void Rect(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, const Color& color)
{
	static u32 indices[6] = { 0, 1, 3, 1, 2, 3 };
	Vector3 positionArray[4] = { p1, p2, p3, p4 };
	Color colorArray[4] = { color, color, color, color };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Color), colorArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);
}

//-----------------------------------------------------------------------------
//!	�l�p�`��
//!	@param	[in]	p1	���_�f�[�^
//!	@param	[in]	p2	���_�f�[�^
//!	@param	[in]	p3	���_�f�[�^
//!	@param	[in]	p4	���_�f�[�^
//-----------------------------------------------------------------------------
void Rect(const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4)
{
	static u32 indices[6] = { 0, 1, 3, 1, 2, 3 };
	Vector3 positionArray[4] = { p1._position , p2._position , p3._position	, p4._position };
	Vector3 normalArray[4]	 = { p1._normal	  , p2._normal	 , p3._normal	, p4._normal   };
	Color colorArray[4]		 = { p1._color	  , p2._color	 , p3._color	, p4._color    };
	Vector2 uvArray[4]		 = { p1._uv		  , p2._uv		 , p3._uv		, p4._uv	   };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), normalArray);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE , sizeof(Color  ), colorArray);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), uvArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

//-----------------------------------------------------------------------------
//!	�l�p�`��
//!	@param	[in]	vertices	���_�f�[�^�z��
//-----------------------------------------------------------------------------
void Rect(const std::array<Vertex, 4>& vertices)
{
	static u32 indices[6] = { 0, 1, 3, 1, 2, 3 };
	Vector3 positionArray[4] = { vertices[0]._position , vertices[1]._position , vertices[2]._position , vertices[3]._position };
	Vector3 normalArray[4]	 = { vertices[0]._normal   , vertices[1]._normal   , vertices[2]._normal   , vertices[3]._normal   };
	Color colorArray[4]		 = { vertices[0]._color	   , vertices[1]._color	   , vertices[2]._color	   , vertices[3]._color    };
	Vector2 uvArray[4]		 = { vertices[0]._uv	   , vertices[1]._uv	   , vertices[2]._uv	   , vertices[3]._uv	   };
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), positionArray);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), normalArray);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE , sizeof(Color  ), colorArray);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), uvArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}


};	//	namespace Render3D


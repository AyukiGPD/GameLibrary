//=============================================================================
//!
//!	@file	stdafx.h
//!	@brief	�v���R���p�C���ς݃w�b�_�[
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//	�x�����b�Z�[�W�̖�����
//=============================================================================
#pragma warning( disable : 4201 )	// warning C4201: ��W���̊g���@�\���g�p����Ă��܂� : �����̍\���̂܂��͋��p�̂ł��B


//	Windows�w�b�_�[
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <ASSERT.h>
#include <process.h>
#include <algorithm>
#include <float.h>
#include <math.h>
#include <time.h>
#include <thread>
#include <mmsystem.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <atomic>
#include <memory>
#include <mutex>

#include <vector>
#include <array>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <numeric>


#if 0
//-----------------------------------------------------------------------------
//	DirectX�֘A
//-----------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dcompiler.h>

// ���C�u�����t�@�C���̃����N�w��
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//---- DirectX Math
#include <DirectXMath.h>

//---- DirectX Tex
#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "../OpenSource/DirectXTex/DirectXTex/Bin/Desktop_2013/Win32/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "../OpenSource/DirectXTex/DirectXTex/Bin/Desktop_2013/Win32/Release/DirectXTex.lib")
#endif

//---- DirectX ToolKit
#include <Inc/Audio.h>
#include <Inc/CommonStates.h>
#include <Inc/DDSTextureLoader.h>
#include <Inc/DirectXHelpers.h>
#include <Inc/Effects.h>
#include <Inc/GeometricPrimitive.h>
#include <Inc/Model.h>
#include <Inc/PrimitiveBatch.h>
#include <Inc/ScreenGrab.h>
#include <Inc/SimpleMath.h>
#include <Inc/SpriteBatch.h>
#include <Inc/SpriteFont.h>
#include <Inc/VertexTypes.h>
#include <Inc/WICTextureLoader.h>

#ifdef _DEBUG
#pragma comment(lib, "../OpenSource/DirectXTK/Bin/Desktop_2013/Win32/Debug/DirectXTK.lib")
#else
#pragma comment(lib, "../OpenSource/DirectXTK/Bin/Desktop_2013/Win32/Release/DirectXTK.lib")
#endif

#endif


//=============================================================================
//	OpenGL�֘A
//=============================================================================
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#pragma comment(lib, "glew32.lib")		//!< glew

//=============================================================================
//	GDI+
//=============================================================================
#pragma warning( push )
#pragma warning ( disable : 4458 )		//	warning C4458 : �N���X �����o�[���B������܂�

#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
#pragma warning( pop )

//=============================================================================
// FBX SDK
//=============================================================================
#include <fbxsdk.h>

//=============================================================================
//	OpenAL
//=============================================================================
#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32.lib")

//=============================================================================
//	Bullet
//=============================================================================
#pragma warning( push )

//	warning���������߂̋L�q
#pragma warning ( disable : 4127 )		//	warning C4127: ���������萔�ł��B
#pragma warning ( disable : 4100 )		//	warning C4100: �����͊֐��̖{�̕��� 1 �x���Q�Ƃ���܂���B
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#ifdef X64

#ifdef _DEBUG
#pragma comment( lib, "BulletCollision_vs2010_x64_debug.lib" )
#pragma comment( lib, "BulletDynamics_vs2010_x64_debug.lib" )
#pragma comment( lib, "BulletSoftBody_vs2010_x64_debug.lib" )
#pragma comment( lib, "LinearMath_vs2010_x64_debug.lib" )
#else
#pragma comment( lib, "BulletCollision_vs2010_x64_release.lib" )
#pragma comment( lib, "BulletDynamics_vs2010_x64_release.lib" )
#pragma comment( lib, "BulletSoftBody_vs2010_x64_release.lib" )
#pragma comment( lib, "LinearMath_vs2010_x64_release.lib" )
#endif // _DEBUG

#else // X64

#ifdef _DEBUG

#pragma comment( lib, "BulletCollision_vs2010_debug.lib" )
#pragma comment( lib, "BulletDynamics_vs2010_debug.lib" )
#pragma comment( lib, "LinearMath_vs2010_debug.lib" )
#else
#pragma comment( lib, "BulletCollision_vs2010.lib" )
#pragma comment( lib, "BulletDynamics_vs2010.lib" )
#pragma comment( lib, "BulletSoftBody_vs2010.lib" )
#pragma comment( lib, "LinearMath_vs2010.lib" )
#endif // _DEBUG

#endif // X64

#pragma warning( pop )

//----	�V�X�e��
#include "typeDef.h"
#include "Macro.h"

#include "OpenGL.h"
#include "System.h"
#include "Timer.h"
#include "Math.h"

#include "Vector.h"
#include "Color.h"
#include "Matrix.h"

#include "Singleton.h"
#include "RefCounter.h"
#include "Tag.h"

#include "ManagerBase.h"

#include "Texture.h"

#include "Shader.h"
#include "FrameRate.h"
#include "CpuUsage.h"
#include "GpuUsage.h"

#include "BaseMesh.h"
#include "BaseObject.h"
#include "Collision.h"






//----	�C���v�b�g
#include "XInputPad.h"
#include "KeyInput.h"
#include "Mouseinput.h"


//----	�`��֘A
#include "BufferObject.h"
#include "drawObj.h"
#include "Particle.h"
#include "FrameBuffer.h"
#include "Filter.h"
#include "DeferredRenderer.h"

//----	���\�[�X
#include "DataLoader.h"
#include "Model.h"
#include "Sound.h"
#include "Font.h"

#include "SystemResource.h"




#include "Camera.h"
#include "Light.h"

//----	�Ǘ�
#include "RenderManager.h"
#include "DebugManager.h"

//----	�Q�[��
#include "MyInclude.h"


//----	���C��
#include "SystemWork.h"
#include "SystemMain.h"





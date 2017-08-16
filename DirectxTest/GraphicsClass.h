#pragma once


#include <windows.h>
#include <d3d11.h>
#include "D3DClass.h"
#include "Camera.h"
#include "ModelClass.h"
#include "Colorshaderclass.h"
#include "Textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include <vector>
#include "bitmapclass.h"
#include "textclass.h"
#include "LoadingScene.h"
#include "frustumclass.h"
#include "MutiTextureShaderClass.h"
#include "BumpMapShaderClass.h"
#include "SpecularMapShaderClass.h"
#include "RenderToTextureClass.h"
#include "DebugWindowClass.h"
#include "SoundClass.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool RenderFromTexture(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, int, float);
	bool RenderToTexture(float, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool RenderScene(float, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool Render(float);
	bool Render2D(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool Render3D(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, float);
	bool CopySurfaceToTexture();
	bool RenderLastFrame(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	vector<ModelClass*> m_Models;
	vector<BitmapClass*> m_Bitmaps;
	vector<int> cameraparts;
	vector<int> terrainparts;
	vector<int> bulletparts;
	vector<int> shellparts;
	vector<int> handparts;
	Colorshaderclass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextClass* m_Text;
	LoadingScene* m_LoadingScene;
	int screenWidth, screenHeight;
	FrustumClass* m_Frustum;
	MultiTextureShaderClass* m_MultiTextureShader;
	BumpMapShaderClass* m_BumpMapShader;
	SpecMapShaderClass* m_SpecMapShader;
	DebugWindowClass* m_DebugWindow;
	vector<RenderTextureClass*> m_RenderTextures;
	ID3D11Texture2D* LastFrameTexture = NULL;
};
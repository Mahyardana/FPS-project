#pragma once
#include <D3D11.h>
#include "Camera.h"
#include "TextClass.h"
#include "D3DClass.h"

class LoadingScene
{
public:
	LoadingScene();
	LoadingScene(LoadingScene&);
	~LoadingScene();
	bool Initialize(D3DClass*, TextClass*, CameraClass*, HWND, int, int);
	bool Render(int, int);

private:
	CameraClass *m_Camera;
	TextClass *m_Text;
	D3DClass *m_D3D;
	int screenWidth, screenHeight;
};
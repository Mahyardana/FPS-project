#include "LoadingScene.h"
#include <string>
#include <chrono>
#include <thread>

LoadingScene::LoadingScene()
{
	m_Camera = 0;
	m_Text = 0;
	m_D3D = 0;

}

LoadingScene::LoadingScene(LoadingScene &)
{
}

LoadingScene::~LoadingScene()
{
}

bool LoadingScene::Initialize(D3DClass* m_D3D, TextClass* m_Text, CameraClass* Camera, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	m_Camera = Camera;
	this->m_D3D = m_D3D;
	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
	this->m_Text = m_Text;
	return true;
}
int step = 0;
bool LoadingScene::Render(int threads, int total)
{
	MSG msg;
	bool done, result;
	int donethreads = total - threads;
	int percent = (donethreads * 100) / total;

	ZeroMemory(&msg, sizeof(MSG));
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		done = true;
	}
	else
	{
		D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
		bool result;
		// Clear the buffers to begin the scene.
		m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// Generate the view matrix based on the camera's position.
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);
		string loading = "Loading Resources";
		string progress = to_string(percent) + " %";
		for (int i = 0; i < step; i++)
		{
			loading += ".";
		}
		m_D3D->TurnZBufferOff();
		m_D3D->TurnOnAlphaBlending();
		m_Text->setColor(1, 1, 1);
		m_Text->setPosition(screenWidth / 2 - 17 * 20, screenHeight / 2 - 20);
		m_Text->Render(m_D3D->GetDeviceContext(), loading, worldMatrix, viewMatrix, orthoMatrix);
		m_Text->setColor(1, (float)donethreads / total, (float)donethreads / total);
		m_Text->setPosition(screenWidth / 2 - (progress.size() * 25), screenHeight / 2 + 50);
		m_Text->Render(m_D3D->GetDeviceContext(), progress, worldMatrix, viewMatrix, orthoMatrix);
		m_D3D->TurnOffAlphaBlending();
		m_D3D->TurnZBufferOn();
		// Present the rendered scene to the screen.
		m_D3D->EndScene();
		Sleep(500);
		step++;
		step %= 4;
	}
	return true;
}



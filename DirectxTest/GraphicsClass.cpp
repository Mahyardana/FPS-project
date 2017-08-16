#include "GraphicsClass.h"
#include "Movement.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <iomanip>
#include <sstream>
int threads = 0;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_Text = 0;
	m_LoadingScene = 0;
	m_Frustum = 0;
	m_MultiTextureShader = 0;
	m_BumpMapShader = 0;
	m_SpecMapShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	SoundClass::CreateSound("BackgroundSound.mp3", true);
	int bg = SoundClass::CreateChannel(0, true);
	SoundClass::setVolume(bg, 0.3);
	float pos[] = { 0,0,0 };
	SoundClass::setArgsAndPlay(bg, pos);
	SoundClass::CreateSound("footstep\\1.wav", true);
	SoundClass::CreateSound("handgun\\sound.wav", false);
	SoundClass::CreateSound("m4a\\sound.wav", false);
	SoundClass::CreateSound("bounce\\1.wav", false);
	SoundClass::CreateSound("bounce\\2.wav", false);
	SoundClass::CreateSound("bounce\\3.wav", false);
	SoundClass::CreateSound("handgun\\reload.wav", false);
	SoundClass::CreateSound("m4a\\reload.wav", false);
	SoundClass::CreateSound("hit.wav", false);
	bool result;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 1.0f, -10.0f);
	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
		return false;
	}

	m_LoadingScene = new LoadingScene;
	if (!m_LoadingScene)
	{
		return false;
	}
	result = m_LoadingScene->Initialize(m_D3D, m_Text, m_Camera, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}
	vector<thread> thcontainer;
	// Create the model object.
	{
		ModelClass* m_Model;

		//model 5
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}

			// Initialize the model object.
			m_Model->setPosition(-15, 1, 0);

			m_Models.push_back(m_Model);
			char* dds5 = "stone01.dds";
			char* dds6 = "bump01.dds";
			char* obj5 = "cube.obj";
			thcontainer.push_back(thread(&ModelClass::Initialize1, m_Model, m_D3D->GetDevice(), obj5, dds5, dds6, &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 7
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}

			char* dds5 = "stone01.dds";
			char* dds6 = "bump01.dds";
			char* obj5 = "cube.obj";
			// Initialize the model object.
			m_Model->setPosition(-20, 1, 0);

			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize, m_Model, m_D3D->GetDevice(), obj5, dds5, &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 8
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}

			// Initialize the model object.
			m_Model->setPosition(-25, 1, 0);
			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize2, m_Model, m_D3D->GetDevice(),
				(char*)"stone\\model.obj", (char*)"stone\\texture.dds", (char*)"stone\\bump.dds", (char*)"stone\\specular.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 9
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}

			// Initialize the model object.
			m_Model->setPosition(0.01f, 0.775f, 0.52f);
			m_Model->isbreakable = false;
			cameraparts.push_back(m_Models.size());
			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize2, m_Model, m_D3D->GetDevice(),
				(char*)"handgun\\slide\\model.obj", (char*)"handgun\\slide\\texture.dds", (char*)"handgun\\slide\\bump.dds", (char*)"handgun\\slide\\specular.dds", &threads));
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 10
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			cameraparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Model->setPosition(0.01f, 0.2f, 0.4f);
			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize2, m_Model, m_D3D->GetDevice(),
				(char*)"handgun\\body\\model.obj", (char*)"handgun\\body\\texture.dds", (char*)"handgun\\body\\bump.dds", (char*)"handgun\\body\\specular.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}


		//model 11
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			cameraparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Model->setPosition(-0.05f, 0.89f, -0.6);
			D3DXMatrixScaling(&m_Model->scale, 0.5, 0.5, 0.5);
			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize1, m_Model, m_D3D->GetDevice(),
				(char*)"m4a\\slide\\model.obj", (char*)"m4a\\slide\\texture.dds", (char*)"m4a\\slide\\bump.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 12
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			cameraparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Model->setPosition(-0.05f, -0.0f, 0.2);
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 0.5, 0.5, 0.5);
			thcontainer.push_back(thread(&ModelClass::Initialize1, m_Model, m_D3D->GetDevice(),
				(char*)"m4a\\body\\model.obj", (char*)"m4a\\body\\texture.dds", (char*)"m4a\\body\\bump.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 17
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.			
			bulletparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Model->setPosition(0, 0.3, 1.5);
			D3DXMatrixScaling(&m_Model->scale, 0.1, 0.1, 0.1);
			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize, m_Model, m_D3D->GetDevice(),
				(char*)"handgun\\bullet\\model.obj", (char*)"handgun\\bullet\\texture.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 16
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			bulletparts.push_back(m_Models.size());
			m_Model->setPosition(0, 0, 0);
			m_Model->isbreakable = false;
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 0.12, 0.12, 0.12);
			thcontainer.push_back(thread(&ModelClass::Initialize, m_Model, m_D3D->GetDevice(),
				(char*)"m4a\\bullet\\model.obj", (char*)"m4a\\bullet\\texture.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 13
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			terrainparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 500, 500, 500);
			m_Model->setPosition(0.0f, -40.0f, 0.0f);
			thcontainer.push_back(thread(&ModelClass::Initialize, m_Model, m_D3D->GetDevice(),
				(char*)"terrain\\sky\\model.obj", (char*)"terrain\\sky\\texture.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 14
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			terrainparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 5, 5, 5);
			m_Model->setPosition(0.0f, 0.0f, 0.0f);
			thcontainer.push_back(thread(&ModelClass::Initialize2, m_Model, m_D3D->GetDevice(),
				(char*)"terrain\\ground\\model.obj", (char*)"terrain\\ground\\texture.dds", (char*)"terrain\\ground\\bump.dds", (char*)"terrain\\ground\\specular.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 18
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.			
			shellparts.push_back(m_Models.size());
			m_Model->isbreakable = false;
			m_Model->setPosition(0, -0.3, 2);
			D3DXMatrixScaling(&m_Model->scale, 0.09, 0.09, 0.09);
			m_Models.push_back(m_Model);
			thcontainer.push_back(thread(&ModelClass::Initialize, m_Model, m_D3D->GetDevice(),
				(char*)"handgun\\shell\\model.obj", (char*)"handgun\\shell\\texture.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 19
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			shellparts.push_back(m_Models.size());
			m_Model->setPosition(0, 0, 0);
			m_Model->isbreakable = false;
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 0.1, 0.1, 0.1);
			thcontainer.push_back(thread(&ModelClass::Initialize, m_Model, m_D3D->GetDevice(),
				(char*)"m4a\\shell\\model.obj", (char*)"m4a\\shell\\texture.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 21
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			handparts.push_back(m_Models.size());
			m_Model->setPosition(0, 0, 0);
			m_Model->isbreakable = false;
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 1, 1, 1);
			thcontainer.push_back(thread(&ModelClass::Initialize2, m_Model, m_D3D->GetDevice(),
				(char*)"hand\\handgun\\model.obj", (char*)"hand\\handgun\\texture.dds", (char*)"hand\\handgun\\bump.dds", (char*)"hand\\handgun\\specular.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}

		//model 20
		{
			m_Model = new ModelClass;
			if (!m_Model)
			{
				return false;
			}
			// Initialize the model object.
			handparts.push_back(m_Models.size());
			m_Model->setPosition(0, 0, 0);
			m_Model->isbreakable = false;
			m_Models.push_back(m_Model);
			D3DXMatrixScaling(&m_Model->scale, 0.5, 0.5, 0.5);
			thcontainer.push_back(thread(&ModelClass::Initialize2, m_Model, m_D3D->GetDevice(),
				(char*)"hand\\m4a\\model.obj", (char*)"hand\\m4a\\texture.dds", (char*)"hand\\m4a\\bump.dds", (char*)"hand\\m4a\\specular.dds", &threads));
			//result = m_Model->Initialize(m_D3D->GetDevice(), obj5, "mk.dds", &threads);
			if (!result)
			{
				MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
				return false;
			}
		}
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	// Create the multitexture shader object.
	m_MultiTextureShader = new MultiTextureShaderClass;
	if (!m_MultiTextureShader)
	{
		return false;
	}

	// Initialize the multitexture shader object.
	result = m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the multitexture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the specular map shader object.
	m_SpecMapShader = new SpecMapShaderClass;
	if (!m_SpecMapShader)
	{
		return false;
	}

	// Initialize the specular map shader object.
	result = m_SpecMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the specular map shader object.", "Error", MB_OK);
		return false;
	}


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	// Create the bump map shader object.
	m_BumpMapShader = new BumpMapShaderClass;
	if (!m_BumpMapShader)
	{
		return false;
	}

	// Initialize the bump map shader object.
	result = m_BumpMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the bump map shader object.", "Error", MB_OK);
		return false;
	}


	// Initialize the light object.
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);



	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}

	//Create the color shader object.
	m_ColorShader = new Colorshaderclass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	// Create the render to texture object.

	//first RenderTexture.
	m_RenderTextures.push_back(new RenderTextureClass);

	// Initialize the render to texture object.
	result = m_RenderTextures[0]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	//Second RenderTexture.
	m_RenderTextures.push_back(new RenderTextureClass);

	// Initialize the render to texture object.
	result = m_RenderTextures[1]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the debug window object.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// Initialize the debug window object.
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 1920, 1080);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the debug window object.", "Error", MB_OK);
		return false;
	}

	// Create the bitmap objects.
	{
		BitmapClass* m_Bitmap;
		m_Bitmap = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}

		// Initialize the bitmap object.
		thcontainer.push_back(thread(&BitmapClass::Initialize, m_Bitmap, m_D3D->GetDevice(), screenWidth, screenHeight, (char*)"UI1.dds", 400, 300, &threads));
		m_Bitmaps.push_back(m_Bitmap);
		m_Bitmap = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}

		// Initialize the bitmap object.
		thcontainer.push_back(thread(&BitmapClass::Initialize, m_Bitmap, m_D3D->GetDevice(), screenWidth, screenHeight, (char*)"UI2.dds", 400, 300, &threads));
		m_Bitmaps.push_back(m_Bitmap);
		m_Bitmap = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}

		// Initialize the bitmap object.
		thcontainer.push_back(thread(&BitmapClass::Initialize, m_Bitmap, m_D3D->GetDevice(), screenWidth, screenHeight, (char*)"handgun_UI.dds", 256, 256, &threads));
		m_Bitmaps.push_back(m_Bitmap);
		m_Bitmap = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}

		// Initialize the bitmap object.
		thcontainer.push_back(thread(&BitmapClass::Initialize, m_Bitmap, m_D3D->GetDevice(), screenWidth, screenHeight, (char*)"m4a_UI.dds", 350, 350, &threads));
		m_Bitmaps.push_back(m_Bitmap);
		m_Bitmap = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}

		// Initialize the bitmap object.
		thcontainer.push_back(thread(&BitmapClass::Initialize, m_Bitmap, m_D3D->GetDevice(), screenWidth, screenHeight, (char*)"target_UI.dds", 10, 10, &threads));
		m_Bitmaps.push_back(m_Bitmap);
	}

	//detach threads
	for (int i = 0; i < thcontainer.size(); i++)
	{
		//thcontainer[i].join();
		thcontainer[i].detach();
	}
	int total = threads;
	while (threads != 0)
	{
		result = m_LoadingScene->Render(threads, total);
		if (!result)
		{
			return false;
		}
	}
	thcontainer.clear();



	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the debug window object.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// Release the render to texture object.
	while (m_RenderTextures.size() > 0)
	{
		m_RenderTextures[0]->Shutdown();
		delete m_RenderTextures[0];
		m_RenderTextures.erase(m_RenderTextures.begin());
	}
	m_RenderTextures.clear();

	// Release the specular map shader object.
	if (m_SpecMapShader)
	{
		m_SpecMapShader->Shutdown();
		delete m_SpecMapShader;
		m_SpecMapShader = 0;
	}

	// Release the bump map shader object.
	if (m_BumpMapShader)
	{
		m_BumpMapShader->Shutdown();
		delete m_BumpMapShader;
		m_BumpMapShader = 0;
	}

	// Release the multitexture shader object.
	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = 0;
	}

	// Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	if (m_LoadingScene)
	{
		delete m_LoadingScene;
		m_LoadingScene = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	// Release the bitmap object.
	while (m_Bitmaps.size() > 0)
	{
		m_Bitmaps[0]->Shutdown();
		delete m_Bitmaps[0];
		m_Bitmaps.erase(m_Bitmaps.begin());
	}
	m_Bitmaps.clear();

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	while (m_Models.size() > 0)
	{
		m_Models[0]->Shutdown();
		delete m_Models[0];
		m_Models.erase(m_Models.begin());
	}
	m_Models.clear();

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

float groundxdiffer;
float groundzdiffer;
bool GraphicsClass::Frame()
{
	Movement::checksurroundings(&m_Models);
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 1 * Timer::getDifference();
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	D3DXVECTOR4 up(0, 1, 0, 1);
	D3DXVECTOR4 forward(0, 0, -1, 1);
	D3DXMATRIX transformmatrix;
	D3DXMatrixRotationYawPitchRoll(&transformmatrix, Movement::getYaw(), Movement::getPitch(), 0);
	D3DXVec4Transform(&up, &up, &transformmatrix);
	D3DXVec4Transform(&forward, &forward, &transformmatrix);
	float ori[] = { forward.x,forward.y,forward.z,up.x,up.y,up.z };
	float pos[] = { Movement::getX(),Movement::getY(),Movement::getZ() };
	SoundClass::setListener(pos, ori);
	m_Camera->SetPosition(Movement::getX(), Movement::getY(), Movement::getZ());
	m_Camera->SetRotation(Movement::getPitch(), Movement::getYaw(), 0);
	static D3DXVECTOR3 *primarypositions = nullptr;
	if (primarypositions == nullptr)
	{
		primarypositions = new D3DXVECTOR3[cameraparts.size()];
		for (int i = 0; i < cameraparts.size(); i++)
		{
			primarypositions[i].x = m_Models[cameraparts[i]]->positionX;
			primarypositions[i].y = m_Models[cameraparts[i]]->positionY;
			primarypositions[i].z = m_Models[cameraparts[i]]->positionZ;
		}
	}
	for (int i = 0; i < cameraparts.size(); i++)
	{
		if (i == Movement::getEquiped() * 2)
		{
			if (Movement::getEquiped() == 0)
			{
				m_Models[cameraparts[i]]->setPosition(Movement::getOriention()*0.6 + primarypositions[i].x, -1.1 + primarypositions[i].y, Movement::getSlideDiffer() + 2.5 + primarypositions[i].z);
				m_Models[handparts[0]]->setPosition(Movement::getOriention()*0.7 + primarypositions[i].x, -1.87 + primarypositions[i].y, 1.43 + primarypositions[i].z);
			}
			else
			{
				m_Models[cameraparts[i]]->setPosition(Movement::getOriention()*0.55 + primarypositions[i].x, -1.22 + primarypositions[i].y, 2.95 + primarypositions[i].z);
				m_Models[handparts[1]]->setPosition(Movement::getOriention()*-0.41 + primarypositions[i].x, -1.89 + primarypositions[i].y, 2.32 + primarypositions[i].z);
			}
		}
		else if (i < Movement::getWeapons().size() * 2)
		{
			m_Models[cameraparts[i]]->setPosition(Movement::getOriention()*0.6 + primarypositions[i].x, -1.1 + primarypositions[i].y, 2.5 + primarypositions[i].z);
		}
		else
		{
			m_Models[cameraparts[i]]->setPosition(Movement::getX(), Movement::getY(), Movement::getZ());
		}
		D3DXMatrixTranslation(&m_Models[cameraparts[i]]->translateafter, Movement::getX(), Movement::getY(), Movement::getZ());
		m_Models[cameraparts[i]]->rotateafterdegreeyaw = D3DXToRadian(Movement::getYaw());
		double temp = (-Movement::getRecoilDiffer() * 8) + Movement::getPitch() + Movement::getWeaponPitch();
		m_Models[cameraparts[i]]->rotateafterdegreepitch = D3DXToRadian(temp);
		D3DXMatrixTranslation(&m_Models[handparts[Movement::getEquiped()]]->translateafter, Movement::getX(), Movement::getY(), Movement::getZ());
		m_Models[handparts[Movement::getEquiped()]]->rotateafterdegreeyaw = D3DXToRadian(Movement::getYaw());
		temp = (-Movement::getRecoilDiffer() * 8) + Movement::getPitch() + Movement::getWeaponPitch();
		m_Models[handparts[Movement::getEquiped()]]->rotateafterdegreepitch = D3DXToRadian(temp);
	}
	for (int i = 0; i < terrainparts.size(); i++)
	{
		if (i == 0)
		{
			static float sky = 0;
			sky += 0.01*Timer::getDifference();
			D3DXMatrixTranslation(&m_Models[terrainparts[i]]->translateafter, Movement::getX(), Movement::getY(), Movement::getZ());
			m_Models[terrainparts[i]]->rotateafterdegreeyaw = sky;
		}
		else if (i == 1)
		{
			static float lastx = -2.5, lastz = -2.5;
			static float x = -2.5, z = -2.5;
			x += lastx - Movement::getX();
			z += lastz - Movement::getZ();
			D3DXMatrixTranslation(&m_Models[terrainparts[i]]->translateafter, Movement::getX() + x, m_Models[terrainparts[i]]->positionY, Movement::getZ() + z);
			if (x >= 2.5)
				x -= 5;
			else if (x <= -2.5)
				x += 5;
			if (z >= 2.5)
				z -= 5;
			else if (z <= -2.5)
				z += 5;
			groundxdiffer = 2.5 + x;
			groundzdiffer = 2.5 + z;
			lastx = Movement::getX();
			lastz = Movement::getZ();
		}
	}
	for (int i = 0; i < bulletparts.size(); i++)
	{
		if (i == 0)
		{
			m_Models[bulletparts[i]]->setPosition(Movement::getOriention()*0.6, -0.13, 3);
		}
		else if (i == 1)
		{
			m_Models[bulletparts[i]]->setPosition(Movement::getOriention()*0.57, -0.2, 4);
		}
		m_Models[bulletparts[i]]->rotatebeforedegreeroll += 10 * Timer::getDifference();
	}
	for (int i = 0; i < shellparts.size(); i++)
	{
		if (i == 0)
		{
			m_Models[shellparts[i]]->setPosition(Movement::getOriention()*0.57, -0.19, 2.3);
		}
		else if (i == 1)
		{
			m_Models[shellparts[i]]->setPosition(Movement::getOriention()*0.52, -0.28, 2.3);
		}
	}
	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	SoundClass::updatesystem();
	return true;
}

bool GraphicsClass::RenderFromTexture(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX projectionMatrix, int index, float alpha)
{
	bool result;
	m_D3D->TurnZBufferOff();
	D3DXMATRIX yawpitchroll;
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + 0.5f);
	float yaw = m_Camera->yaw;
	if (yaw > 2 * D3DX_PI)
	{
		yaw -= 2 * D3DX_PI;
	}
	else if (yaw < 0)
	{
		yaw += 2 * D3DX_PI;
	}
	if (yaw >= (float)D3DX_PI / 2 && yaw <= (float)(3 * D3DX_PI) / 2)
	{
		D3DXMatrixRotationYawPitchRoll(&yawpitchroll, m_Camera->yaw, m_Camera->pitch, 0);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&yawpitchroll, m_Camera->yaw, m_Camera->pitch, 0);
	}
	// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 1920, 1080, 0, 0);
	if (!result)
	{
		return false;
	}
	// Render the debug window using the texture shader.
	int blur = 1;
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
		yawpitchroll*translate*orthoMatrix, m_RenderTextures[index]->GetShaderResourceView(), blur, alpha);
	if (!result)
	{
		return false;
	}

	m_D3D->TurnZBufferOn();
	return true;
}

bool GraphicsClass::RenderToTexture(float rotation, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX projectionMatrix)
{
	static bool isfirst = true;
	bool result;

	if (!isfirst)
	{
		// Set the render target to be the render to texture.
		m_RenderTextures[1]->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
		// Clear the render to texture.
		m_RenderTextures[1]->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);
		RenderFromTexture(worldMatrix, viewMatrix, orthoMatrix, projectionMatrix, 0, 1);
	}
	// Set the render target to be the render to texture.
	m_RenderTextures[0]->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	// Clear the render to texture.
	m_RenderTextures[0]->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 0.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene(rotation, worldMatrix, viewMatrix, orthoMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	if (!isfirst)
	{
		RenderFromTexture(worldMatrix, viewMatrix, orthoMatrix, projectionMatrix, 1, 1);
	}
	isfirst = false;
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene(float rotation, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	//render3d objects
	Render3D(worldMatrix, viewMatrix, projectionMatrix, rotation);

	//render2d objects
	Render2D(worldMatrix, viewMatrix, orthoMatrix, projectionMatrix);

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Render scene to Texture
	//RenderToTexture(rotation, worldMatrix, viewMatrix, orthoMatrix, projectionMatrix);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	//RenderFromTexture(worldMatrix, viewMatrix, orthoMatrix, projectionMatrix, 0, 1);
	//RenderFromTexture(worldMatrix, viewMatrix, orthoMatrix, projectionMatrix, 1, 0.5);

	// Render Scene
	RenderScene(rotation, worldMatrix, viewMatrix, orthoMatrix, projectionMatrix);
	RenderLastFrame(worldMatrix, viewMatrix, orthoMatrix, projectionMatrix, 0.997);
	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	static long timer = 0;
	if (timer > 0)
	{
		CopySurfaceToTexture();
		timer = 0;
	}
	timer += Timer::getDifferencems();
	m_D3D->TurnOffAlphaBlending();
	return true;
}
int numberofrenderedobjects = 0;
bool GraphicsClass::Render2D(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	D3DXMATRIX yawpitchroll;
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + 0.5f);
	float yaw = m_Camera->yaw;
	if (yaw > 2 * D3DX_PI)
	{
		yaw -= 2 * D3DX_PI;
	}
	else if (yaw < 0)
	{
		yaw += 2 * D3DX_PI;
	}
	if (yaw >= (float)D3DX_PI / 2 && yaw <= (float)(3 * D3DX_PI) / 2)
	{
		D3DXMatrixRotationYawPitchRoll(&yawpitchroll, m_Camera->yaw, m_Camera->pitch, 0);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&yawpitchroll, m_Camera->yaw, m_Camera->pitch, 0);
	}
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	{
		result = m_Bitmaps[0]->Render(m_D3D->GetDeviceContext(), screenWidth - 400, screenHeight - 300);
		if (!result)
		{
			return false;
		}
		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmaps[0]->GetIndexCount(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix, m_Bitmaps[0]->GetTexture(), 0, 1);
		if (!result)
		{
			return false;
		}

		result = m_Bitmaps[1]->Render(m_D3D->GetDeviceContext(), 0, screenHeight - 300);
		if (!result)
		{
			return false;
		}
		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmaps[1]->GetIndexCount(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix, m_Bitmaps[1]->GetTexture(), 0, 1);
		if (!result)
		{
			return false;
		}
		int equiped = Movement::getEquiped();
		if (equiped == 0)
		{
			result = m_Bitmaps[2]->Render(m_D3D->GetDeviceContext(), 60, screenHeight - 240);
			if (!result)
			{
				return false;
			}
			// Render the bitmap with the texture shader.
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmaps[2]->GetIndexCount(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix, m_Bitmaps[2]->GetTexture(), 0, 1);
			if (!result)
			{
				return false;
			}
			m_Text->setColor(0, 0, 0);
			m_Text->setPosition(screenWidth - 300, screenHeight - 170);
			stringstream ss;
			ss << setw(2) << setfill('0') << Movement::getWeapons()[equiped]->getRemaining();
			ss << "/" << Movement::getWeapons()[equiped]->getMaxclipsize();
			result = m_Text->Render(m_D3D->GetDeviceContext(), ss.str(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix);
		}
		else if (equiped == 1)
		{
			result = m_Bitmaps[3]->Render(m_D3D->GetDeviceContext(), 10, screenHeight - 290);
			if (!result)
			{
				return false;
			}
			// Render the bitmap with the texture shader.
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmaps[3]->GetIndexCount(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix, m_Bitmaps[3]->GetTexture(), 0, 1);
			if (!result)
			{
				return false;
			}
			m_Text->setColor(0, 0, 0);
			m_Text->setPosition(screenWidth - 300, screenHeight - 170);
			stringstream ss;
			ss << setw(2) << setfill('0') << Movement::getWeapons()[equiped]->getRemaining();
			ss << "/" << Movement::getWeapons()[equiped]->getMaxclipsize();
			result = m_Text->Render(m_D3D->GetDeviceContext(), ss.str(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix);
		}
		result = m_Bitmaps[4]->Render(m_D3D->GetDeviceContext(), screenWidth / 2 + 35, screenHeight / 2 + 10);
		if (!result)
		{
			return false;
		}
		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmaps[4]->GetIndexCount(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix, m_Bitmaps[4]->GetTexture(), 0, 1);
		if (!result)
		{
			return false;
		}
		m_Text->setColor(0, 0, 0);
		m_Text->setPosition(screenWidth - 300, screenHeight - 170);
		stringstream ss;
		ss << setw(2) << setfill('0') << Movement::getWeapons()[equiped]->getRemaining();
		ss << "/" << Movement::getWeapons()[equiped]->getMaxclipsize();
		result = m_Text->Render(m_D3D->GetDeviceContext(), ss.str(), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix);
	}
	int fps = Timer::getFPS();
	// Render the text strings.
	m_Text->setColor(1, 0, 1);
	m_Text->setPosition(0, 0);
	result = m_Text->Render(m_D3D->GetDeviceContext(), "Number Of Renderd Objects:" + to_string(numberofrenderedobjects), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix);
	if (!result)
	{
		return false;
	}
	m_Text->setColor(0, 1, 0);
	m_Text->setPosition(screenWidth - 250, 0);
	result = m_Text->Render(m_D3D->GetDeviceContext(), "FPS:" + to_string(fps), worldMatrix, viewMatrix, yawpitchroll*translate*orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();
}

bool GraphicsClass::Render3D(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float rotation)
{
	bool refreshing = Movement::getRefreshState();
	static float degree = 0;
	numberofrenderedobjects = 0;
	bool renderModel;
	bool result;
	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	int j = 0, k = 0, p = 0, q = 0, r = 0;
	vector<bullet*> *bullets;
	bullets = &Movement::getBullets();
	vector<shell*> *shells = &Movement::getShells();
	for (int i = 0; i < m_Models.size(); i++)
	{
		if (refreshing)
		{
			m_Models[i]->isbreaked = false;
			m_Models[i]->healthcolor = 1;
		}
		float positionX = 0, positionY = 0, positionZ = 0, sizeX = 0, sizeY = 0, sizeZ = 0;
		bool isbullet = false;
		bool isground = false;
		bool isshell = false;
		bool isdustdoor = false;
		bool ishand = false;
		//m_Models[i]->setPosition(m_Models[i]->positionX, m_Models[i]->positionY, m_Models[i]->positionZ + 10*Timer::getDifference());
		m_Models[i]->GetData(&positionX, &positionY, &positionZ, &sizeX, &sizeY, &sizeZ);
		//renderModel = true;
		// Initialize the light object.
		m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(cosf(D3DXToRadian(degree)), -1.0f, sinf(D3DXToRadian(degree)));
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(32.0f);
		if ((j >= cameraparts.size() || i != cameraparts[j]) &&
			(k >= terrainparts.size() || i != terrainparts[k]) &&
			(p >= bulletparts.size() || i != bulletparts[p]) &&
			(q >= shellparts.size() || i != shellparts[q]) &&
			(r >= handparts.size() || i != handparts[r]))
		{
			renderModel = m_Frustum->CheckRectangle(positionX, positionY, positionZ, sizeX, sizeY, sizeZ);
			//m_Models[i]->rotatebeforedegreeyaw = rotation;
		}
		else if (j < cameraparts.size() && i == cameraparts[j])
		{
			if (j < Movement::getEquiped() * 2 + 2 && j >= Movement::getEquiped() * 2)
				renderModel = true;
			else
				renderModel = false;
			if (j == 2)
			{
				isdustdoor = true;
			}
			j++;
		}
		else if (k < terrainparts.size() && i == terrainparts[k])
		{
			if (k == 0)
			{
				// Initialize the light object.
				m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Light->SetDirection(0.0f, 1.0f, 0.0f);
				m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Light->SetSpecularPower(2.0f);
			}
			if (k == 1)
			{
				isground = true;
			}
			renderModel = true;
			k++;
		}
		else if (p < bulletparts.size() && i == bulletparts[p])
		{
			isbullet = true;
			renderModel = true;
			p++;
		}
		else if (q < shellparts.size() && i == shellparts[q])
		{
			isshell = true;
			renderModel = true;
			q++;
		}
		else if (r < handparts.size() && i == handparts[r])
		{
			ishand = true;
			if (r == Movement::getEquiped())
			{
				renderModel = true;
			}
			else
			{
				renderModel = false;
			}
			r++;
		}
		if (!m_Models[i]->isbreaked&&renderModel)
		{
			numberofrenderedobjects++;
			D3DXMatrixRotationYawPitchRoll(&m_Models[i]->rotatebefore, m_Models[i]->rotatebeforedegreeyaw, m_Models[i]->rotatebeforedegreepitch, m_Models[i]->rotatebeforedegreeroll);
			D3DXMatrixRotationYawPitchRoll(&m_Models[i]->rotateafter, m_Models[i]->rotateafterdegreeyaw, m_Models[i]->rotateafterdegreepitch, m_Models[i]->rotateafterdegreeroll);
			D3DXMatrixTranslation(&m_Models[i]->translate, positionX, positionY, positionZ);

			D3DXMATRIX transformer;


			m_Models[i]->Render(m_D3D->GetDeviceContext());

			// Render the model using the light shader.
			{
				if (!isground && !isbullet && !isshell && !isdustdoor && !m_Models[i]->multitextured && !m_Models[i]->specularmapped)
				{
					transformer = m_Models[i]->scale*m_Models[i]->rotatebefore*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
						m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Models[i]->healthcolor);
				}
				else if (!isground && !isbullet && !isshell && !isdustdoor && !m_Models[i]->specularmapped&&m_Models[i]->multitextured)
				{
					transformer = m_Models[i]->scale*m_Models[i]->rotatebefore*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
					result = m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
						m_Models[i]->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Models[i]->healthcolor);
				}
				else if (isground)
				{
					m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
					numberofrenderedobjects--;
					D3DXVECTOR3 translate, scale;
					D3DXQUATERNION rotate;
					bool render = true;
					groundxdiffer /= 5;
					groundzdiffer /= 5;
					for (int m = -20; m <= 20; m++)
					{
						for (int n = -20; n <= 20; n++)
						{
							//render = m_Frustum->CheckRectangle(positionX + (m*sizeX), positionY, positionZ + (n*sizeZ), sizeX * 2, sizeY * 2, sizeZ * 2);
							if (render)
							{
								float alpha = 1;
								numberofrenderedobjects++;
								D3DXMatrixTranslation(&m_Models[i]->translate, positionX + m*sizeX, positionY, positionZ + n*sizeZ);
								transformer = m_Models[i]->scale*m_Models[i]->rotatebefore*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
								result = m_SpecMapShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
									m_Models[i]->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(),
									m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Models[i]->healthcolor, 1);
							}
						}
					}
				}
				else if (isbullet)
				{
					numberofrenderedobjects--;
					for (int m = 0; m < bullets->size(); m++)
					{
						if (bullets->at(m)->weaponindex == p - 1 && !bullets->at(m)->isjobdone)
						{
							numberofrenderedobjects++;
							D3DXMatrixRotationYawPitchRoll(&m_Models[i]->rotatebefore, 0, 0, m_Models[i]->rotatebeforedegreeroll);
							D3DXMatrixRotationYawPitchRoll(&m_Models[i]->rotateafter, D3DXToRadian(bullets->at(m)->yaw), D3DXToRadian(bullets->at(m)->pitch), 0);
							D3DXMatrixTranslation(&m_Models[i]->translate, 0 + positionX, 0 + positionY, bullets->at(m)->step + positionZ);
							D3DXMatrixTranslation(&m_Models[i]->translateafter, bullets->at(m)->x, bullets->at(m)->y, bullets->at(m)->z);
							transformer = m_Models[i]->scale*m_Models[i]->rotatebefore*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
							result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
								m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
								m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Models[i]->healthcolor);
						}
						else if (bullets->at(m)->isjobdone)
						{
							//delete bullets->at(m);
							bullets->erase(bullets->begin() + m);
							m--;
						}
					}
				}
				else if (isshell)
				{
					numberofrenderedobjects--;
					for (int m = 0; m < shells->size(); m++)
					{
						if (shells->at(m)->weaponindex == q - 1 && !shells->at(m)->isjobdone)
						{
							numberofrenderedobjects++;
							D3DXMatrixRotationYawPitchRoll(&m_Models[i]->rotateafter, D3DXToRadian(shells->at(m)->yaw), D3DXToRadian(shells->at(m)->pitch), 0);
							D3DXMatrixRotationYawPitchRoll(&m_Models[i]->rotatebefore, 0, D3DXToRadian(shells->at(m)->selfpitch), D3DXToRadian(shells->at(m)->roll));
							D3DXMATRIX rotateyaw;
							D3DXMatrixRotationYawPitchRoll(&rotateyaw, D3DXToRadian(shells->at(m)->selfyaw), 0, 0);
							D3DXMATRIX rotateroll;
							D3DXMatrixRotationYawPitchRoll(&rotateroll, 0, 0, D3DXToRadian(shells->at(m)->roll));
							D3DXMatrixTranslation(&m_Models[i]->translate, shells->at(m)->stepx + positionX, shells->at(m)->stepy + positionY, positionZ);
							D3DXMatrixTranslation(&m_Models[i]->translateafter, shells->at(m)->x, shells->at(m)->y, shells->at(m)->z);
							transformer = m_Models[i]->scale*m_Models[i]->rotatebefore*rotateroll*rotateyaw*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
							result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
								m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
								m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Models[i]->healthcolor);
						}
						else if (shells->at(m)->isjobdone)
						{
							//delete shells->at(m);
							shells->erase(shells->begin() + m);
							m--;
						}
					}
				}
				else if (isdustdoor)
				{
					D3DXMATRIX rollrotate;
					D3DXMATRIX translate1, translate2;
					D3DXMatrixTranslation(&translate1, 0, -0.04, 0);
					D3DXMatrixTranslation(&translate2, 0, 0.04, 0);
					float degree = D3DXToRadian(Movement::getWeapons()[1]->getDoorDegree());
					D3DXMatrixRotationYawPitchRoll(&rollrotate, 0, 0, degree);
					transformer = m_Models[i]->scale*translate1*rollrotate*translate2*m_Models[i]->rotatebefore*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
					result = m_BumpMapShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
						m_Models[i]->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Models[i]->healthcolor);
				}
				else
				{
					transformer = m_Models[i]->scale*m_Models[i]->rotatebefore*m_Models[i]->translate*m_Models[i]->rotateafter*m_Models[i]->translateafter;
					result = m_SpecMapShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), transformer, viewMatrix, projectionMatrix,
						m_Models[i]->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Models[i]->healthcolor, m_Models[i]->alphachannel);
				}
			}
			if (!result)
			{
				return false;
			}
		}
		D3DXVECTOR3 bullet;
		D3DXMATRIX bmatrix;
		positionX += m_Models[i]->centerX;
		positionY += m_Models[i]->centerY;
		positionZ += m_Models[i]->centerZ;
		for (int m = 0; m < bullets->size() && m_Models[i]->isbreakable && !m_Models[i]->isbreaked; m++)
		{
			bullet.x = bullets->at(m)->x;
			bullet.y = bullets->at(m)->y;
			bullet.z = bullets->at(m)->z;
			int index = bullets->at(m)->weaponindex;
			D3DXMatrixRotationYawPitchRoll(&m_Models[bulletparts[index]]->rotateafter, D3DXToRadian(bullets->at(m)->yaw), D3DXToRadian(bullets->at(m)->pitch), 0);
			D3DXMatrixTranslation(&m_Models[bulletparts[index]]->translate, 0 + m_Models[bulletparts[index]]->positionX, 0 + m_Models[bulletparts[index]]->positionY, bullets->at(m)->step + m_Models[bulletparts[index]]->positionZ);
			D3DXMatrixTranslation(&m_Models[bulletparts[index]]->translateafter, bullets->at(m)->x, bullets->at(m)->y, bullets->at(m)->z);
			bmatrix = m_Models[bulletparts[index]]->translate*m_Models[bulletparts[index]]->rotateafter*m_Models[bulletparts[index]]->translateafter;
			D3DXVECTOR3 scale;
			D3DXQUATERNION rotate;
			D3DXMatrixDecompose(&scale, &rotate, &bullet, &bmatrix);
			if (bullet.x <= positionX + sizeX / 2 && bullet.x >= positionX - sizeX / 2
				&& bullet.y <= positionY + sizeY / 2 && bullet.y >= positionY - sizeY / 2
				&& bullet.z <= positionZ + sizeZ / 2 && bullet.z >= positionZ - sizeZ / 2
				)
			{
				renderModel = false;
				float pos[] = { bullet.x ,bullet.y ,bullet.z };
				SoundClass::setArgsAndPlay(SoundClass::CreateChannel(SND_HIT, false), pos);
				m_Models[i]->hit();
				bullets->at(m)->isjobdone = true;
			}
		}
	}
	if (refreshing)
		Movement::refreshScene();
	//degree += 10 * Timer::getDifference();
	return true;
}
UINT32 i = 0;
bool GraphicsClass::CopySurfaceToTexture()
{
	if (LastFrameTexture != NULL)
	{
		LastFrameTexture->Release();
	}
	D3D11_TEXTURE2D_DESC description;
	m_D3D->pSurface->GetDesc(&description);
	HRESULT result;
	if (FAILED(m_D3D->GetDevice()->CreateTexture2D(&description, NULL, &LastFrameTexture)))
		return false;
	m_D3D->GetDeviceContext()->CopyResource(LastFrameTexture, m_D3D->pSurface);
	return true;
}

bool GraphicsClass::RenderLastFrame(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, D3DXMATRIX projectionMatrix, float alpha)
{
	static bool firsttime = true;
	HRESULT result1;
	ID3D11ShaderResourceView *res = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	if (!firsttime)
	{
		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		result1 = m_D3D->GetDevice()->CreateShaderResourceView(LastFrameTexture, NULL, &res);
		if (FAILED(result1))
		{
			return false;
		}
		bool result;
		m_D3D->TurnZBufferOff();
		D3DXMATRIX yawpitchroll;
		D3DXMATRIX translate;
		D3DXMatrixTranslation(&translate, m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z + 0.5f);
		float yaw = m_Camera->yaw;
		if (yaw > 2 * D3DX_PI)
		{
			yaw -= 2 * D3DX_PI;
		}
		else if (yaw < 0)
		{
			yaw += 2 * D3DX_PI;
		}
		if (yaw >= (float)D3DX_PI / 2 && yaw <= (float)(3 * D3DX_PI) / 2)
		{
			D3DXMatrixRotationYawPitchRoll(&yawpitchroll, m_Camera->yaw, m_Camera->pitch, 0);
		}
		else
		{
			D3DXMatrixRotationYawPitchRoll(&yawpitchroll, m_Camera->yaw, m_Camera->pitch, 0);
		}
		// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 1920, 1080, 0, 0);
		if (!result)
		{
			return false;
		}
		//// Render the debug window using the texture shader.
		//int blur = Movement::getPitchDiffer();
		//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
		//	yawpitchroll*translate*orthoMatrix, res, blur, alpha);
		if (!result)
		{
			return false;
		}
		res->Release();
		m_D3D->TurnZBufferOn();
		i++;
	}
	else
	{
		firsttime = false;
	}
	return true;
}

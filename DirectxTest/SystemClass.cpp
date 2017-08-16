#include "SystemClass.h"
#include "Movement.h"
#include <chrono>
#include <thread>
int screenW = 1920;
int screenH = 1080;
int pitch = 0;
int yaw = 0;
void pointerthread()
{
	POINT p;
	while (1)
	{
		GetCursorPos(&p);
		if (p.x != screenW / 2 || p.y != screenH / 2)
		{
			pitch = (p.y - screenH / 2) * 4;
			yaw = (p.x - screenW / 2) * 4;
		}
		SetCursorPos(screenW / 2, screenH / 2);
		Sleep(17);
	}
}
void SystemClass::keyboardthread()
{
	while (true)
	{
		if (m_Input->IsKeyDown('Q'))
		{
			Movement::nextWeapon();
			m_Input->KeyUp('Q');
		}

		if (m_Input->IsKeyDown('R'))
		{
			Movement::reload();
			m_Input->KeyUp('R');
		}

		if (m_Input->IsKeyDown('O'))
		{
			Movement::changeOriention();
			m_Input->KeyUp('O');
		}

		if (m_Input->IsKeyDown(VK_ESCAPE))
		{
			exit(0);
		}

		if (m_Input->IsKeyDown('W'))
		{
			if (m_Input->IsKeyDown(VK_SHIFT))
			{
				Movement::sprint_forward();
			}
			else
			{
				Movement::move_forward();
			}
		}

		if (m_Input->IsKeyDown('A'))
		{
			Movement::move_left();
		}

		if (m_Input->IsKeyDown('D'))
		{
			Movement::move_right();
		}

		if (m_Input->IsKeyDown('S'))
		{
			Movement::move_backward();
		}
		if (m_Input->IsKeyDown(VK_F1))
		{
			Timer::toggleSlowMotion();
			m_Input->KeyUp(VK_F1);
		}
		if (m_Input->IsKeyDown(VK_F5))
		{
			Movement::refreshScene();
			m_Input->KeyUp(VK_F5);
		}
	}
}
SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass &)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	thread(pointerthread).detach();
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	m_Input->Initialize();

	//thread(&SystemClass::keyboardthread, this).detach();

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{

	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
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
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}
bool mouselup = false;
bool isshooting = false;
LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	Movement::look_pitch(pitch);
	Movement::look_yaw(yaw);
	pitch = 0;
	yaw = 0;
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		isshooting = true;
		return 0;
	}

	case WM_LBUTTONUP:
	{
		mouselup = true;
		isshooting = false;
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}
bool SystemClass::Frame()
{
	POINT p;
	Timer::UpdateTimer();
	bool result;
	GetCursorPos(&p);
	if (p.x != screenW / 2 || p.y != screenH / 2)
	{
		pitch = (p.y - screenH / 2) * 4;
		yaw = (p.x - screenW / 2) * 4;
	}
	SetCursorPos(screenW / 2, screenH / 2);
	static double lastshot = 0;
	static float recoildiffer = 0;
	static int shotcounter = 0;
	lastshot += Timer::getDifferencems();
	if (isshooting&&Movement::getWeapons()[Movement::getEquiped()]->getRemaining() > 0 && !Movement::getRealoadingState() && !Movement::getChangingState())
	{
		if (Movement::getWeapons()[Movement::getEquiped()]->getFireratems() < lastshot)
		{
			Movement::shoot();
			lastshot = 0;
			float temp = Movement::getWeapons()[Movement::getEquiped()]->getRecoil();
			//if (shotcounter == 0)
			//{
			//}
			//else
			//{
			//	differ = Movement::getWeapons()[Movement::getEquiped()].getRecoil() / 4;
			//	recoildiffer += differ;
			//}
			recoildiffer = temp;
			Movement::look_pitch(-temp / Timer::getDifference());
			shotcounter++;
		}
	}
	if (m_Input->IsKeyDown('Q'))
	{
		Movement::nextWeapon();
		m_Input->KeyUp('Q');
	}

	if (m_Input->IsKeyDown('R'))
	{
		Movement::reload();
		m_Input->KeyUp('R');
	}

	if (m_Input->IsKeyDown('O'))
	{
		Movement::changeOriention();
		m_Input->KeyUp('O');
	}

	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		exit(0);
	}

	if (m_Input->IsKeyDown('W'))
	{
		if (m_Input->IsKeyDown(VK_SHIFT))
		{
			Movement::sprint_forward();
		}
		else
		{
			Movement::move_forward();
		}
	}

	if (m_Input->IsKeyDown('A'))
	{
		Movement::move_left();
	}

	if (m_Input->IsKeyDown('D'))
	{
		Movement::move_right();
	}

	if (m_Input->IsKeyDown('S'))
	{
		Movement::move_backward();
	}
	if (m_Input->IsKeyDown(VK_F1))
	{
		Timer::toggleSlowMotion();
		m_Input->KeyUp(VK_F1);
	}
	if (m_Input->IsKeyDown(VK_F5))
	{
		Movement::refreshScene();
		m_Input->KeyUp(VK_F5);
	}
	if (recoildiffer > 0)
	{
		float temp = Movement::getWeapons()[Movement::getEquiped()]->getRecoil() * 4;
		recoildiffer -= temp*Timer::getDifference();
		Movement::setRecoilDiffer(recoildiffer);
		Movement::look_pitch(temp);
	}
	else if (recoildiffer < 0)
	{
		recoildiffer = 0;
	}

	if (mouselup)
	{
		shotcounter = 0;
	}


	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}
	mouselup = false;
	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenHeight = screenH;
		screenWidth = screenW;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, (LPCSTR)m_applicationName, (LPCSTR)m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass((LPCSTR)m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

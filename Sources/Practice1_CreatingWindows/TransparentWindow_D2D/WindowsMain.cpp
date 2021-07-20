#include "WindowsMain.h"
#include <windowsx.h>
#include "DPI.h"
#include "MouseEventManager.h"

// ========================================================================================================

#pragma region Windows Events

int WindowsMain::OnWindowsCreate()
{
	MouseEventManager::m_dispatcher.set_dispatcher(m_renderer.m_dispatcher);
	m_program.SetUpdatePointer(m_renderer.GetNeedUpdate());
	m_program.m_dispatcher.set_dispatcher(m_renderer.m_dispatcher);
	//m_program.INI();
	m_renderer.AddGraphic(&m_program);
	m_renderer.event_UpdateHolder.Add(&m_program, &program::Update);
	m_renderer.event_INI.AddEvent(&m_program, &program::INI);
	m_renderer.PreInitialize(static_cast<int>(m_windowStyle), m_hwnd);
	return 0;
	//return m_renderer.Initialize(static_cast<int>(m_windowStyle), m_hwnd);
}

void WindowsMain::OnWindowsClose()
{
	m_renderer.WindowClosing();
}

void WindowsMain::OnWindowsDestroy()
{
	//m_renderer.DestroyRenderer();
	m_renderer.MT_RenderStop();
}

void WindowsMain::OnWindowsResize()
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	//D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
	m_renderer.ResizeT(rc.right, rc.bottom);
}

#pragma endregion

// ========================================================================================================

#pragma region Input Events

void WindowsMain::OnKeyDown(UINT Key)
{
	if (Key == 0x41) // a
	{
		++keyHolder;
	}
	if (Key == VK_TAB)
	{
		++keyHolder;
	}
}

void WindowsMain::OnKeyUp(UINT Key)
{
	// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	if (Key == 0x41) // a
	{
		std::wstring text = L"BLA x " + std::to_wstring(keyHolder);
		SetWindowText(m_hwnd, text.c_str());
	}
	if (Key == VK_TAB)
	{
		std::wstring text = L"TAB x " + std::to_wstring(keyHolder);
		SetWindowText(m_hwnd, text.c_str());
	}
	if (Key == VK_ESCAPE)
	{
		std::wstring text = L"ESCAPE x " + std::to_wstring(keyHolder);
		SetWindowText(m_hwnd, text.c_str());
		/*if (MessageBox(m_hwnd, L"Naozaj skoncit?", L"Moja aplikacia", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(m_hwnd);
		}*/
	}
	keyHolder = 0;
}

void WindowsMain::OnLeftMouseButtonDown(int x, int y, DWORD flag)
{
	
}

void WindowsMain::OnRightMouseButtonDown(int x, int y, DWORD flag)
{
}

void WindowsMain::OnLeftMouseButtonUp(int x, int y, DWORD flag)
{
}

void WindowsMain::OnRightMouseButtonUp(int x, int y, DWORD flag)
{
}

void WindowsMain::OnMouseMove(int x, int y, DWORD flag)
{
	MouseEventManager::MouseActive();
	MouseEventManager::SetMousePos(x, y);
	//m_renderer.UpdateMouseMove(x, y); // stare nepouzivat
}

void WindowsMain::OnMouseLeave()
{
	MouseEventManager::MouseLeave();
}

#pragma endregion

// ========================================================================================================

WindowsMain::WindowsMain()
{
	LPWSTR cursor = IDC_ARROW;
	hCursor = LoadCursor(NULL, cursor);
	SetCursor(hCursor);

	keyHolder = 0;
}

void WindowsMain::SetOptions(RenderOptionsManager rom)
{
	m_renderer.SetOptions(rom);
}

void WindowsMain::Render()
{
	//m_renderer.Render();
	m_renderer.MT_RenderStart();
}

void WindowsMain::Update()
{
	m_program.Update();
}

#pragma region Windows Create

BOOL WindowsMain::Create(std::wstring name)
{
	return Create(name, WindowsStyle::Base
		, static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT)
		, static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT)
	);
}

BOOL WindowsMain::Create(std::wstring name, int width, int height)
{
	return Create(name, WindowsStyle::Base
		, static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT)
		, width, height
	);
}

BOOL WindowsMain::Create(std::wstring name, WindowsStyle style)
{
	return Create(name, style
		, static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT)
		, static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT)
	);
}

BOOL WindowsMain::Create(std::wstring name, WindowsStyle style, int width, int height)
{
	return Create(name, style
		, width, height
		, static_cast<int>(CW_USEDEFAULT), static_cast<int>(CW_USEDEFAULT)
	);
}

BOOL WindowsMain::Create(std::wstring name, WindowsStyle style, int positionLeft, int positionTop, int width, int height)
{
	DWORD baseStyle = WS_OVERLAPPEDWINDOW;
	DWORD exStyle = WS_EX_APPWINDOW;

	m_windowStyle = style;

	switch (style)
	{
	case WindowsStyle::Border: 
	{
		baseStyle = WS_BORDER; 
		break;
	}
	case WindowsStyle::NoStyle: 
	{
		baseStyle = WS_POPUP; 
		if (width == static_cast<int>(CW_USEDEFAULT)) width = 720;
		if (height == static_cast<int>(CW_USEDEFAULT)) height = 450;
		break;
	}
	case WindowsStyle::Transparent: 
	{
		baseStyle = WS_POPUP;
		exStyle = WS_EX_LAYERED;
		if (width == static_cast<int>(CW_USEDEFAULT)) width = 720;
		if (height == static_cast<int>(CW_USEDEFAULT)) height = 450;
		break;
	}
	}

	return this->CreateInternal(
		name.c_str()
		, baseStyle
		, exStyle
		, positionLeft
		, positionTop
		, width
		, height
	);
}

#pragma endregion

PCWSTR WindowsMain::ClassName() const
{
	return L"WindowsMain";
}

// ========================================================================================================

LRESULT WindowsMain::HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	
	switch (Msg)
	{
	case WM_CREATE:
	{
		return OnWindowsCreate();
	}
	break;
	case WM_CLOSE:
	{
		OnWindowsClose();
		break;
	}
	case WM_SETCURSOR:
	{
		if (LOWORD(lParam) == HTCLIENT)
		{
			//HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
			SetCursor(hCursor);
			return TRUE;
		}
	}
	break;
	case WM_DESTROY:
	{
		OnWindowsDestroy();
		PostQuitMessage(0);
		return 0;
	}
	break;
	case WM_PAINT:
	{
		
		return 0;
	}
	break;
	case WM_SIZE:
	{
		OnWindowsResize();
		return 0;
	}
	case WM_KEYUP:
	{
		OnKeyUp(static_cast<UINT>(wParam));
		return 0;
	}
	break;
	case WM_KEYDOWN:
	{
		OnKeyDown((UINT)wParam);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		OnLeftMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<DWORD>(wParam));
		return 0;
	}
	break;
	case WM_LBUTTONUP:
	{
		OnLeftMouseButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<DWORD>(wParam));
		return 0;
	}
	break;
	case WM_MOUSEMOVE:
	{
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<DWORD>(wParam));
		return 0;
	}
	break;
	case WM_MOUSELEAVE:
	{
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<DWORD>(wParam));
		return 0;
	}
	break;
	break;
	}

	return DefWindowProc(m_hwnd, Msg, wParam, lParam);
}

// ========================================================================================================


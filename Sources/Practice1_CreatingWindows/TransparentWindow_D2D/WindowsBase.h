#pragma once
#include <Windows.h>

template <typename DERIVED_TYPE>
class WindowsBase
{
protected:
	HWND m_hwnd;

	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam) = 0;
public:
	WindowsBase();
	HWND Window() const;
protected:
	BOOL CreateInternal(
		PCWSTR lpWindowName
		, DWORD dwStyle
		, DWORD dwExStyle = 0
		, int x = static_cast<int>(CW_USEDEFAULT)
		, int y = static_cast<int>(CW_USEDEFAULT)
		, int nWidth = static_cast<int>(CW_USEDEFAULT)
		, int nHeight = static_cast<int>(CW_USEDEFAULT)
		, HWND hwndParent = 0
		, HMENU hMenu = 0
	);
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

// ========================================================================================================

template<typename DERIVED_TYPE>
inline WindowsBase<DERIVED_TYPE>::WindowsBase()
	: m_hwnd(NULL)
{
}

// --------------------------------------------------------------------------------------------------------

template<typename DERIVED_TYPE>
inline HWND WindowsBase<DERIVED_TYPE>::Window() const
{
	return m_hwnd;
}

// --------------------------------------------------------------------------------------------------------

template<typename DERIVED_TYPE>
inline BOOL WindowsBase<DERIVED_TYPE>::CreateInternal(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hwndParent, HMENU hMenu)
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc; // DERIVED_TYPE::
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();
	wc.hbrBackground = NULL;

	RegisterClass(&wc);

	m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight,
		hwndParent, hMenu, GetModuleHandle(NULL), this);

	return (m_hwnd ? TRUE : FALSE);
}

// --------------------------------------------------------------------------------------------------------

template<typename DERIVED_TYPE>
inline LRESULT WindowsBase<DERIVED_TYPE>::WindowProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	DERIVED_TYPE* pThis = NULL;

	if (Msg == WM_CREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->m_hwnd = hwnd;
	}
	else
	{
		pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		//pThis = static_cast<DERIVED_TYPE*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->HandleMessage(Msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, Msg, wParam, lParam);
	}
}

// --------------------------------------------------------------------------------------------------------

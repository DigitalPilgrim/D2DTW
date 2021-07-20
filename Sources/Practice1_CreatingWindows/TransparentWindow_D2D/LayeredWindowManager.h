#pragma once

#include <Windows.h>
#include <string>

class LayeredWindowManager
{
	POINT m_sourcePosition;
	POINT m_windowPosition;
	SIZE m_size;
	BLENDFUNCTION m_blend;
	UPDATELAYEREDWINDOWINFO m_info;

public:
	LayeredWindowManager(
		unsigned int width = 0
		, unsigned int height = 0
	)
		: m_sourcePosition()
		, m_windowPosition()
		, m_blend()
		, m_info()
	{
		m_size = { static_cast<LONG>(width), static_cast<LONG>(height) };

		m_info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
		m_info.pptSrc = &m_sourcePosition;
		m_info.pptDst = &m_windowPosition;
		m_info.psize = &m_size;
		m_info.pblend = &m_blend;
		m_info.dwFlags = ULW_ALPHA;

		m_blend.SourceConstantAlpha = 255;
		m_blend.AlphaFormat = AC_SRC_ALPHA;
		//m_blend.BlendOp = AC_SRC_OVER;
	}

	void Update(HWND window, HDC source)
	{
		m_info.hdcSrc = source;

		if (UpdateLayeredWindowIndirect(window, &m_info) == FALSE)
		{
			/*DWORD e = GetLastError();
			std::wstring text = L"UpdateLayeredWindowIndirect() Failed\nError number " + std::to_wstring(e);
			MessageBox(NULL, text.c_str(), L"ERROR", MB_OK);*/
		}
	}

	void SetSize(unsigned int width, unsigned int height)
	{
		m_size.cx = static_cast<LONG>(width);
		m_size.cy = static_cast<LONG>(height);
	}

	void SetWindowPosition(int X, int Y)
	{
		m_windowPosition.x = static_cast<LONG>(X);
		m_windowPosition.y = static_cast<LONG>(Y);
	}

	void SetSourcePosition(int X, int Y)
	{
		m_sourcePosition.x = static_cast<LONG>(X);
		m_sourcePosition.y = static_cast<LONG>(Y);
	}

	unsigned int GetWidth() const { return static_cast<unsigned int>(m_size.cx); }

	unsigned int GetHeight() const { return static_cast<unsigned int>(m_size.cy); }
};
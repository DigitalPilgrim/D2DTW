#pragma once

#include "WindowsBase.h"
#include <string>

#include "RendererD2D.h"
#include "ObjectHolder.h"
#include "program.h"
#include "RenderOptions.h"

enum class WindowsStyle
{
	Base
	, NoStyle
	, Border
	, Transparent
};

class WindowsMain : public WindowsBase<WindowsMain>
{
	HCURSOR hCursor;

	RendererD2D m_renderer;
	program m_program;

	int OnWindowsCreate();
	void OnWindowsClose();
	void OnWindowsDestroy();
	void OnWindowsResize();

	void OnKeyDown(UINT Key);
	void OnKeyUp(UINT Key);
	void OnLeftMouseButtonDown(int x, int y, DWORD flag);
	void OnRightMouseButtonDown(int x, int y, DWORD flag);
	void OnLeftMouseButtonUp(int x, int y, DWORD flag);
	void OnRightMouseButtonUp(int x, int y, DWORD flag);
	void OnMouseMove(int x, int y, DWORD flag);
	void OnMouseLeave();

	WindowsStyle m_windowStyle;

	int keyHolder;
public:
	WindowsMain();

	void SetOptions(RenderOptionsManager rom);

	void Render();
	void Update();

	BOOL Create(std::wstring name);
	BOOL Create(std::wstring name, int width, int height);
	BOOL Create(std::wstring name, WindowsStyle style);
	BOOL Create(std::wstring name, WindowsStyle style, int width, int height);
	BOOL Create(std::wstring name, WindowsStyle style, int positionLeft, int positionTop, int width, int height);

	PCWSTR ClassName() const override;
	LRESULT HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam) override;
};
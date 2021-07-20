#pragma once
#pragma warning(disable : 4996)
#include <d2d1.h>
#include <d3d10misc.h>
#include <d3d10_1.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "D3D10")
#pragma comment(lib, "D3D10_1")

//#include <wincodec.h>
//#pragma comment(lib, "windowscodecs")
//Dwmapi.lib
//windowscodecs.lib

#include "GraphicObject.h"
#include "ObjectHolder.h"
#include "RenderOptions.h"
#include "LayeredWindowManager.h"
#include "WICHelper.h"
#include <list>
#include <thread>
#include <mutex>
#include "UpdateHolder.h"

#include <dwrite.h>
#pragma comment(lib, "dwrite")

#include <meaof_dispatcher.h>
#include <meaof_thread.h>
#include <meaof_event.h>

class RendererD2D
{
	ID2D1Factory* m_factory;
	ID2D1Factory* get_factory() const { return m_factory; }

	IDWriteFactory* m_DWriteFactory;

	ID2D1HwndRenderTarget* m_hwndRenderTarget;
	ID2D1HwndRenderTarget* get_hwndRenderTarget() const { return m_hwndRenderTarget; }

	ID2D1RenderTarget* m_renderTarget;
	ID2D1RenderTarget* get_renderTarget() const { return m_renderTarget; }
	ID2D1GdiInteropRenderTarget* m_gdiInteropRenderTarget;
	ID2D1GdiInteropRenderTarget* get_gdiInteropRenderTarget() const { return m_gdiInteropRenderTarget; }

	ID3D10Device1* m_DX10device;
	ID3D10Texture2D* m_DX10texture;
	IDXGISurface * m_DXGIsurface;

	LayeredWindowManager m_lwm;
	LayeredWindowManager get_lwm() const { return m_lwm; }
	WICHelper m_wic;
	WICHelper get_wic() const { return m_wic; }

	ID2D1SolidColorBrush* m_colorBrush;
	//ID2D1LinearGradientBrush* m_linearGradientBrush;

	D2D1_POINT_2F m_mouse_point;
	D2D1_SIZE_U m_window_size;

	HWND m_hwnd;

	ObjectRenderHolder* m_objects_render_holder;
	RenderOptionsManager Options;

	HRESULT hrCreateResources;
	HRESULT hrRender;
	int returnInitialized;
	D2D1_FACTORY_TYPE multithreadingInitialize;

	bool m_needUpdate;
	bool m_windowClosing;
	int m_windowStyle;
	bool m_IsRendering;

	std::thread m_th_render;
	std::mutex MX_RENDER;

	meaof::thread_control m_thread;
public:
	meaof::dispatcher m_dispatcher;
	UpdateHolder event_UpdateHolder;
	meaof::event_manager_simple event_INI;

public:
	RendererD2D();
	RendererD2D(const RendererD2D& r);
	~RendererD2D();

	void SetOptions(RenderOptionsManager rom);

	void PreInitialize(int windowStyle, HWND hwnd);
	int Initialize(int windowStyle, HWND hwnd);
	HRESULT CreateDWrite();
	HRESULT CreateResources();
	void DiscardResources();
	void DestroyRenderer();

	void WindowClosing();

	void AddGraphic(ObjectRenderHolder* obj);

	void MT_RenderStart();
	void MT_RenderStop();
private:
	void MT_Render();
public:

	void Render();
	void Update();
	void Resize(meaof::obj_args& args);
	void ResizeT(UINT32 width = 0.0f, UINT32 height = 0.0f);

	bool* GetNeedUpdate() { return &m_needUpdate; }

	void UpdateMouseMove(int x, int y);
	//void UpdateFocus();
	//void Update..()
private:
	void InitializeD2DHWND();
	void InitializeD2DWIC();
	void InitializeD3DDGXI();
	void CreateResourcesD2DHWND();
	void CreateResourcesD2DWIC();
	void CreateResourcesD3DDGXI();
	HRESULT CreateOtherResources(ID2D1RenderTarget* render, ID2D1Factory* factory);
	void RenderD2DHWND();
	void RenderD2DWIC();
	void RenderD3DDGXI();
	void ResizeD2DHWND();
	void ResizeD2DWIC();
	void ResizeD3DDGXI();
	void OnRender(ID2D1RenderTarget* render, ID2D1SolidColorBrush * scb = NULL);
	HRESULT OnCreateResources(ID2D1RenderTarget* render);
	void OnDiscardResources();
	void CallFromOptions(//RendererD2D* render
		void(RendererD2D::*functionD2D)(void)
		, void(RendererD2D::*functionD2DWIC)(void)
		, void(RendererD2D::*functionD3DDGXI)(void)
		, void(RendererD2D::* functionD2DTransparent)(void) = nullptr
		, void(RendererD2D::* functionD2DWICTransparent)(void) = nullptr
		, void(RendererD2D::* functionD3DDGXITransparent)(void) = nullptr
	);
public:
	RendererD2D& operator=(const RendererD2D& r);
};
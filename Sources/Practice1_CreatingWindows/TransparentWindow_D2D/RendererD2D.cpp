#include "RendererD2D.h"
#include "SafeRelease.h"
#include "WindowsMain.h"
#include "WICRenderTarget.h"
#include "DPI.h"
#include "MouseEventManager.h"

RendererD2D::RendererD2D()
	: m_factory(NULL)
	, m_gdiInteropRenderTarget(NULL)
	, m_hwndRenderTarget(NULL)
	, m_renderTarget(NULL)
	, m_needUpdate(false)
	, m_windowClosing(false)
	, m_windowStyle(0)
	, m_mouse_point()
	, m_objects_render_holder(nullptr)
	, m_IsRendering(false)
	, m_DX10device(NULL)
{
	
}

RendererD2D::RendererD2D(const RendererD2D& r)
	: m_factory(NULL)
	, m_gdiInteropRenderTarget(NULL)
	, m_hwndRenderTarget(NULL)
	, m_renderTarget(NULL)
	, m_needUpdate(false)
	, m_windowClosing(false)
	, m_windowStyle(0)
	, m_mouse_point()
	, m_objects_render_holder(nullptr)
	, m_IsRendering(false)
	, m_DX10device(NULL)
	, m_DX10texture(NULL)
	, m_DXGIsurface(NULL)
{
	// toto je vytvorene kvolu m_dispatcher a vkladanie objektu, inak vyhodi error
}

RendererD2D::~RendererD2D()
{
	DestroyRenderer();
}

void RendererD2D::SetOptions(RenderOptionsManager rom)
{
	Options = rom;
}

void RendererD2D::PreInitialize(int windowStyle, HWND hwnd)
{
	m_windowStyle = windowStyle;
	m_hwnd = hwnd;
}

int RendererD2D::Initialize(int windowStyle, HWND hwnd)
{
	returnInitialized = 0;
	m_windowStyle = windowStyle;
	m_hwnd = hwnd;

	multithreadingInitialize = D2D1_FACTORY_TYPE_SINGLE_THREADED;
	switch (Options.Multithreading)
	{
	case RenderMultithreading::MULTI: multithreadingInitialize = D2D1_FACTORY_TYPE_MULTI_THREADED;
	}
	CallFromOptions(// Non Transparent Window
		&RendererD2D::InitializeD2DHWND,
		&RendererD2D::InitializeD2DWIC,
		&RendererD2D::InitializeD3DDGXI
		// Transparent Window
		/*&RendererD2D::InitializeD2DHWND,
		&RendererD2D::InitializeD2DWIC,
		&RendererD2D::InitializeD3DDGXI*/
	);
	return returnInitialized;
}

HRESULT RendererD2D::CreateDWrite()
{
	return DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED
		, __uuidof(IDWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_DWriteFactory)
	);
}

HRESULT RendererD2D::CreateResources()
{
	hrCreateResources = S_OK;
	
	CallFromOptions(// Non Transparent Window
		&RendererD2D::CreateResourcesD2DHWND,
		&RendererD2D::CreateResourcesD2DWIC,
		&RendererD2D::CreateResourcesD3DDGXI
		// Transparent Window
		/*&RendererD2D::CreateResourcesD2DWIC,
		&RendererD2D::CreateResourcesD2DWIC,
		&RendererD2D::CreateResourcesD3DDGXI*/
	);

	return hrCreateResources;
}

void RendererD2D::DiscardResources()
{
	OnDiscardResources();
	SafeRelease(&m_colorBrush);
	SafeRelease(&m_hwndRenderTarget);
	SafeRelease(&m_renderTarget);
	SafeRelease(&m_gdiInteropRenderTarget);
	SafeRelease(&m_DX10texture);
	SafeRelease(&m_DXGIsurface);
}

void RendererD2D::DestroyRenderer()
{
	//MT_RenderStop();
	m_objects_render_holder = nullptr;
	DiscardResources();
	SafeRelease(&m_factory);
	SafeRelease(&m_DX10device);
}

void RendererD2D::WindowClosing()
{
	m_windowClosing = true;
}

void RendererD2D::AddGraphic(ObjectRenderHolder* obj)
{
	m_objects_render_holder = obj;
}

void RendererD2D::MT_RenderStart()
{
	m_thread.create(this, &RendererD2D::MT_Render);
	/*if (!m_th_render.joinable())
	{
		m_th_render = std::thread(&RendererD2D::MT_Render, this);
	}*/
}

void RendererD2D::MT_RenderStop()
{
	m_dispatcher.invoke_shutdown();
	m_thread.join();
}

void RendererD2D::MT_Render()
{
	m_IsRendering = true;
	bool RUN = true;

	int ok = Initialize(static_cast<int>(m_windowStyle), m_hwnd);
	event_INI.RaiseEvent();
	m_dispatcher.resolve_thread_id();

	if (!SUCCEEDED(hrRender))
	{
		// ERROR
	}
	if (ok == 0)
	{
		m_dispatcher.run_step_start();
		while (RUN)
		{
			if (m_dispatcher.run_step() == false) break;
			event_UpdateHolder.Update();
			Render();
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}

		//DestroyRenderer();
	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::Render()
{
	if (m_windowClosing) return;
	//if (Options.Multithreading == RenderMultithreading::SINGLE)
	hrRender = CreateResources();
	if (SUCCEEDED(hrRender))
	{
		if (m_needUpdate)
		{

			MouseEventManager::CheckHit();

			CallFromOptions(
				&RendererD2D::RenderD2DHWND,
				&RendererD2D::RenderD2DWIC,
				&RendererD2D::RenderD3DDGXI
			);

			if (FAILED(hrRender) || hrRender == D2DERR_RECREATE_TARGET)
			{
				DiscardResources();
			}

			m_needUpdate = false;
		}

	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::Update()
{
	m_needUpdate = true;
}

// ----------------------------------------------------------------------------------------

void RendererD2D::Resize(meaof::obj_args& args)
{
	UINT32 width, height;
	args.get_object(width);
	args.get_object(height);
	m_window_size = D2D1::SizeU(width, height);
	CallFromOptions(
		&RendererD2D::ResizeD2DHWND,
		&RendererD2D::ResizeD2DWIC,
		&RendererD2D::ResizeD3DDGXI
	);

	Update();
}

void RendererD2D::ResizeT(UINT32 width, UINT32 height)
{
	meaof::obj_args args;
	args.AddObject(meaof::object(width));
	args.AddObject(meaof::object(height));

	if (Options.Multithreading == RenderMultithreading::MULTI)
	{
		Resize(args);
	}
	else
	{
		
		m_dispatcher.send_message(meaof::delegate_object(this, &RendererD2D::Resize, args));
	}
}

void RendererD2D::UpdateMouseMove(int x, int y)
{
	/*
		!!! STARE NEPOUZIVAT !!!
	*/
	D2D1_POINT_2F pixels = DPIScale::PixelToDips(x, y);
	//GraphicObject* go = nullptr;
	bool hit = false;
	std::list< GraphicObject*>::iterator it = m_objects_render_holder->GetList().end();
	--it;
	for (;; it--)
	{
		if ((*it)->HitTest(x, y))
		{
			//go = (*it);
			hit = true;
			break;
		}
		if (it == m_objects_render_holder->GetList().begin()) break;
	}
	/*for (GraphicObject* o : m_objects_render_holder->GetList())
	{
		if (o->HitTest(pixels.x, pixels.y))
		{
			go = o;
			break;
		}
	}*/
	if (hit) // go != nullptr
	{
		//MouseEventManager::ResolveHit(go);
		MouseEventManager::ResolveHitPre();
	}
	else
	{
		MouseEventManager::ResolveNoHitPre();
	}

}

// 
// ========================================================================================
// INITIALIZERS

void RendererD2D::InitializeD2DHWND()
{
	if (FAILED(D2D1CreateFactory(multithreadingInitialize, &m_factory)))
	{
		returnInitialized = -1;
	}
	else
	{
		DPIScale::Initialize(m_factory);

		HRESULT hr = CreateDWrite();
		returnInitialized = static_cast<int>(hr);
	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::InitializeD2DWIC()
{
	if (FAILED(D2D1CreateFactory(multithreadingInitialize, &m_factory)))
	{
		returnInitialized = -1;
	}
	else
	{
		DPIScale::Initialize(m_factory);
		HRESULT hr = CreateDWrite();
		returnInitialized = static_cast<int>(hr);
	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::InitializeD3DDGXI()
{
	if (FAILED(D2D1CreateFactory(multithreadingInitialize, &m_factory)))
	{
		returnInitialized = -1;
	}
	else
	{
		DPIScale::Initialize(m_factory);
		HRESULT hr = CreateDWrite();

		UINT driverLevel = 0;
		UINT level = 0;

		if (SUCCEEDED(hr))
		{
			const D3D10_DRIVER_TYPE driverType[] =
			{
				D3D10_DRIVER_TYPE_HARDWARE
				, D3D10_DRIVER_TYPE_WARP
			};

			const D3D10_FEATURE_LEVEL1 levelAttempts[] =
			{
				D3D10_FEATURE_LEVEL_10_0,
				D3D10_FEATURE_LEVEL_9_3,
				D3D10_FEATURE_LEVEL_9_2,
				D3D10_FEATURE_LEVEL_9_1,
			};

			for (; driverLevel < ARRAYSIZE(driverType); driverLevel++)
			{
				for (; level < ARRAYSIZE(levelAttempts); level++)
				{
					hr = D3D10CreateDevice1(
						0
						, driverType[driverLevel] //D3D10_DRIVER_TYPE::D3D10_DRIVER_TYPE_HARDWARE
						, 0
						, D3D10_CREATE_DEVICE_BGRA_SUPPORT
						, levelAttempts[level]
						, D3D10_1_SDK_VERSION
						, &m_DX10device
					);

					if (SUCCEEDED(hr))
					{
						break;
					}
				}

				if (SUCCEEDED(hr))
				{
					break;
				}
			}
		}
		returnInitialized = static_cast<int>(hr);
	}
}

// 
// ========================================================================================
// RESOURCES

void RendererD2D::CreateResourcesD2DHWND()
{
	if (m_hwndRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		hrCreateResources = m_factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT
				, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			)
			, D2D1::HwndRenderTargetProperties(m_hwnd, size
			/*, D2D1_PRESENT_OPTIONS_NONE | D2D1_PRESENT_OPTIONS_IMMEDIATELY*/) // , D2D1_PRESENT_OPTIONS_IMMEDIATELY
			, &m_hwndRenderTarget
		);

		CreateOtherResources(m_hwndRenderTarget, m_factory);
		
		hrCreateResources = OnCreateResources(m_hwndRenderTarget);
	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::CreateResourcesD2DWIC()
{
	if (m_renderTarget == NULL)
	{
		RECT rc;
		RECT rcPos;
		GetClientRect(m_hwnd, &rc);
		GetWindowRect(m_hwnd, &rcPos);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		m_lwm.SetSize(size.width, size.height);
		//m_lwm.SetSourcePosition(rc.left, rc.top);

		m_lwm.SetWindowPosition(rcPos.left, rcPos.top);
		//m_lwm.SetSourcePosition(rcPos.left, rcPos.top);

		m_wic.CreateBitmap(size.width, size.height);

		hrCreateResources = m_factory->CreateWicBitmapRenderTarget(
			m_wic.GetBitmap()
			, D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT
				, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
				, 0.0f, 0.0f
				, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
			)
			, &m_renderTarget
		);

		CreateOtherResources(m_renderTarget, m_factory);

		if (SUCCEEDED(hrCreateResources))
		{
			hrCreateResources = OnCreateResources(m_renderTarget);
			if (SUCCEEDED(hrCreateResources))
			{
				hrCreateResources = m_renderTarget->QueryInterface(&m_gdiInteropRenderTarget);
			}
		}
	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::CreateResourcesD3DDGXI()
{
	if (m_DX10texture == NULL)
	{

		RECT rc;
		RECT rcPos;
		GetClientRect(m_hwnd, &rc);
		GetWindowRect(m_hwnd, &rcPos);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		m_lwm.SetSize(size.width, size.height);
		//m_lwm.SetSourcePosition(rc.left, rc.top);

		m_lwm.SetWindowPosition(rcPos.left, rcPos.top);
		//m_lwm.SetSourcePosition(rcPos.left, rcPos.top);

		D3D10_TEXTURE2D_DESC description = {};
		description.ArraySize = 1;
		description.BindFlags = D3D10_BIND_RENDER_TARGET;
		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.Width = size.width;
		description.Height = size.height;
		description.MipLevels = 1;
		description.SampleDesc.Count = 1;
		description.MiscFlags = D3D10_RESOURCE_MISC_GDI_COMPATIBLE;
		
		hrCreateResources = m_DX10device->CreateTexture2D(
			&description,
			0, // no initial data
			&m_DX10texture);
		if (SUCCEEDED(hrCreateResources))
		{
			hrCreateResources = m_DX10texture->QueryInterface(&m_DXGIsurface);
			if (SUCCEEDED(hrCreateResources))
			{
				hrCreateResources = m_factory->CreateDxgiSurfaceRenderTarget(
					m_DXGIsurface
					, D2D1::RenderTargetProperties(
						D2D1_RENDER_TARGET_TYPE_DEFAULT
						, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
						, 0.0f, 0.0f
						, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
					)
					, &m_renderTarget
				);
				if (SUCCEEDED(hrCreateResources))
				{
					CreateOtherResources(m_renderTarget, m_factory);
					hrCreateResources = OnCreateResources(m_renderTarget);
					if (SUCCEEDED(hrCreateResources))
					{
						hrCreateResources = m_renderTarget->QueryInterface(&m_gdiInteropRenderTarget);
					} // SUCCEEDED OnCreateResources
				} // SUCCEEDED m_factory->CreateDxgiSurfaceRenderTarget
			} // SUCCEEDED m_DX10texture->QueryInterface
		} // SUCCEEDED m_DX10device->CreateTexture2D
	}
}

// ----------------------------------------------------------------------------------------

HRESULT RendererD2D::CreateOtherResources(ID2D1RenderTarget* render, ID2D1Factory* factory)
{
	if (SUCCEEDED(hrCreateResources))
	{
		hrCreateResources = render->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_colorBrush);
	}
	return hrCreateResources;
}

// RESOURCES
// ========================================================================================
// RENDERING

void RendererD2D::RenderD2DHWND()
{
	m_hwndRenderTarget->BeginDraw();
	m_hwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_hwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
	OnRender(m_hwndRenderTarget, m_colorBrush);
	hrRender = m_hwndRenderTarget->EndDraw();
	
}

// ----------------------------------------------------------------------------------------

void RendererD2D::RenderD2DWIC()
{
	m_renderTarget->BeginDraw();
	m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_renderTarget->Clear(D2D1::ColorF(0, 0.0f));
	OnRender(m_renderTarget, m_colorBrush);
	{
		WICRenderTarget wrt(m_gdiInteropRenderTarget);
		m_lwm.Update(m_hwnd, wrt);
	}

	hrRender = m_renderTarget->EndDraw();
}

// ----------------------------------------------------------------------------------------

void RendererD2D::RenderD3DDGXI()
{
	m_renderTarget->BeginDraw();
	m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_renderTarget->Clear(D2D1::ColorF(0, 0.0f));
	OnRender(m_renderTarget, m_colorBrush);
	{
		WICRenderTarget wrt(m_gdiInteropRenderTarget);
		m_lwm.Update(m_hwnd, wrt);
	}

	hrRender = m_renderTarget->EndDraw();
}

// RENDERING
// ========================================================================================
// RESIZING

void RendererD2D::ResizeD2DHWND()
{
	/*RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);*/
	if (m_hwndRenderTarget != NULL)
	{
		m_hwndRenderTarget->Resize(m_window_size);
	}
}

// ----------------------------------------------------------------------------------------

void RendererD2D::ResizeD2DWIC()
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
	RECT rcPos;
	GetWindowRect(m_hwnd, &rcPos);

	m_lwm.SetSize(size.width, size.height);
	m_lwm.SetWindowPosition(rcPos.left, rcPos.top);
}

// ----------------------------------------------------------------------------------------

void RendererD2D::ResizeD3DDGXI()
{
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
	RECT rcPos;
	GetWindowRect(m_hwnd, &rcPos);

	m_lwm.SetSize(size.width, size.height);
	m_lwm.SetWindowPosition(rcPos.left, rcPos.top);
}

// RENDERING
// ========================================================================================
// 

void RendererD2D::OnRender(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	if (m_objects_render_holder != nullptr)
		m_objects_render_holder->Render(render, scb);
}

// ----------------------------------------------------------------------------------------

HRESULT RendererD2D::OnCreateResources(ID2D1RenderTarget* render)
{
	HRESULT hr = S_OK;
	if (m_objects_render_holder != nullptr)
		hr = m_objects_render_holder->CreateResources(render, m_factory, m_DWriteFactory);
	return hr;
}

// ----------------------------------------------------------------------------------------

void RendererD2D::OnDiscardResources()
{
	if (m_objects_render_holder != nullptr)
		m_objects_render_holder->DiscardResources();
}

// RENDERING
// ========================================================================================
//

void RendererD2D::CallFromOptions(
	//RendererD2D* render
	void(RendererD2D::* functionD2D)(void)
	, void(RendererD2D::* functionD2DWIC)(void)
	, void(RendererD2D::* functionD3DDGXI)(void)
	, void(RendererD2D::* functionD2DTransparent)(void)
	, void(RendererD2D::* functionD2DWICTransparent)(void)
	, void(RendererD2D::* functionD3DDGXITransparent)(void))
{

	if (functionD2DTransparent == nullptr) functionD2DTransparent = functionD3DDGXI;
	if (functionD2DWICTransparent == nullptr) functionD2DWICTransparent = functionD2DWIC;
	if (functionD3DDGXITransparent == nullptr) functionD3DDGXITransparent = functionD3DDGXI;

	switch (static_cast<WindowsStyle>(m_windowStyle))
	{
		case WindowsStyle::Base:
		case WindowsStyle::Border:
			//case WindowsStyle::TransparentHwnd:
		case WindowsStyle::NoStyle:
		{
			switch (Options.Type)
			{
				case RenderOptions::D2D: 
				{
					(this->*functionD2D)();
					break;
				}
				case RenderOptions::D2DWIC:
				{
					(this->*functionD2DWIC)();
					break;
				}
				case RenderOptions::D3DDXGI:
				{
					(this->*functionD3DDGXI)();
					break;
				}
			}
			break;
		}
		case WindowsStyle::Transparent:
		{
			switch (Options.Type)
			{
				case RenderOptions::D2D:
				{
					(this->*functionD2DTransparent)();
					break;
				}
				case RenderOptions::D2DWIC:
				{
					(this->*functionD2DWICTransparent)();
					break;
				}
				case RenderOptions::D3DDXGI:
				{
					(this->*functionD3DDGXITransparent)();
					break;
				}
			}
			break;
		}
	} // switch (static_cast<WindowsStyle>(m_windowStyle))
}

RendererD2D& RendererD2D::operator=(const RendererD2D& r)
{
		if (this != &r)
		{
			// toto je tu vytvorene kvoli m_dispatcher a vkladanie objektu don
		}
		return *this;
}
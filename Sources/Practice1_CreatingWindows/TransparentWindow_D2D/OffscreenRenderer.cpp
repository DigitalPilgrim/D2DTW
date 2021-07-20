#include "OffscreenRenderer.h"
#include "RendererD2D.h"
#include "SafeRelease.h"

OffscreenRenderer::OffscreenRenderer()
	: m_Bitmap(NULL)
	, m_BitmapRenderTarget(NULL)
	, m_renderTarget(NULL)
	, m_BitMapFilled(false)
	, m_NeedUpdate(false)
{
}

OffscreenRenderer::~OffscreenRenderer()
{
	SafeRelease(&m_BitmapRenderTarget);
	SafeRelease(&m_Bitmap);
}

ID2D1BitmapRenderTarget* OffscreenRenderer::Render()
{
	return m_BitmapRenderTarget;
}

bool OffscreenRenderer::IsCashed() const
{
	return m_BitMapFilled;
}

bool OffscreenRenderer::NeedUpdate() const
{
	return m_NeedUpdate;
}

void OffscreenRenderer::NeedUpdate(bool value)
{
	m_NeedUpdate = value;
}

const ID2D1Bitmap* OffscreenRenderer::GetBitmap() const
{
	return m_Bitmap;
}

bool OffscreenRenderer::CreateRender(D2D1_SIZE_F desiredSize)
{
	DiscardResources();
	m_BitMapFilled = false;

	if (SUCCEEDED(m_renderTarget->CreateCompatibleRenderTarget(desiredSize, &m_BitmapRenderTarget)))
	{
		return true;
	}
	return false;
}

bool OffscreenRenderer::CreateBitmap()
{
	m_BitMapFilled = false;
	SafeRelease(&m_Bitmap);
	if (SUCCEEDED(m_BitmapRenderTarget->GetBitmap(&m_Bitmap)))
	{
		m_BitMapFilled = true;
		return true;
	}
	return false;
}

void OffscreenRenderer::Render(D2D1_RECT_F& destination)
{
	m_renderTarget->DrawBitmap(m_Bitmap
		//, destination
		, D2D1::RectF(0.0f, 0.0f, destination.right, destination.bottom)
		, 1.0f
		, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	);
}

void OffscreenRenderer::Render(D2D1_RECT_F& destination, D2D1_RECT_F& source)
{
	m_renderTarget->DrawBitmap(m_Bitmap
		, D2D1::RectF(0.0f, 0.0f, destination.right, destination.bottom)
		, 1.0f
		, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		, source
	);
}

void OffscreenRenderer::CreateResources(ID2D1RenderTarget* render)
{
	m_renderTarget = render;
	m_NeedUpdate = true;
}

void OffscreenRenderer::DiscardResources()
{
	SafeRelease(&m_BitmapRenderTarget);
	SafeRelease(&m_Bitmap);
}

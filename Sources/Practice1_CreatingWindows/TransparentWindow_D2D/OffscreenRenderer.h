#pragma once
#include <d2d1.h>

class RendererD2D;

class OffscreenRenderer
{
	ID2D1RenderTarget* m_renderTarget;
	ID2D1BitmapRenderTarget* m_BitmapRenderTarget;
	ID2D1Bitmap* m_Bitmap;

	bool m_BitMapFilled;
	bool m_NeedUpdate;
public:
	OffscreenRenderer();
	~OffscreenRenderer();
	ID2D1BitmapRenderTarget* Render();
	bool IsCashed() const;
	bool NeedUpdate() const;
	void NeedUpdate(bool value);
	const ID2D1Bitmap* GetBitmap() const;

	bool CreateRender(D2D1_SIZE_F desiredSize);
	bool CreateBitmap();
	void Render(D2D1_RECT_F& destination);
	void Render(D2D1_RECT_F& destination, D2D1_RECT_F& source);

	void CreateResources(ID2D1RenderTarget* render);
	void DiscardResources();
};
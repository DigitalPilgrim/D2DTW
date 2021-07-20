#pragma once

# include <d2d1.h>
#include <dwrite.h>
#include <list>
#include <meaof_dispatcher.h>
#include "Events.h"
#include "OffscreenRenderer.h"

class GraphicObject : public IEvents
{
protected:

	D2D1_COLOR_F m_color;
	D2D1_COLOR_F m_colorStroke;
	ID2D1SolidColorBrush* m_colorBrush;

	D2D1::Matrix3x2F m_transport;
	D2D1::Matrix3x2F m_mxPos;
	D2D1::Matrix3x2F m_mxRot;
	D2D1::Matrix3x2F m_mxSkew;
	D2D1::Matrix3x2F m_mxScale;
	D2D1_POINT_2F m_size;
	D2D1_SIZE_F m_scale;
	D2D1_POINT_2F m_pos;
	float m_angle;

	// clip area
	D2D1_RECT_F m_area;
	// absolute clip area
	D2D1_RECT_F m_AClipArea;
	// objekt na zohladnenie rodicovkej oblasti
	D2D1_RECT_F m_cp_area;
	bool m_cp_area_update;

	bool m_RenderClipArea;
	bool m_RenderAClipArea;
	bool m_RenderCPArea;

	ID2D1Factory* m_factory;
	ID2D1RectangleGeometry* m_mouse_detector;
	OffscreenRenderer m_oRender;

	std::list< GraphicObject*> m_childrens;
	GraphicObject* m_parent;

	bool m_Update;
	bool m_UpdateColor;
	bool m_UpdateColorStroke;
	bool m_UpdateSize;
	bool m_UpdatePos;
	bool m_optimized;

	bool m_IsClip;
	bool m_IsHitTestVisible;

	HRESULT CreateGraphicObjectResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text = NULL);
	void DiscardGraphicObjectResources();

	void SetTransform(ID2D1RenderTarget* render);
	void FinishTransform(ID2D1RenderTarget* render);

	HRESULT m_result;

public:
	GraphicObject();
	/*GraphicObject(const GraphicObject& g);*/
	virtual ~GraphicObject() {}

	meaof::dispatcher_holder m_dispatcher;

	virtual void Update() {}
protected:
	virtual void Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) {}; // = 0;
public:
	virtual HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text = NULL) { return 0; }; // = 0; // , ID2D1Factory * facotry
	virtual void DiscardResources() {}; // = 0;
	virtual bool HitTest(float x, float y) { return false; }

	bool HitTestG(bool HitTrue, float x, float y, bool HitArea);

	virtual void DrawObject(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL);
protected:
	void DrawHelpers(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL);
public:

	virtual void Size(D2D1_POINT_2F size);
	virtual D2D1_POINT_2F Size() const;
	virtual void Position(D2D1_POINT_2F position);
	virtual D2D1_POINT_2F Position() const;
	virtual void Color(D2D1_COLOR_F color);
	virtual D2D1_COLOR_F Color() const;

	virtual void Rotate(float angle);
	virtual void Rotate(float angle, D2D_POINT_2F origin);
	virtual void Translate(D2D1_POINT_2F value);
	virtual void Scale(D2D1_POINT_2F value);
	virtual void Scale(D2D1_POINT_2F value, D2D_POINT_2F origin);
	virtual D2D1_SIZE_F Scale();
	virtual float Rotate(bool ReturnAngle = true);
	virtual D2D1_POINT_2F Translate();
	//virtual D2D1::Matrix3x2F Transformations();

	void AddChild(GraphicObject* o);
	std::list<GraphicObject*>& GetChildrens() { return m_childrens; }

	void SetHitTestVisible(bool TrueVisible);
	bool IsHitTestVisible(float x, float y) const;

	bool IsCliped() const { return m_IsClip; }
	void SetIsCliped(bool TrueIsClip) { m_IsClip = TrueIsClip; }

	void SetOptimized(bool OptimizedTrue) { m_optimized = true; }

	D2D1_RECT_F GetAbsoluteClipArea() const;

	void SetRenderClipArea(bool RenderTrue) { m_RenderClipArea = RenderTrue; }
	void SetRenderAClipArea(bool RenderTrue) { m_RenderAClipArea = RenderTrue; }
	void SetRenderCPArea(bool RenderTrue) { m_RenderCPArea = RenderTrue; }

	/*GraphicObject& operator=(const GraphicObject& g);*/

	void OnMouseOver(meaof::obj_args& args) override {}
	void OnMouseEnter(meaof::obj_args& args) override {}
	void OnMouseLeave(meaof::obj_args& args) override {}
protected:
	void AddParent(GraphicObject* o);
	void DrawChildrens(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL);
	void CreateResourcesChildrens(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text = NULL);
	void DiscardResourcesChildrens();
	void SetClipArea();

	HRESULT CreateMouseDetector();
};
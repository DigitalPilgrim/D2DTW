#pragma once

#include "TX_TextBox.h"
#include "GO_Ellipse.h"
#include "GO_Rectangle.h"
#include "ObjectHolder.h"
#include "GO_GeometryEllipse.h"
#include "MyTimer.h"
#include "ControlCanvas.h"
#include <list>

class program : public ObjectRenderHolder
{
	GO_Ellipse m_ellipse_test[3];
	TX_TextBox m_textBox;
	GO_Rectangle m_rect;
	GO_GeometryEllipse m_gellipse;
	GO_Ellipse* m_gellipses;
	int m_geMax;
	GO_Rectangle m_rectBox;
	GO_GeometryEllipse m_elInsideBox;
	GO_GeometryEllipse m_elInsideBox2;
	ControlCanvas m_cCanvas;

	MyTimer::Timer m_timer;
	MyTimer::Timer m_update;

	bool* m_updateRender;

	int scaleDirection;

	float sinFlow;
	float sinFlow2;

public:
	program();
	~program();

	void INI();

	meaof::dispatcher_holder m_dispatcher;
	
	void Render(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text = NULL) override;
	void DiscardResources() override;

	void Update();

	void SetUpdatePointer(bool* p) 
	{ m_updateRender = p; }
};
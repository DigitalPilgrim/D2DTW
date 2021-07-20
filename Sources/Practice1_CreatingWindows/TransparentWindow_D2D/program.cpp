#include "program.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "RandomGenerator.h"
#include "DPI.h"
#include "MouseEventManager.h"

program::program()
{
	// INI
	{
		m_updateRender = nullptr;
		m_timer.SetDuration(20);
		m_update.SetDuration(20);
		scaleDirection = 0;
		sinFlow = 0.0f;
		sinFlow2 = 0.0f;
	}
}

program::~program()
{
	delete[] m_gellipses;
}

void program::INI()
{
	// ------------------------------------------------------------------------------------
	// Ellipse list
	{
		m_ellipse_test[0].Color(D2D1::ColorF(D2D1::ColorF::Azure));
		m_ellipse_test[1].Color(D2D1::ColorF(D2D1::ColorF::BurlyWood));
		m_ellipse_test[2].Color(D2D1::ColorF(D2D1::ColorF::DarkBlue));

		m_ellipse_test[0].Position(D2D1::Point2F(0.0f, 0.0f));
		m_ellipse_test[1].Position(D2D1::Point2F(250.0f, 150.0f));
		m_ellipse_test[2].Position(D2D1::Point2F(300.0f, 350.0f));

		m_ellipse_test[0].Size(D2D1::Point2F(100.0f, 100.f));
		m_ellipse_test[1].Size(D2D1::Point2F(200.0f, 100.0f));
		m_ellipse_test[2].Size(D2D1::Point2F(100.0f, 100.0f));

		/*m_ellipse_test[0].SetRenderAClipArea(true);
		m_ellipse_test[1].SetRenderAClipArea(true);
		m_ellipse_test[2].SetRenderAClipArea(true);*/

		m_ellipse_test[0].m_dispatcher = m_dispatcher;
		m_ellipse_test[1].m_dispatcher = m_dispatcher;
		m_ellipse_test[2].m_dispatcher = m_dispatcher;

	}

	// ------------------------------------------------------------------------------------

	{
		m_graphics.push_back(&m_ellipse_test[2]);
		m_graphics.push_back(&m_ellipse_test[1]);
		m_graphics.push_back(&m_ellipse_test[0]);
	}

	// ------------------------------------------------------------------------------------
	// Text
	{
		m_textBox.Text(L"Text Box");
		m_textBox.Size(D2D1::Point2F(200.0f, 48.0f));
		m_textBox.Position(D2D1::Point2F(200.0f, 200.0f));
		m_textBox.Rotate(90.f);
		m_textBox.FontSize(48.f);
		m_textBox.SetOptimized(true);
		//m_textBox.SetRenderAClipArea(true);

		m_graphics.push_back(&m_textBox);
	}

	// ------------------------------------------------------------------------------------
	// Rect
	{
		m_rect.Color(D2D1::ColorF(D2D1::ColorF::Coral));
		m_rect.Size(D2D1::Point2F(100.0f, 30.0f));
		m_rect.Position(D2D1::Point2F(300.0f, 300.0f));
		m_rect.Rotate(45.0f);
		//m_rect.SetRenderAClipArea(true);
		m_rect.m_dispatcher = m_dispatcher;

		m_graphics.push_back(&m_rect);
	}

	// ------------------------------------------------------------------------------------
	// Ellipse Geometry
	{
		m_gellipse.Color(D2D1::ColorF(D2D1::ColorF::AntiqueWhite));
		m_gellipse.Size(D2D1::Point2F(50.0f, 50.0f));
		m_gellipse.Position(D2D1::Point2F(200.0f, 400.0f));
		m_gellipse.m_dispatcher = m_dispatcher;
		m_gellipse.SetOptimized(true);

		m_graphics.push_back(&m_gellipse);
	}

	// ------------------------------------------------------------------------------------
	// Rect Box test
	{
		m_rectBox.Size(D2D1::Point2F(400.0f, 300.0f));
		m_rectBox.Position(D2D1::Point2F(100.0f, 100.0f));
		m_rectBox.Color(D2D1::ColorF(D2D1::ColorF::LightBlue)); //Blue
		m_rectBox.m_dispatcher = m_dispatcher;
		//m_rectBox.SetRenderAClipArea(true);
		m_rectBox.SetIsCliped(true);
		//m_rectBox.SetRenderCPArea(true);
		//m_graphics.push_back(&m_rectBox);
		m_graphics.push_front(&m_rectBox);

		m_elInsideBox.Size(D2D1::Point2F(100.0f, 100.0f));
		m_elInsideBox.Position(D2D1::Point2F(-10.0f, 50.0f));
		m_elInsideBox.Color(D2D1::ColorF(D2D1::ColorF::Yellow));
		//m_elInsideBox.SetIsCliped(true);
		m_elInsideBox.m_dispatcher = m_dispatcher;
		//m_elInsideBox.SetRenderAClipArea(true);
		//m_elInsideBox.SetRenderCPArea(true);
		m_rectBox.AddChild(&m_elInsideBox);

		m_elInsideBox2.Size(D2D1::Point2F(25.0f, 25.0f));
		m_elInsideBox2.Position(D2D1::Point2F(-10.0f, -10.0f));
		m_elInsideBox2.Color(D2D1::ColorF(D2D1::ColorF::Green));
		m_elInsideBox2.m_dispatcher = m_dispatcher;
		//m_elInsideBox2.SetRenderAClipArea(true);
		//m_elInsideBox2.SetRenderCPArea(true);
		m_elInsideBox.AddChild(&m_elInsideBox2);
		m_rectBox.Update();
	}
	// ------------------------------------------------------------------------------------
	// List of Ellipse Geometry
	{
		m_cCanvas.Size(D2D1::Point2F(30.0f, 40.0f));
		m_graphics.push_front(&m_cCanvas);
		m_cCanvas.SetRenderAClipArea(true);
		m_geMax = 4000;
		m_gellipses = new GO_Ellipse[m_geMax];
		float pos = 15.0f;
		float px = 0.0;
		float py = 0.0;
		float s = 20.0f;
		int i = 0;
		int x = 1;
		for (int it = 0; it < m_geMax; it++)
		{
			m_gellipses[it].Color(D2D1::ColorF(D2D1::ColorF::AntiqueWhite, 0.5f));
			/*float size = RandGen::Float(5.0f, 15.0f);
			m_gellipses[it].Size(D2D1::Point2F(size, size));*/
			m_gellipses[it].Size(D2D1::Point2F(s, s));

			m_gellipses[it].Position(D2D1::Point2F(px, py));
			m_gellipses[it].m_dispatcher = m_dispatcher;
			py += pos;
			//m_graphics.push_front(&m_gellipses[it]);
			m_cCanvas.AddChild(&m_gellipses[it]);
			++i;
			if (i >= 50) {
				i = 0; py = 0.0f; px = pos * x; ++x;
			}
		}

	}
	// ------------------------------------------------------------------------------------

	MouseEventManager::SetRoot(m_graphics);
}

void program::Render(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	for (GraphicObject* g : m_graphics)
	{
		g->DrawObject(render, scb);
	}
}

HRESULT program::CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text)
{
	HRESULT hr = S_OK;
	for (GraphicObject* g : m_graphics)
	{
		hr = g->CreateResources(render, factory, text);
		if (!SUCCEEDED(hr)) break;
	}
	return hr;
}

void program::DiscardResources()
{
	for (GraphicObject* g : m_graphics)
	{
		g->DiscardResources();
	}
}

void program::Update()
{
	{
		//if (m_update.Check())
		{
			float elapsed = m_update.Elapsed();

			// Ellipse Update
			{
				const float maxScale = 300.0f;
				const float minScale = 50.0f;
				const float step = 1.0f;
				GO_Ellipse* ge = &m_ellipse_test[1];
				D2D_SIZE_F s = ge->Scale();
				D2D_POINT_2F scale = { s.width, s.height };
				D2D_POINT_2F size = ge->Size();

				const float minScaleP = minScale / size.x;
				const float maxScaleP = maxScale / size.x - minScaleP;
				//sinf(flow + 0.1f * elapsed);
				float si = (sinf(sinFlow) + 1.0f) / 2.0f;
				//si *= si;
				scale.x = minScaleP + (maxScaleP * si);// * elapsed;
				scale.y = minScaleP + (maxScaleP * si);
				if (sinFlow >= (static_cast<float>(M_PI) * 2.0f)) sinFlow -= (static_cast<float>(M_PI) * 2.0f);// 0.0f - step * elapsed;
				//if (sinFlow >= 1.0f) sinFlow = 0.0f - step * elapsed;
				sinFlow = sinFlow + step * elapsed;

				ge->Scale(scale);
				//ge->Scale(scale, ge->Position());
				*m_updateRender = true;

				D2D1_POINT_2F pos = { 200.0f , 400.0f };


				m_gellipse.Size(D2D1::Point2F(50.f * scale.x, 50.0f * scale.y));
				//m_gellipse.Position(D2D1::Point2F(pos.x + pos.x * scale.x, pos.y));

			}

			// Text Update
			{
				/*const float step = 15.0f;
				float rotte = m_textBox.Rotate();
				if (rotte >= 360.0f) 
					rotte -= 360.0f;
				m_textBox.Rotate(rotte + step * elapsed);*/
			}

			// Rect Update
			{
				const float step = 100.0f;
				float rotte = m_rect.Rotate();
				if (rotte >= 360.0f)
					rotte -= 360.0f;
				m_rect.Rotate(rotte + step * elapsed);
			}

			// Ellipses Update
			{
				//const float minSize = 5.0f;
				//const float maxSize = 15.0f;
				//const float speed = 4.f;
				//float pos = (sinf(sinFlow2) + 1.0f) / 2.0f;
				//if (sinFlow2 >= (static_cast<float>(M_PI) * 2.0f)) sinFlow2 -= (static_cast<float>(M_PI) * 2.0f);
				//sinFlow2 = sinFlow2 + speed * elapsed;
				//for (int i = 0; i < m_geMax; i++)
				//{
				//	//D2D1_POINT_2F size = e.Size();
				//	float sizePercent = minSize + ((maxSize - minSize) * pos);
				//	//sizePercent = (sinf(sizePercent * static_cast<float>(M_PI) * 2.0f) + 1.0f) / 2.0f;
				//	//sizePercent = minSize + (maxSize * sizePercent);
				//	m_gellipses[i].Size(D2D1::Point2F(sizePercent, sizePercent));
				//}
				
				//for (auto& e : m_gellipses)
				//{
				//	//D2D1_POINT_2F size = e.Size();
				//	float sizePercent = minSize + ((maxSize - minSize) * pos);
				//	//sizePercent = (sinf(sizePercent * static_cast<float>(M_PI) * 2.0f) + 1.0f) / 2.0f;
				//	//sizePercent = minSize + (maxSize * sizePercent);
				//	e.Size(D2D1::Point2F(sizePercent, sizePercent));
				//}
			}

			// BOX Update
			{
				
				const float MaxWidth = 500.0f;
				const float MinWidth = 100.f;
				float step = m_rectBox.Position().x;
				step = step + 10.0f * elapsed;
				if (step < MaxWidth)
				{
					m_rectBox.Position(D2D1::Point2F(step, m_rectBox.Position().y));
				}
			}
		}
	}
}

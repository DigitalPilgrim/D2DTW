#pragma once
#include <Windows.h>
#include <d2d1.h>

class WICRenderTarget
{
    ID2D1GdiInteropRenderTarget* m_renderTarget;
    HDC m_dc;
public:
    WICRenderTarget(ID2D1GdiInteropRenderTarget* renderTarget) :
        m_renderTarget(renderTarget),
        m_dc(0) {

        m_renderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &m_dc);
    }

    ~WICRenderTarget() {
        RECT rect = {};
        m_renderTarget->ReleaseDC(&rect);
    }

    operator HDC() const {
        return m_dc;
    }
};
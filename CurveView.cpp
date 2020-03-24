//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CurveView.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "HspGradatorApp.h"
#include "CurveView.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCurveView::CCurveView() : 
	CView(), 
	p_guide(nullptr), 
	p_curve(nullptr), 
	wndSize(0, 0), 
	borderSize(16), 
	textHeight(0), 
	contentOrigin(0, 0), 
	contentSize(0, 0), 
	pointSize(12), 
	memDc(), 
	memBmp(), 
	labelString(), 
	gridColor(RGB(128, 128, 128)), 
	textColor(::GetSysColor(COLOR_WINDOWTEXT)), 
	curveColor(::GetSysColor(COLOR_WINDOWTEXT)), 
	guideColor(RGB(0, 192, 0)), 
	gridPen(PS_SOLID, 1, gridColor), 
	curvePen(PS_SOLID, 3, curveColor), 
	guidePen(PS_SOLID, 3, guideColor),  
	p_selectedDataPoint(nullptr), 
	moveOffset(0, 0), 
	mouseState(EMouseState::none)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CCurveView, CView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TRect<float> CCurveView::createSelectionRect(const TPoint<float>& p) const
{
	const float psh = pointSize / 2.0F;
	const TPoint<float> rs(psh / contentSize.cx, psh / contentSize.cy);
	return TRect<float>(p, p).expand(rs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::update(const CGuide* const p_g, CCurve* const p_s)
{
	p_guide = p_g;
	p_curve = p_s;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::refresh()
{
	memDc.FillSolidRect(0, 0, wndSize.cx, wndSize.cy, ::GetSysColor(COLOR_3DLIGHT));
	if (contentSize.cx > 0 && contentSize.cy > 0)
	{
		// Grid
		if (CPen* const p_op = memDc.SelectObject(&gridPen))
		{
			for (int i = 0; i < 11; ++i)
			{
				const int x = static_cast<int>(i / 10.0F * contentSize.cx);
				memDc.MoveTo(contentOrigin.x + x, contentOrigin.y);
				memDc.LineTo(contentOrigin.x + x, contentOrigin.y + contentSize.cy);
				const int y = static_cast<int>(i / 10.0F * contentSize.cy);
				memDc.MoveTo(contentOrigin.x, contentOrigin.y + y);
				memDc.LineTo(contentOrigin.x + contentSize.cx, contentOrigin.y + y);
			}
			memDc.SelectObject(p_op);
		}
		// Guide
		if (p_guide)
		{
			if (CPen* const p_op = memDc.SelectObject(&guidePen))
			{
				if (!p_guide->empty())
				{
					const float s = p_guide->size() - 1.0F;
					moveTo({(*p_guide)[0], 0.0F});
					for (size_t i = 1; i < p_guide->size(); ++i) lineTo({(*p_guide)[i], i / s});
				}
				memDc.SelectObject(p_op);
			}
		}
		if (p_curve)
		{
			// Curve
			if (CPen* const p_op = memDc.SelectObject(&curvePen))
			{
				const CCurve& dpv = *p_curve;
				for (size_t i = 1; i < dpv.size(); ++i)
				{
					if (dpv[i].x > dpv[i - 1].x)
					{
						moveTo(dpv[i - 1]);
						for (int j = xOut(dpv[i - 1].x) + 1; j < xOut(dpv[i].x); ++j)
						{
							memDc.LineTo(j, yOut(limit(p_curve->calcValue(xIn(j)), 0.0F, 1.0F)));
						}
						lineTo(dpv[i]);
					}
				}
				memDc.SelectObject(p_op);
			}
			// Data points
			for (const CDataPoint& p : *p_curve)
			{
				drawPoint(p, p.angular ? RGB(0, 255, 0) : RGB(255, 0, 0));
			}
		}
	}
	// Label
	memDc.SetTextAlign(TA_LEFT | TA_BASELINE);
	memDc.TextOut(borderSize, borderSize + textHeight / 3, labelString);
	Invalidate(FALSE);
	UpdateWindow();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView - OVERRIDES
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CCurveView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs)) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW));
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnDraw(CDC* p_dc)
{
	p_dc->BitBlt(0, 0, wndSize.cx, wndSize.cy, &memDc, 0, 0, SRCCOPY);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CCurveView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CCurveView::OnCreate(LPCREATESTRUCT p_cs)
{
	if (CView::OnCreate(p_cs) == -1) return -1;
	// Create graphic resources
	const CSize bs(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
	CClientDC dc(this);
	if (!memDc.CreateCompatibleDC(&dc)) return -1;
	if (!memBmp.CreateCompatibleBitmap(&dc, bs.cx, bs.cy)) return -1;
	if (!memDc.SelectObject(&memBmp)) return -1;
	// Set modes
	memDc.SetBkMode(TRANSPARENT);
	memDc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	// Label height
	textHeight = memDc.GetTextExtent(L"A").cy;
	contentOrigin.x = borderSize;
	contentOrigin.y = borderSize + textHeight;
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	wndSize = {cx, cy};
	contentSize = {cx - 2 * borderSize, cy - 2 * borderSize - textHeight};
	refresh();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnLButtonDown(UINT flags, CPoint point)
{
	SetCapture();
	if (p_curve)
	{
		const TRect<float> sr(createSelectionRect(pIn(point)));
		p_selectedDataPoint = p_curve->findDataPoint(sr);
		if (p_selectedDataPoint)
		{
			moveOffset = point - pOut(*p_selectedDataPoint);
			mouseState = flags & MK_CONTROL ? EMouseState::leftCtrl : EMouseState::left;
			refresh();
		}
	}
	CView::OnLButtonDown(flags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnLButtonUp(UINT flags, CPoint point)
{
	if (p_curve && p_selectedDataPoint)
	{
		if (mouseState == EMouseState::leftCtrl && !(flags & MK_SHIFT))
		{
			// Switch angularity
			p_selectedDataPoint->angular = !p_selectedDataPoint->angular;
			p_curve->initControlPointVect();
		}
		refresh();
		sendMessageToMainWnd(GM_CURVE_CHANGED);
	}
	p_selectedDataPoint = nullptr;
	mouseState = EMouseState::none;
	::ReleaseCapture();
	CView::OnLButtonUp(flags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnLButtonDblClk(UINT flags, CPoint point)
{
	if (p_curve && !p_selectedDataPoint)
	{
		const TRect<float> sr(createSelectionRect(pIn(point)));
		if (!p_curve->findDataPoint(sr))
		{
			// Reset Curve
			*p_curve = {{{0.0F, 0.0F}, true}, {{1.0F, 1.0F}, true}};
			p_curve->initControlPointVect();
			refresh();
			sendMessageToMainWnd(GM_CURVE_CHANGED);
		}
	}
	CView::OnLButtonDblClk(flags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnRButtonDown(UINT flags, CPoint point)
{
	SetCapture();
	if (p_curve)
	{
		const TRect<float> sr(createSelectionRect(pIn(point)));
		p_selectedDataPoint = p_curve->findDataPoint(sr);
		if (p_selectedDataPoint) mouseState = EMouseState::right;
	}
	CView::OnRButtonDown(flags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnRButtonUp(UINT flags, CPoint point)
{
	if (p_curve)
	{
		if (p_selectedDataPoint)
		{
			// Remove point at mouse position
			p_curve->removeDataPoint(p_selectedDataPoint);
		}
		else
		{
			// Insert point at mouse position
			const int x = limit<int>(point.x, contentOrigin.x, contentOrigin.x + contentSize.cx);
			const int y = limit<int>(point.y, contentOrigin.y, contentOrigin.y + contentSize.cy);
			p_curve->addDataPoint(pIn(x, y), flags & MK_CONTROL);
		}
		refresh();
		sendMessageToMainWnd(GM_CURVE_CHANGED);
	}
	p_selectedDataPoint = nullptr;
	mouseState = EMouseState::none;
	::ReleaseCapture();
	CView::OnRButtonUp(flags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnMouseMove(UINT flags, CPoint point)
{
	if (p_curve && p_selectedDataPoint && mouseState == EMouseState::left)
	{
		// Move data point
		p_selectedDataPoint->x = limit(xIn(point.x - moveOffset.x), 0.0F, 1.0F);
		p_selectedDataPoint->y = limit(yIn(point.y - moveOffset.y), 0.0F, 1.0F);
		p_curve->limitDataPointPosition(p_selectedDataPoint);
		p_curve->initControlPointVect();
		refresh();
		sendMessageToMainWnd(GM_CURVE_CHANGED, 1U);
	}
	CView::OnMouseMove(flags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CurveView.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "CurveView.h"
#include "HspGradatorApp.h"
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
	memoryDc(), 
	memoryBitmap(), 
	gridPen(), 
	guidePen(), 
	curvePen(), 
	pointSize(12), 
	borderSize(0), 
	textHeight(0), 
	wndSize(0, 0), 
	contentOrigin(0, 0), 
	contentSize(0, 0), 
	labelString(), 
	p_selectedDataPoint(nullptr), 
	mouseState(MouseState::none)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CCurveView, CView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ama::Rectangle<float> CCurveView::createSelectionRectangle(const ama::Point<float>& p) const
{
	const float psh = static_cast<float>(pointSize) * 0.5F;
	const ama::Point<float> rs(psh / contentSize.cx, psh / contentSize.cy);
	return ama::Rectangle<float>(p, p).expand(rs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::update(const Guide* const p_g, Curve* const p_c)
{
	p_guide = p_g;
	p_curve = p_c;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::draw()
{
	memoryDc.FillSolidRect(0, 0, wndSize.cx, wndSize.cy, ::GetSysColor(COLOR_3DLIGHT));
	if (contentSize.cx > 0 && contentSize.cy > 0)
	{
		// Grid
		if (CPen* const p_op = memoryDc.SelectObject(&gridPen))
		{
			for (int i = 0; i < 11; ++i)
			{
				const int x = static_cast<int>(i / 10.0F * contentSize.cx);
				memoryDc.MoveTo(contentOrigin.x + x, contentOrigin.y);
				memoryDc.LineTo(contentOrigin.x + x, contentOrigin.y + contentSize.cy);
				const int y = static_cast<int>(i / 10.0F * contentSize.cy);
				memoryDc.MoveTo(contentOrigin.x, contentOrigin.y + y);
				memoryDc.LineTo(contentOrigin.x + contentSize.cx, contentOrigin.y + y);
			}
			memoryDc.SelectObject(p_op);
		}
		// Guide
		if (p_guide)
		{
			if (CPen* const p_op = memoryDc.SelectObject(&guidePen))
			{
				if (!p_guide->empty())
				{
					const float s = p_guide->size() - 1.0F;
					moveTo({(*p_guide)[0], 0.0F});
					for (size_t i = 1; i < p_guide->size(); ++i) lineTo({(*p_guide)[i], i / s});
				}
				memoryDc.SelectObject(p_op);
			}
		}
		if (p_curve)
		{
			// Curve
			if (CPen* const p_op = memoryDc.SelectObject(&curvePen))
			{
				const ama::DataPointVect<float>& dpv = p_curve->getDataPointVect();
				for (size_t i = 1; i < dpv.size(); ++i)
				{
					if (dpv[i].x > dpv[i - 1].x)
					{
						moveTo(dpv[i - 1]);
						for (int j = xOut(dpv[i - 1].x) + 1; j < xOut(dpv[i].x); ++j)
						{
							memoryDc.LineTo(j, yOut(ama::limit(p_curve->calcValue(xIn(j)), 0.0F, 1.0F)));
						}
						lineTo(dpv[i]);
					}
				}
				memoryDc.SelectObject(p_op);
			}
			// Data points
			for (const DataPoint& p : p_curve->getDataPointVect())
			{
				drawPoint(p, p.angular ? RGB(0, 255, 0) : RGB(255, 0, 0));
			}
		}
	}
	// Label
	memoryDc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	memoryDc.SetTextAlign(TA_LEFT | TA_BASELINE);
	memoryDc.TextOutW(borderSize, borderSize + textHeight / 3, labelString.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::refresh()
{
	Invalidate(FALSE);
	UpdateWindow();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView - OVERRIDES
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CCurveView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (CView::PreCreateWindow(cs))
	{
		cs.dwExStyle |= WS_EX_CLIENTEDGE;
		cs.style &= ~WS_BORDER;
		cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW));
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnDraw(CDC* pDC)
{
	draw();
	pDC->BitBlt(0, 0, wndSize.cx, wndSize.cy, &memoryDc, 0, 0, SRCCOPY);
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
int CCurveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == 0)
	{
		if (memoryDc.CreateCompatibleDC(nullptr))
		{
			CClientDC dc(this);
			const CSize mws( // Maximum window size
				::GetSystemMetrics(SM_CXSCREEN), 
				::GetSystemMetrics(SM_CYSCREEN));
			if (memoryBitmap.CreateCompatibleBitmap(&dc, mws.cx, mws.cy))
			{
				if (memoryDc.SelectObject(&memoryBitmap))
				{
					memoryDc.SetBkMode(TRANSPARENT);
					memoryDc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
					const int lw = (std::max)(static_cast<int>(mws.cy) / 1000, 1);
					gridPen.CreatePen(PS_SOLID, lw, RGB(128, 128, 128));
					guidePen.CreatePen(PS_SOLID, lw * 2, RGB(0, 192, 0));
					curvePen.CreatePen(PS_SOLID, lw * 3, ::GetSysColor(COLOR_WINDOWTEXT));
					pointSize = mws.cy / 100;
					borderSize = mws.cy / 75;
					textHeight = memoryDc.GetTextExtent(L"A").cy;
					contentOrigin.x = borderSize;
					contentOrigin.y = borderSize + textHeight;
					return 0;
				}
			}
		}
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	wndSize = {cx, cy};
	contentSize = {cx - 2 * borderSize, cy - 2 * borderSize - textHeight};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnLButtonDown(UINT nflags, CPoint pnt)
{
	SetCapture();
	if (p_curve)
	{
		const ama::Rectangle<float> sra(createSelectionRectangle(pIn(pnt)));
		p_selectedDataPoint = p_curve->findDataPoint(sra);
		if (p_selectedDataPoint)
		{
			mouseState = nflags & MK_CONTROL ? MouseState::leftCtrl : MouseState::left;
			refresh();
		}
	}
	CView::OnLButtonDown(nflags, pnt);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnLButtonUp(UINT nflags, CPoint pnt)
{
	if (p_curve && p_selectedDataPoint)
	{
		if (mouseState == MouseState::leftCtrl && !(nflags & MK_SHIFT))
		{
			// Switch angularity
			p_selectedDataPoint->angular = !p_selectedDataPoint->angular;
			p_curve->initControlPointVect();
		}
		refresh();
		sendMessageToMainWnd(GM_CURVE_CHANGED);
	}
	p_selectedDataPoint = nullptr;
	mouseState = MouseState::none;
	::ReleaseCapture();
	CView::OnLButtonUp(nflags, pnt);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnLButtonDblClk(UINT nflags, CPoint pnt)
{
	if (p_curve && !p_selectedDataPoint)
	{
		const ama::Rectangle<float> sra(createSelectionRectangle(pIn(pnt)));
		if (!p_curve->findDataPoint(sra))
		{
			// Reset Curve
			p_curve->resetDataPointVect();
			p_curve->initControlPointVect();
			refresh();
			sendMessageToMainWnd(GM_CURVE_CHANGED);
		}
	}
	CView::OnLButtonDblClk(nflags, pnt);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnRButtonDown(UINT nflags, CPoint pnt)
{
	SetCapture();
	if (p_curve)
	{
		const ama::Rectangle<float> sra(createSelectionRectangle(pIn(pnt)));
		p_selectedDataPoint = p_curve->findDataPoint(sra);
		if (p_selectedDataPoint) mouseState = MouseState::right;
	}
	CView::OnRButtonDown(nflags, pnt);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnRButtonUp(UINT nflags, CPoint pnt)
{
	if (p_curve)
	{
		if (p_selectedDataPoint)
		{
			// Remove pnt at mouse position
			p_curve->removeDataPoint(p_selectedDataPoint);
		}
		else
		{
			// Insert pnt at mouse position
			const int x = ama::limit<int>(pnt.x, contentOrigin.x, contentOrigin.x + contentSize.cx);
			const int y = ama::limit<int>(pnt.y, contentOrigin.y, contentOrigin.y + contentSize.cy);
			p_curve->addDataPoint(pIn(x, y), nflags & MK_CONTROL);
		}
		refresh();
		sendMessageToMainWnd(GM_CURVE_CHANGED);
	}
	p_selectedDataPoint = nullptr;
	mouseState = MouseState::none;
	::ReleaseCapture();
	CView::OnRButtonUp(nflags, pnt);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCurveView::OnMouseMove(UINT nflags, CPoint pnt)
{
	static CPoint prevPoint;
	if (p_curve && p_selectedDataPoint && mouseState == MouseState::left)
	{
		// Move data pnt
		const ama::Point<float> dp(pIn(pnt) - pIn(prevPoint)); // Difference pnt
		p_selectedDataPoint->x = ama::limit(p_selectedDataPoint->x + dp.x, 0.0F, 1.0F);
		p_selectedDataPoint->y = ama::limit(p_selectedDataPoint->y + dp.y, 0.0F, 1.0F);
		p_curve->limitDataPointPosition(p_selectedDataPoint);
		p_curve->initControlPointVect();
		refresh();
		sendMessageToMainWnd(GM_CURVE_CHANGED, 1U);
	}
	prevPoint = pnt;
	CView::OnMouseMove(nflags, pnt);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

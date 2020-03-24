//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramView.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "HspGradatorApp.h"
#include "HistogramView.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramArray
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramArray - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramArray::init(const CTrueColorImage& mi)
{
	for (CHistogram& h : *this)
	{
		h.fill(0.0F);
		h.setAverage(0.0F);
	}
	if (const CSize is(mi.getSize()); is.cx > 0 && is.cy > 0)
	{
		TColor<long long> sc(0, 0, 0);
		long long sp = 0;
		for (int j = 0; j < is.cy; ++j)
		{
			for (int i = 0; i < is.cx; ++i)
			{
				const TColor<BYTE> c(mi.getPixel<TColor<BYTE>>(i, j));
				const BYTE p = roundTo<BYTE>(calcPerceivedBrightness(TColor<float>(c.r, c.g, c.b)));
				// Count values
				++(*this)[0][c.r];
				++(*this)[1][c.g];
				++(*this)[2][c.b];
				++(*this)[3][p];
				// Sum up values
				sc.r += c.r;
				sc.g += c.g;
				sc.b += c.b;
				sp += p;
			}
		}
		// Normalize values
		float m = 0.0F;
		for (int i = 0; i < 3; ++i)
		{
			for (const float& v : (*this)[i]) if (m < v) m = v;
		}
		if (m > 0.0F)
		{
			for (int i = 0; i < 3; ++i)
			{
				for (float& v : (*this)[i]) v /= m;
			}
		}
		m = 0.0F;
		for (const float& v : (*this)[3]) if (m < v) m = v;
		if (m > 0.0F)
		{
			for (float& v : (*this)[3]) v /= m;
		}
		// Take the average
		const float s = static_cast<float>(is.cx * is.cy);
		(*this)[0].setAverage(sc.r / s);
		(*this)[1].setAverage(sc.g / s);
		(*this)[2].setAverage(sc.b / s);
		(*this)[3].setAverage(sp / s / 2.55F);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CHistogramView::CHistogramView() : 
	p_histogramArray(nullptr), 
	wndSize(0, 0), 
	textHeight(0), 
	memDc(), 
	memBmp()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CHistogramView, CView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::update(const CHistogramArray* const p_ha)
{
	p_histogramArray = p_ha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::refresh()
{
	if (p_histogramArray)
	{
		const CSize ds(10, 10); // Distance size
		memDc.FillSolidRect(0, 0, wndSize.cx, wndSize.cy, ::GetSysColor(COLOR_3DLIGHT));
		const int hw = (wndSize.cx - ds.cx) / 4;
		CRect hr(ds.cx, ds.cy, hw, wndSize.cy - ds.cy - textHeight); // Histogram rect
		CRect tr(ds.cx, hr.bottom, hw, hr.bottom + textHeight); // Text rect
		const std::array<COLORREF, 4> ca{RGB(255, 0, 0),  RGB(0, 255, 0), RGB(0, 0, 255), ::GetSysColor(COLOR_WINDOWTEXT)};
		//const std::array<CString, 4> sa{L"R", L"G", L"B", L"P"};
		//const CString sa(L"RGBP");
		wchar_t* sa(L"RGBP");
		for (int i = 0; i < 4; ++i)
		{
			const COLORREF bc = RGB(128, 128, 128);
			memDc.FillSolidRect(hr.left, hr.top - 1, hr.Width(), 1, bc);
			memDc.FillSolidRect(hr.left, hr.bottom, hr.Width(), 1, bc);
			memDc.FillSolidRect(hr.left - 1, hr.top - 1, 1, hr.Height() + 2, bc);
			memDc.FillSolidRect(hr.right, hr.top - 1, 1, hr.Height() + 2, bc);
			for (int j = 0; j < hr.Width(); ++j)
			{
				const float v = j / (hr.Width() - 1.0F);
				if (hr.Height() > 0)
				{
					const int t = static_cast<int>((*p_histogramArray)[i].calcValue(v) * hr.Height());
					memDc.FillSolidRect(hr.left + j, hr.bottom - t, 1, t, ca[i]);
				}
			}
			memDc.SetTextAlign(TA_LEFT | TA_TOP);
			memDc.TextOut(tr.left, tr.top + textHeight / 3, CString(sa[i]));
			memDc.SetTextAlign(TA_RIGHT | TA_TOP);
			CString as;
			as.Format(L"A=%.1f", (*p_histogramArray)[i].getAverage());
			memDc.TextOut(tr.right, tr.top + textHeight / 3, as);
			hr.OffsetRect(hw, 0);
			tr.OffsetRect(hw, 0);
		}
	}
	Invalidate(FALSE);
	UpdateWindow();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - OVERRIDES
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CHistogramView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs)) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW));
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::OnDraw(CDC* p_dc)
{
	p_dc->BitBlt(0, 0, wndSize.cx, wndSize.cy, &memDc, 0, 0, SRCCOPY);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHistogramView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CHistogramView::OnCreate(LPCREATESTRUCT p_cs)
{
	if (CView::OnCreate(p_cs) == -1) return -1;
	// Create graphic resources
	const CSize bs(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN) / 5);
	CClientDC dc(this);
	if (!memDc.CreateCompatibleDC(&dc)) return -1;
	if (!memBmp.CreateCompatibleBitmap(&dc, bs.cx, bs.cy)) return -1;
	if (!memDc.SelectObject(&memBmp)) return -1;
	// Set modes
	memDc.SetBkMode(TRANSPARENT);
	memDc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	memDc.SetTextAlign(TA_LEFT | TA_TOP);
	// Label height
	textHeight = memDc.GetTextExtent(L"A").cy;
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::OnSize(UINT type, int cx, int cy)
{
	CView::OnSize(type, cx, cy);
	wndSize = {cx, cy};
	refresh();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

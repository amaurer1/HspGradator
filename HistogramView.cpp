//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramView.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "HistogramView.h"
#include "HspGradatorApp.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Histogram
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Histogram - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Histogram::calcValue(const float v) const
{
	return (*this)[static_cast<int>(v * (static_cast<float>(size()) - 1.0F))];
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramArray
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramArray - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HistogramArray::init(const CTrueColorImage& mi)
{
	for (Histogram& h : *this)
	{
		h.fill(0.0F);
		h.setAverage(0.0F);
	}
	if (const CSize is(mi.getSize()); is.cx > 0 && is.cy > 0)
	{
		ama::Color<long long> sc(0, 0, 0);
		long long sp = 0;
		for (int j = 0; j < is.cy; ++j)
		{
			for (int i = 0; i < is.cx; ++i)
			{
				const ama::Color<BYTE> c(mi.getPixel<ama::Color<BYTE>>(i, j));
				const BYTE p = ama::round<BYTE>(calcPerceivedBrightness(ama::Color<float>(c.r, c.g, c.b)));
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
	memoryDc(), 
	memoryBitmap(), 
	borderSize(0), 
	textHeight(0), 
	wndSize(0, 0)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CHistogramView, CView)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::update(const HistogramArray* const p_ha)
{
	p_histogramArray = p_ha;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::draw()
{
	if (p_histogramArray)
	{
		const CSize ds(borderSize, borderSize); // Distance size
		memoryDc.FillSolidRect(0, 0, wndSize.cx, wndSize.cy, ::GetSysColor(COLOR_3DLIGHT));
		const int hw = (wndSize.cx - ds.cx) / 4;
		CRect hr(ds.cx, ds.cy, hw, wndSize.cy - ds.cy - textHeight); // Histogram rect
		CRect tr(ds.cx, hr.bottom, hw, hr.bottom + textHeight); // Text rect
		const std::array<COLORREF, 4> ca{RGB(255, 0, 0),  RGB(0, 255, 0), RGB(0, 0, 255), ::GetSysColor(COLOR_WINDOWTEXT)};
		const std::wstring sa(L"RGBP");
		for (int i = 0; i < 4; ++i)
		{
			const COLORREF bc = RGB(128, 128, 128);
			memoryDc.FillSolidRect(hr.left, hr.top - 1, hr.Width(), 1, bc);
			memoryDc.FillSolidRect(hr.left, hr.bottom, hr.Width(), 1, bc);
			memoryDc.FillSolidRect(hr.left - 1, hr.top - 1, 1, hr.Height() + 2, bc);
			memoryDc.FillSolidRect(hr.right, hr.top - 1, 1, hr.Height() + 2, bc);
			for (int j = 0; j < hr.Width(); ++j)
			{
				const float v = j / (hr.Width() - 1.0F);
				if (hr.Height() > 0)
				{
					const int t = static_cast<int>((*p_histogramArray)[i].calcValue(v) * hr.Height());
					memoryDc.FillSolidRect(hr.left + j, hr.bottom - t, 1, t, ca[i]);
				}
			}
			memoryDc.SetTextAlign(TA_LEFT | TA_TOP);
			memoryDc.TextOutW(tr.left, tr.top + textHeight / 3, &sa[i], 1);
			memoryDc.SetTextAlign(TA_RIGHT | TA_TOP);
			const std::wstring as(std::format(L"A={:.1f}", (*p_histogramArray)[i].getAverage()));
			memoryDc.TextOutW(tr.right, tr.top + textHeight / 3, as.data());
			hr.OffsetRect(hw, 0);
			tr.OffsetRect(hw, 0);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::refresh()
{
	Invalidate(FALSE);
	UpdateWindow();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - OVERRIDES
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CHistogramView::PreCreateWindow(CREATESTRUCT& cs)
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
void CHistogramView::OnDraw(CDC* pDC)
{
	draw();
	pDC->BitBlt(0, 0, wndSize.cx, wndSize.cy, &memoryDc, 0, 0, SRCCOPY);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHistogramView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CHistogramView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
					memoryDc.SetTextAlign(TA_LEFT | TA_TOP);
					borderSize = mws.cy / 120;
					textHeight = memoryDc.GetTextExtent(L"A").cy;
					return 0;
				}
			}
		}
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHistogramView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	wndSize = {cx, cy};
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

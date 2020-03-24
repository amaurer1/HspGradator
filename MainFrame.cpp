//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MainFrame.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "HspGradatorApp.h"
#include "MainFrame.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CFixedSplitterWnd
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CFixedSplitterWnd - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CFixedSplitterWnd::CFixedSplitterWnd() :
	CSplitterWnd(), 
	wndSize(0, 0), 
	curveVisibleArray({false, true, true})
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFixedSplitterWnd::RecalcLayout()
{
	HDWP hdwp = ::BeginDeferWindowPos(8);
	auto showView = [this, &hdwp](const int i, const CPoint& p, const CSize& s)
	{
		if (const CWnd* const p_wnd = GetPane(i, 0))
		{
			const UINT f = SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_SHOWWINDOW;
			hdwp = ::DeferWindowPos(hdwp, p_wnd->GetSafeHwnd(), nullptr, p.x, p.y, s.cx, s.cy, f);
		}
	};
	auto hideView = [this, &hdwp](const int i)
	{
		if (const CWnd* const p_wnd = GetPane(i, 0))
		{
			const UINT f = SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE;
			hdwp = ::DeferWindowPos(hdwp, p_wnd->GetSafeHwnd(), nullptr, 0, 0, 0, 0, f);
		}
	};
	const int n = curveVisibleArray[0] + curveVisibleArray[1] + curveVisibleArray[2];
	if (wndSize.cx < wndSize.cy)
	{
		showView(3, {0, 0}, {wndSize.cx, wndSize.cy / 2});
		showView(4, {0, wndSize.cy / 2}, {wndSize.cx, wndSize.cy / 6});
		if (n == 3)
		{
			const int px = wndSize.cx / 3;
			const int py = wndSize.cy / 2 + wndSize.cy / 6;
			showView(0, {0, py}, {px, wndSize.cy - py});
			showView(1, {px, py}, {px, wndSize.cy - py});
			showView(2, {px * 2, py}, {wndSize.cx - px * 2, wndSize.cy - py});
		}
		else if (n == 2)
		{
			const int px = wndSize.cx / 2;
			const int py = wndSize.cy / 2 + wndSize.cy / 6;
			if (curveVisibleArray[0] && curveVisibleArray[1])
			{
				showView(0, {0, py}, {px, wndSize.cy - py});
				showView(1, {px, py}, {wndSize.cx - px, wndSize.cy - py});
				hideView(2);
			}
			if (curveVisibleArray[0] && curveVisibleArray[2])
			{
				showView(0, {0, py}, {px, wndSize.cy - py});
				hideView(1);
				showView(2, {px, py}, {wndSize.cx - px, wndSize.cy - py});
			}
			if (curveVisibleArray[1] && curveVisibleArray[2])
			{
				hideView(0);
				showView(1, {0, py}, {px, wndSize.cy - py});
				showView(2, {px, py}, {wndSize.cx - px, wndSize.cy - py});
			}
		}
		else if (n == 1)
		{
			const int px = wndSize.cx;
			const int py = wndSize.cy / 2 + wndSize.cy / 6;
			if (curveVisibleArray[0])
			{
				showView(0, {0, py}, {px, wndSize.cy - py});
				hideView(1);
				hideView(2);
			}
			if (curveVisibleArray[1])
			{
				hideView(0);
				showView(1, {0, py}, {px, wndSize.cy - py});
				hideView(2);
			}
			if (curveVisibleArray[2])
			{
				hideView(0);
				hideView(1);
				showView(2, {0, py}, {px, wndSize.cy - py});
			}
		}
	}
	else
	{
		if (n == 3)
		{
			const int px = wndSize.cx / 3;
			const int py = wndSize.cy / 3;
			showView(0, {0, 0}, {px, py});
			showView(1, {0, py}, {px, py});
			showView(2, {0, py * 2}, {px, wndSize.cy - 2 * py});
		}
		else if (n == 2)
		{
			const int px = wndSize.cx / 3;
			const int py = wndSize.cy / 2;
			if (curveVisibleArray[0] && curveVisibleArray[1])
			{
				showView(0, {0, 0}, {px, py});
				showView(1, {0, py}, {px, wndSize.cy - py});
				hideView(2);
			}
			if (curveVisibleArray[0] && curveVisibleArray[2])
			{
				showView(0, {0, 0}, {px, py});
				hideView(1);
				showView(2, {0, py}, {px, wndSize.cy - py});
			}
			if (curveVisibleArray[1] && curveVisibleArray[2])
			{
				hideView(0);
				showView(1, {0, 0}, {px, py});
				showView(2, {0, py}, {px, wndSize.cy - py});
			}
		}
		else if (n == 1)
		{
			const int px = wndSize.cx / 3;
			const int py = wndSize.cy;
			if (curveVisibleArray[0])
			{
				showView(0, {0, 0}, {px, py});
				hideView(1);
				hideView(2);
			}
			if (curveVisibleArray[1])
			{
				hideView(0);
				showView(1, {0, 0}, {px, py});
				hideView(2);
			}
			if (curveVisibleArray[2])
			{
				hideView(0);
				hideView(1);
				showView(2, {0, 0}, {px, py});
			}
		}
		const int px = wndSize.cx / 3;
		const int py = wndSize.cy / 5 * 4;
		showView(3, {px, 0}, {wndSize.cx - px, py});
		showView(4, {px, py}, {wndSize.cx - px, wndSize.cy - py});
	}
	::EndDeferWindowPos(hdwp);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CFixedSplitterWnd - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CFixedSplitterWnd, CSplitterWnd)
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_VIEW_CURVE_1, ID_VIEW_CURVE_3, &CFixedSplitterWnd::OnViewCurve)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_CURVE_1, ID_VIEW_CURVE_3, &CFixedSplitterWnd::OnUpdateViewCurve)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFixedSplitterWnd::OnSize(UINT type, int cx, int cy)
{
	//
	const TRgb<float> cc1(63.0F / 255.0F, 62.0F / 255.0F, 146.0F / 255.0F);
	const TCieHcl<float> cc2(convertToCieHcl(cc1));
	const TRgb<float> cc3(convertToRgb(cc2));
	const float l = cc2.l * 100.0F;
	const float c = cc2.c * 100.0F;
	const float h = cc2.h * 360.0F;
	//
	wndSize = {cx, cy};
	CSplitterWnd::OnSize(type, cx, cy);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFixedSplitterWnd::OnViewCurve(UINT id)
{
	std::array<bool, 3>& a = curveVisibleArray;
	const int i = id - ID_VIEW_CURVE_1;
	if (a[i])
	{
		if (a[0] + a[1] + a[2] > 1)
		{
			a[i] = false;
			RecalcLayout();
		}
	}
	else
	{
		a[i] = true;
		RecalcLayout();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CFixedSplitterWnd::OnUpdateViewCurve(CCmdUI* p_cmdUI)
{
	p_cmdUI->SetCheck(curveVisibleArray[p_cmdUI->m_nID - ID_VIEW_CURVE_1]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - STATIC DATA
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static UINT indicators[] = {ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMainFrame::CMainFrame() : 
	splitterWnd(), 
	p_curve1View(nullptr), 
	p_curve2View(nullptr), 
	p_curve3View(nullptr), 
	p_imageView(nullptr), 
	p_histogramView(nullptr), 
	statusBar(), 
	pathName(), 
	filterIndex(0), 
	image(), 
	originalImage(), 
	modifiedImage(), 
	originalHistogramArray(), 
	modifiedHistogramArray(), 
	colorModel(EColorModel::hsp), 
	guideArray(), 
	curveArray(), 
	instantMode(true), 
	allowClamping(true), 
	imageMode(EImageMode::modified)
{
	curveArray.fill({ECurveType::finiteSpline, {{{0.0F, 0.0F}, true}, {{1.0F, 1.0F}, true}}});
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateColorModelLabels()
{
	auto updateMenu = [this](LPCTSTR s1, LPCTSTR s2, LPCTSTR s3)
	{
		if (CMenu* const p_m = GetMenu())
		{
			p_m->ModifyMenu(ID_VIEW_CURVE_1, MF_BYCOMMAND | MF_STRING, ID_VIEW_CURVE_1, s1);
			p_m->ModifyMenu(ID_VIEW_CURVE_2, MF_BYCOMMAND | MF_STRING, ID_VIEW_CURVE_2, s2);
			p_m->ModifyMenu(ID_VIEW_CURVE_3, MF_BYCOMMAND | MF_STRING, ID_VIEW_CURVE_3, s3);
		}
	};
	auto updateCurveViews = [this](LPCTSTR s1, LPCTSTR s2, LPCTSTR s3)
	{
		p_curve1View->setLabelString(s1);
		p_curve2View->setLabelString(s2);
		p_curve3View->setLabelString(s3);
	};
	switch (colorModel)
	{
		case EColorModel::rgb:
			updateMenu(L"&Red Curve\tA", L"&Green Curve\tS", L"&Blue Curve\tD");
			updateCurveViews(L"RGB - Red", L"RGB - Green", L"RGB - Blue");
			break;
		case EColorModel::hsv:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Value Curve\tD");
			updateCurveViews(L"HSV - Hue", L"HSV - Saturation", L"HSV - Value");
			break;
		case EColorModel::hsl:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Lightness Curve\tD");
			updateCurveViews(L"HSL - Hue", L"HSL - Saturation", L"HSL - Lightness");
			break;
		case EColorModel::hsg:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Gray Scale Curve\tD");
			updateCurveViews(L"HSG - Hue", L"HSG - Saturation", L"HSG - Gray Scale");
			break;
		case EColorModel::hsp:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Perceived Brightness Curve\tD");
			updateCurveViews(L"HSP - Hue", L"HSP - Saturation", L"HSP - Perceived Brightness");
			break;
		case EColorModel::cieHcl:
			updateMenu(L"&Hue Curve\tA", L"&Chroma Curve\tS", L"&Lightness Curve\tD");
			updateCurveViews(L"CIE HCL - Hue", L"CIE HCL - Chroma", L"CIE HCL - Lightness");
			break;
		default: break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateImagePathLabel()
{
	statusBar.SetPaneText(1, !pathName.IsEmpty() ? pathName : L"Empty");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateImageSizeLabel()
{
	if (const CSize is = image.getSize(); is.cx > 0 && is.cy > 0)
	{
		CString s;
		s.Format(L"%d x %d px", is.cx, is.cy);
		statusBar.SetPaneText(2, s);
	}
	else
	{
		statusBar.SetPaneText(2, L"");
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateCurveTypeLabel()
{
	switch (curveArray[0].getCurveType())
	{
		case ECurveType::lineSegments: statusBar.SetPaneText(3, L"Line Segments Curve"); break;
		case ECurveType::cubicSpline: statusBar.SetPaneText(3, L"Cubic Spline Curve"); break;
		case ECurveType::finiteSpline: statusBar.SetPaneText(3, L"Finite Spline Curve"); break;
		default: break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateImageModeLabel()
{
	statusBar.SetPaneText(4, imageMode == EImageMode::original ? L"Original Image" : L"Modified Image");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateCalcTimeLabel(const int t)
{
	if (t >= 0)
	{
		CString s;
		s.Format(L"%dms", t);
		statusBar.SetPaneText(5, s);
	}
	else
	{
		statusBar.SetPaneText(5, L"");
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::refreshCurveViews()
{
	p_curve1View->refresh();
	p_curve2View->refresh();
	p_curve3View->refresh();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::refreshModifiedData()
{
	if (imageMode == EImageMode::modified)
	{
		p_imageView->refresh();
		p_histogramView->refresh();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::createGuideArray()
{
	const LONG sl = 100; // Maximum sample amount per length
	const CSize is(originalImage.getSize()); // Image size
	const CSize ss((std::min)(is.cx, sl), (std::min)(is.cy, sl)); // Sample size
	const int sa = ss.cx * ss.cy; // Sample area
	for (CGuide& g : guideArray) g.resize(sa);
	if (sa > 0)
	{
		for (int j = 0; j < ss.cy; ++j)
		{
			const int sj = static_cast<int>(j / (ss.cy - 1.0F) * (is.cy - 1.0F));
			const int o = j * ss.cx;
			for (int i = 0; i < ss.cx; ++i)
			{
				const int si = static_cast<int>(i / (ss.cx - 1.0F) * (is.cx - 1.0F));
				const TRgb<float> oc(originalImage.getPixel<TColor<float>>(si, sj));
				switch (colorModel)
				{
					case EColorModel::rgb:
					{
						guideArray[0][o + i] = oc.r;
						guideArray[1][o + i] = oc.g;
						guideArray[2][o + i] = oc.b;
						break;
					}
					case EColorModel::hsv:
					{
						const THsv<float> tc(convertToHsv(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.v;
						break;
					}
					case EColorModel::hsl:
					{
						const THsl<float> tc(convertToHsl(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.l;
						break;
					}
					case EColorModel::hsg:
					{
						const THsg<float> tc(convertToHsg(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.g;
						break;
					}
					case EColorModel::hsp:
					{
						const THsp<float> tc(convertToHsp(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.p;
						break;
					}
					case EColorModel::cieHcl:
					{
						const TCieHcl<float> tc(convertToCieHcl(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.c;
						guideArray[2][o + i] = tc.l;
						break;
					}
					default: break;
				}
			}
		}
		// Sort and reduce size by averaging
		for (CGuide& g : guideArray)
		{
			sort(g.begin(), g.end());
			if (sa > sl)
			{
				const int l = sa / sl;
				for (int i = 0; i < sl; ++i)
				{
					const int o = roundTo<int>(i * sa / static_cast<float>(sl));
					float s = 0.0F;
					for (int j = o; j < o + l; ++j) s += g[j];
					g[i] = s / l;
				}
				g.resize(sl);
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::calcModifiedImage(const CTrueColorImage& oi, CTrueColorImage& mi)
{
	if (!oi.IsNull())
	{
		using namespace std::chrono;
		const auto start = high_resolution_clock::now();
		#pragma omp parallel for
		for (int j = 0; j < oi.GetHeight(); ++j)
		{
			for (int i = 0; i < oi.GetWidth(); ++i)
			{
				const TRgb<float> oc(oi.getPixel<TColor<float>>(i, j));
				switch (colorModel)
				{
					case EColorModel::rgb:
					{
						TRgb<float> tc(oc);
						tc.r = limit(curveArray[0].calcValue(tc.r), 0.0F, 1.0F);
						tc.g = limit(curveArray[1].calcValue(tc.g), 0.0F, 1.0F);
						tc.b = limit(curveArray[2].calcValue(tc.b), 0.0F, 1.0F);
						mi.setPixel(i, j, tc);
						break;
					}
					case EColorModel::hsv:
					{
						THsv<float> tc(convertToHsv(oc));
						tc.h = limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.v = limit(curveArray[2].calcValue(tc.v), 0.0F, 1.0F);
						mi.setPixel(i, j, convertToRgb(tc));
						break;
					}
					case EColorModel::hsl:
					{
						THsl<float> tc(convertToHsl(oc));
						tc.h = limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.l = limit(curveArray[2].calcValue(tc.l), 0.0F, 1.0F);
						mi.setPixel(i, j, convertToRgb(tc));
						break;
					}
					case EColorModel::hsg:
					{
						THsg<float> tc(convertToHsg(oc));
						tc.h = limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.g = limit(curveArray[2].calcValue(tc.g), 0.0F, 1.0F);
						mi.setPixel(i, j, convertToRgb(tc));
						break;
					}
					case EColorModel::hsp:
					{
						THsp<float> tc(convertToHsp(oc));
						tc.h = limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.p = limit(curveArray[2].calcValue(tc.p), 0.0F, 1.0F);
						mi.setPixel(i, j, convertToRgb(tc));
						break;
					}
					case EColorModel::cieHcl:
					{
						TCieHcl<float> c1(convertToCieHcl(oc));
						c1.h = limit(curveArray[0].calcValue(c1.h), 0.0F, 1.0F);
						c1.c = limit(curveArray[1].calcValue(c1.c), 0.0F, 1.0F);
						c1.l = limit(curveArray[2].calcValue(c1.l), 0.0F, 1.0F);
						const TRgb<float> c2(convertToRgb(c1));
						if (allowClamping)
						{
							mi.setPixel(i, j, TRgb<float>(
								limit(c2.r, 0.0F, 1.0F), 
								limit(c2.g, 0.0F, 1.0F), 
								limit(c2.b, 0.0F, 1.0F)));
						}
						else
						{
							if (isInRange(c2.r, c2.g, c2.b)) mi.setPixel(i, j, c2);
							else mi.setPixel(i, j, TRgb<float>(0.0F, 0.0F, 0.0F));
						}
						break;
					}
					default: break;
				}
			}
		}
		const auto end = high_resolution_clock::now();
		const auto diff = duration_cast<milliseconds>(end - start).count();
		updateCalcTimeLabel(static_cast<int>(diff));
	}
	else
	{
		updateCalcTimeLabel(-1);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - OVERRIDE
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs)) return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT, CCreateContext* p_cc)
{
	if (!splitterWnd.CreateStatic(this, 5, 1, WS_CHILD | WS_VISIBLE | WS_BORDER)) return FALSE;
	if (!splitterWnd.CreateView(0, 0, RUNTIME_CLASS(CCurveView), {0, 0}, p_cc)) return FALSE;
	if (!splitterWnd.CreateView(1, 0, RUNTIME_CLASS(CCurveView), {0, 0}, p_cc)) return FALSE;
	if (!splitterWnd.CreateView(2, 0, RUNTIME_CLASS(CCurveView), {0, 0}, p_cc)) return FALSE;
	if (!splitterWnd.CreateView(3, 0, RUNTIME_CLASS(CImageView), {0, 0}, p_cc)) return FALSE;
	if (!splitterWnd.CreateView(4, 0, RUNTIME_CLASS(CHistogramView), {0, 0}, p_cc)) return FALSE;
	p_curve1View = static_cast<CCurveView*>(splitterWnd.GetPane(0, 0));
	p_curve2View = static_cast<CCurveView*>(splitterWnd.GetPane(1, 0));
	p_curve3View = static_cast<CCurveView*>(splitterWnd.GetPane(2, 0));
	p_imageView = static_cast<CImageView*>(splitterWnd.GetPane(3, 0));
	p_histogramView = static_cast<CHistogramView*>(splitterWnd.GetPane(4, 0));
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (splitterWnd.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;
	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_MENUSELECT()
	ON_COMMAND(ID_FILE_LOAD_IMAGE, &CMainFrame::OnFileLoadImage)
	ON_COMMAND(ID_FILE_SAVE_IMAGE_AS, &CMainFrame::OnFileSaveImageAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_IMAGE_AS, &CMainFrame::OnUpdateFileSaveImageAs)
	ON_COMMAND_RANGE(ID_COLOR_MODEL_RGB, ID_COLOR_MODEL_CIE_HCL, &CMainFrame::OnColorModel)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COLOR_MODEL_RGB, ID_COLOR_MODEL_CIE_HCL, &CMainFrame::OnUpdateColorModel)
	ON_COMMAND_RANGE(ID_CURVE_TYPE_LINE_SEGMENTS, ID_CURVE_TYPE_FINITE_SPLINE, &CMainFrame::OnCurveType)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CURVE_TYPE_LINE_SEGMENTS, ID_CURVE_TYPE_FINITE_SPLINE, &CMainFrame::OnUpdateCurveType)
	ON_COMMAND(ID_OPTIONS_INSTANT_MODE, &CMainFrame::OnOptionsInstantMode)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_INSTANT_MODE, &CMainFrame::OnUpdateOptionsInstantMode)
	ON_COMMAND(ID_OPTIONS_ALLOW_CLAMPING, &CMainFrame::OnOptionsAllowClamping)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOW_CLAMPING, &CMainFrame::OnUpdateOptionsAllowClamping)
	ON_COMMAND_RANGE(ID_VIEW_ORIGINAL_IMAGE, ID_VIEW_MODIFIED_IMAGE, &CMainFrame::OnViewImage)
	ON_COMMAND(ID_VIEW_SWITCH_IMAGE, &CMainFrame::OnViewSwitchImage)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_ORIGINAL_IMAGE, ID_VIEW_MODIFIED_IMAGE, &CMainFrame::OnUpdateViewImage)
	ON_MESSAGE(GM_CURVE_CHANGED, &CMainFrame::OnCurveChanged)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT p_cs)
{
	if (CFrameWnd::OnCreate(p_cs) == -1) return -1;
	if (!statusBar.Create(this)) return -1;
	statusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	const int sx = ::GetSystemMetrics(SM_CXSCREEN);
	statusBar.SetPaneInfo(1, IDI_IMAGE_PATH, SBPS_NORMAL, sx * 28 / 100);
	statusBar.SetPaneInfo(2, IDI_IMAGE_SIZE, SBPS_NORMAL, sx * 7 / 100);
	statusBar.SetPaneInfo(3, IDI_CURVE_TYPE, SBPS_NORMAL, sx * 7 / 100);
	statusBar.SetPaneInfo(4, IDI_IMAGE_MODE, SBPS_NORMAL, sx * 7 / 100);
	statusBar.SetPaneInfo(5, IDI_CALC_TIME, SBPS_NORMAL, sx * 7 / 100);
	// Update
	updateColorModelLabels();
	updateImagePathLabel();
	updateImageSizeLabel();
	updateCurveTypeLabel();
	updateImageModeLabel();
	p_curve1View->update(&guideArray[0], &curveArray[0]);
	p_curve2View->update(&guideArray[1], &curveArray[1]);
	p_curve3View->update(&guideArray[2], &curveArray[2]);
	p_imageView->update(&modifiedImage);
	p_histogramView->update(&modifiedHistogramArray);
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnMenuSelect(UINT itemId, UINT flags, HMENU hSysMenu)
{
	auto updateLabel = [this] (const CString& t) {statusBar.SetPaneText(0, t);};
	switch (itemId)
	{
		case ID_VIEW_CURVE_1:
			switch (colorModel)
			{
				case EColorModel::rgb: updateLabel(L"Show the red curve"); return;
				case EColorModel::hsv: updateLabel(L"Show the hue curve"); return;
				case EColorModel::hsl: updateLabel(L"Show the hue curve"); return;
				case EColorModel::hsg: updateLabel(L"Show the hue curve"); return;
				case EColorModel::hsp: updateLabel(L"Show the hue curve"); return;
				case EColorModel::cieHcl: updateLabel(L"Show the hue curve"); return;
				default: break;
			}
			break;
		case ID_VIEW_CURVE_2:
			switch (colorModel)
			{
				case EColorModel::rgb: updateLabel(L"Show the green curve"); return;
				case EColorModel::hsv: updateLabel(L"Show the saturation curve"); return;
				case EColorModel::hsl: updateLabel(L"Show the saturation curve"); return;
				case EColorModel::hsg: updateLabel(L"Show the saturation curve"); return;
				case EColorModel::hsp: updateLabel(L"Show the saturation curve"); return;
				case EColorModel::cieHcl: updateLabel(L"Show the chroma curve"); return;
				default: break;
			}
			break;
		case ID_VIEW_CURVE_3:
			switch (colorModel)
			{
				case EColorModel::rgb: updateLabel(L"Show the blue curve"); return;
				case EColorModel::hsv: updateLabel(L"Show the value curve"); return;
				case EColorModel::hsl: updateLabel(L"Show the lightness curve"); return;
				case EColorModel::hsg: updateLabel(L"Show the gray scale curve"); return;
				case EColorModel::hsp: updateLabel(L"Show the perceived brightness curve"); return;
				case EColorModel::cieHcl: updateLabel(L"Show the lightness curve"); return;
				default: break;
			}
			break;
		default: break;
	}
	CFrameWnd::OnMenuSelect(itemId, flags, hSysMenu);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnFileLoadImage()
{
	CFileDialog dlg(
		TRUE, 0, 0, 
		OFN_ENABLESIZING | OFN_FILEMUSTEXIST, 
		L"BMP Files (*.bmp)|*.bmp|"
		L"PNG Files (*.png)|*.png|"
		L"JPEG Files (*.jpg;*.jpeg)|*.jpg;*.jpeg|"
		L"All Bitmap Files (*.bmp;*.png;*.jpg;*.jpeg)|*.bmp;*.png;*.jpg;*.jpeg||");
	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.nFilterIndex = 4;
	if (dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
		filterIndex = getFilterIndex(pathName);
		if (filterIndex > 0)
		{
			auto destroyImages = [this]()
			{
				image.Destroy();
				originalImage.Destroy();
				createGuideArray();
				modifiedImage.Destroy();
				originalHistogramArray.init(originalImage);
				modifiedHistogramArray.init(modifiedImage);
			};
			auto updateEmpty = [this]()
			{
				pathName.Empty();
				filterIndex = 0;
				updateImagePathLabel();
				updateImageSizeLabel();
				refreshCurveViews();
				p_imageView->refresh();
				p_histogramView->refresh();
			};
			::AfxGetApp()->DoWaitCursor(1);
			image.Destroy();
			if (const int b = image.load(pathName); b == 24)
			{
				if (const CSize is(image.getSize()); is.cx > 1 && is.cy > 1)
				{
					// Calculate new size, create and copy the images
					auto calcNewSize = [&is]()
					{
						const CSize ds(
							::GetSystemMetrics(SM_CXSCREEN) / 3 * 2, 
							::GetSystemMetrics(SM_CYSCREEN) / 3 * 2);
						const float isr = is.cy / static_cast<float>(is.cx);
						const float nsr = ds.cy / static_cast<float>(ds.cx);
						const CSize ns = isr < nsr ? 
							CSize(ds.cx, static_cast<LONG>(ds.cx * isr)) : 
							CSize(static_cast<LONG>(ds.cy / isr), ds.cy);
						return CSize((std::min)(is.cx, ns.cx), (std::min)(is.cy, ns.cy));
					};
					const CSize ns(calcNewSize());
					originalImage.create(ns, 24);
					modifiedImage.create(ns, 24);
					for (int j = 0; j < ns.cy; ++j)
					{
						const int iy = static_cast<int>(j / (ns.cy - 1.0F) * (is.cy - 1.0F));
						for (int i = 0; i < ns.cx; ++i)
						{
							const int ix = static_cast<int>(i / (ns.cx - 1.0F) * (is.cx - 1.0F));
							originalImage.setPixel(i, j, image.getPixel<TColor<BYTE>>(ix, iy));
						}
					}
					// Update
					createGuideArray();
					calcModifiedImage(originalImage, modifiedImage);
					originalHistogramArray.init(originalImage);
					modifiedHistogramArray.init(modifiedImage);
					::AfxGetApp()->DoWaitCursor(-1);
					updateImagePathLabel();
					updateImageSizeLabel();
					pathName = removeImgExt(pathName);
					p_imageView->recalcLayout();
					refreshCurveViews();
					p_imageView->refresh();
					p_histogramView->refresh();
				}
				else
				{
					destroyImages();
					::AfxGetApp()->DoWaitCursor(-1);
					::AfxMessageBox(L"Image too small");
					updateEmpty();
				}
			}
			else if (b >= 0)
			{
				destroyImages();
				::AfxGetApp()->DoWaitCursor(-1);
				::AfxMessageBox(L"No 24-bit image");
				updateEmpty();
			}
			else
			{
				::AfxGetApp()->DoWaitCursor(-1);
				::AfxMessageBox(L"Load error");
				updateEmpty();
			}
		}
		else
		{
			::AfxMessageBox(L"Wrong file extension");
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnFileSaveImageAs()
{
	if (!image.IsNull())
	{
		CFileDialog dlg(
			FALSE, 0, 0,
			OFN_ENABLESIZING,
			L"BMP Files (*.bmp)|*.bmp|"
			L"PNG Files (*.png)|*.png|"
			L"JPEG Files (*.jpg;*.jpeg)|*.jpg;*.jpeg||");
		OPENFILENAME& ofn = dlg.GetOFN();
		wcscpy_s(ofn.lpstrFile, _MAX_FNAME, pathName);
		ofn.nFilterIndex = filterIndex;
		if (dlg.DoModal() == IDOK)
		{
			::AfxGetApp()->DoWaitCursor(1);
			if (CTrueColorImage ni; ni.create(image.getSize(), image.GetBPP()))
			{
				CString pn(removeImgExt(dlg.GetPathName()));
				switch (dlg.m_ofn.nFilterIndex)
				{
					case 1: pn += L".bmp"; break;
					case 2: pn += L".png"; break;
					case 3: pn += L".jpeg"; break;
					default: break;
				}
				if (std::filesystem::exists(LPCTSTR(pn)))
				{
					CString ms;
					ms.Format(L"The file %s already exists.\nDo you want to replace it?", LPCTSTR(pn));
					if (::AfxMessageBox(ms, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2) == IDNO)
					{
						::AfxGetApp()->DoWaitCursor(-1);
						return;
					}
				}
				calcModifiedImage(image, ni);
				if (ni.Save(pn) != S_OK)
				{
					::AfxGetApp()->DoWaitCursor(-1);
					::AfxMessageBox(L"Save error");
				}
			}
			else
			{
				::AfxGetApp()->DoWaitCursor(-1);
				::AfxMessageBox(L"Save error");
			}
			::AfxGetApp()->DoWaitCursor(-1);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateFileSaveImageAs(CCmdUI* p_cmdUI)
{
	p_cmdUI->Enable(!image.IsNull());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnColorModel(UINT id)
{
	colorModel = static_cast<EColorModel>(id - ID_COLOR_MODEL_RGB);
	createGuideArray();
	calcModifiedImage(originalImage, modifiedImage);
	modifiedHistogramArray.init(modifiedImage);
	updateColorModelLabels();
	refreshCurveViews();
	refreshModifiedData();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateColorModel(CCmdUI* p_cmdUI)
{
	p_cmdUI->SetCheck(static_cast<UINT>(colorModel) == p_cmdUI->m_nID - ID_COLOR_MODEL_RGB);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnCurveType(UINT id)
{
	ECurveType t = static_cast<ECurveType>(id - ID_CURVE_TYPE_LINE_SEGMENTS);
	for (CCurve& c : curveArray)
	{
		c.setCurveType(t);
		c.initControlPointVect();
	}
	calcModifiedImage(originalImage, modifiedImage);
	modifiedHistogramArray.init(modifiedImage);
	updateCurveTypeLabel();
	refreshCurveViews();
	refreshModifiedData();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateCurveType(CCmdUI* p_cmdUI)
{
	p_cmdUI->SetRadio(static_cast<UINT>(curveArray[0].getCurveType()) == p_cmdUI->m_nID - ID_CURVE_TYPE_LINE_SEGMENTS);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnOptionsInstantMode()
{
	instantMode = !instantMode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateOptionsInstantMode(CCmdUI* p_cmdUI)
{
	p_cmdUI->SetCheck(instantMode ? 1 : 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnOptionsAllowClamping()
{
	allowClamping = !allowClamping;
	calcModifiedImage(originalImage, modifiedImage);
	modifiedHistogramArray.init(modifiedImage);
	refreshModifiedData();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateOptionsAllowClamping(CCmdUI* p_cmdUI)
{
	p_cmdUI->SetCheck(allowClamping ? 1 : 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnViewImage(UINT id)
{
	if (id == ID_VIEW_MODIFIED_IMAGE)
	{
		if (imageMode == EImageMode::original)
		{
			imageMode = EImageMode::modified;
			updateImageModeLabel();
			p_imageView->update(&modifiedImage);
			p_histogramView->update(&modifiedHistogramArray);
			p_imageView->refresh();
			p_histogramView->refresh();
		}
	}
	else if (id == ID_VIEW_ORIGINAL_IMAGE)
	{
		if (imageMode == EImageMode::modified)
		{
			imageMode = EImageMode::original;
			updateImageModeLabel();
			p_imageView->update(&originalImage);
			p_histogramView->update(&originalHistogramArray);
			p_imageView->refresh();
			p_histogramView->refresh();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnViewSwitchImage()
{
	if (imageMode == EImageMode::modified)
	{
		OnViewImage(ID_VIEW_ORIGINAL_IMAGE);
	}
	else
	{
		OnViewImage(ID_VIEW_MODIFIED_IMAGE);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateViewImage(CCmdUI* p_cmdUI)
{
	p_cmdUI->SetRadio(static_cast<UINT>(imageMode) == p_cmdUI->m_nID - ID_VIEW_ORIGINAL_IMAGE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnCurveChanged(WPARAM wp, LPARAM)
{
	if (wp == 0U || wp == 1U && instantMode)
	{
		calcModifiedImage(originalImage, modifiedImage);
		modifiedHistogramArray.init(modifiedImage);
		refreshModifiedData();
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - GLOBAL FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int getFilterIndex(const CString& pn)
{
	const CString ts(pn.Right(4));
	if (!ts.CompareNoCase(L".bmp")) return 1;
	if (!ts.CompareNoCase(L".png")) return 2;
	if (!ts.CompareNoCase(L".jpg")) return 3;
	if (!pn.Right(4).CompareNoCase(L".jpeg")) return 3;
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CString removeImgExt(const CString& pn)
{
	const CString ts(pn.Right(4));
	if (!ts.CompareNoCase(L".bmp") || !ts.CompareNoCase(L".png") || !ts.CompareNoCase(L".jpg"))
	{
		return pn.Left(pn.GetLength() - 4);
	}
	if (!pn.Right(5).CompareNoCase(L".jpeg"))
	{
		return pn.Left(pn.GetLength() - 5);
	}
	return pn;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

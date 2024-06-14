//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MainFrame.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019-2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "pch.h"
#include "framework.h"
#include "MainFrame.h"
#include "HspGradatorApp.h"
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
void CFixedSplitterWnd::OnSize(UINT nType, int cx, int cy)
{
	wndSize = {cx, cy};
	CSplitterWnd::OnSize(nType, cx, cy);
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
void CFixedSplitterWnd::OnUpdateViewCurve(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(curveVisibleArray[pCmdUI->m_nID - ID_VIEW_CURVE_1]);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - STATIC DATA
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const std::array<UINT, 6> CMainFrame::indicatorArray = {ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CMainFrame::CMainFrame() : 
	splitterWnd(), 
	statusBar(), 
	p_curve1View(nullptr), 
	p_curve2View(nullptr), 
	p_curve3View(nullptr), 
	p_imageView(nullptr), 
	p_histogramView(nullptr), 
	image(), 
	originalImage(), 
	modifiedImage(), 
	originalHistogramArray(), 
	modifiedHistogramArray(), 
	guideArray(), 
	curveArray(), 
	imageFilePath(), 
	filterIndex(0), 
	colorModel(ColorModel::hsp), 
	instantMode(true), 
	allowClamping(true), 
	imageMode(ImageMode::modified)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - ELEMENT FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateColorModelMenu()
{
	auto updateMenu = [this](LPCTSTR s1, LPCTSTR s2, LPCTSTR s3)
	{
		if (CMenu* const p_m = GetMenu())
		{
			p_m->ModifyMenuW(ID_VIEW_CURVE_1, MF_BYCOMMAND | MF_STRING, ID_VIEW_CURVE_1, s1);
			p_m->ModifyMenuW(ID_VIEW_CURVE_2, MF_BYCOMMAND | MF_STRING, ID_VIEW_CURVE_2, s2);
			p_m->ModifyMenuW(ID_VIEW_CURVE_3, MF_BYCOMMAND | MF_STRING, ID_VIEW_CURVE_3, s3);
		}
	};
	switch (colorModel)
	{
		case ColorModel::rgb:
			updateMenu(L"&Red Curve\tA", L"&Green Curve\tS", L"&Blue Curve\tD");
			break;
		case ColorModel::hsv:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Value Curve\tD");
			break;
		case ColorModel::hsl:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Lightness Curve\tD");
			break;
		case ColorModel::hsg:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Gray Scale Curve\tD");
			break;
		case ColorModel::hsp:
			updateMenu(L"&Hue Curve\tA", L"&Saturation Curve\tS", L"&Perceived Brightness Curve\tD");
			break;
		case ColorModel::cieHcl:
			updateMenu(L"&Hue Curve\tA", L"&Chroma Curve\tS", L"&Lightness Curve\tD");
			break;
		default: break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateColorModelLabels()
{
	auto updateCurveViews = [this](std::wstring s1, std::wstring s2, std::wstring s3)
	{
		p_curve1View->setLabelString(std::move(s1));
		p_curve2View->setLabelString(std::move(s2));
		p_curve3View->setLabelString(std::move(s3));
	};
	switch (colorModel)
	{
		case ColorModel::rgb:
			updateCurveViews(L"RGB - Red", L"RGB - Green", L"RGB - Blue");
			break;
		case ColorModel::hsv:
			updateCurveViews(L"HSV - Hue", L"HSV - Saturation", L"HSV - Value");
			break;
		case ColorModel::hsl:
			updateCurveViews(L"HSL - Hue", L"HSL - Saturation", L"HSL - Lightness");
			break;
		case ColorModel::hsg:
			updateCurveViews(L"HSG - Hue", L"HSG - Saturation", L"HSG - Gray Scale");
			break;
		case ColorModel::hsp:
			updateCurveViews(L"HSP - Hue", L"HSP - Saturation", L"HSP - Perceived Brightness");
			break;
		case ColorModel::cieHcl:
			updateCurveViews(L"CIE-HCL - Hue", L"CIE-HCL - Chroma", L"CIE-HCL - Lightness");
			break;
		default: break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateImageFilePathStatusText()
{
	statusBar.SetPaneText(1, !imageFilePath.empty() ? imageFilePath.c_str() : L"Empty");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateImageSizeStatusText()
{
	if (const CSize is = image.getSize(); is.cx > 0 && is.cy > 0)
	{
		statusBar.SetPaneText(2, std::format(L"{} x {} px", is.cx, is.cy).data());
	}
	else
	{
		statusBar.SetPaneText(2, L"");
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateCurveTypeStatusText()
{
	switch (curveArray[0].getType())
	{
		case Curve::Type::lineSegments: statusBar.SetPaneText(3, L"Line Segments Curve"); break;
		case Curve::Type::cubicSpline: statusBar.SetPaneText(3, L"Cubic Spline Curve"); break;
		case Curve::Type::finiteSpline: statusBar.SetPaneText(3, L"Finite Spline Curve"); break;
		default: break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateImageModeStatusText()
{
	statusBar.SetPaneText(4, imageMode == ImageMode::original ? L"Original Image" : L"Modified Image");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::updateCalcTimeStatusText(const int t)
{
	if (t >= 0)
	{
		statusBar.SetPaneText(5, std::format(L"{}ms", t).data());
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
	if (imageMode == ImageMode::modified)
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
	for (Guide& g : guideArray) g.resize(sa);
	if (sa > 0)
	{
		for (int j = 0; j < ss.cy; ++j)
		{
			const int sj = static_cast<int>(j / (ss.cy - 1.0F) * (is.cy - 1.0F));
			const int o = j * ss.cx;
			for (int i = 0; i < ss.cx; ++i)
			{
				const int si = static_cast<int>(i / (ss.cx - 1.0F) * (is.cx - 1.0F));
				const ama::Rgb<float> oc(originalImage.getPixel<ama::Color<float>>(si, sj));
				switch (colorModel)
				{
					case ColorModel::rgb:
					{
						guideArray[0][o + i] = oc.r;
						guideArray[1][o + i] = oc.g;
						guideArray[2][o + i] = oc.b;
						break;
					}
					case ColorModel::hsv:
					{
						const ama::Hsv<float> tc(ama::convertToHsv(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.v;
						break;
					}
					case ColorModel::hsl:
					{
						const ama::Hsl<float> tc(ama::convertToHsl(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.l;
						break;
					}
					case ColorModel::hsg:
					{
						const ama::Hsg<float> tc(ama::convertToHsg(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.g;
						break;
					}
					case ColorModel::hsp:
					{
						const ama::Hsp<float> tc(ama::convertToHsp(oc));
						guideArray[0][o + i] = tc.h;
						guideArray[1][o + i] = tc.s;
						guideArray[2][o + i] = tc.p;
						break;
					}
					case ColorModel::cieHcl:
					{
						const ama::CieHcl<float> tc(ama::convertToCieHcl(oc));
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
		for (Guide& g : guideArray)
		{
			std::sort(g.begin(), g.end());
			if (sa > sl)
			{
				const int l = sa / sl;
				for (int i = 0; i < sl; ++i)
				{
					const int o = ama::round(i * sa / static_cast<float>(sl));
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
		const auto st = steady_clock::now(); // Start time
		#pragma omp parallel for
		for (int j = 0; j < oi.GetHeight(); ++j)
		{
			for (int i = 0; i < oi.GetWidth(); ++i)
			{
				const ama::Rgb<float> oc(oi.getPixel<ama::Color<float>>(i, j));
				switch (colorModel)
				{
					case ColorModel::rgb:
					{
						ama::Rgb<float> tc(oc);
						tc.r = ama::limit(curveArray[0].calcValue(tc.r), 0.0F, 1.0F);
						tc.g = ama::limit(curveArray[1].calcValue(tc.g), 0.0F, 1.0F);
						tc.b = ama::limit(curveArray[2].calcValue(tc.b), 0.0F, 1.0F);
						mi.setPixel(i, j, tc);
						break;
					}
					case ColorModel::hsv:
					{
						ama::Hsv<float> tc(ama::convertToHsv(oc));
						tc.h = ama::limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = ama::limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.v = ama::limit(curveArray[2].calcValue(tc.v), 0.0F, 1.0F);
						mi.setPixel(i, j, ama::convertToRgb(tc));
						break;
					}
					case ColorModel::hsl:
					{
						ama::Hsl<float> tc(ama::convertToHsl(oc));
						tc.h = ama::limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = ama::limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.l = ama::limit(curveArray[2].calcValue(tc.l), 0.0F, 1.0F);
						mi.setPixel(i, j, ama::convertToRgb(tc));
						break;
					}
					case ColorModel::hsg:
					{
						ama::Hsg<float> tc(ama::convertToHsg(oc));
						tc.h = ama::limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = ama::limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.g = ama::limit(curveArray[2].calcValue(tc.g), 0.0F, 1.0F);
						mi.setPixel(i, j, ama::convertToRgb(tc));
						break;
					}
					case ColorModel::hsp:
					{
						ama::Hsp<float> tc(ama::convertToHsp(oc));
						tc.h = ama::limit(curveArray[0].calcValue(tc.h), 0.0F, 1.0F);
						tc.s = ama::limit(curveArray[1].calcValue(tc.s), 0.0F, 1.0F);
						tc.p = ama::limit(curveArray[2].calcValue(tc.p), 0.0F, 1.0F);
						mi.setPixel(i, j, ama::convertToRgb(tc));
						break;
					}
					case ColorModel::cieHcl:
					{
						ama::CieHcl<float> c1(ama::convertToCieHcl(oc));
						c1.h = ama::limit(curveArray[0].calcValue(c1.h), 0.0F, 1.0F);
						c1.c = ama::limit(curveArray[1].calcValue(c1.c), 0.0F, 1.0F);
						c1.l = ama::limit(curveArray[2].calcValue(c1.l), 0.0F, 1.0F);
						const ama::Rgb<float> c2(ama::convertToRgb(c1));
						if (allowClamping)
						{
							mi.setPixel(i, j, ama::Rgb<float>(
								ama::limit(c2.r, 0.0F, 1.0F), 
								ama::limit(c2.g, 0.0F, 1.0F), 
								ama::limit(c2.b, 0.0F, 1.0F)));
						}
						else
						{
							if (ama::isInRange(c2.r, c2.g, c2.b)) mi.setPixel(i, j, c2);
							else mi.setPixel(i, j, ama::Rgb<float>(0.0F, 0.0F, 0.0F));
						}
						break;
					}
					default: break;
				}
			}
		}
		const long long ct = duration_cast<milliseconds>(steady_clock::now() - st).count();
		updateCalcTimeStatusText(static_cast<int>(ct));
	}
	else
	{
		updateCalcTimeStatusText();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - OVERRIDE
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (CFrameWnd::PreCreateWindow(cs))
	{
		cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
		cs.lpszClass = AfxRegisterWndClass(0);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	if (splitterWnd.CreateStatic(this, 5, 1, WS_CHILD | WS_VISIBLE | WS_BORDER) && 
		splitterWnd.CreateView(0, 0, RUNTIME_CLASS(CCurveView), {0, 0}, pContext) && 
		splitterWnd.CreateView(1, 0, RUNTIME_CLASS(CCurveView), {0, 0}, pContext) && 
		splitterWnd.CreateView(2, 0, RUNTIME_CLASS(CCurveView), {0, 0}, pContext) && 
		splitterWnd.CreateView(3, 0, RUNTIME_CLASS(CImageView), {0, 0}, pContext) && 
		splitterWnd.CreateView(4, 0, RUNTIME_CLASS(CHistogramView), {0, 0}, pContext))
	{
		p_curve1View = static_cast<CCurveView*>(splitterWnd.GetPane(0, 0));
		p_curve2View = static_cast<CCurveView*>(splitterWnd.GetPane(1, 0));
		p_curve3View = static_cast<CCurveView*>(splitterWnd.GetPane(2, 0));
		p_imageView = static_cast<CImageView*>(splitterWnd.GetPane(3, 0));
		p_histogramView = static_cast<CHistogramView*>(splitterWnd.GetPane(4, 0));
		return TRUE;
	}
	return FALSE;
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
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == 0)
	{
		if (statusBar.Create(this))
		{
			statusBar.SetIndicators(
				indicatorArray.data(), 
				static_cast<int>(indicatorArray.size()));
			const int sx = ::GetSystemMetrics(SM_CXSCREEN);
			statusBar.SetPaneInfo(1, IDI_IMAGE_PATH, SBPS_NORMAL, sx * 28 / 100);
			statusBar.SetPaneInfo(2, IDI_IMAGE_SIZE, SBPS_NORMAL, sx * 7 / 100);
			statusBar.SetPaneInfo(3, IDI_CURVE_TYPE, SBPS_NORMAL, sx * 7 / 100);
			statusBar.SetPaneInfo(4, IDI_IMAGE_MODE, SBPS_NORMAL, sx * 7 / 100);
			statusBar.SetPaneInfo(5, IDI_CALC_TIME, SBPS_NORMAL, sx * 7 / 100);
			// Update
			updateColorModelMenu();
			updateColorModelLabels();
			updateImageFilePathStatusText();
			updateImageSizeStatusText();
			updateCurveTypeStatusText();
			updateImageModeStatusText();
			p_curve1View->update(&guideArray[0], &curveArray[0]);
			p_curve2View->update(&guideArray[1], &curveArray[1]);
			p_curve3View->update(&guideArray[2], &curveArray[2]);
			p_imageView->update(&modifiedImage);
			p_histogramView->update(&modifiedHistogramArray);
			return 0;
		}
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	auto setStatusBarText = [this] (const std::wstring& s) {statusBar.SetPaneText(0, s.data());};
	switch (nItemID)
	{
		case ID_VIEW_CURVE_1:
			switch (colorModel)
			{
				case ColorModel::rgb: setStatusBarText(L"Show the red curve"); return;
				case ColorModel::hsv: setStatusBarText(L"Show the hue curve"); return;
				case ColorModel::hsl: setStatusBarText(L"Show the hue curve"); return;
				case ColorModel::hsg: setStatusBarText(L"Show the hue curve"); return;
				case ColorModel::hsp: setStatusBarText(L"Show the hue curve"); return;
				case ColorModel::cieHcl: setStatusBarText(L"Show the hue curve"); return;
				default: break;
			}
			break;
		case ID_VIEW_CURVE_2:
			switch (colorModel)
			{
				case ColorModel::rgb: setStatusBarText(L"Show the green curve"); return;
				case ColorModel::hsv: setStatusBarText(L"Show the saturation curve"); return;
				case ColorModel::hsl: setStatusBarText(L"Show the saturation curve"); return;
				case ColorModel::hsg: setStatusBarText(L"Show the saturation curve"); return;
				case ColorModel::hsp: setStatusBarText(L"Show the saturation curve"); return;
				case ColorModel::cieHcl: setStatusBarText(L"Show the chroma curve"); return;
				default: break;
			}
			break;
		case ID_VIEW_CURVE_3:
			switch (colorModel)
			{
				case ColorModel::rgb: setStatusBarText(L"Show the blue curve"); return;
				case ColorModel::hsv: setStatusBarText(L"Show the value curve"); return;
				case ColorModel::hsl: setStatusBarText(L"Show the lightness curve"); return;
				case ColorModel::hsg: setStatusBarText(L"Show the gray scale curve"); return;
				case ColorModel::hsp: setStatusBarText(L"Show the perceived brightness curve"); return;
				case ColorModel::cieHcl: setStatusBarText(L"Show the lightness curve"); return;
				default: break;
			}
			break;
		default: break;
	}
	CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnFileLoadImage()
{
	CFileDialog dlg(TRUE, 0, 0, 
		OFN_ENABLESIZING | OFN_FILEMUSTEXIST, 
		L"BMP Files (*.bmp)|*.bmp|"
		L"PNG Files (*.png)|*.png|"
		L"JPG Files (*.jpg)|*.jpg|"
		L"JPEG Files (*.jpeg)|*.jpeg|"
		L"All Bitmap Files (*.bmp;*.png;*.jpg;*.jpeg)|*.bmp;*.png;*.jpg;*.jpeg||", 
		nullptr, 0, FALSE);
	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.nFilterIndex = 5;
	if (dlg.DoModal() == IDOK)
	{
		imageFilePath = dlg.GetPathName().GetString();
		if (filterIndex = getFilterIndex(imageFilePath); filterIndex > 0)
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
				imageFilePath.clear();
				filterIndex = 0;
				updateImageFilePathStatusText();
				updateImageSizeStatusText();
				refreshCurveViews();
				p_imageView->refresh();
				p_histogramView->refresh();
			};
			::AfxGetApp()->DoWaitCursor(1);
			image.Destroy();
			if (const int b = image.load(imageFilePath.c_str()); b == 24)
			{
				if (const CSize is(image.getSize()); is.cx > 1 && is.cy > 1)
				{
					// Calculate new size, create and copy the images
					auto calcNewSize = [&is]()
					{
						const CSize ds(
							::GetSystemMetrics(SM_CXSCREEN) * 2 / 3, 
							::GetSystemMetrics(SM_CYSCREEN) * 2 / 3);
						const float iar = is.cx / static_cast<float>(is.cy);
						const float nar = ds.cx / static_cast<float>(ds.cy);
						const CSize ns = iar < nar ? 
							CSize(static_cast<LONG>(ds.cy * iar), ds.cy) : 
							CSize(ds.cx, static_cast<LONG>(ds.cx / iar));
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
							originalImage.setPixel(i, j, image.getPixel<ama::Color<BYTE>>(ix, iy));
						}
					}
					// Update
					createGuideArray();
					calcModifiedImage(originalImage, modifiedImage);
					originalHistogramArray.init(originalImage);
					modifiedHistogramArray.init(modifiedImage);
					::AfxGetApp()->DoWaitCursor(-1);
					updateImageFilePathStatusText();
					updateImageSizeStatusText();
					imageFilePath = removeImgExt(imageFilePath);
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
		CFileDialog dlg(FALSE, 0, 0, 
			OFN_ENABLESIZING, 
			L"BMP Files (*.bmp)|*.bmp|"
			L"PNG Files (*.png)|*.png|"
			L"JPG Files (*.jpg)|*.jpg|"
			L"JPEG Files (*.jpeg)|*.jpeg||", 
			nullptr, 0, FALSE);
		OPENFILENAME& ofn = dlg.GetOFN();
		wcscpy_s(ofn.lpstrFile, _MAX_FNAME, imageFilePath.c_str());
		ofn.nFilterIndex = filterIndex;
		if (dlg.DoModal() == IDOK)
		{
			::AfxGetApp()->DoWaitCursor(1);
			if (CTrueColorImage ni; ni.create(image.getSize(), image.GetBPP()))
			{
				std::filesystem::path fp(removeImgExt(dlg.GetPathName().GetString()));
				switch (dlg.m_ofn.nFilterIndex)
				{
					case 1: fp += L".bmp"; break;
					case 2: fp += L".png"; break;
					case 3: fp += L".jpg"; break;
					case 4: fp += L".jpeg"; break;
					default: break;
				}
				if (std::filesystem::exists(fp))
				{
					if (::AfxMessageBox(std::format(L"The file {} already exists.\nDo you want to replace it?", fp.c_str()).data(),
						MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2) == IDNO)
					{
						::AfxGetApp()->DoWaitCursor(-1);
						return;
					}
				}
				calcModifiedImage(image, ni);
				if (ni.Save(fp.c_str()) != S_OK)
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
void CMainFrame::OnUpdateFileSaveImageAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!image.IsNull());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnColorModel(UINT id)
{
	colorModel = static_cast<ColorModel>(id - ID_COLOR_MODEL_RGB);
	createGuideArray();
	calcModifiedImage(originalImage, modifiedImage);
	modifiedHistogramArray.init(modifiedImage);
	updateColorModelMenu();
	updateColorModelLabels();
	refreshCurveViews();
	refreshModifiedData();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateColorModel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(static_cast<UINT>(colorModel) == pCmdUI->m_nID - ID_COLOR_MODEL_RGB);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnCurveType(UINT id)
{
	Curve::Type t = static_cast<Curve::Type>(id - ID_CURVE_TYPE_LINE_SEGMENTS);
	for (Curve& c : curveArray)
	{
		c.setType(t);
		c.initControlPointVect();
	}
	calcModifiedImage(originalImage, modifiedImage);
	modifiedHistogramArray.init(modifiedImage);
	updateCurveTypeStatusText();
	refreshCurveViews();
	refreshModifiedData();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateCurveType(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(static_cast<UINT>(curveArray[0].getType()) == pCmdUI->m_nID - ID_CURVE_TYPE_LINE_SEGMENTS);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnOptionsInstantMode()
{
	instantMode = !instantMode;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateOptionsInstantMode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(instantMode ? 1 : 0);
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
void CMainFrame::OnUpdateOptionsAllowClamping(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(allowClamping ? 1 : 0);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnViewImage(UINT id)
{
	if (id == ID_VIEW_MODIFIED_IMAGE)
	{
		if (imageMode == ImageMode::original)
		{
			imageMode = ImageMode::modified;
			updateImageModeStatusText();
			p_imageView->update(&modifiedImage);
			p_imageView->refresh();
			p_histogramView->update(&modifiedHistogramArray);
			p_histogramView->refresh();
		}
	}
	else if (id == ID_VIEW_ORIGINAL_IMAGE)
	{
		if (imageMode == ImageMode::modified)
		{
			imageMode = ImageMode::original;
			updateImageModeStatusText();
			p_imageView->update(&originalImage);
			p_imageView->refresh();
			p_histogramView->update(&originalHistogramArray);
			p_histogramView->refresh();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnViewSwitchImage()
{
	if (imageMode == ImageMode::modified)
	{
		OnViewImage(ID_VIEW_ORIGINAL_IMAGE);
	}
	else
	{
		OnViewImage(ID_VIEW_MODIFIED_IMAGE);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CMainFrame::OnUpdateViewImage(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(static_cast<UINT>(imageMode) == pCmdUI->m_nID - ID_VIEW_ORIGINAL_IMAGE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CMainFrame::OnCurveChanged(WPARAM wParam, LPARAM /*lParam*/)
{
	if (wParam == 0U || wParam == 1U && instantMode)
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
int getFilterIndex(const std::filesystem::path& fp)
{
	std::wstring fe(fp.extension());
	std::transform(fe.cbegin(), fe.cend(), fe.begin(), towlower);
	if (fe == L".bmp") return 1;
	if (fe == L".png") return 2;
	if (fe == L".jpg") return 3;
	if (fe == L".jpeg") return 4;
	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::filesystem::path removeImgExt(const std::filesystem::path& ofp)
{
	std::filesystem::path nfp(ofp);
	std::wstring fe(nfp.extension());
	std::transform(fe.cbegin(), fe.cend(), fe.begin(), towlower);
	if (fe == L".bmp" || fe == L".png" || fe == L".jpg"  || fe == L".jpeg")
	{
		nfp.replace_extension();
	}
	return nfp;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

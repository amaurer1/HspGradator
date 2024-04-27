//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MainFrame.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CurveView.h"
#include "ImageView.h"
#include "HistogramView.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CFixedSplitterWnd
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CFixedSplitterWnd : public CSplitterWnd
{
// ELEMENT DATA
private:
	CSize wndSize;
	std::array<bool, 3> curveVisibleArray;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CFixedSplitterWnd();

// OVERRIDES
public:
	virtual void RecalcLayout() final;

// MESSAGE MAP FUNCTIONS
protected:
	afx_msg void OnSize(UINT, int, int);
	afx_msg void OnViewCurve(UINT);
	afx_msg void OnUpdateViewCurve(CCmdUI*);
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CMainFrame : public CFrameWnd
{
// ENUMS
private:
	enum class ColorModel {rgb, hsv, hsl, hsg, hsp, cieHcl};
	enum class ImageMode {original, modified};

// STATIC DATA
private:
	static const std::array<UINT, 6> indicatorArray;

// ELEMENT DATA
private:
	CFixedSplitterWnd splitterWnd;
	CStatusBar statusBar;
	CCurveView* p_curve1View;
	CCurveView* p_curve2View;
	CCurveView* p_curve3View;
	CImageView* p_imageView;
	CHistogramView* p_histogramView;
	CTrueColorImage image;
	CTrueColorImage originalImage;
	CTrueColorImage modifiedImage;
	HistogramArray originalHistogramArray;
	HistogramArray modifiedHistogramArray;
	std::array<Guide, 3> guideArray;
	std::array<Curve, 3> curveArray;
	std::filesystem::path imageFilePath;
	int filterIndex;
	ColorModel colorModel;
	bool instantMode;
	bool allowClamping;
	ImageMode imageMode;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

// ELEMENT FUNCTIONS
private:
	void updateColorModelMenu();
	void updateColorModelLabels();
	void updateImageFilePathStatusText();
	void updateImageSizeStatusText();
	void updateCurveTypeStatusText();
	void updateImageModeStatusText();
	void updateCalcTimeStatusText(const int = -1);
	void refreshCurveViews();
	void refreshModifiedData();
	void createGuideArray();
	void calcModifiedImage(const CTrueColorImage&, CTrueColorImage&);

// OVERRIDES
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT&) final;
	virtual BOOL OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) final;
	virtual BOOL OnCreateClient(LPCREATESTRUCT, CCreateContext*) final;

// MESSAGE MAP FUNCTIONS
protected:
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnMenuSelect(UINT, UINT, HMENU);
	afx_msg void OnFileLoadImage();
	afx_msg void OnFileSaveImageAs();
	afx_msg void OnUpdateFileSaveImageAs(CCmdUI*);
	afx_msg void OnColorModel(UINT);
	afx_msg void OnUpdateColorModel(CCmdUI*);
	afx_msg void OnCurveType(UINT);
	afx_msg void OnUpdateCurveType(CCmdUI*);
	afx_msg void OnOptionsInstantMode();
	afx_msg void OnUpdateOptionsInstantMode(CCmdUI*);
	afx_msg void OnOptionsAllowClamping();
	afx_msg void OnUpdateOptionsAllowClamping(CCmdUI*);
	afx_msg void OnViewImage(UINT);
	afx_msg void OnViewSwitchImage();
	afx_msg void OnUpdateViewImage(CCmdUI*);
	afx_msg LRESULT OnCurveChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CMainFrame - GOBAL FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int getFilterIndex(const std::filesystem::path&);
std::filesystem::path removeImgExt(const std::filesystem::path&);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

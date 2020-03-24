//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MainFrame.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
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
enum class EColorModel {rgb, hsv, hsl, hsg, hsp, cieHcl};
enum class EImageMode {original, modified};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CMainFrame : public CFrameWnd
{
// ELEMENT DATA
private:
	CFixedSplitterWnd splitterWnd;
	CCurveView* p_curve1View;
	CCurveView* p_curve2View;
	CCurveView* p_curve3View;
	CImageView* p_imageView;
	CHistogramView* p_histogramView;
	CStatusBar statusBar;
	CString pathName;
	int filterIndex;
	CTrueColorImage image;
	CTrueColorImage originalImage;
	CTrueColorImage modifiedImage;
	CHistogramArray originalHistogramArray;
	CHistogramArray modifiedHistogramArray;
	EColorModel colorModel;
	std::array<CGuide, 3> guideArray;
	std::array<CCurve, 3> curveArray;
	bool instantMode;
	bool allowClamping;
	EImageMode imageMode;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

// ELEMENT FUNCTIONS
private:
	void updateColorModelLabels();
	void updateImagePathLabel();
	void updateImageSizeLabel();
	void updateCurveTypeLabel();
	void updateImageModeLabel();
	void updateCalcTimeLabel(const int);
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
int getFilterIndex(const CString&);
CString removeImgExt(const CString&);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

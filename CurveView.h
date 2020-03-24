//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CurveView.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "InterpolationCurve.h"
#include "Function.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum class EMouseState {none, left, leftCtrl, right};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
using CDataPoint = TDataPoint<float>;
using CCurve = TInterpolationCurve<float>;
using CGuide = std::vector<float>;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCurveView : public CView
{
// ELEMENT DATA
private:
	const CGuide* p_guide;
	CCurve* p_curve;
	CSize wndSize;
	const int borderSize;
	int textHeight;
	CPoint contentOrigin;
	CSize contentSize;
	const int pointSize;
	CDC memDc;
	CBitmap memBmp;
	CString labelString;
	const COLORREF gridColor;
	const COLORREF textColor;
	const COLORREF curveColor;
	const COLORREF guideColor;
	CPen gridPen;
	CPen curvePen;
	CPen guidePen;
	bool gridVisible;
	CDataPoint* p_selectedDataPoint;
	CPoint moveOffset;
	EMouseState mouseState;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CCurveView();
	DECLARE_DYNCREATE(CCurveView)

// ELEMENT FUNCTIONS
private:
	int xOut(const float x) const {return roundTo<int>(x * contentSize.cx + contentOrigin.x);}
	int yOut(const float y) const {return roundTo<int>((1.0F - y) * contentSize.cy + contentOrigin.y);}
	CPoint pOut(const TPoint<float>& p) const {return {xOut(p.x), yOut(p.y)};}
	float xIn(const int x) const {return (x - contentOrigin.x) / static_cast<float>(contentSize.cx);}
	float yIn(const int y) const {return (contentOrigin.y + contentSize.cy - y) / static_cast<float>(contentSize.cy);}
	TPoint<float> pIn(const CPoint& p) const {return {xIn(p.x), yIn(p.y)};}
	TPoint<float> pIn(const int x, const int y) const {return {xIn(x), yIn(y)};}
	void moveTo(const TPoint<float>& p) {memDc.MoveTo(pOut(p));}
	void lineTo(const TPoint<float>& p) {memDc.LineTo(pOut(p));}
	void drawPoint(const TPoint<float>& p, const COLORREF c) {memDc.FillSolidRect(xOut(p.x) - pointSize / 2, yOut(p.y) - pointSize / 2, pointSize, pointSize, c);}
	TRect<float> createSelectionRect(const TPoint<float>&) const;
public:
	void setLabelString(CString ls) {labelString = ls;}
	void update(const CGuide* const, CCurve* const);
	void refresh();

// OVERRIDES
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT&) final;
	virtual void OnDraw(CDC*) final;

// MESSAGE MAP FUNCTIONS
protected:
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnSize(UINT, int, int);
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnLButtonDblClk(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	afx_msg void OnRButtonUp(UINT, CPoint);
	afx_msg void OnMouseMove(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

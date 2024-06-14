//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CurveView.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019-2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <format>
#include "InterpolationCurve.h"
#include "Function.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CCurveView
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
using DataPoint = ama::DataPoint<float>;
using Curve = ama::InterpolationCurve<float>;
using Guide = std::vector<float>;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCurveView : public CView
{
// ENUMS
private:
	enum class MouseState {none, left, leftCtrl, right};

// ELEMENT DATA
private:
	const Guide* p_guide;
	Curve* p_curve;
	CDC memoryDc;
	CBitmap memoryBitmap;
	CPen gridPen;
	CPen guidePen;
	CPen curvePen;
	int pointSize;
	int borderSize;
	int textHeight;
	CSize wndSize;
	CPoint contentOrigin;
	CSize contentSize;
	std::wstring labelString;
	DataPoint* p_selectedDataPoint;
	MouseState mouseState;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CCurveView();
	DECLARE_DYNCREATE(CCurveView)

// ELEMENT FUNCTIONS
private:
	int xOut(const float x) const {return ama::round(x * contentSize.cx + contentOrigin.x);}
	int yOut(const float y) const {return ama::round((1.0F - y) * contentSize.cy + contentOrigin.y);}
	CPoint pOut(const ama::Point<float>& p) const {return {xOut(p.x), yOut(p.y)};}
	float xIn(const int x) const {return (x - contentOrigin.x) / static_cast<float>(contentSize.cx);}
	float yIn(const int y) const {return (contentOrigin.y + contentSize.cy - y) / static_cast<float>(contentSize.cy);}
	ama::Point<float> pIn(const CPoint& p) const {return {xIn(p.x), yIn(p.y)};}
	ama::Point<float> pIn(const int x, const int y) const {return {xIn(x), yIn(y)};}
	void moveTo(const ama::Point<float>& p) {memoryDc.MoveTo(pOut(p));}
	void lineTo(const ama::Point<float>& p) {memoryDc.LineTo(pOut(p));}
	void drawPoint(const ama::Point<float>& p, const COLORREF c) {memoryDc.FillSolidRect(xOut(p.x) - pointSize / 2, yOut(p.y) - pointSize / 2, pointSize, pointSize, c);}
	ama::Rectangle<float> createSelectionRectangle(const ama::Point<float>&) const;
public:
	void setLabelString(std::wstring&& ls) {labelString = ls;}
	void update(const Guide* const, Curve* const);
	void draw();
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

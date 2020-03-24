//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramView.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Function.h"
#include "Point.h"
#include "ColorModels.h"
#include "CieHcl.h"
#include "TrueColorImage.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogram
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CHistogram : public std::array<float, 256>
{
// ELEMENT DATA
private:
	float average;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CHistogram() : std::array<float, 256>(), average(0) {}

// ELEMENT FUNCTIONS
public:
	float getAverage() const {return average;}
	void setAverage(const float a) {average = a;}
	float calcValue(const float v) const {return (*this)[static_cast<int>(v * (size() - 1.0F))];}
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramArray
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CHistogramArray : public std::array<CHistogram, 4>
{
// ELEMENT DATA
private:

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CHistogramArray() : std::array<CHistogram, 4>() {}

// ELEMENT FUNCTIONS
public:
	void init(const CTrueColorImage&);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHistogramView
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CHistogramView : public CView
{
// ELEMENT DATA
private:
	const CHistogramArray* p_histogramArray;
	CSize wndSize;
	int textHeight;
	CDC memDc;
	CBitmap memBmp;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CHistogramView();
	DECLARE_DYNCREATE(CHistogramView)

// ELEMENT FUNCTIONS
public:
	void update(const CHistogramArray* const);
	void refresh();

// OVERRIDES
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT&) final;
	virtual void OnDraw(CDC*) final;

// MESSAGE MAP FUNCTIONS
protected:
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnSize(UINT, int, int);
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

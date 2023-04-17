//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramView.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <format>
#include "Function.h"
#include "Point.h"
#include "ColorModel.h"
#include "CieHcl.h"
#include "TrueColorImage.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Histogram
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class Histogram : public std::array<float, 256>
{
// ELEMENT DATA
private:
	float average;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	Histogram() : std::array<float, 256>(), average(0) {}

// ELEMENT FUNCTIONS
public:
	float getAverage() const {return average;}
	void setAverage(const float a) {average = a;}
	float calcValue(const float v) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HistogramArray
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class HistogramArray : public std::array<Histogram, 4>
{
// ELEMENT DATA
private:

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	HistogramArray() : std::array<Histogram, 4>() {}

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
	const HistogramArray* p_histogramArray;
	CDC memoryDc;
	CBitmap memoryBitmap;
	int borderSize;
	int textHeight;
	CSize wndSize;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CHistogramView();
	DECLARE_DYNCREATE(CHistogramView)

// ELEMENT FUNCTIONS
public:
	void update(const HistogramArray* const);
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
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

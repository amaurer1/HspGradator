//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ImageView.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <filesystem>
#include "Function.h"
#include "ColorModel.h"
#include "CieHcl.h"
#include "Point.h"
#include "TrueColorImage.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CImageView
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CImageView : public CView
{
// ELEMENT DATA
private:
	const CTrueColorImage* p_image;
	CSize wndSize;
	CPoint imageOrigin;
	CSize imageSize;

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CImageView();
	DECLARE_DYNCREATE(CImageView)

// ELEMENT FUNCTIONS
public:
	void update(const CTrueColorImage* const);
	void recalcLayout();
	void refresh();

// OVERRIDES
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT&) final;
	virtual void OnDraw(CDC*) final;

// MESSAGE MAP FUNCTIONS
protected:
	afx_msg void OnSize(UINT, int, int);
	afx_msg void OnLButtonDblClk(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

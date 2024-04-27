//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HspGradatorApp.h
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2023 Adrian Maurer. All rights reserved.
// Distributed under the GPL-3.0 software license (https://opensource.org/licenses/GPL-3.0).
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "resource.h"
#include "Message.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CAboutDlg
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CAboutDlg : public CDialog
{
// DIALOG DATA
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_ABOUT_DLG};
#endif

// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CAboutDlg();

// MESSAGE MAP FUNCTIONS
protected:
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHspGradatorApp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CHspGradatorApp : public CWinApp
{
// CONSTRUCTION / DESTRUCTION / ASSIGNMENT
public:
	CHspGradatorApp();

// OVERRIDES
public:
	virtual BOOL InitInstance() final;

// MESSAGE MAP FUNCTIONS
protected:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern CHspGradatorApp theApp;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void sendMessageToMainWnd(UINT msg, WPARAM wp = 0)
{
	if (CWnd* const p_mw = ::AfxGetMainWnd())
	{
		p_mw->SendMessage(msg, wp);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
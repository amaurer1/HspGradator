//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HspGradatorApp.cpp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Adrian Maurer. All rights reserved.
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
// CAboutDlg
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CAboutDlg - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : 
	CDialog(IDD_ABOUT_DLG)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CAboutDlg - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHspGradatorApp
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHspGradatorApp - CONSTRUCTION / DESTRUCTION / ASSIGNMENT
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CHspGradatorApp::CHspGradatorApp() : 
	CWinApp()
{
	SetAppID(L"CHspGradatorApp.AppID.NoVersion");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHspGradatorApp - OVERRIDES
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CHspGradatorApp::InitInstance()
{
	CWinApp::InitInstance();
	CFrameWnd* p_mf = new CMainFrame;
	if (p_mf->LoadFrame(IDR_MAIN_FRAME, WS_OVERLAPPEDWINDOW))
	{
		HICON hi = LoadIcon(IDR_MAIN_FRAME);
		p_mf->SetIcon(hi, TRUE);
		p_mf->SetIcon(hi, FALSE);
		p_mf->ShowWindow(SW_SHOWMAXIMIZED);
		p_mf->UpdateWindow();
		m_pMainWnd = p_mf;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CHspGradatorApp - MESSAGE MAP FUNCTIONS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CHspGradatorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CHspGradatorApp::OnAppAbout)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CHspGradatorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CHspGradatorApp theApp;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

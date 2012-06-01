// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "PictureBox.h"
#include "resource.h"
#include <windowsx.h>

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
	public CMessageFilter, public CIdleHandler
{
private:
	CRichEditCtrl m_richEditText;
	CRichEditCtrl m_richEditHTML;
	CRichEditCtrl m_richEditFormat;
	CRichEditCtrl m_richEditRaw;
	CPictureBox m_pictureBox;
	CFont m_font;
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE,OnSize)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int cx = (int)(short)LOWORD(lParam);
		int cy = (int)(short)HIWORD(lParam);
		CRect rc;
		GetClientRect(&rc);
		int top = 50;
		int w = (rc.Width()-30)/3;
		int h = 300;
		m_richEditFormat.MoveWindow(CRect(15,top,rc.Width()/2-15,top+150));
		m_richEditRaw.MoveWindow(CRect(rc.Width()/2-15,top,rc.Width()-15,top+150));
		m_richEditText.MoveWindow(CRect(15,top+150,15+w,rc.bottom-15));
		m_richEditHTML.MoveWindow(CRect(15+w,top+150,2*w+15,rc.bottom-15));
		m_pictureBox.MoveWindow(CRect(2*w+15,top+150,3*w+15,rc.bottom-15));
		return 0L;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		
		SetDlgItemInt(IDC_EDITFormat,1);
		CRect rc;
		GetClientRect(&rc);
		int top = 50;
		int w = (rc.Width()-30)/3;
		int h = 300;
		m_richEditFormat.Create(
			m_hWnd,
			CRect(15,top,rc.Width()/2-15,top+150),
			L"",
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN|WS_DLGFRAME ,
			0);
		m_richEditRaw.Create(
			m_hWnd,
			CRect(rc.Width()/2-15,top,rc.Width()-15,top+150),
			L"",
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN|WS_DLGFRAME ,
			0);
		m_richEditText.Create(
			m_hWnd,
			CRect(15,top+150,15+w,rc.bottom-15),
			L"",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE | ES_LEFT |WS_VSCROLL|WS_DLGFRAME,
			0);
		m_richEditHTML.Create(
			m_hWnd,
			CRect(15+w,top+150,2*w+15,rc.bottom-15),
			L"",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE | ES_LEFT |WS_VSCROLL|WS_DLGFRAME,
			0);
		m_pictureBox.Create(
			m_hWnd,
			CRect(2*w+15,top+150,3*w+15,rc.bottom-15),
			L"",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP |WS_VSCROLL|WS_DLGFRAME,
			0);
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT); 
		m_font.Attach(hFont); 
		//m_font.CreatePointFont(90,_T("Î¢ÈíÑÅºÚ"));
		m_richEditHTML.SetFont(hFont);
		m_richEditFormat.SetFont(hFont);
		m_richEditFormat.SetBackgroundColor(RGB(239,234,50));
		m_richEditRaw.SetBackgroundColor(RGB(255,209,75));
		m_richEditText.SetBackgroundColor(RGB(255,173,50));
		m_richEditHTML.SetBackgroundColor(RGB(189,135,50));
		m_richEditRaw.SetFont(hFont);
		m_richEditText.SetFont(hFont);
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}


	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

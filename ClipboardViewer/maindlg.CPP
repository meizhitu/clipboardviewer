#include "StdAfx.h"
#include "MainDlg.h"

TCHAR *StrClipboardFormats[17]={
	_T("CF_TEXT"),
	_T("CF_BITMAP"),
	_T("CF_METAFILEPICT"),
	_T("CF_SYLK"),
	_T("CF_DIF"),
	_T("CF_TIFF"),
	_T("CF_OEMTEXT"),
	_T("CF_DIB"),
	_T("CF_PALETTE"),
	_T("CF_PENDATA"),
	_T("CF_RIFF"),
	_T("CF_WAVE"),
	_T("CF_UNICODETEXT"),
	_T("CF_ENHMETAFILE"),
	_T("CF_HDROP"),
	_T("CF_LOCALE"),
	_T("CF_DIBV5")
};

CString GetClipFormatName(int uFormat,int htmlFormat)
{
	if (uFormat <= 17)
	{
		return StrClipboardFormats[uFormat-1];
	}
	else if (uFormat == htmlFormat)
	{
		return _T("CF_HTML");
	}
	else
	{
		TCHAR szFormatName[256];
		if (GetClipboardFormatName(uFormat, szFormatName, 
			sizeof(szFormatName))) 
		{
			return szFormatName; 
		}
		return _T("NONE");
	}
}

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_richEditFormat.SetWindowText(_T(""));
	m_richEditText.SetWindowText(_T(""));
	m_richEditHTML.SetWindowText(_T(""));
	m_richEditRaw.SetWindowText(_T(""));
	m_pictureBox.SetHBitmap(NULL);
	if (::OpenClipboard(NULL))
	{
		UINT uFormat = 0;
		char format[256] = {0};
		UINT htmlFormat = ::RegisterClipboardFormat(_T("HTML Format"));
		while(uFormat = ::EnumClipboardFormats(uFormat))
		{
			CString formatText;
			formatText.Format(_T("%d:%s\n"),uFormat,GetClipFormatName(uFormat,htmlFormat));
			m_richEditFormat.AppendText(formatText);
			if (htmlFormat == uFormat)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HANDLE handle = ::GetClipboardData(uFormat);
					CHAR* data = (CHAR*)GlobalLock(handle);
					CString unicodeData(data);
					m_richEditHTML.AppendText(unicodeData);
				}
			}
			else if (uFormat == CF_UNICODETEXT)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HANDLE handle = ::GetClipboardData(uFormat);
					TCHAR* data = (TCHAR*)GlobalLock(handle);
					m_richEditText.AppendText(data);
				}
			}
			else if (uFormat == CF_BITMAP)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HBITMAP hBitmap = (HBITMAP)GetClipboardData(uFormat);
					m_pictureBox.SetHBitmap(hBitmap);
				}
			}
		}
		int selectFormat = GetDlgItemInt(IDC_EDITFormat);
		if (selectFormat > 0)
		{
			if (::IsClipboardFormatAvailable(selectFormat))
			{
				HANDLE handle = ::GetClipboardData(selectFormat);
				if (selectFormat == CF_UNICODETEXT)
				{
					m_richEditRaw.AppendText((TCHAR*)GlobalLock(handle));
				}
				else
				{
					char* data = (char*)GlobalLock(handle);
					CString unicodeData(data);
					m_richEditRaw.AppendText(unicodeData);
				}
			}
		}
		CloseClipboard();
	}
	return 0;
}

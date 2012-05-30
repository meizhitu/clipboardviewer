#include "StdAfx.h"
#include "MainDlg.h"

TCHAR *StrClipboardFormats[17]={_T("cf_text"),_T("cf_bitmap"),_T("cf_metafilepict"),_T("cf_sylk"),
_T("cf_dif"),_T("cf_tiff"),_T("cf_oemtext"),_T("cf_dib"),_T("cf_palette"),
_T("cf_pendata"),_T("cf_riff"),_T("cf_wave"),_T("cf_unicodetext"),
_T("cf_enhmetafile"),_T("cf_hdrop"),_T("cf_locale"),_T("cf_max")};

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
		return _T("NONE");
	}
}

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_richEditFormat.SetWindowText(_T(""));
	m_richEditText.SetWindowText(_T(""));
	m_richEditHTML.SetWindowText(_T(""));
	m_pictureBox.SetHBitmap(NULL);
	if (::OpenClipboard(NULL))
	{
		UINT uFormat = 0;
		char format[256] = {0};
		UINT htmlFormat = ::RegisterClipboardFormat(_T("HTML Format"));
		while(uFormat = ::EnumClipboardFormats(uFormat))
		{
			CString formatText;
			formatText.Format(_T("%d:%s\t"),uFormat,GetClipFormatName(uFormat,htmlFormat));
			m_richEditFormat.AppendText(formatText);
			if (htmlFormat == uFormat)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HANDLE handle = ::GetClipboardData(uFormat);
					char* data = (char*)GlobalLock(handle);
					CString unicodeData(data);
					m_richEditHTML.AppendText(unicodeData);
				}
				continue;
			}
			else if (uFormat == CF_TEXT)
			{
				/*if (::IsClipboardFormatAvailable(uFormat))
				{
					HANDLE handle = ::GetClipboardData(uFormat);
					char* data = (char*)GlobalLock(handle);
					CString unicodeData(data);
					m_richEditText.AppendText(unicodeData);
				}
				continue;*/
			}
			else if (uFormat == CF_UNICODETEXT)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HANDLE handle = ::GetClipboardData(uFormat);
					TCHAR* data = (TCHAR*)GlobalLock(handle);
					m_richEditText.AppendText(data);
				}
				continue;
			}
			else if (uFormat == CF_BITMAP)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HBITMAP hBitmap = (HBITMAP)GetClipboardData(uFormat);
					m_pictureBox.SetHBitmap(hBitmap);
					continue;
				}
			}
		}
		CloseClipboard();
	}
	return 0;
}

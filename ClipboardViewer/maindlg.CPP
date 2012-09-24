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

CString ConvertUtf8(char* strUtf8)
{
	int len = MultiByteToWideChar( CP_UTF8, 0, strUtf8, -1, NULL, 0 );
	if ( len == 0 )
		return CString();
	CString ret;
	LPWSTR lpBuffer = ret.GetBuffer( len );
	MultiByteToWideChar( CP_UTF8, 0, strUtf8, -1, lpBuffer, len );
	ret.ReleaseBuffer();
	return ret;
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

					CString unicodeData = ConvertUtf8(data);
					m_richEditHTML.AppendText(unicodeData);
					GlobalUnlock(handle);
				}
			}
			else if (uFormat == CF_UNICODETEXT)
			{
				if (::IsClipboardFormatAvailable(uFormat))
				{
					HANDLE handle = ::GetClipboardData(uFormat);
					TCHAR* data = (TCHAR*)GlobalLock(handle);
					m_richEditText.AppendText(data);
					GlobalUnlock(handle);

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
				HGLOBAL hMem = ::GetClipboardData(selectFormat);
				if (IsDlgButtonChecked(IDC_CHECKBinary))
				{
					LPVOID pvdata = GlobalLock(hMem);
					DWORD uDataSize = GlobalSize(hMem);
					BYTE* pbyData  = new BYTE [ uDataSize ];
					// Copy the data to the newly-allocated memory.
					CopyMemory ( pbyData, pvdata, uDataSize );
					UINT        uOffset;
					UINT        uBytesInThisLine;
					UINT        uLineOffset;

					CString     sLine (_T(' '), 64);            // preallocate enough space for a line
					CString     sWork;
					for ( uOffset = 0; uOffset < uDataSize; uOffset += 8 )
					{
						// How many bytes will be in the next line?  Max of 8.
						uBytesInThisLine = uDataSize - uOffset;
						if ( uBytesInThisLine > 8 )
						{
							uBytesInThisLine = 8;
						}
						// First part of the line - the starting offset.
						sLine.Format ( _T("%08X  "), uOffset );
						// Now loop through the data and add on the hex value of each byte.
						for ( uLineOffset = 0; uLineOffset < uBytesInThisLine; uLineOffset++ )
						{
							sWork.Format ( _T("%02X "), pbyData[uOffset + uLineOffset] );
							sLine += sWork;
							if ( 3 == uLineOffset || 7 == uLineOffset )
								sLine += ' ';
						}

						// If there were less than 8 bytes in this line, pad the line with
						// spaces so the ASCII representation will be in the right column.
						if ( uBytesInThisLine < 8 )
						{
							sLine += CString(_T(' '), 1 + 3 * (8 - uBytesInThisLine) );

							if ( uBytesInThisLine < 4 )
								sLine += _T(' ');
						}

						// Add on the ASCII representation
						for ( uLineOffset = 0; uLineOffset < uBytesInThisLine; uLineOffset++ )
						{
							// If the next byte isn't printable, show a period instead.
							if ( isprint ( pbyData[uOffset + uLineOffset] ))
								sLine += (TCHAR) pbyData[uOffset + uLineOffset];
							else if (uLineOffset+1 < uBytesInThisLine)
							{
								BYTE b[3];
								b[0] = pbyData[uOffset + uLineOffset];
								b[1] = pbyData[uOffset + uLineOffset+1];
								b[2] = 0;
								CString chinese(b);
								uLineOffset++;
								sLine += chinese;
							}
							else
								sLine += _T('.');
						}
						sLine += _T('\n');
						m_richEditRaw.AppendText(sLine);
					}
				}
				else if (selectFormat == CF_UNICODETEXT)
				{
					m_richEditRaw.AppendText((TCHAR*)GlobalLock(hMem));
				}
				else
				{
					char* data = (char*)GlobalLock(hMem);
					CString unicodeData(data);
					if (IsDlgButtonChecked(IDC_CHECKUnicode))
					{
						unicodeData= (TCHAR*)data;//ConvertUtf8(data);
					}
					m_richEditRaw.AppendText(unicodeData);
				}
				
				GlobalUnlock(hMem);
			}
		}

		
		CloseClipboard();
	}
	return 0;
}

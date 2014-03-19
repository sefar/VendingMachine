#pragma once
#include "stdafx.h"
#include "Machine.h"
#include "ChangeTray.h"
#include "DisplayArea.h"

#define BUFFER_SIZE 1000

CMachine::CMachine()
{
	m_changeTray = new CChangeTray();
	m_dispArea = new CDisplayArea();
}


CMachine::~CMachine()
{
	delete m_dispArea;
	delete m_changeTray;
}

HRESULT CMachine::_ReadFile(__in PCWSTR szFilePath, __out PWSTR* pszFileContent)
{
	HRESULT     hr = E_FAIL;
	HANDLE      hFile = INVALID_HANDLE_VALUE;
	DWORD       cchFile;
	DWORD       cbRead;
	WCHAR       szLocalFilePath[MAX_PATH];
	char *      pchBuf = 0;

	if (ExpandEnvironmentStrings(szFilePath, szLocalFilePath, ARRAYSIZE(szLocalFilePath)))
	{
		hFile = CreateFile(szLocalFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto Cleanup;
		}

		cchFile = GetFileSize(hFile, NULL);
		if (cchFile == INVALID_FILE_SIZE)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto Cleanup;
		}

		pchBuf = new char[cchFile + 1];
		*pszFileContent = new WCHAR[cchFile + 1];
		if (!pchBuf || !(*pszFileContent))
		{
			hr = E_OUTOFMEMORY;
			goto Cleanup;
		}

		if (!ReadFile(hFile, pchBuf, cchFile, &cbRead, 0))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto Cleanup;
		}
		pchBuf[cbRead] = 0;

		int ret = MultiByteToWideChar(CP_ACP, 0, pchBuf, -1, *pszFileContent, cchFile + 1);

		hr = (0 == ret) ? E_FAIL : S_OK;
	}

Cleanup:
	if (pchBuf)
	{
		delete[] pchBuf;
	}
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	return hr;

	if (!ReadFile(hFile, pchBuf, cchFile, &cbRead, 0))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
}

HRESULT CMachine::LoadInventory(__in PCWSTR szFilePath)
{
	PWSTR pszFileContent;
	HRESULT hr = _ReadFile(szFilePath, &pszFileContent);
	
	if (SUCCEEDED(hr))
	{
		SetInventory(pszFileContent);
	}

	return hr;
}

HRESULT CMachine::SetInventory(__in PWSTR szInventoryData)
{
	WCHAR* pCurrent = szInventoryData;

	while (pCurrent && *pCurrent)
	{
		if (lstrcmp(pCurrent, L"Actions")
		{
			RunCommands(pCurrent + lstrlen(L"Actions"));
		}
		else if (*pCurrent == '$')
		{
			_ReadCurrency(&pCurrent);
		}
		else if ((*pCurrent >= 'a' && *pCurrent <= 'z') ||
				 (*pCurrent >= 'A' && *pCurrent <= 'Z'))
		{
			_ReadItem(&pCurrent);
		}
		else
		{
			return E_INVALIDARG;
		}
	}
	return S_OK;
}

void CMachine::_ReadItem(__inout WCHAR** pCurrent)
{
	WCHAR* szLocation;
	WCHAR* szName;
	UINT count;
	double value;
	_ReadCSVLine(pCurrent, &szLocation, &szName, &value, &count);

	// Read Items
	m_dispArea->AddItem(szLocation[0], _wtoi(szLocation + 1), szName, value, count);

}
void CMachine::_ReadCurrency(__inout WCHAR** pCurrent)
{
	UINT count;
	double value;
	WCHAR* szLocation;

	_ReadCSVLine(pCurrent, &szLocation, NULL, &value, &count);
	m_changeTray->AddInventory(value, count, _wtoi(szLocation + 1));
}

void CMachine::_ReadCSVLine(__inout WCHAR** pCurrent, __out_opt WCHAR** location, __out_opt WCHAR** name, __out double* value, __out UINT* count)
{
	WCHAR* szValue;
	WCHAR* szCount;

	WCHAR* pPlaceHolder = *pCurrent;

	if (location)
	{
		*location = pPlaceHolder;
	}

	while (*(++pPlaceHolder) != L',');
	*pPlaceHolder = L'\0';
	++pPlaceHolder;

	if (name)
	{
		*name = pPlaceHolder;

		while (*(++pPlaceHolder) != L',');
		*pPlaceHolder = L'\0';
		++pPlaceHolder;
	}

	szValue = pPlaceHolder;
	while (*(++pPlaceHolder) != L',');
	*pPlaceHolder = L'\0';

	szCount = ++pPlaceHolder;

	while (*(++pPlaceHolder) != L'\r');
	*pPlaceHolder = '\0';
	pPlaceHolder += 2;

	*count = _wtoi(szCount);
	*value = _wtof(szValue);

	*pCurrent = pPlaceHolder;
}

HRESULT CMachine::LoadCommands(__in PCWSTR szFilePath)
{
	PWSTR pszFileContent;
	HRESULT hr = _ReadFile(szFilePath, &pszFileContent);

	if (SUCCEEDED(hr))
	{
		hr = RunCommands(pszFileContent);
	}
	return hr;
}

HRESULT CMachine::RunCommands(__in PWSTR szCommandData)
{
	while (szCommandData && *szCommandData)
	{
		if (*szCommandData >= L'A' && *szCommandData <= L'Z' ||
			*szCommandData >= L'z' && *szCommandData >= L'z')
		{
			_SetLetter(*szCommandData);
			szCommandData++;
		}
		else if (*szCommandData >= L'0' && *szCommandData <= L'9')
		{
			*(szCommandData + 1) = L'\0';
			_SetNumber(_wtoi(szCommandData));
			szCommandData++;
		}
		else if (*szCommandData == L'#')
		{
			_Reset();
			szCommandData++;
		}
		else if (*szCommandData == '$')
		{

			szCommandData += 4;
		}
		else
		{
			// What?
		}

		if (_HasValidSelection() && _HasEnoughChange())
		{
			_DispenseItem();
		}
		szCommandData++;
	}
	return S_OK;
}

HRESULT CMachine::OutputReport(__in PCWSTR szFilePath)
{

	return S_OK;
}
#pragma once

class CChangeTray;
class CDisplayArea;

class CMachine
{
public:
	CMachine();
	~CMachine();
	HRESULT LoadInventory(__in PCWSTR szFilePath);
	HRESULT SetInventory(__in PWSTR szInventoryData);

	HRESULT LoadCommands(__in PCWSTR szFilePath);
	HRESULT RunCommands(__in PWSTR szCommandData);
	HRESULT OutputReport(__in PCWSTR szFilePath);

protected:
	HRESULT _ReadFile(__in PCWSTR szFilePath, __out PWSTR* pszFileContent);
	typedef enum
	{
		Position,
		Value,
		Name,
		Count
	} eDataField;

	typedef enum
	{
		Change,
		Candy
	};

	void _ReadCurrency(__inout WCHAR** pCurrent);
	void _ReadCSVLine(__inout WCHAR** pCurrent, __out_opt WCHAR** location, __out_opt WCHAR** name, __out double* value, __out UINT* count);
	void _ReadItem(__inout WCHAR** pCurrent);

	CDisplayArea* m_dispArea;
	CChangeTray* m_changeTray;
};


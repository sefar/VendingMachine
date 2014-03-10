#pragma once

class CMachine;

struct Item
{
	Item(__in WCHAR letter, __in UINT number, __in WCHAR* nm, __in double value, __in UINT count) : letter(letter), number(number), value(value), count(count) 
	{
		lstrcpy(name, nm);
	}
	double value;
	UINT count;
	WCHAR letter;
	UINT number;
	WCHAR name[100];
};

class CDisplayArea
{
public:
	CDisplayArea();
	~CDisplayArea();

	bool Dispense(__in WCHAR letter, __in UINT number);
	void AddItem(__in WCHAR letter, __in UINT number, __in WCHAR* name, __in double value, __in UINT count);


private:
	vector<Item> m_ItemInventory;
};


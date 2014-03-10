#include "stdafx.h"
#include "DisplayArea.h"
#include "Machine.h"

CDisplayArea::CDisplayArea()
{
}


CDisplayArea::~CDisplayArea()
{
}

void CDisplayArea::AddItem(__in WCHAR letter, __in UINT number, __in WCHAR* name, __in double value, __in UINT count)
{
	m_ItemInventory.push_back(Item(letter, number, name, value, count));
}

bool CDisplayArea::Dispense(__in WCHAR letter, __in UINT number)
{
	bool fRet = false;
	for (size_t i = 0; i < m_ItemInventory.size(); i++)
	{
		if (m_ItemInventory[i].letter == letter &&
			m_ItemInventory[i].number == number)
		{
			if (m_ItemInventory[i].count != 0)
			{
				m_ItemInventory[i].count--;
				fRet = true;
			}
		}
	}
	return fRet;
}
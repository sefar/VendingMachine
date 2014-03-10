#pragma once
#include "stdafx.h"
#include "ChangeTray.h"


CChangeTray::CChangeTray()
{
}

CChangeTray::~CChangeTray()
{
}

void CChangeTray::AddInventory(__in double value, __in UINT count, __in UINT slot)
{
	m_ChangeInventory.push_back(Change(value, count, slot));
	sort(m_ChangeInventory.begin(), m_ChangeInventory.end(), ChangeCompare);
}

void CChangeTray::AddChange(__in double value)
{
	AddChange(value, 1);
}

void CChangeTray::AddChange(__in double value, __in UINT count)
{
	// Make a note of the value we are storing.
	m_ChangeDepositedTotal += value*count;

	// Deposit the value to our changebox.
	m_changeBoxTotal += value*count;
}

void CChangeTray::RefundChange()
{
	for (vector<Change>::iterator it = m_ChangeInventory.begin();
		it != m_ChangeInventory.end() && m_ChangeDepositedTotal != 0; it++)
	{
		// We'll start with the largest change we have and work our way down.
		UINT floor = UINT(m_ChangeDepositedTotal / it->value);
		
		if (floor > 0)
		{
			if (it->count > floor)
			{
				m_ChangeDepositedTotal -= floor*it->value;
				it->count -= floor;
			}
			else
			{
				m_ChangeDepositedTotal -= it->count*it->value;
				it->count = 0;
			}
		}
	}

	while (m_ChangeDepositedTotal)
	{
		for (vector<Change>::reverse_iterator rit = m_ChangeInventory.rbegin();
			rit != m_ChangeInventory.rend(); rit++)
		{
			if (rit->count != 0)
			{
				UINT ceiling = UINT((m_ChangeDepositedTotal / rit->value) + 1);
				if (ceiling > 0)
				{
					if (rit->count > ceiling)
					{
						m_ChangeDepositedTotal -= ceiling*rit->value;
						rit->count -= ceiling;
					}
					else
					{
						m_ChangeDepositedTotal -= rit->count*rit->value;
						rit->count = 0;
					}
				}
			}
		}
	}
}

void CChangeTray::DepositChange()
{
	m_ChangeDepositedTotal = 0;
}

void CChangeTray::PrintReport(__out ostream stream)
{
	stream << "test";
}
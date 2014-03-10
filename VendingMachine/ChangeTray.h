#pragma once
#include "Object.h"

struct Change
{
	Change(__in double value, __in UINT count, __in UINT slot) : value(value), count(count), slot(slot) {}
	double value;
	UINT count;
	UINT slot;
};

static bool ChangeCompare(Change i, Change j) { return (i.value < j.value); }

class CChangeTray
{
public:
	CChangeTray();
	~CChangeTray();
	void AddInventory(__in double value, __in UINT count, __in UINT slot);
	void AddChange(__in double value);
	void AddChange(__in double value, __in UINT count);
	void DepositChange();
	void RefundChange();
	void PrintReport(__out ostream stream);

private:
	vector<Change> m_ChangeInventory;
	double m_ChangeDepositedTotal;
	double m_changeBoxTotal;
};


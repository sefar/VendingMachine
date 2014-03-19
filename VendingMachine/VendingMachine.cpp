// VendingMachine.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "Machine.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CMachine VM;
	VM.LoadInventory(argv[1]);
	VM.LoadCommands(argv[2]);
	VM.OutputReport(argv[3]);

	return 0;
}


//#pragma once
#include "ltr11api.h"
#include "ltrapi.h"
#include "ltr42api.h"

static TLTR42 ltr42;

static int Startltr42(){ //запуск лтр42
	int res;
	const CHAR crate = 0;
	res = LTR42_Init(&ltr42); 
	if (res == 0)
	{
	res = LTR42_Open(&ltr42, (int)0, (WORD)0, &crate, (INT)4);
		if (res == 0) 
		{
		ltr42.Marks.StartMark_Mode = 0;
		res = LTR42_Config(&ltr42);
		}
	
	}

	return res;
	delete[]&res;
	
}

static void startandwrite(WORD Output) 
{
	int res;
	res = LTR42_WritePort(&ltr42, Output);
	res;
}

static void rotate(WORD Output) {
	WORD buff = Output;
	for (int i = 0; i < 33; i++)
	{
		Output = buff;
		startandwrite(Output);
		Output = 0;
		startandwrite(Output);
		Sleep(18000);

	}
}
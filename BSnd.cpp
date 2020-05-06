//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BSnd.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall BEEPSOUND::BEEPSOUND(int BMd)
{
	BeepMode = BMd;						// 機種判別番号
}
//---------------------------------------------------------------------------
__fastcall BEEPSOUND::~BEEPSOUND()
{
	BeepMode = 0;
}
//---------------------------------------------------------------------------
void __fastcall BEEPSOUND::Option(short cycle)
{
	long clk;
	short cnt;

	/* Beep音タイマカウンタの設定 */
	switch (BeepMode) {
	case 1:								//*** [DOS/V] ***
		clk = 1193180;					// 基準クロック
		cnt = (short)(clk / cycle);

		asm {
			mov	dx,43h;
			mov	al,10110110b;			// mode
			out	dx,al;
			mov	dx,42h;
			mov	ax,cnt;
			out	dx,al;
			mov	al,ah;
			out	dx,al;
		}
		break;
	case 2:								//*** [PC-98] ***
		clk = 2457600;					/* 基準クロック */
		cnt = (short)(clk / cycle);

		asm {
			mov	dx,77h;
			mov	al,01110110b;			// mode
			out	dx,al;
			mov	ax,cnt;
			mov	dx,73h;
			out	dx,al;
			mov	al,ah;
			out	dx,al;
		}
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall BEEPSOUND::Start(void)
{
	switch (BeepMode) {
	case 1:								//*** Beep On [DOS/V] ***
		asm {
			mov	dx,61h;
			in	al,dx;
			or	al,03h;
			out	dx,al;
		}
		break;
	case 2:								//*** Beep On [PC-98] ***
		asm {
			mov	dx,35h;
			in	al,dx;
			and	al,0F7h;
			out	dx,al;
		}
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall BEEPSOUND::Stop(void)
{
	switch (BeepMode) {
	case 1:								//*** Beep Off [DOS/V] ***
		asm {
			mov	dx,61h;
			in	al,dx;
			and	al,0FCh;
			out	dx,al;
		}
		break;
	case 2:								//*** Beep Off [PC-98] ***
		asm {
			mov	dx,35h;
			in	al,dx;
			or	al,08h;
			out	dx,al;
		}
		break;
	}
}
//---------------------------------------------------------------------------

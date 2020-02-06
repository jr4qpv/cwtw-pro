//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LingBuf.h"
//---------------------------------------------------------------------------
__fastcall LINGBUF::LINGBUF(int size)
{
	buf = new DWORD[size];
	LINGBUF::size = buf ? size : 0;
	cnt = 0;
	rdp = 0;
	wrp = rdp;
}
//---------------------------------------------------------------------------
__fastcall LINGBUF::~LINGBUF()
{
	if (buf)
		delete buf;
}
//---------------------------------------------------------------------------
int __fastcall LINGBUF::bufwr(DWORD data)
{
	if (cnt >= size-2)
		return 0;						// =0:書き込めない

	buf[wrp] = data;
	if (++wrp >= size)
		wrp = 0;
	cnt++;

	return cnt;							// >0:書き込めた
}
//---------------------------------------------------------------------------
DWORD __fastcall LINGBUF::bufrd(void)
{
	DWORD data;

	do {
		if (cnt <= 0)
			return 0;					// =0:空で読み込めない

		data = buf[rdp];
		if (++rdp >= size)
			rdp = 0;
		cnt--;
	} while (data == 0);

	return data;						// !=0:読み込みデータ
}
//---------------------------------------------------------------------------

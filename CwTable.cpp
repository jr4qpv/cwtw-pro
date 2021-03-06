﻿//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CwTable.h"

//---------------------------------------------------------------------------

//*******( 欧文用モールス符号テーブル )*******
static DECODE etable[] = {
	{ 0x0006, L'A', 0 },
	{ 0x0095, L'B', 0 },
	{ 0x0099, L'C', 0 },
	{ 0x0025, L'D', 0 },
	{ 0x0001, L'E', 0 },
	{ 0x0059, L'F', 0 },
	{ 0x0029, L'G', 0 },
	{ 0x0055, L'H', 0 },
	{ 0x0005, L'I', 0 },
	{ 0x006A, L'J', 0 },
	{ 0x0026, L'K', 0 },
	{ 0x0065, L'L', 0 },
	{ 0x000A, L'M', 0 },
	{ 0x0009, L'N', 0 },
	{ 0x002A, L'O', 0 },
	{ 0x0069, L'P', 0 },
	{ 0x00A6, L'Q', 0 },
	{ 0x0019, L'R', 0 },
	{ 0x0015, L'S', 0 },
	{ 0x0002, L'T', 0 },
	{ 0x0016, L'U', 0 },
	{ 0x0056, L'V', 0 },
	{ 0x001A, L'W', 0 },
	{ 0x0096, L'X', 0 },
	{ 0x009A, L'Y', 0 },
	{ 0x00A5, L'Z', 0 },
	{ 0x01AA, L'1', 0 },
	{ 0x016A, L'2', 0 },
	{ 0x015A, L'3', 0 },
	{ 0x0156, L'4', 0 },
	{ 0x0155, L'5', 0 },
	{ 0x0255, L'6', 0 },
	{ 0x0295, L'7', 0 },
	{ 0x02A5, L'8', 0 },
	{ 0x02A9, L'9', 0 },
	{ 0x02AA, L'0', 0 },
	{ 0x0666, L'.', 0 },
	{ 0x0A5A, L',', 0 },
	{ 0x0A95, L':', 0 },
	{ 0x05A5, L'?', 0 },
	{ 0x06A9, L'\'', 0 },
	{ 0x0956, L'-', 0 },
	{ 0x0269, L'(', 0 },
	{ 0x09A6, L')', 0 },
	{ 0x0259, L'/', 0 },
	{ 0x0659, L'"', 0 },
	{ 0x0699, L'@', 0 },			/* 2004/05/03新ﾓｰﾙｽ符号追加 */
	{ 0x5555, L'H', L'H' },			/* 連続：欧文訂正符号 */
	{ 0x0256, L'B', L'T' },			/* 連続：本文符号 */
	{ 0x0199, L'A', L'R' },			/* 連続：送信終了符号 */
	{ 0x0195, L'A', L'S' },			/* 連続：送信待機符号 */
	{ 0x0566, L'V', L'A' },			/* 連続：通信完了符号(連続文字) */
#if 0	/* 重複 */
	{ 0x0256, L'=', 0 },
	{ 0x0199, L'+', 0 },
#endif

	{ 0x0169, L'ﾟ', 0 },
	{ 0x0965, L'ﾎ', L'ﾎ' },			/* 連続：指定 */
	{ 0x0596, L'ｳ', L'ｳ' },			/* 連続：記事 */
	{ 0x096A, L'ﾎ', L'ﾚ' },			/* 連続：和文へ切り替え */
	{ 0x0159, L'ﾗ', L'ﾀ' },			/* 連続：和文訂正符号(欧文へ切り替え) */
	{ 0x01A6, L'ｰ', 0 },
	{ 0x0665, L'｣', 0 },			/* 段落 */
	{ 0xC000, L' ', 0 },			/* SP(聞き取りモードで使用) */
	{ 0, 0, 0 }						/* ターミネータ */
};

//*******( 和文用モールス符号テーブル )*******
static DECODE jtable[] = {
	{ 0x0006, L'ｲ', 0 },
	{ 0x0066, L'ﾛ', 0 },
	{ 0x0095, L'ﾊ', 0 },
	{ 0x0099, L'ﾆ', 0 },
	{ 0x0025, L'ﾎ', 0 },
	{ 0x0001, L'ﾍ', 0 },
	{ 0x0165, L'ﾄ', 0 },
	{ 0x0059, L'ﾁ', 0 },
	{ 0x0029, L'ﾘ', 0 },
	{ 0x0055, L'ﾇ', 0 },
	{ 0x0269, L'ﾙ', 0 },
	{ 0x006A, L'ｦ', 0 },
	{ 0x0026, L'ﾜ', 0 },
	{ 0x0065, L'ｶ', 0 },
	{ 0x000A, L'ﾖ', 0 },
	{ 0x0009, L'ﾀ', 0 },
	{ 0x002A, L'ﾚ', 0 },
	{ 0x00A9, L'ｿ', 0 },
	{ 0x0069, L'ﾂ', 0 },
	{ 0x00A6, L'ﾈ', 0 },
	{ 0x0019, L'ﾅ', 0 },
	{ 0x0015, L'ﾗ', 0 },
	{ 0x0002, L'ﾑ', 0 },
	{ 0x0016, L'ｳ', 0 },
	{ 0x0196, L'ヰ', 0 },
//	{ 0x0196, L'`', 0 },   			/* 'ヰ'の代替え文字 */
	{ 0x005A, L'ﾉ', 0 },
	{ 0x0195, L'ｵ', 0 },
	{ 0x0056, L'ｸ', 0 },
	{ 0x001A, L'ﾔ', 0 },
	{ 0x0096, L'ﾏ', 0 },
	{ 0x009A, L'ｹ', 0 },
	{ 0x00A5, L'ﾌ', 0 },
	{ 0x00AA, L'ｺ', 0 },
	{ 0x026A, L'ｴ', 0 },
	{ 0x019A, L'ﾃ', 0 },
	{ 0x029A, L'ｱ', 0 },
	{ 0x0266, L'ｻ', 0 },
	{ 0x0265, L'ｷ', 0 },
	{ 0x025A, L'ﾕ', 0 },
	{ 0x0256, L'ﾒ', 0 },
	{ 0x0166, L'ﾐ', 0 },
	{ 0x0299, L'ｼ', 0 },
	{ 0x01A5, L'ヱ', 0 },
//	{ 0x01A5, L'~', 0 },			/* 'ヱ'の代替え文字 */
	{ 0x0296, L'ﾋ', 0 },
	{ 0x0259, L'ﾓ', 0 },
	{ 0x01A9, L'ｾ', 0 },
	{ 0x02A6, L'ｽ', 0 },
	{ 0x0199, L'ﾝ', 0 },
	{ 0x0005, L'ﾞ', 0 },
	{ 0x0169, L'ﾟ', 0 },
	{ 0x01AA, L'1', 0 },
	{ 0x016A, L'2', 0 },
	{ 0x015A, L'3', 0 },
	{ 0x0156, L'4', 0 },
	{ 0x0155, L'5', 0 },
	{ 0x0255, L'6', 0 },
	{ 0x0295, L'7', 0 },
	{ 0x02A5, L'8', 0 },
	{ 0x02A9, L'9', 0 },
	{ 0x02AA, L'0', 0 },
	{ 0x01A6, L'ｰ', 0 },
	{ 0x0666, L'､', 0 },
	{ 0x0665, L'｣', 0 },			/* 段落 */
	{ 0x09A6, L'(', 0 },
	{ 0x0659, L')', 0 },
	{ 0x0965, L'ﾎ', L'ﾎ' },			/* 連続：指定 */
	{ 0x0596, L'ｳ', L'ｳ' },			/* 連続：記事 */
	{ 0x096A, L'ﾎ', L'ﾚ' },			/* 連続：和文開始 */
	{ 0x0159, L'ﾗ', L'ﾀ' },			/* 連続：和文訂正符号(欧文へ切り替え) */

	{ 0x0A5A, L',', 0 },
	{ 0x0A95, L':', 0 },
	{ 0x05A5, L'?', 0 },
	{ 0x06A9, L'\'', 0 },
	{ 0x0956, L'-', 0 },
	{ 0x5555, L'H', L'H' },			/* 連続：欧文訂正符号 */
	{ 0x0566, L'V', L'A' },			/* 連続：通信完了符号 */
	{ 0xC000, L' ', 0 },			/* SP(聞き取りモードで使用) */
	{ 0, 0, 0}						/* ターミネータ */
};

//---------------------------------------------------------------------------
__fastcall CWTABLE::CWTABLE()
{
}
//---------------------------------------------------------------------------
__fastcall CWTABLE::~CWTABLE()
{
}
//---------------------------------------------------------------------------
static DECODE* __fastcall _search_code(DECODE* p, Char code)
{
	while (p->code) {
		if ((p->code) == code) {
			return p;
		}
		p++;						    // 次のテーブルデータへ
	}

	return 0;
}

DECODE* __fastcall CWTABLE::search_code(int jmode, Char c)
{
	DECODE* p;

#if 1	/* 代替え文字の処理(従来互換) */
	if (c == L'`')						// 'ヰ'の代替え文字 ?
		c = L'ヰ';
	else if (c == L'~')					// 'ヱ'の代替え文字 ?
		c = L'ヱ';
	else
		;
#endif	

	if (jmode) {						// カナ(和文ﾃｰﾌﾞﾙを優先)
		p = _search_code(jtable, c);
		if (!p)
			p = _search_code(etable, c);
	}
    else {								// 英数(欧文ﾃｰﾌﾞﾙを優先)
		p = _search_code(etable, c);
		if (!p)
			p = _search_code(jtable, c);
	}

	return p;
}
//---------------------------------------------------------------------------
static DECODE* __fastcall _search_data(DECODE* p, DWORD data) {
	while (p->code) {
		if ((p->data) == data) {
			return p;
		}
		p++;							// 次のテーブルデータへ
	}

	return 0;
}

DECODE* __fastcall CWTABLE::search_data(int jmode, DWORD data)
{
	DECODE* p;
	
	if (jmode)
		p = _search_data(jtable, data);
	else
		p = _search_data(etable, data);

	return p;
}
//---------------------------------------------------------------------------
int __fastcall CWTABLE::ask_cwunit(DECODE *p)
{
	int i, cnt;
	WORD tdata;

	cnt = 0;

	tdata = p->data;
	for (i=0; i<8; i++) {
		if ((tdata & 0xc000) == 0)          // ｢0｣のｽｷｯﾌﾟ
			tdata <<= 2 ;                   // 最下位には｢0｣がシフト
	}

	for (i=0; i<8 ;i++) {
		if ((tdata & 0xc000) == 0x4000) {			// 短音？
			cnt += (DOT_UNIT+OFF_UNIT);		// 2
		}
		else if ((tdata & 0x0c000) == 0x8000) {		// 長音？
			cnt += (BAR_UNIT+OFF_UNIT);		// 4
		}
		else if ((tdata & 0x0c000) == 0xc000) {		// 空白(語間)？
			cnt += ((WORD_SPAN-CHAR_SPAN)-(CHAR_SPAN-OFF_UNIT));	// 2
		}
		else {						        // 文字終り
            cnt += (CHAR_SPAN-OFF_UNIT);	// 2
			return cnt;				        // 正常
		}

		tdata <<= 2 ;				        // 最下位には｢0｣がシフト
	}

    return -1;
}
//---------------------------------------------------------------------------

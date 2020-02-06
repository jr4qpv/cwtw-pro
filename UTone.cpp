//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <winuser.h>
#ifndef WIN32
#define WIN32	/* for mmsystem.h */
#endif
#include <mmsystem.h>

#include "UMain.h"
#include "UTone.h"
#include "BSnd.h"
#include "MSnd.h"
#include "DSnd.h"
#include "CwTable.h"
#include "Constant.h"
#include "Keytable.h"
#include "CWTW-Pro_def.h"
//---------------------------------------------------------------------------
CWTABLE *CWtbl;
BEEPSOUND *BSnd;
MIDISOUND *MSnd;
#if DSound_Use
DIRSOUND *DSnd;
#endif
//---------------------------------------------------------------------------
#define	DIRSND_DEBUG		0
#define	KEYTIME_DEBUG		0
#define	SQUEEZE_TIMNG		8

#define	TERMINATE_DATA		1
#define	RENZOKU_ON_DATA		2
#define	RENZOKU_OFF_DATA    3
#define	NEWLINE_DATA		4
#define	LAPSTART_DATA		5
#define	LAPSTOP_DATA		6
#define	KANA_ON_DATA		7
#define	KANA_OFF_DATA		8

DECODE termdata	= {0, TERMINATE_DATA};
DECODE renzoku1 = {0, RENZOKU_ON_DATA};
DECODE renzoku0 = {0, RENZOKU_OFF_DATA};
DECODE newline  = {0, NEWLINE_DATA};
DECODE lapstart = {0, LAPSTART_DATA};
DECODE lapstop  = {0, LAPSTOP_DATA};
DECODE kana_on	= {0, KANA_ON_DATA};
DECODE kana_off	= {0, KANA_OFF_DATA};

#define __max(a,b) (((a) > (b)) ? (a) : (b))
#define __min(a,b) (((a) < (b)) ? (a) : (b))
//---------------------------------------------------------------------------
static void CALLBACK tonetimeProc(UINT nTimerId, UINT msg, DWORD tonestmp,
						   DWORD para1, DWORD para2)
{
	TONESTM* p = (TONESTM *)tonestmp;
	p->ToneTime();
}

static void CALLBACK keytimeProc(UINT nTimerId, UINT msg, DWORD tonestmp,
					   DWORD para1, DWORD para2)
{
	TONESTM* p = (TONESTM *)tonestmp;
	p->KeyTime();
}

__fastcall TONESTM::TONESTM(int Mod, TONESPEC *tspp, int JId, HANDLE RSh,
						int Opt, HWND hwin)
{
	TIMECAPS	tc;
	short		cycle;

	nTimerId = NULL;
	timerflag = 0;
	JoyId = JId;						// ｼﾞｮｲｽﾃｨｯｸ番号
	RSHandle = RSh;						// RS-232Cﾊﾝﾄﾞﾙ
	cycle = keytable[tspp->ToneKey].cyc; // 周波数
	unittime = 6000 / tspp->ToneSpd;	// 1文字10ﾕﾆｯﾄで計算[mS]
    crtime = tspp->CrTime;              // 改行間隔時間
	unitcount = 0;
	lapcount = 0;
	renzoku = 0;

	switch (tspp->AutoCR) {				// 送信練習時の自動改行指定
	case 1:
		autocr_mode = 10; break;		// 短:10ﾕﾆｯﾄ検出で自動改行する
	case 2:
		autocr_mode = 50; break;		// 中:10ﾕﾆｯﾄ検出で自動改行する
	case 3:
		autocr_mode = 100; break;		// 長:10ﾕﾆｯﾄ検出で自動改行する
	default:
		autocr_mode = 0; break;			// 自動改行しない
	}

	CwDisp_mode = Opt & 0x10;			// 符号表示
    j_mode = Opt & 0x20;				// 和文モード
	a_mode = Opt & 0x40;				// 自動切替モード
	elkey_mode = Opt & 0x80;			// エレキーモード

    //*****( CWテーブルクラスを生成する )*****
	CWtbl = new CWTABLE();
	
    //*****( サウンドデバイスを作成する )*****
	SoundFlag = tspp->SoundSel;			// サウンド指定
	switch (SoundFlag) {
    case 1:
    	BSnd = new BEEPSOUND(tspp->BeepMode); break;
	case 2:
	    MSnd = new MIDISOUND(tspp->MidiNo); break;
    case 3:
#if DSound_Use
		DSnd = new DIRSOUND(tspp->DSndNo, hwin);
		key_ch = DSnd->MakeWave(5000, cycle, tspp->DSndWave);
	    dot_ch = DSnd->MakeWave(unittime, cycle, tspp->DSndWave);
	    bar_ch = DSnd->MakeWave(unittime * 3, cycle, tspp->DSndWave);
		DSnd->MainStart();
#endif
        break;
	}

	//*****( 高精度MIDIタイマーをセットアップする )*****
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	if ((nTimerRes = unittime / 2) <= 2)
		nTimerRes = 2;					// 最小を２に制限する
	nTimerRes = __min(__max(tc.wPeriodMin, nTimerRes), tc.wPeriodMax);

	timeBeginPeriod(nTimerRes);
	if (Mod >= CwSend_mode) {			// 送信練習モード？
		if (RSHandle != INVALID_HANDLE_VALUE) {
			EscapeCommFunction(RSHandle, SETDTR);	// DTR=ON
		}

		nTimerId = timeSetEvent(10, 5, keytimeProc,
						(DWORD)this, TIME_PERIODIC);
	}
	else {
		nTimerId = timeSetEvent(unittime, nTimerRes, tonetimeProc,
						(DWORD)this, TIME_PERIODIC);
	}
	timerflag = 1;
}
//---------------------------------------------------------------------------
__fastcall TONESTM::~TONESTM()
{
	// 高精度MIDIタイマーをシャットダウンする
	if (timerflag) {
		timeKillEvent(nTimerId);
	}
	timeEndPeriod(nTimerRes);

	// サウンドデバイスをクローズする
	switch (SoundFlag) {
	case 1:
		delete BSnd; break;
	case 2:
		delete MSnd; break;
	case 3:
#if DSound_Use
		DSnd->MainStop();
	    DSnd->UnLoadWave(key_ch);
	    DSnd->UnLoadWave(dot_ch);
	    DSnd->UnLoadWave(bar_ch);
	    delete DSnd;
#endif
    	break;
	}

    //*****( CWテーブルクラスを削除する )*****
    delete CWtbl;
}
//---------------------------------------------------------------------------
BOOL __fastcall TONESTM::Post_ToneMsg(Char c)
{
	return PostMessage(Main->Handle, USR_TONEMSG, (WPARAM)c, 0);
}
//---------------------------------------------------------------------------
void __fastcall TONESTM::OutOption(BYTE TNo, BYTE Key, Byte Vol, BYTE Eco)
{
	short cycle;
	cycle = keytable[Key].cyc; 		// 周波数

	switch (SoundFlag) {
	case 1:
		BSnd->Option(cycle); break;
	case 2:
		MSnd->Option(TNo, Key, Vol, Eco); break;
	case 3:
#if DSound_Use
		DSnd->Option(Vol);
#endif
		break;
	}
}
//---------------------------------------------------------------------------
int __fastcall TONESTM::LapStart(void)
{
	return ToneWr(&lapstart);
}
//---------------------------------------------------------------------------
int __fastcall TONESTM::LapStop(void)
{
	return ToneWr(&lapstop);
}
//---------------------------------------------------------------------------
DWORD __fastcall TONESTM::Terminate(void)
{
	ToneWr(&termdata);			  		// 停止要求
	do {
		Sleep(100);
	} while (timerflag);				// タイマー停止になるまで待つ
	Sleep(100);

	return laptime;
}
//---------------------------------------------------------------------------
int __fastcall TONESTM::ToneWrChar(Char c)
{
// 戻り値：
//   =0:書き込めない, =-1:制御コード他を書き込んだ
//   >0:文字を書き込んだ（UNIT数を返す）
	DECODE* p;

	if (bufchk() < 0)
		return 0;						// 書き込めない

	switch (c) {
    case '[':
        if (ToneWr(&renzoku1) == 0)
            return 0;                   // 書き込めない
        else
            return -1;                  // 制御コードを書き込んだ
	case ']':
		if (ToneWr(&renzoku0) == 0)
            return 0;                   // 書き込めない
        else
            return -1;                  // 制御コードを書き込んだ
	case '\n': case '\r':
    	if (ToneWr(&newline) == 0)
            return 0;                   // 書き込めない
        else
            return -1;                  // 制御コードを書き込んだ
	case KANA_ON_CHR:
		j_mode = 1;						// カナ
		if (ToneWr(&kana_on) == 0)
            return 0;                   // 書き込めない
        else
            return -1;                  // 制御コードを書き込んだ
    case KANA_OFF_CHR:
		j_mode = 0;						// 英数
    	if (ToneWr(&kana_off) == 0)
            return 0;                   // 書き込めない
        else
            return -1;                  // 制御コードを書き込んだ
	}

	// CWテーブルの検索
	p = CWtbl->search_code(j_mode, c);
	if (p) {
		if (ToneWr(p) == 0)
            return 0;                   // 書き込めない
        else
            return CWtbl->ask_cwunit(p);	// UNIT数を返す
    }
	else
		return -1;						// 無視
}
//---------------------------------------------------------------------------
void __fastcall TONESTM::cw_on(int mode)
{
	switch (SoundFlag) {
    case 1:
		BSnd->Start(); break;
	case 2:
		MSnd->Start(); break;
	case 3:
#if DSound_Use
		if (mode == 1)
			DSnd->Start(dot_ch, 0);		// 短点
		else if (mode == 2)
			DSnd->Start(bar_ch, 0);		// 長点
		else
			DSnd->Start(key_ch, 1);		// 連続
#endif
		break;
    }

	if (RSHandle != INVALID_HANDLE_VALUE) {
		EscapeCommFunction(RSHandle, SETRTS);	// RTS=On
	}
}
//---------------------------------------------------------------------------
void __fastcall TONESTM::cw_off(int mode)
{
	switch (SoundFlag) {
    case 1:
		BSnd->Stop(); break;
	case 2:
		MSnd->Stop(); break;
	case 3:
#if DSound_Use
		if (mode == 0)
		    DSnd->Stop(key_ch);				// 連続
#endif
		break;
   	}

	if (RSHandle != INVALID_HANDLE_VALUE) {
		EscapeCommFunction(RSHandle, CLRRTS);	// RTS=Off
    }
}
//---------------------------------------------------------------------------
int __fastcall TONESTM::get_btn(void)
{
	int btn = 0;

	// キーボードからの入力処理
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)	// Shiftキー？
		btn |= 1;
	if (GetAsyncKeyState(VK_MENU) & 0x8000)		// ALTキー？
		btn |= 1;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)	// Ctrlキー？
		btn |= 2;

	//　ジョイスティックからの入力処理
	if (JoyId >= 0) {
		JOYINFO ji;

		if (joyGetPos(JoyId, &ji) == JOYERR_NOERROR) {
			if (ji.wButtons & JOY_BUTTON1)		// 短点?
				btn |= 1;
		    if (ji.wButtons & JOY_BUTTON2)		// 長点?
				btn |= 2;
		}
	}

	// RS-232Cからの入力処理
	if (RSHandle != INVALID_HANDLE_VALUE) {
		DWORD stat;

		if (GetCommModemStatus(RSHandle, &stat)) {
			if (stat & MS_DSR_ON)           	// 短点?
				btn |= 1;
			if (stat & MS_CTS_ON)				// 長点?
	 			btn |= 2;
		}
	}

	return btn;
}

//---------------------------------------------------------------------------
void TONESTM::ToneTime(void)
{
	static int mode = -1;
	static int cnt = 0;
	static DWORD tdata;
	static Char tcode;
	int i;
	DECODE* datap;

	(unitcount)++;
#if 0
	if ((unitcount) < 10) {
		Char buf[64];
		swprintf(buf, L"GetTime->%d", timeGetTime());
		OutputDebugString(buf);
	}
#endif

	while (mode < 0) {
        if (Main->Pause_Flag)           // 一時停止?
            return;
		if ((datap = ToneRd()) == 0)
        	return;						// ﾊﾞｯﾌｧｰが空
		tdata = datap->data;
		tcode = datap->code;
		cnt = 0;

		if (tdata == 0) {
			switch (tcode) {
			case TERMINATE_DATA:
				timeKillEvent(nTimerId);	// タイマーを停止
				timerflag = 0;
				return;
			case RENZOKU_ON_DATA:
				renzoku = 1;
				if (!CwDisp_mode)
					Post_ToneMsg('[');
				continue;
			case RENZOKU_OFF_DATA:
            	renzoku = 0;
				tcode = 0;
				mode = 0;
				if (!CwDisp_mode)
					Post_ToneMsg(']');
				break;
			case NEWLINE_DATA:
				mode = 4;
				Post_ToneMsg('\n');
				return;
			case LAPSTART_DATA:
				unitcount = 0;
				lapcount = 0;
				laptime = timeGetTime();
				continue;
			case LAPSTOP_DATA:
				lapcount = unitcount;
				laptime = timeGetTime() - laptime;
				continue;
            case KANA_ON_DATA:
				Post_ToneMsg(KANA_ON_CHR);
				continue;
			case KANA_OFF_DATA:
				Post_ToneMsg(KANA_OFF_CHR);
				continue;
			}
			return;
		}
		if (tcode == 0) {
////			if (hmo) {
////				midiOutShortMsg(hmo, tdata);
////			}
			return;
		}

		for (i=0; i<8; i++) {
			if ((tdata & 0xc000) == 0)	// ｢0｣のｽｷｯﾌﾟ
				tdata <<= 2;			// 最下位には｢0｣をｼﾌﾄ
		}
        mode = (tdata >> 14) & 3;		// ループ脱出
    }
	cnt++;

	switch (mode) {
    case 1:								// 短音
		if (cnt == 1) {
			cw_on(1);
			return;
		}
		else if (cnt == (DOT_UNIT+1)) {	// 2
			cw_off(1);
			if (CwDisp_mode)
				Post_ToneMsg('･');
			break;						// 次の符号へ
		}
		else {
			return;
		}

	case 2:								// 長音
  		if (cnt == 1) {
			cw_on(2);
			return;
		}
		else if (cnt == (BAR_UNIT+1)) {	// 4
			cw_off(2);
			if (CwDisp_mode)
				Post_ToneMsg('-');
			break;						// 次の符号へ
		}
		else {
			return;
		}

	case 3:								// 語間(空白)
		if (cnt == (WORD_SPAN-(CHAR_SPAN*2)+1)) {	// 2
			if (CwDisp_mode)
				Post_ToneMsg(' ');
        	break;						// 次の符号へ
        }
		else {
			return;
		}


	case 4:								// 改行
    	if (cnt > (int)(crtime / unittime)) {
			mode = -1;
			return;
        }
		else {
			return;
		}

	default:							// 字間(文字終わり)
 		if (cnt == (CHAR_SPAN-1)) {		// 2
			mode = -1;					// 次文字の処理へ
			if (tcode) {
				if (CwDisp_mode)
					Post_ToneMsg(' ');
				else
					Post_ToneMsg(tcode);
			}
			return;
		}
		else {
			return;
		}
	}

	tdata <<=2;
	mode = (tdata >> 14) & 3;
	cnt = 0;

	if ((renzoku) && (mode == 0)) {
    	if (!CwDisp_mode)
			Post_ToneMsg(tcode);
		mode = -1;						// 次文字の処理へ
	}
	return;
}
//---------------------------------------------------------------------------
static int __fastcall unit_adj(int count)
{
	static int ptr = -1;				// バッファーが未初期化
	static int adj_buff[4];

	if (ptr == -1) {
		adj_buff[0] = count;
		adj_buff[1] = count;
		adj_buff[2] = count;
	}

	if (--ptr < 0)
		ptr = 3;
	adj_buff[ptr] = count;

	return (adj_buff[0]+adj_buff[1]+adj_buff[2]+adj_buff[3]) >> 2;
}


void TONESTM::KeyTime(void)
{
	static int mode = -1;
	static int cnt = 0;
	static int unit;
	static int btn, btn0, btnmem;
	static int dot_chkcnt, bar_chkcnt;
	static DWORD tdata, data;
	static WORD tcode;
	DECODE* datap;

	(unitcount)++;

	if ((datap = ToneRd()) != 0) {
		tdata = datap->data;
		tcode = datap->code;

		if (tdata == 0) {
			switch (tcode) {
			case TERMINATE_DATA:
				timeKillEvent(nTimerId);	// タイマーを停止
				timerflag = 0;
				mode = -1;
				return;
			case KANA_OFF_DATA:
				j_mode = 0; 				// 欧文モードへ切り替え
				Post_ToneMsg(KANA_OFF_CHR);
				break;
            case KANA_ON_DATA:
				j_mode = 1; 				// 和文モードへ切り替え
				Post_ToneMsg(KANA_ON_CHR);
				break;
			}
			return;
		}
    }

	if ((btn = get_btn()) < 0)
		return;								// 入力不正
	btnmem |= (btn ^ btn0) & btn;			// Onｴｯｼﾞ情報の記憶
	btn0 = btn;								// ｷｰ情報の記憶

	if (cnt < 0x7fffffff)
	       	cnt++;

	switch (mode) {
	case 0:									//*** キー検出のチェック ***
		if (elkey_mode) {
			//=====( エレキー検出のチェック )======
			if ((btnmem & 3) || (btn & 3)) {
				cw_on(0);
				cnt = 0;
				data <<= 2;

				if (btnmem & 1) {
		            data += 1;
					mode = 3;
				}
				else if (btnmem & 2) {
					data += 2;
					mode = 4;
				}
				else if (btn & 1) {
		            data += 1;
					mode = 3;
				}
				else if (btn & 2) {
					data += 2;
					mode = 4;
				}
				else {
					mode = -1;				// 不正
				}
				break;
			}
		}
		else {
			//=====( 縦振キー検出のチェック )======
			if (btn & 3) {
				cw_on(0);

				// 速度調整(高速側)
				if (cnt <= unit) {
					unit = unit_adj(cnt);
		        }

				cnt = 0;
	            mode = 2;
				break;
			}
		}

	case 1:									//*** スペースのループ処理 ***
		//=====( スペース時の処理 )=====
		if ((cnt == unit*2) && (!CwDisp_mode)) {

			datap = CWtbl->search_data(j_mode, data);
			if (datap) {
				if (datap->cod2 != 0) {
					Post_ToneMsg('[');
					Post_ToneMsg(datap->code);
					Post_ToneMsg(datap->cod2);
					Post_ToneMsg(']');
				}
				else {
					Post_ToneMsg(datap->code);
				}

				// 自動切り替え処理
				if (a_mode) {
					if (j_mode &&						
						((datap->code == L'ﾗ') && (datap->cod2 == L'ﾀ'))) {
						j_mode = 0; 	// 欧文モードへ切り替え
						Post_ToneMsg(KANA_OFF_CHR);
					}
					else if (!j_mode &&
						 ((datap->code == L'ﾎ') && (datap->cod2 == L'ﾚ'))) {
						j_mode = 1; 	// 和文モードへ切り替え
						Post_ToneMsg(KANA_ON_CHR);
					}
					else
					;
                }
			}

			data = 0;
		}

        if (cnt == unit*5)
			Post_ToneMsg(' ');

		if (autocr_mode != 0) {				 // 自動改行指定?
        	if (cnt == unit * autocr_mode)
				Post_ToneMsg('\n');
		}

        break;								// Onまで待つ

	case 2:									//*** マークのループ処理 ***
		//=====( マーク時の処理 )=====
    	if (!(btn & 3)) {					// Offまで待つ
			cw_off(0);

			// 速度調整(低速側)
            if (cnt/3 > unit) {
				unit = unit_adj(cnt/3);
			}

            // 長点･短点の判断
            data <<= 2;
            if (cnt > unit*2) {
            	data += 2;					// 長点
				if (CwDisp_mode)
					Post_ToneMsg('-');
            }
            else {
           		data += 1;					// 短点
				if (CwDisp_mode)
					Post_ToneMsg('･');
			}

			cnt = 0;
            mode = 0;
        }
        break;

	case 3:									//*** エレキー短点時間ｶｳﾝﾄ処理 ***
		if (cnt == dot_chkcnt) {
			if ((btn & 3) == 3) {
				btnmem |= 2;				// 長点の記憶
			}
		}
		else if (cnt >= unit) {
			cw_off(0);
			btnmem &= ~1;					// 短点ﾒﾓﾘｰのｸﾘｱ
			cnt = 0;
			mode = 5;
			if (CwDisp_mode)
				Post_ToneMsg('･');
        }
		break;

	case 4:									//*** エレキー長点時間ｶｳﾝﾄ処理 ***
		if (cnt == bar_chkcnt) {
			if ((btn & 3) == 3) {
				btnmem |= 1;				// 短点の記憶
			}
		}
		else if (cnt >= unit*3) {
			cw_off(0);
			btnmem &= ~2;					// 長点ﾒﾓﾘｰのｸﾘｱ
			cnt = 0;
			mode = 5;
			if (CwDisp_mode)
				Post_ToneMsg('-');
        }
		break;

	case 5:									//*** ｽﾍﾟｰｽ時間ｶｳﾝﾄ処理 ***
		if (cnt >= unit) {
			mode = 0;
		}
		break;

    default:
		cw_off(0);

		if (elkey_mode) {
			unit = unittime / 10;
			dot_chkcnt = unit / SQUEEZE_TIMNG;		// 短点ｽｸｰｲｽﾞﾁｪｯｸﾀｲﾐﾝｸﾞ
			if (dot_chkcnt == 0)
				dot_chkcnt = 1;
			bar_chkcnt = unit*3 / SQUEEZE_TIMNG;	// 長点ｽｸｰｲｽﾞﾁｪｯｸﾀｲﾐﾝｸﾞ
			if (bar_chkcnt == 0)
				bar_chkcnt = 1;
			cnt = unit * 10;
		}
		else {
			unit = 0x10000;
			cnt = 0;
		}

		data = 0;
        btn0 = 0;	  						// 前回のｷｰ状態
		btnmem = 0;							// Onｴｯｼﾞﾒﾓﾘｰ
        mode = 0;
        break;
    }
////char dbuff[128];
////swprintf(dbuff, L"rc=%d, btn=%d", (int)rc, ji.wButtons);
////OutputDebugString(dbuff);

	return;
}
//---------------------------------------------------------------------------

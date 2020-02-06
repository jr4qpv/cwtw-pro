//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <stdio.h>
#include <filectrl.hpp>

#include "ToneThd.h"
#include "UMain.h"
#include "UTone.h"
#include "UEnv.h"
#include "UEdit.h"
#include "Constant.h"
#include "KbdTbl.h"
//---------------------------------------------------------------------------
TONESTM* tone;
LINGBUF* kbdbuff;

#define	START_MSG		";***Start***"
#define	COMPLETE_MSG	";***Complete***"
#define	ABORT_MSG		";***Abort***"

//*******( 乱数発生用 文字テーブル )*******
String ernd_table = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
String jrnd_table = L"ｲﾛﾊﾆﾎﾍﾄﾁﾘﾇﾙｦﾜｶﾖﾀﾚｿﾂﾈﾅﾗﾑｳﾉｵｸﾔﾏｹﾌｺｴﾃｱｻｷﾕﾒﾐｼﾋﾓｾｽﾝﾞﾟ";
String xrnd_table = L"ｲﾛﾊﾆﾎﾍﾄﾁﾘﾇﾙｦﾜｶﾖﾀﾚｿﾂﾈﾅﾗﾑｳ`ﾉｵｸﾔﾏｹﾌｺｴﾃｱｻｷﾕﾒﾐｼ~ﾋﾓｾｽﾝﾞﾟ";
String esym_table = L".,:?\'-()/\"@";
String jsym_table = L"ｰ､｣()";
String num_table  = L"1234567890";
String daku1table = L"ｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾊﾋﾌﾍﾎ";     // 濁点文字
String daku2table = L"ﾊﾋﾌﾍﾎ";                    // 半濁点文字

//---------------------------------------------------------------------------
__fastcall ToneThread::ToneThread(bool CreateSuspended, int mode, HWND hwin)
	: TThread(CreateSuspended)
{
	TestTime = 0;
	cw_mode = mode;						// 練習モード
	whandle = hwin;						// Windowのﾊﾝﾄﾞﾙ(DirectSoundで必要必要)
	kana_flag = false;
}
//---------------------------------------------------------------------------
int __fastcall ToneThread::KbdWr(int key)
{
	return kbdbuff->bufwr(key);
}
//---------------------------------------------------------------------------
int __fastcall ToneThread::ToneChar(Char c)
{
    // 戻り値 >=0:発音CWユニット数, =-1:中断検出

	// カナ／英数の自動切替判定
	int judge_detect = 0;
	int tmp = kana_judge;
    int cnt;
	kana_judge = 99;

	if (Main->AMode->Checked) {
		switch (c) {
		case L'_':
        	c = ' ';					// 空白文字に変換
			break;

		case KANA_ON_CHR:
        	kana_flag = true;
            break;

        case KANA_OFF_CHR:
        	kana_flag = false;
            break;

	    case L'[':
	    	kana_judge = 0;
	    	break;

		case L']':
			if (tmp == 2) {
				judge_detect = 1;		// カナへ切り替え
				break;
			}
			if (tmp == -2) {
				judge_detect = -1;		// 英数へ切り替え
				break;
			}
			break;

		case L'ﾎ':
	    	if (tmp == 0)
	        	kana_judge = 1;
			break;

		case L'ﾚ':
			if (tmp == 1)
	        	kana_judge = 2;
	        break;

	    case L'ﾗ':
	    case L'B':
	    	if (tmp == 0)
				kana_judge = -1;
	        break;

	    case L'ﾀ':
	    case L'T':
	    	if (tmp == -1)
	        	kana_judge = -2;
	        break;
		}
	}

	// 特殊文字の処理
	switch (c) {
	case L'_':
       	c = ' ';						// 空白文字に変換
		break;

    case L'#':
    	c = '\n';						// 改行文字に変換
		break;

	case KANA_ON_CHR:
       	kana_flag = true;
		break;

    case KANA_OFF_CHR:
		kana_flag = false;
		break;
	}

	// 指定文字を書き込めるまで出力
	while ((cnt = tone->ToneWrChar(c)) == 0) {	// 書き込めるまでループ
		if (Terminated)
			return -1;					// 中断検出で書き込めない
		Sleep(250);
	}

	// 自動切替判定結果の切替文字を出力
	if (judge_detect) {
		if (judge_detect > 0) {
			kana_flag = true;
			c = KANA_ON_CHR;
		}
        else {
			kana_flag = false;
            c = KANA_OFF_CHR;
        }

		while ((tone->ToneWrChar(c)) == 0) {	// 書き込めるまでループ
			if (Terminated)
				return -1;				// 中断検出で書き込めない
			Sleep(250);
		}
	}

    if (cnt < 0)                        // 制御コードなど?
        cnt = 0;
	return cnt;							// 正常に書き込めた(CWユニット数)
}
//---------------------------------------------------------------------------
int __fastcall ToneThread::ToneStr(String str)
{
    int i, j, cnt;

    cnt = 0;
    for (i=1; i<=str.Length(); i++) {
		if ((j = ToneChar(str[i])) < 0)
			return -1;					// 中断検出で書き込めない
        cnt += j;
	}

	return cnt;							// 正常に書き込めた(CWユニット数)
}
//---------------------------------------------------------------------------
int __fastcall ToneThread::ToneFile(String fname)
{
	int i, len;
	Char c;
	PChar s;
	
	if (!FileExists(fname)) {
		ShowMessage( fname + "ファイルが\n見つかりません");
		return -1;		   				// 異常
	}

	ToneFileName = MinimizeName(fname, Main->Status->Canvas, 250);
	Synchronize(OutputStatus);			// ファイル名の表示
	
	TStreamReader* fs = new TStreamReader(fname, TEncoding::UTF8, true, 1024);
	while (fs->EndOfStream == false)
	{
		String text = fs->ReadLine().UpperCase();
		if ((len = text.Length()) == 0) {	// 空行 ?
			ToneChar('\n');
			continue;
		}

#if 1	/* 全角文字を半角文字に変換する */
		DWORD flags = LCMAP_HALFWIDTH;	// 半角文字に変換
		const int size = len + 1;
		if (size < 128) {				// バッファに入る時だけ変換する
			s = _msgbuff;
			LCMapString(GetUserDefaultLCID(),
						flags,
						text.c_str(),
						size,
						(LPWSTR)s,		// 変換した文字列の格納先
						size);
			s[len] = 0;
		}
		else {
			s = text.c_str();
		}
#else
		s = text.c_str();
#endif		
//		ShowMessage(s);					/* for debug */

		for (i=0; i<len; i++) {
			c = s[i];
			if (c == L';') {
				if (s[i+1] == L'|') {	// メッセージ出力モード ?
					c = L'|';
					wcsncpy(_msgbuff, &s[i+2], 128);
					_msgbuff[128] = 0;
				}
				break;
			}

			if (ToneChar(c) < 0) {
				fs->Close();
				return 1;  				// 中断検出した
			}
			
		}
		if (c == L';') continue;		// ';'から改行までの文字は無視
		if (c == L'|') {
			if (Env->CM_Mode->Checked)
				Synchronize(DisplayMessage);	// メッセージの表示
			continue;
		}
		
		ToneChar('\n');
	}	
	fs->Close();

	return 0;							// 正常
}
//---------------------------------------------------------------------------
int __fastcall ToneThread::ToneMemo(void)
{
	int i;
    String s;

	for (i=0; i<EditFile->Memo->Lines->Count; i++) {
		s = EditFile->Memo->Lines->Strings[i];
		if (s.Length() != 0) {
			if (s[1] == ';')
				continue;				// コメント行なので無視
			ToneStr(s);
		}
		ToneChar('\n');
	}

	return 0;							// 正常
}
//---------------------------------------------------------------------------
int __fastcall ToneThread::ToneEmpty(void)
{
	while (tone->bufchk() != 0) {		// バッファーが空になるまで待つ
		if (Terminated)
			return 1;					// 中断検出した
		Sleep(250);
	}

	return 0;							// 中断検出しない
}
//---------------------------------------------------------------------------
void __fastcall ToneThread::OutputStatus(void)
{
	Main->Status->Panels->Items[0]->Text = ToneFileName;
}
//---------------------------------------------------------------------------
void __fastcall ToneThread::DisplayMessage(void)
{
	Main->Memo->Lines->Add(_msgbuff);
}
//---------------------------------------------------------------------------
void __fastcall ToneThread::DisplayStart(void)
{
	Main->Memo->Lines->Add("");
	Main->Memo->Lines->Add(START_MSG);
}
//---------------------------------------------------------------------------
void __fastcall ToneThread::DisplayComplete(void)
{
	Main->Memo->Lines->Add(COMPLETE_MSG);
	Main->Memo->Lines->Add("");
}
//---------------------------------------------------------------------------
void __fastcall ToneThread::DisplayAbort(void)
{
	Main->Memo->Lines->Add(ABORT_MSG);
	Main->Memo->Lines->Add("");
}
//---------------------------------------------------------------------------
void __fastcall ToneThread::Execute()
{
	int i, j, n, units;
	Char c, c1, sep;
	String fn;
	String rndbuf;
	TONESPEC tsp;

	kana_judge = 99;						// [ﾎﾚ]/[ﾗﾀ]判定ﾌﾗｸﾞ

	int j_flag = Main->JMode->ItemIndex & 1;		// 和文指定？
	int l_flag = (Main->LMode->Checked == True);	// ループ指定？

	int JId = -1;
	HANDLE RSh = INVALID_HANDLE_VALUE;
	switch (Env->KIMode->ItemIndex) {
	case 1:
		RSh = Env->GetRSHandle(); break;
	case 2:
		JId = Env->GetJoyDevNo(); break;
	}

	int  Opt = 0;
    if (Env->DispMod->ItemIndex == 2)
		Opt |= 0x10;					// 符号表示指定
	if (j_flag)
		Opt |= 0x20;					// 和文指定
	if (Main->AMode->Checked)
		Opt |= 0x40;					// 自動切替指定
	if (Env->DKMode->ItemIndex)
		Opt |= 0x80;					// エレキーモード

	tsp.SoundSel = Env->SndSel->ItemIndex;		// サウンド選択
	tsp.BeepMode = Env->BeepMod->ItemIndex;		// BEEPﾓｰﾄﾞ(機種)
	tsp.MidiNo = Env->MidiDev->ItemIndex;		// MIDIﾄﾞﾗｲﾊﾞ番号
	tsp.DSndNo = Env->DSndDev->ItemIndex;		// WAVEﾄﾞﾗｲﾊﾞ番号
	tsp.DSndWave = Env->DSndWave->ItemIndex;	// 波形番号
	tsp.ToneSpd = Main->ToneSpdExe->Position;	// 発信ｽﾋﾟｰﾄﾞ
	tsp.CrTime = Env->CrTime->Position * 1000;	// 改行間隔時間[ms]
	tsp.ToneNo = (BYTE)(Env->ToneNo->Position);	// 音色番号
	tsp.ToneKey = (BYTE)(Env->ToneKey->Position);	// 音程番号
	tsp.ToneVol = (BYTE)(Env->ToneVol->Position);	// ボリューム
	tsp.ToneEco = (BYTE)(Env->ToneEco->Position);	// 残響
	tsp.AutoCR  = Env->SendAutoCR->ItemIndex;	// 送信練習時の自動改行

	kbdbuff = new LINGBUF;
	tone = new TONESTM(cw_mode, &tsp, JId, RSh, Opt, whandle);
	tone->OutOption(tsp.ToneNo, tsp.ToneKey, tsp.ToneVol, tsp.ToneEco);

	//*****( 乱数用文字テーブルの作成 )*****
	randomize();

	if (j_flag) {
        if (Main->XMode->Checked) {
            if (Main->YMode->Checked)
                rndbuf = xrnd_table + jsym_table;
            else
                rndbuf = xrnd_table;
        }
        else {
            if (Main->YMode->Checked)
                rndbuf = jrnd_table + jsym_table;
            else
		        rndbuf = jrnd_table;
        }
		kana_flag = true;
    	Main->Status->Panels->Items[4]->Text = "カナ";
	}
	else {
        if (Main->YMode->Checked)
            rndbuf = ernd_table + esym_table;
        else
		    rndbuf = ernd_table;
		kana_flag = false;
    	Main->Status->Panels->Items[4]->Text = "英数";
    }

	if (Main->NMode->Checked)
		rndbuf = rndbuf + num_table;

	int len = rndbuf.Length();

	//*****( モード別処理 )*****
	Synchronize(DisplayStart);
	do {
		Sleep(500);

		switch (cw_mode) {
		//=====( 耳慣らし )=====
		case WarmUp_mode:
			Main->Status->Panels->Items[0]->Text = "耳慣らし受信練習";
			ToneStr(rndbuf);
			ToneChar('\n');
			break;

		//=====( キーボード入力 )=====
		case KeyIn_mode:
			Main->Status->Panels->Items[0]->Text = "キーボード入力受信練習";
			while (!Terminated) {
				i = kbdbuff->bufrd();
                j = (i >> 8) & 0x01;
				i &= 0xFF;

                if (i == 0) {
					Sleep(50);				// キー入力待ち
                    continue;
				}

				if (kana_flag)
                	j += 2;
                c = keyboard_table[i][j];

				switch (c) {
				case 0:
					break;					// 無視
				case (Char)0xF1:
                   	ToneStr(Env->KbdMem1->Text); break;
				case (Char)0xF2:
                   	ToneStr(Env->KbdMem2->Text); break;
				case (Char)0xF3:
                   	ToneStr(Env->KbdMem3->Text); break;
				case (Char)0xF4:
                   	ToneStr(Env->KbdMem4->Text); break;
				case (Char)0xF5:
                   	ToneStr(Env->KbdMem5->Text); break;
				case (Char)0xF6:
                   	ToneStr(Env->KbdMem6->Text); break;
				case (Char)0xF7:
                   	ToneStr(Env->KbdMem7->Text); break;
				case (Char)0xF8:
                   	ToneStr(Env->KbdMem8->Text); break;
				case (Char)0xF9:
                   	ToneStr(Env->KbdMem9->Text); break;
				case (Char)0xFA:
                   	ToneStr(Env->KbdMem10->Text); break;

				case (Char)0x24:			/* HOMEｷｰ? */
					ToneFile(Main->DataFile->Text); break;

				default:
					ToneChar(c); break;
				}
			}
			break;

		//=====( 文字ランダム )=====
		case FileChar_mode:
			if (EditFile->Memo->Lines->Count >= 1) {
		 		rndbuf = EditFile->Memo->Lines->Strings[0];
				len = rndbuf.Length();
		 	}

		//=====( 乱数暗文 )=====
		case CharRand_mode:
		    Main->Status->Panels->Items[0]->Text = "乱数受信練習";
            i = Env->RndTime->Position;         // 練習時間
            units = tsp.ToneSpd * 10 * i;       // 練習時間文の総ユニット数

#if 0   /// for debug ///
            i = ToneStr(L"PARIS ");
            Main->Status->Panels->Items[0]->Text = "units=" + IntToStr(i);
#else
	        if (j_flag)
				units -= ToneStr(L"HR HR [ﾎﾚ] ");
			else
				units -= ToneStr(L"HR HR [BT] ");
#endif

            while (units > 0) {
				if (Terminated)
					break;

                if (Env->RndRandom->Checked) {
                    // 区切文字数ランダム
                    i = Env->RndCount->Position - Env->RndCntMin->Position;
                    if (i > 0) {
                        n = random(i + 1) + Env->RndCntMin->Position;
                    }
                    else {
                        n = Env->RndCount->Position;    // 固定にする
                    }
                }
                else {
                    // 区切文字数固定
                    n = Env->RndCount->Position;        // 乱数文字数
                }

                sep = L' ';
                if (j_flag) {
                    switch (Env->RndSeparator->ItemIndex) {
                    case 0:
                        n = 0;                         // 区切文字なし
                        break;
                    case 2:
                        n = 60;
                        sep = L'?';
                        break;
                    default:
                        sep = L' ';
                        break;
                    }
                }


                i = n;
				do {
					c = rndbuf[random(len)+1];

                    if (c == L'ﾞ') {
                        // 濁点文字の発生
                        j = random(daku1table.Length());
                        c1 = daku1table[j+1];
                        units -= ToneChar(c1);
                    }
                    if (c == L'ﾟ') {
                        // 半濁点文字の発生
                        j = random(daku2table.Length());
                        c1 = daku2table[j+1];
                        units -= ToneChar(c1);
                    }
					units -= ToneChar(c);
				} while (--i > 0);

                if (n > 0) {
                    units -= ToneChar(sep);
                    if (sep == '?')
                        units -= ToneChar('\n');
                }
			}

			if (j_flag)
				ToneStr(L"[ﾗﾀ]");
			else
				ToneStr(L"[AR]");
			ToneChar('\n');
			break;

		//=====( テキストファイル )=====
		case FileText_mode:
			fn = Main->DataFile->Text;
			if (ExtractFilePath(fn).IsEmpty())
				fn = Env->DataDir->Text + fn;

			ToneFile(fn);
			break;

		//=====( ファイルランダム )=====
		case FileRand_mode:
			j = Main->get_random(EditFile->Memo->Lines->Count);

			fn = EditFile->Memo->Lines->Strings[j];
			if (ExtractFilePath(fn).IsEmpty()) {
                fn = ExtractFilePath(EditFile->StatusBar->SimpleText) + fn;
            }

			ToneFile(fn);
	    	break;

		//=====( ワードランダム )=====
		case FileWord_mode:
            i = Env->RndTime->Position;         // 練習時間
            units = tsp.ToneSpd * 10 * i;       // 練習時間文の総ユニット数

			if (j_flag)
				units -= ToneStr(L"HR HR [ﾎﾚ] ");
			else
				units -= ToneStr(L"HR HR [BT] ");

            while (units > 0) {
				if (Terminated)
					break;

				j = Main->get_random(EditFile->Memo->Lines->Count);

				units -= ToneStr(EditFile->Memo->Lines->Strings[j]);
			}

			if (j_flag)
				ToneStr(L"[ﾗﾀ]");
			else
				ToneStr(L"[AR]");
			ToneChar('\n');
			break;

		//=====( ラインランダム )=====
		case FileLine_mode:
			j = Main->get_random(EditFile->Memo->Lines->Count);

			ToneStr(EditFile->Memo->Lines->Strings[j]);
			ToneChar('\n');
			break;

		//=====( 受信練習リピート )=====
		case Repeat_mode:
			Main->Status->Panels->Items[0]->Text = "受信練習リピート";
            ToneMemo();
            break;



		//=====( 送信練習モード )=====
		case CwSend_mode:
			if (Opt & 0x80)					// エレキーモード？
				Main->Status->Panels->Items[0]->Text = "エレキー送信練習"
                			" ([Shift]:短点キー, [Ctrl]:長点キー)";
			else
				Main->Status->Panels->Items[0]->Text = "縦振キー送信練習 ([Shift]:キー)";

			if (Env->KIMode->ItemIndex == 1) {
				if (RSh == INVALID_HANDLE_VALUE) {
					ShowMessage("RS-232Cが未接続です！");
					break;
				}
			}

			if (Env->KIMode->ItemIndex == 2) {
				if (Env->JoyDevCnt == 0) {
					ShowMessage("ジョイスティックが未接続です！");
					break;
				}
			}

			while (!Terminated) {
				i = kbdbuff->bufrd();
                j = (i >> 8) & 0x01;
				i &= 0xFF;

                if (i == 0) {
					Sleep(100);				// キー入力待ち
                    continue;
				}

                c = keyboard_table[i][j];

				switch (c) {
				case KANA_ON_CHR:
					ToneChar(c); break;
	        	case KANA_OFF_CHR:
					ToneChar(c); break;
				}
			}
			break;

		//=====( PARIS基準 )=====
		default:
			l_flag = 0;
			tone->LapStart();
			ToneStr(L"PARIS ");
			tone->LapStop();
			ToneChar('\n');
			break;
		}

		//=====( 発音完了待ち )=====
		ToneEmpty();					// バッファーが空になるまで待つ
		if (Terminated) {
			l_flag = 0;
			tone->bufclr();				// バッファーをクリア
		}
	} while (l_flag);

	//*****( 終了処理 )*****
	TestTime = tone->Terminate();		// 出力完了待ち
	if (RSh != INVALID_HANDLE_VALUE) {
		CloseHandle(RSh);
	}
	delete tone;
	delete kbdbuff;
    if (Terminated)
		Synchronize(DisplayAbort);
	else
		Synchronize(DisplayComplete);
}
//---------------------------------------------------------------------------

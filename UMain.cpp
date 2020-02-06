//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>
#include <winbase.h>
#include <stdio.h>
#include <mmsystem.h>
#include <ShellAPI.h>
#include <ctype.h>
#include <dir.h>
#include <Vcl.FileCtrl.hpp>

#include "UMain.h"
#include "UEnv.h"
#include "URegInf.h"
#include "UReg.h"
#include "UEdit.h"
#include "UAbout.h"
#include "ToneThd.h"
#include "MyRandom.h"
#include "debug.h"
#include "Constant.h"
#include "CWTW-Pro_def.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMain *Main;
static ToneThread* ToneThd;
static TMYRANDOM Rnd;
//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
    : TForm(Owner)
{
    Pause_Flag = false;
    ToolPause->Down = false;
    Repeat_Start = 0;
    Repeat_End = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMain::UsrToneMsgEvent(TMessage &Message)
{
	String buff = "";

#if 0
	DebugPrint(IntToStr(Message.LParam));	// for debug
#endif
	Char ch = (Char)Message.WParam;

	switch (Env->DispMod->ItemIndex) {
	case 0:								// 大文字
		ch = towupper(ch); break;
	case 1:								// 小文字
		ch = towlower(ch); break;
	case 2:								// 符号
		break;
	default:							// 未表示
		ch = 0; break;
	}

	switch (ch) {
	case L'\n':
		buff += L"\r\n";				// 改行
		break;
	case KANA_OFF_CHR:
    	Status->Panels->Items[4]->Text = "英数";
		break;
	case KANA_ON_CHR:
    	Status->Panels->Items[4]->Text = "カナ";
        break;
	default:
		buff += ch;
        if (ch != L' ')
            charcnt++;
    }

	if (buff.Length() != 0) {
		/* カーソル位置に文字列表示 */
#if 1
		Memo->SelText = buff;
#else	/* 以前は下で行っていたがUnicodeになってカナがうまく表示できない */
		Memo->Perform(EM_REPLACESEL, 0, (LPARAM)buff.c_str());	// SendMessage
#endif		
        Status->Panels->Items[3]->Text = IntToStr(charcnt) + "文字";
    }
}
//---------------------------------------------------------------------------
void __fastcall TMain::StartBtnClick(TObject *Sender)
{
	int i;
    int repeat_flag;

	if (StartBtn->Enabled == false)
    	return;

 	if (RMode->ItemIndex >= FileText_mode) {
		if (EditFile->LoadFileCwt(DataFile->Text) >= 0) {
			if (EditFile->Mode > 0)
				RMode->ItemIndex = EditFile->Mode;
			SetDataFile(DataFile->Text);    // ファイルリストへ追加

            // ファイル名の表示（長すぎる時は省略）
			Status->Panels->Items[0]->Text = MinimizeName(EditFile->StatusBar->SimpleText,
                                    Status->Canvas, 250);
		}
		else {
			return;
		}
	}


	// 発音開始
   	if (SMode->ItemIndex) {
   		cwt_mode = CwSend_mode;			    // 送信練習モード
		ToolRetry->Enabled = false;
		MNRetry->Enabled = false;
    }
   	else {
		// リピート位置の記憶
		Repeat_Start = Repeat_End;			// リピート開始行番号の記憶
		Repeat_End = Memo->Lines->Count;	// リピート終了行番号の記憶
        if (Repeat_Start > Repeat_End)
			Repeat_Start = Repeat_End;

    	repeat_flag = dynamic_cast<TComponent *>(Sender)->Tag;
        if (repeat_flag) {
			EditFile->Memo->Clear();
			for (i=Repeat_Start; i<Repeat_End; i++) {
				EditFile->Memo->Lines->Add(Memo->Lines->Strings[i]);
			}
			EditFile->TrimMemo();
			cwt_mode = Repeat_mode;			// 受信練習リピート
        	LMode->Checked = false;         // リピート時はループは禁止
        }
        else {
			cwt_mode = RMode->ItemIndex;	// 受信練習モード
			ToolRetry->Enabled = true;
			MNRetry->Enabled = true;
		}
    }

	ToneThd = new ToneThread(false, cwt_mode, Main->Handle);
	ToneThd->OnTerminate = ThreadDone;
	StartBtn->Enabled = false;
    MNStart->Enabled = false;
	ToolStart->Down = true;
	StopBtn->Enabled = true;
    MNStop->Enabled = true;
    ToolStop->Down = false;

    Pause_Flag = false;
    ToolPause->Down = false;
    ToolPause->Enabled = true;
    MNPause->Enabled = true;

    Status->Panels->Items[2]->Text = "経過：";
    Status->Panels->Items[3]->Text = "";

    charcnt = 0;
	StartTime = Now();
	Timer->Enabled = true;

	Memo->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNEnvClick(TObject *Sender)
{
	Env->ShowModal();
	ToneSpdExe->Position = Env->ToneSpd->Position;
}
//---------------------------------------------------------------------------
void __fastcall TMain::StopBtnClick(TObject *Sender)
{
	if (StartBtn->Enabled == true)
    	return;

    Pause_Flag = false;
    ToolPause->Down = false;
    ToolPause->Enabled = false;
    MNPause->Enabled = false;
	ToneThd->Terminate();				// 中断通知
}
//---------------------------------------------------------------------------
void __fastcall TMain::ThreadDone(TObject *Sender)
{
	StartBtn->Enabled = true;
    MNStart->Enabled = true;
	ToolStart->Down = false;;
	StopBtn->Enabled = false;
    MNStop->Enabled = false;
    ToolStop->Down = true;

	Timer->Enabled = false;
	Status->Panels->Items[0]->Text = "";
	Status->Panels->Items[4]->Text = "";
	cwt_mode = -1;
}
//---------------------------------------------------------------------------
void __fastcall TMain::ToolFileOpenClick(TObject *Sender)
{
	OpenDialog->InitialDir = Env->DataDir->Text;
	if (OpenDialog->Execute()) {
		DataFile->Text = OpenDialog->FileName;

		if ((EditFile->LoadFileCwt(DataFile->Text)) >= 0) {
			if (EditFile->Mode > 0)
				RMode->ItemIndex = EditFile->Mode;
			else
				RMode->ItemIndex = FileText_mode;

			SMode->ItemIndex = 0;		// 受信モード選択
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::ToolFileViewClick(TObject *Sender)
{
	if (DataFile->Text == "") {
    	ShowMessage("データファイルの指定がありません");
        return;
    }

	if (EditFile->LoadFile(DataFile->Text) < 0) {
		ShowMessage("データファイルがオープン出来ません！");
		return;
	}
///		EditFile->PackMemo();			// 不要行の削除

    EditFile->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMain::RModeClick(TObject *Sender)
{
	if (RMode->ItemIndex <= CharRand_mode) {
		NMode->Enabled = true;
        XMode->Enabled = true;
        YMode->Enabled = true;
	}
	else {
		NMode->Enabled = false;
        XMode->Enabled = false;
        YMode->Enabled = false;
	}

	if (StartBtn->Enabled == false) {
		ShowMessage("練習実行中はモード変更は受け付けられません\n"
        			"変更は次の練習開始時から有効となります");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender)
{
#ifdef REGISTORY_BASE_KEY
    // ライセンスファイルの存在確認
    if (FileExists(ExtractFilePath(Application->ExeName) + LICENSE_FNAME))
        PRO_Mode = true;                        // 送信機能あり版モード
    else
        PRO_Mode = false;                       // 送信機能なし版モード
#else	
	PRO_Mode = true;							// Open Source版
#endif

    // 初期化処理
    if (PRO_Mode) {
        Main->Caption = ProTitle+ " "+Revcode;  // タイトル変更
    }
    else {
        Main->Caption = FreeTitle+ " "+Revcode; // タイトル変更
    }
	
	cwt_mode = -1;
}
//---------------------------------------------------------------------------
void __fastcall TMain::FormShow(TObject *Sender)
{
	TRegIniFile *ef;
    TFontStyles fs;

	// ロゴの表示
	fs << fsItalic;
	Logo->Canvas->Brush->Color = clYellow;
	Logo->Canvas->Pen->Color = clTeal;
    Logo->Canvas->Rectangle(0, 0, 125, 30);	// 外枠

    Logo->Canvas->Brush->Style = bsClear;
    Logo->Canvas->Font->Size = 18;
    Logo->Canvas->Font->Style = fs;

    if (PRO_Mode) {
	    Logo->Canvas->Font->Color = clSilver;
        Logo->Canvas->TextOut(6, 5, ProTitle);	// 影部分
	    Logo->Canvas->Font->Color = clBlue;
		Logo->Canvas->TextOut(3, 2, ProTitle);	// 文字本体
	}
    else {
	    Logo->Canvas->Font->Color = clSilver;
        Logo->Canvas->TextOut(28, 5, FreeTitle); // 影部分
	    Logo->Canvas->Font->Color = clBlue;
	    Logo->Canvas->TextOut(25, 2, FreeTitle); // 文字本体
    }

	// 起動メッセージの表示
	Main->Memo->Font = Env->FonSmpl->Font;	// フォントの設定

	Memo->Lines->Add(Progname);
	Memo->Lines->Add(Cpymsg);
	Memo->Lines->Add("");

////    Main->Memo->DefAttributes->Name = Env->FonSmpl->Font->Name;
////    Main->Memo->DefAttributes->Size = Env->FonSmpl->Font->Size;

	// 初回起動時の処理
	bool first = false;						// 初回起動フラグ
	String instdate_str;
	ef = new TRegIniFile(REGISTORY_BASE_ENV);
	String RevStr = ef->ReadString("SETUP", "Revision", "");
	if (RevStr != Revcode+" "+Revdate) {	// バージョンが不一致 初回起動？
		ef->WriteString("SETUP", "Revision", Revcode+" "+Revdate);
		ef->WriteString("SETUP", "Installed", DateTimeToStr(Now()));
		first = true;
	}
	instdate_str = ef->ReadString("SETUP", "Installed", "");
	RegInf->InstDay->Caption = instdate_str;    // ｲﾝｽﾄｰﾙ日
	delete ef;

	if (first) {
		EditFile->LoadFile(Env->ExePath + DOCUMENT_FNAME);
	    EditFile->ShowModal();				// ドキュメントの表示
	}

    // ライセンス情報の確認
    License_Ok = true;                      // ﾗｲｾﾝｽOK
    RegInf_show = false;			        // ﾗｲｾﾝｽの警告表示

    if (PRO_Mode) {
#ifdef REGISTORY_BASE_KEY
	    // ライセンスキーのチェック
	    ef = new TRegIniFile(REGISTORY_BASE_KEY);
	    if (ef->ReadString("CWTW", "KEY",  "") == License) {
            Status->Panels->Items[0]->Text = "License check OK";
        }
        else {
            License_Ok = false;             // ﾗｲｾﾝｽNG
            Status->Panels->Items[0]->Text = "License free mode";
  #if 0			
            try {
	            TDateTime dt = instdate_str;    // ｲﾝｽﾄｰﾙ日
		        dt += 30;						// ｲﾝｽﾄｰﾙ日から4週間後の日時
		        if ((!first) && (Now() > dt))
			        RegInf_show = true;		// 4週間以上経過は警告表示する
            }
            catch (...) {
		        Status->Panels->Items[0]->Text = "Installed datetime format illegal";
		        RegInf_show = true;         // 日付変換不正時は警告表示する
            }
  #endif			
	    }
	    delete ef;
#endif		
    }
    else {
        // フリーウェア版動作設定(CWTWモード)
	    SMode->Enabled = false;				// 送信練習禁止
        Status->Panels->Items[0]->Text = "CWTW free mode";
    }

	if (License_Ok)
		MNRegist->Enabled = false;
	else
		MNRegist->Enabled = true;

	// 練習モードの復帰処理
	if (Env->KeepMod->Checked == true) {
		ef = new TRegIniFile(REGISTORY_BASE_ENV);

//		Left = ef->ReadInteger("MODE", "Left", 100);    // 2005/01/04下へ移動
//		Top = ef->ReadInteger("MODE", "Top", 100);      // 2005/01/04下へ移動
		ToneSpdExe->Position = (short)ef->ReadInteger("MODE",
	        						"ToneSpd", Env->ToneSpd->Position);
		SMode->ItemIndex = ef->ReadInteger("MODE", "SMode", 0);
		RMode->ItemIndex = ef->ReadInteger("MODE", "RMode", 0);
		JMode->ItemIndex = ef->ReadInteger("MODE", "JMode", 0);
		AMode->Checked   = ef->ReadBool("MODE", "AMode", true);
		NMode->Checked   = ef->ReadBool("MODE", "NMode", false);
		XMode->Checked   = ef->ReadBool("MODE", "XMode", false);
		YMode->Checked   = ef->ReadBool("MODE", "YMode", false);
		LMode->Checked   = ef->ReadBool("MODE", "LMode", false);
		DataFile->Text   = ef->ReadString("MODE", "DataFile", "");

		delete ef;
	}
	else {
		ToneSpdExe->Position = Env->ToneSpd->Position;
    }

	// ウィンドウ位置／サイズの復帰処理
	if ((GetAsyncKeyState(VK_SHIFT) | GetAsyncKeyState(VK_CONTROL)) & 0x8000) {
        // SHFT+CTRLを押しながら起動した時は、デフォルトのウィンドウサイズ
    }
	else {
		if (Env->KeepWindow->Checked == true) {
			ef = new TRegIniFile(REGISTORY_BASE_ENV);

			Left = ef->ReadInteger("Window", "Left", Left);
			Top = ef->ReadInteger("Window", "Top", Top);
			Width = ef->ReadInteger("Window", "Width", Width);
			Height = ef->ReadInteger("Window", "Height", Height);

			delete ef;
		}
    }

	DragAcceptFiles(Handle, true);

	if (ParamCount() > 0) {
		DataFile->Text = ParamStr(1);

		if ((EditFile->LoadFileCwt(DataFile->Text)) >= 0) {
			if (EditFile->Mode > 0)
				RMode->ItemIndex = EditFile->Mode;
			else
				RMode->ItemIndex = FileText_mode;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNRevDispClick(TObject *Sender)
{
	if (AboutBox->ShowModal() == mrYes) {		// 送金登録ボタン？
		if (Regist->ShowModal() == mrOk) {		// 送金完了？
			License_Ok = true;
            RegInf_show = false;			    // ﾗｲｾﾝｽの警告表示
			MNRegist->Enabled = false;
            Status->Panels->Items[0]->Text = "";
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNDocDispClick(TObject *Sender)
{
	if ((GetAsyncKeyState(VK_SHIFT) | GetAsyncKeyState(VK_CONTROL)) & 0x8000) {
		int RetValue = Application->MessageBox(L"環境設定情報を削除します。",
					L"確認", MB_ICONEXCLAMATION | MB_OKCANCEL);
		if (RetValue == IDOK) {
			Env->KeepMod->Checked = false;

			TRegIniFile *ef;
			ef = new TRegIniFile(REGISTORY_BASE);
			ef->EraseSection("CWTW");
			delete ef;
		}
		return;
	}

	EditFile->LoadFile(Env->ExePath + DOCUMENT_FNAME);
	EditFile->ShowModal();				// ドキュメントの表示
}
//---------------------------------------------------------------------------
void __fastcall TMain::FormPaint(TObject *Sender)
{
	switch (Env->SndSel->ItemIndex) {
	case 1:
		Status->Panels->Items[1]->Text = "音色： SYSTEM BEEP";
		break;
	case 2:
		Status->Panels->Items[1]->Text = "音色： " + Env->ToneName->Caption;
		break;
	case 3:
		Status->Panels->Items[1]->Text = "音色： WAVE Sound";
		break;
	default:
		Status->Panels->Items[1]->Text = "音色： (Off)";
		break;
	}
}
//---------------------------------------------------------------------------
#if 0
void __fastcall TMain::WMGetMinMaxInfo(TWMGetMinMaxInfo &Msg)
{
	Msg.MinMaxInfo->ptMaxSize.x = 580;		// 最大化したときの大きさ
	Msg.MinMaxInfo->ptMaxSize.y = 450;
	Msg.MinMaxInfo->ptMaxPosition.x = 30;	// 最大化したときの位置
	Msg.MinMaxInfo->ptMaxPosition.y = 30;
	Msg.MinMaxInfo->ptMaxTrackSize.x = 580;	// サイズ変更時の最大の大きさ
	Msg.MinMaxInfo->ptMaxTrackSize.y = 450;
	Msg.MinMaxInfo->ptMinTrackSize.x = 100;	// サイズ変更時の最小の大きさ
	Msg.MinMaxInfo->ptMinTrackSize.y = 100;
}
#endif
//---------------------------------------------------------------------------
void __fastcall TMain::WMDropFiles(TWMDropFiles &Msg)
{
	// ドロップされたファイルの総数を得る
    int Count = DragQueryFile((HDROP)Msg.Drop, 0xFFFFFFFF, NULL, 255);
	Char FileName[MAX_PATH + 10];

	for (int i=0; i<Count; i++) {
		// ファイル名を得る
        DragQueryFile((HDROP)Msg.Drop, i, FileName, 255);
		// ファイルの設定
		DataFile->Text = FileName;

		if ((EditFile->LoadFileCwt(DataFile->Text)) >= 0) {
			if (EditFile->Mode > 0)
				RMode->ItemIndex = EditFile->Mode;
			else
				RMode->ItemIndex = FileText_mode;
		}
	}

	DragFinish((HDROP)Msg.Drop);
}
//---------------------------------------------------------------------------
void __fastcall TMain::SetDataFile(String fn) {
	// ファイルリストへ追加
	int i = DataFile->Items->IndexOf(fn);	// fnのサーチ
	if (i >= 0)
		DataFile->Items->Delete(i);
	DataFile->Items->Insert(0, fn);
	if (DataFile->Items->Count >= 15)
		DataFile->Items->Delete(15);

	DataFile->Text = fn;
}
//---------------------------------------------------------------------------
void __fastcall TMain::DataFileKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN) {
		if ((EditFile->LoadFileCwt(DataFile->Text)) >= 0) {
			if (EditFile->Mode > 0)
				RMode->ItemIndex = EditFile->Mode;
			else
				RMode->ItemIndex = FileText_mode;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::DataFileClick(TObject *Sender)
{
    _wchdir(ExtractFilePath(Application->ExeName).c_str());

	if ((EditFile->LoadFileCwt(DataFile->Text)) >= 0) {
		if (EditFile->Mode > 0)
			RMode->ItemIndex = EditFile->Mode;
		else
			RMode->ItemIndex = FileText_mode;

		SMode->ItemIndex = 0;			// 受信モード選択
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::MemoKeyPress(TObject *Sender, char &Key)
{
	Key = NULL;							// キー入力を全て無視
}
//---------------------------------------------------------------------------
void __fastcall TMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	TRegIniFile *ef;

	if (StopBtn->Enabled == True) {		// 練習中？
        Pause_Flag = false;
		ToneThd->Terminate();		  	// 中断通知

		while (StopBtn->Enabled == True) {	// スレッド終了まで待つ
			Application->ProcessMessages();
		}
	}

	// 練習モードの保存処理
	if (Env->KeepMod->Checked == true) {
		ef = new TRegIniFile(REGISTORY_BASE_ENV);

//		ef->WriteInteger("MODE", "Left", Left); // 2005/01/04下へ移動
//		ef->WriteInteger("MODE", "Top", Top);   // 2005/01/04下へ移動
		ef->WriteInteger("MODE", "ToneSpd", ToneSpdExe->Position);
		ef->WriteInteger("MODE", "SMode", SMode->ItemIndex);
		ef->WriteInteger("MODE", "RMode", RMode->ItemIndex);
		ef->WriteInteger("MODE", "JMode", JMode->ItemIndex);
		ef->WriteBool("MODE", "AMode", AMode->Checked);
		ef->WriteBool("MODE", "NMode", NMode->Checked);
		ef->WriteBool("MODE", "XMode", XMode->Checked);
		ef->WriteBool("MODE", "YMode", YMode->Checked);
		ef->WriteBool("MODE", "LMode", LMode->Checked);
		ef->WriteString("MODE", "DataFile", DataFile->Text);

		delete ef;
	}

	// ウインドウ位置／サイズの保存処理
	if (Env->KeepWindow->Checked == true) {
		ef = new TRegIniFile(REGISTORY_BASE_ENV);

		ef->WriteInteger("Window", "Left", Left);
		ef->WriteInteger("Window", "Top", Top);
		ef->WriteInteger("Window", "Width", Width);
		ef->WriteInteger("Window", "Height", Height);

		delete ef;
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain::MNRegistClick(TObject *Sender)
{
    int RetValue = Regist->ShowModal();		// 送金登録
	if (RetValue == mrOk) {					// 登録完了？
		License_Ok = true;
        RegInf_show = false;			    // ﾗｲｾﾝｽの警告表示
		MNRegist->Enabled = false;
        Status->Panels->Items[0]->Text = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::SModeClick(TObject *Sender)
{
	switch (SMode->ItemIndex) {
	case 1:
        // 送信練習モード
		RMode->Enabled = false;
        LMode->Enabled = false;
        NMode->Enabled = false;
        XMode->Enabled = false;
        YMode->Enabled = false;

        // ライセンス確認表示
	    if (RegInf_show) {
            MessageBeep(MB_ICONEXCLAMATION);

		    int RetValue = RegInf->ShowModal();	// シェアウェア情報の表示
		    if (RetValue == mrCancel)
			    Close();
            else if (RetValue == mrYes) {		// 登録完了？
			    License_Ok = true;
                RegInf_show = false;	        // ﾗｲｾﾝｽの警告表示
            }
		    else {
                RegInf_show = false;	        // ﾗｲｾﾝｽの警告表示
            }
	    }
        break;
	default:
        // 受信練習モード
		RMode->Enabled = true;
		LMode->Enabled = true;
		NMode->Enabled = true;
        XMode->Enabled = true;
        YMode->Enabled = true;
        break;
	}

	if (StartBtn->Enabled == false) {
		ShowMessage("練習実行中はモード変更は受け付けられません\n"
        			"変更は次の練習開始時から有効となります");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNLicDispClick(TObject *Sender)
{
#ifdef REGISTORY_BASE_KEY
	if ((GetAsyncKeyState(VK_SHIFT) | GetAsyncKeyState(VK_CONTROL)) & 0x8000) {
		int RetValue = Application->MessageBox(L"ライセンス情報を削除します。",
					L"確認", MB_ICONEXCLAMATION | MB_OKCANCEL);
		if (RetValue == IDOK) {
			TRegIniFile *ef;
			ef = new TRegIniFile(REGISTORY_BASE_KEY);
			ef->EraseSection("CWTW");
			delete ef;
		}
		return;
	}
#endif	

	EditFile->LoadFile(Env->ExePath + LICENSE_FNAME);
    EditFile->ShowModal();				// ドキュメントの表示
}
//---------------------------------------------------------------------------
void __fastcall TMain::TimerTimer(TObject *Sender)
{
	Status->Panels->Items[2]->Text = "経過： " + FormatDateTime("n'分' ss'秒'", Now()-StartTime);
}
//---------------------------------------------------------------------------
void __fastcall TMain::ToneSpdExeTxtKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_RETURN) {
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNFileEditClick(TObject *Sender)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	String cmd;

	if (DataFile->Text == "") {
		ShowMessage("データファイルの指定がありません");
		return;
	}

///	if (ExtractFilePath(DataFile->Text).IsEmpty())
///		fn = Env->DataDir->Text + DataFile->Text;
///	else
///    	fn = DataFile->Text;
	cmd = "NOTEPAD.EXE " + DataFile->Text;

	// エディターの起動
	GetStartupInfo(&si);
	CreateProcess(NULL, cmd.c_str(),
					NULL, NULL, false, 0, NULL,
					Env->DataDir->Text.c_str(),	/* 作業ﾃﾞｨﾚｸﾄﾘ */
					&si,&pi);

	// 終了待ち
#if 0
	while (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT)
    	Application->ProcessMessages();
#endif

}
//---------------------------------------------------------------------------
void __fastcall TMain::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
#if 0
	Status->Panels->Items[5]->Text = IntToHex(Key,2);	// for DEBUG
#endif

	if (StopBtn->Enabled != true) {		    // 練習中？
        if (Key == VK_RETURN)               // CR ?
            StartBtnClick(StartBtn);
		return;
    }

    if (Key == VK_ESCAPE) {                 // ESC ?
        StopBtnClick(StopBtn);
        return;
    }

	int code = Key;
	if (Shift.Contains(ssShift))
		code += 0x100;

    Key = NULL;							    // キー入力を全て無視

	if (cwt_mode == KeyIn_mode) {
    	switch (code & 0x00FF) {
		case VK_F12:
		case VK_HOME:
			OpenDialog->InitialDir = Env->DataDir->Text;
			if (OpenDialog->Execute() == false)
				return;

			DataFile->Text = OpenDialog->FileName;
			if (FileExists(DataFile->Text) == false) {
				ShowMessage("指定のファイルが見つかりません");
				return;
			}
			break;
		}

	   	while (ToneThd->KbdWr(code) == 0) {	// 書き込めるまで待つ
	    	Application->ProcessMessages();
		}
	}


	if (cwt_mode == CwSend_mode) {
	   	while (ToneThd->KbdWr(code) == 0) {	// 書き込めるまで待つ
	    	Application->ProcessMessages();
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TMain::ToneSpdExeTxtChange(TObject *Sender)
{
#if 0	/* この処理があると何故かストップを受け付けない？ */
	if (StartBtn->Enabled == false) {
		ShowMessage("練習実行中はモード変更は受け付けられません\n"
        			"変更は次の練習開始時から有効となります");
	}
#endif
}
//---------------------------------------------------------------------------
void __fastcall TMain::HomePageDispClick(TObject *Sender)
{
	ShellExecute(Application->Handle, L"open", String(HomePageURL).c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNFileSaveClick(TObject *Sender)
{
	String msg;

	if (SaveDialog->Execute()) {
		msg = "\"" + SaveDialog->FileName + "\" の\nファイルに練習画面を保存しますか？";
		int RetValue = Application->MessageBox(msg.c_str(),
					L"テキストファイルに保存", MB_ICONQUESTION | MB_OKCANCEL);
		if (RetValue == IDOK) {
			Memo->Lines->SaveToFile(SaveDialog->FileName);
//          ShowMessage(SaveDialog->FileName);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMain::MNDispClearClick(TObject *Sender)
{
	int RetValue = Application->MessageBox(L"練習画面をクリアしますか？",
        	L"確認", MB_ICONQUESTION | MB_OKCANCEL);
	if (RetValue == IDOK) {
        Memo->Clear();
		ToolRetry->Enabled = false;
		MNRetry->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMain::ToolPauseClick(TObject *Sender)
{
    if (Pause_Flag) {
        Pause_Flag = false;
        ToolPause->Down = false;
    }
    else {
        Pause_Flag = true;
        ToolPause->Down = true;
    }
}
//---------------------------------------------------------------------------
int __fastcall TMain::get_random(int num)
{
	return Rnd.get_random(num);
}
//---------------------------------------------------------------------------

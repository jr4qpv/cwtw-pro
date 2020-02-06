//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>
#include <stdio.h>
#include <io.h>
#include <winuser.h>
#ifndef WIN32
#define WIN32	/* for mmsystem.h */
#endif
#include <mmsystem.h>
#include <dsound.h>

#include "UEnv.h"
#include "UMain.h"
#include "UEdit.h"
#include "ToneThd.h"
#include "Keytable.h"
#include "Tonename.h"
#include "Constant.h"
#include "debug.h"
#include "CWTW-Pro_def.h"
//---------------------------------------------------------------------------
#define	DSndMax		10
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
static ToneThread* ToneThd;
GUID DSndGUID[DSndMax];
TEnv *Env;
//---------------------------------------------------------------------------
__fastcall TEnv::TEnv(TComponent* Owner)
	: TForm(Owner)
{
	ExePath = ExtractFilePath(Application->ExeName);
}
//---------------------------------------------------------------------------
void __fastcall TEnv::SaveEnv(void)
{
	Save_SndSel = SndSel->ItemIndex;
	Save_DSndDev = DSndDev->ItemIndex;
	Save_DSndWave = DSndWave->ItemIndex;
	Save_MidiDev = MidiDev->ItemIndex;
	Save_JoyDev  = JoyDev->ItemIndex;
	Save_RSDev   = RSDev->ItemIndex;
	Save_SendAutoCR = SendAutoCR->ItemIndex;
	Save_BeepMod = BeepMod->ItemIndex;
	Save_KIMode  = KIMode->ItemIndex;
	Save_DKMode  = DKMode->ItemIndex;
	Save_DispMod = DispMod->ItemIndex;
	Save_ToneSpd = ToneSpd->Position;
	Save_ToneNo  = ToneNo->Position;

    Save_RndTime = RndTime->Position;
    Save_CrTime = CrTime->Position;
	Save_RndCount = RndCount->Position;
    Save_RndCntMin = RndCntMin->Position;
    Save_RndRandom = RndRandom->Checked;
	Save_RndSeparator = RndSeparator->ItemIndex;

	Save_ToneKey = ToneKey->Position;
	Save_ToneVol = ToneVol->Position;
	Save_ToneEco = ToneEco->Position;
	Save_DataDir = DataDir->Text;
	Save_CM_Mode = CM_Mode->Checked;
	Save_KeepMod = KeepMod->Checked;
	Save_KeepWindow = KeepWindow->Checked;

    Save_BoldMode = FonSmpl->Font->Style.Contains(fsBold);
    Save_ItalicMode = FonSmpl->Font->Style.Contains(fsItalic);
	Save_FonName = FonSmpl->Font->Name;
	Save_FonSize = FonSmpl->Font->Size;
}
//---------------------------------------------------------------------------
void __fastcall TEnv::RestoreEnv(void)
{
	SndSel->ItemIndex = Save_SndSel;
	DSndDev->ItemIndex = Save_DSndDev;
	DSndWave->ItemIndex= Save_DSndWave;
	MidiDev->ItemIndex = Save_MidiDev;
	JoyDev->ItemIndex  = Save_JoyDev;
	RSDev->ItemIndex   = Save_RSDev;
	SendAutoCR->ItemIndex = Save_SendAutoCR;
	BeepMod->ItemIndex = Save_BeepMod;
	KIMode->ItemIndex  = Save_KIMode;
	DKMode->ItemIndex  = Save_DKMode;
	DispMod->ItemIndex = Save_DispMod;
	ToneSpd->Position  = Save_ToneSpd;
	ToneNo->Position   = Save_ToneNo;

    RndTime->Position = Save_RndTime;
    CrTime->Position = Save_CrTime;
	RndCount->Position = Save_RndCount;
    RndCntMin->Position = Save_RndCntMin;
    RndRandom->Checked = Save_RndRandom;
	RndSeparator->ItemIndex = Save_RndSeparator;

	ToneKey->Position  = Save_ToneKey;
	ToneVol->Position  = Save_ToneVol;
	ToneEco->Position  = Save_ToneEco;
	DataDir->Text      = Save_DataDir;
    CM_Mode->Checked   = Save_CM_Mode;
	KeepMod->Checked   = Save_KeepMod;
	KeepWindow->Checked = Save_KeepWindow;

    FonSmpl->Font->Style.Clear();
    if (Save_BoldMode)
        FonSmpl->Font->Style = FonSmpl->Font->Style << fsBold;
    if (Save_ItalicMode)
        FonSmpl->Font->Style = FonSmpl->Font->Style << fsItalic;
	FonSmpl->Font->Name = Save_FonName;
	FonSmpl->Font->Size = Save_FonSize;
}
//---------------------------------------------------------------------------
int __fastcall TEnv::CompareEnv(void)
{
	if (Save_SndSel != SndSel->ItemIndex)
		return 1;
	if (Save_DSndDev != DSndDev->ItemIndex)
		return 1;
	if (Save_DSndWave != DSndWave->ItemIndex)
		return 1;
	if (Save_MidiDev != MidiDev->ItemIndex)
		return 1;
	if (Save_JoyDev != JoyDev->ItemIndex)
		return 1;
	if (Save_RSDev != RSDev->ItemIndex)
		return 1;
	if (Save_SendAutoCR != SendAutoCR->ItemIndex)
		return 1;
	if (Save_BeepMod != BeepMod->ItemIndex)
		return 1;
	if (Save_KIMode != KIMode->ItemIndex)
		return 1;
	if (Save_DKMode != DKMode->ItemIndex)
		return 1;
	if (Save_DispMod != DispMod->ItemIndex)
		return 1;
	if (Save_ToneSpd != ToneSpd->Position)
		return 1;
	if (Save_ToneNo != ToneNo->Position)
		return 1;

	if (Save_RndTime != RndTime->Position)
		return 1;
	if (Save_CrTime != CrTime->Position)
		return 1;
	if (Save_RndCount != RndCount->Position)
		return 1;
	if (Save_RndCntMin != RndCntMin->Position)
		return 1;
	if (Save_RndRandom != RndRandom->Checked)
		return 1;
	if (Save_RndSeparator != RndSeparator->ItemIndex)
		return 1;

	if (Save_ToneKey != ToneKey->Position)
		return 1;
	if (Save_ToneVol != ToneVol->Position)
		return 1;
	if (Save_ToneEco != ToneEco->Position)
		return 1;
	if (Save_CM_Mode != CM_Mode->Checked)
		return 1;
	if (Save_KeepMod != KeepMod->Checked)
		return 1;
	if (Save_KeepWindow != KeepWindow->Checked)
		return 1;

	if (Save_DataDir != DataDir->Text)
		return 1;

    if (Save_BoldMode != FonSmpl->Font->Style.Contains(fsBold))
        return 1;
    if (Save_ItalicMode != FonSmpl->Font->Style.Contains(fsItalic))
        return 1;
	if (Save_FonName != FonSmpl->Font->Name)
		return 1;
	if (Save_FonSize != FonSmpl->Font->Size)
		return 1;

	return 0;							// 全て一致
}
//---------------------------------------------------------------------------
int __fastcall TEnv::EnvFileWr(void)
{
	TRegIniFile *ef;
    bool BoldMode, ItalicMode;

	// 環境設定データの書き込み
	ef = new TRegIniFile(REGISTORY_BASE_ENV);
	ef->WriteString("ENV", "DataDir", DataDir->Text);

	ef->WriteString("ENV", "FonName", FonSmpl->Font->Name);
	ef->WriteInteger("ENV", "FonSize", FonSmpl->Font->Size);
    BoldMode = FonSmpl->Font->Style.Contains(fsBold);
    ItalicMode = FonSmpl->Font->Style.Contains(fsItalic);
    ef->WriteBool("ENV", "FonBold", BoldMode);
    ef->WriteBool("ENV", "FonItalic", ItalicMode);

	ef->WriteInteger("ENV", "SndSel", SndSel->ItemIndex);
	ef->WriteInteger("ENV", "DSndDev", DSndDev->ItemIndex);
	ef->WriteInteger("ENV", "DSndWave", DSndWave->ItemIndex);
	ef->WriteInteger("ENV", "MidiDev", MidiDev->ItemIndex);
	ef->WriteInteger("ENV", "JoyDev", JoyDev->ItemIndex);
	ef->WriteInteger("ENV", "RSDev", RSDev->ItemIndex);
	ef->WriteInteger("ENV", "SendAutoCR", SendAutoCR->ItemIndex);
	ef->WriteInteger("ENV", "BeepMod", BeepMod->ItemIndex);
	ef->WriteInteger("ENV", "KIMode", KIMode->ItemIndex);
	ef->WriteInteger("ENV", "DKMode", DKMode->ItemIndex);
	ef->WriteInteger("ENV", "DispMod", DispMod->ItemIndex);
	ef->WriteInteger("ENV", "ToneSpd", ToneSpd->Position);
	ef->WriteInteger("ENV", "ToneNo", ToneNo->Position);

	ef->WriteInteger("ENV", "RndTime", RndTime->Position);
	ef->WriteInteger("ENV", "CrTime", CrTime->Position);
	ef->WriteInteger("ENV", "RndCount", RndCount->Position);
	ef->WriteInteger("ENV", "RndCntMin", RndCntMin->Position);
	ef->WriteBool("ENV", "RndRandom", RndRandom->Checked);
	ef->WriteInteger("ENV", "RndSeparator", RndSeparator->ItemIndex);

	ef->WriteInteger("ENV", "ToneKey", ToneKey->Position);
	ef->WriteInteger("ENV", "ToneVol", ToneVol->Position);
	ef->WriteInteger("ENV", "ToneEco", ToneEco->Position);
	ef->WriteBool("ENV", "CM_Mode", CM_Mode->Checked);
	ef->WriteBool("ENV", "KeepMod", KeepMod->Checked);
	ef->WriteBool("ENV", "KeepWindow", KeepWindow->Checked);

	ef->WriteString("ENV", "KbdMem1", KbdMem1->Text);
	ef->WriteString("ENV", "KbdMem2", KbdMem2->Text);
	ef->WriteString("ENV", "KbdMem3", KbdMem3->Text);
	ef->WriteString("ENV", "KbdMem4", KbdMem4->Text);
	ef->WriteString("ENV", "KbdMem5", KbdMem5->Text);
	ef->WriteString("ENV", "KbdMem6", KbdMem6->Text);
	ef->WriteString("ENV", "KbdMem7", KbdMem7->Text);
	ef->WriteString("ENV", "KbdMem8", KbdMem8->Text);
	ef->WriteString("ENV", "KbdMem9", KbdMem9->Text);
	ef->WriteString("ENV", "KbdMem10", KbdMem10->Text);

	delete ef;

	return 0;
}
//---------------------------------------------------------------------------
int __fastcall TEnv::EnvFileRd(void)
{
	TRegIniFile *ef;
    bool BoldMode, ItalicMode;

	// 環境設定データの読み出し
	ef = new TRegIniFile(REGISTORY_BASE_ENV);
	SndSel->ItemIndex  = ef->ReadInteger("ENV", "SndSel", SndSelDefault);
	DSndDev->ItemIndex = ef->ReadInteger("ENV", "DSndDev", 0);
	DSndWave->ItemIndex= ef->ReadInteger("ENV", "DSndWave", 0);
	MidiDev->ItemIndex = ef->ReadInteger("ENV", "MidiDev", MidiDevDefault);
	JoyDev->ItemIndex  = ef->ReadInteger("ENV", "JoyDev", 0);
	RSDev->ItemIndex   = ef->ReadInteger("ENV", "RSDev", 0);
   	SendAutoCR->ItemIndex = ef->ReadInteger("ENV", "SendAutoCR", 1);
	BeepMod->ItemIndex = ef->ReadInteger("ENV", "BeepMod", BeepModeDefault);
	KIMode->ItemIndex  = ef->ReadInteger("ENV", "KIMode", 0);
	DKMode->ItemIndex  = ef->ReadInteger("ENV", "DKMode", 0);
	DispMod->ItemIndex = ef->ReadInteger("ENV", "DispMod", 0);
	ToneSpd->Position  = (short)ef->ReadInteger("ENV", "ToneSpd", 50);
	ToneNo->Position   = (short)ef->ReadInteger("ENV", "ToneNo", 81);

	RndTime->Position = (short)ef->ReadInteger("ENV", "RndTime", 2);
	CrTime->Position = (short)ef->ReadInteger("ENV", "CrTime", 1);
	RndCount->Position = (short)ef->ReadInteger("ENV", "RndCount", 5);
	RndCntMin->Position = (short)ef->ReadInteger("ENV", "RndCntMin", 2);
	RndRandom->Checked   = ef->ReadBool("ENV", "RndRandom", false);
	RndSeparator->ItemIndex = ef->ReadInteger("ENV", "RndSeparator", 0);

	ToneKey->Position  = ef->ReadInteger("ENV", "ToneKey", 81);
	ToneVol->Position  = ef->ReadInteger("ENV", "ToneVol", 100);
	ToneEco->Position  = ef->ReadInteger("ENV", "ToneEco", 0);

	FonSmpl->Font->Size= ef->ReadInteger("ENV", "FonSize",
							Env->FonSmpl->Font->Size);
	FonSmpl->Font->Name= ef->ReadString("ENV", "FonName",
							Env->FonSmpl->Font->Name);
	BoldMode           = ef->ReadBool("ENV", "FonBold", false);
	ItalicMode         = ef->ReadBool("ENV", "FonItalic", false);
    FonSmpl->Font->Style.Clear();
    if (BoldMode)
        FonSmpl->Font->Style = FonSmpl->Font->Style << fsBold;
    if (ItalicMode)
        FonSmpl->Font->Style = FonSmpl->Font->Style << fsItalic;

	DataDir->Text      = ef->ReadString("ENV", "DataDir", "DATA\\");

	CM_Mode->Checked   = ef->ReadBool("ENV", "CM_Mode", true);
	KeepMod->Checked   = ef->ReadBool("ENV", "KeepMod", false);
	KeepWindow->Checked= ef->ReadBool("ENV", "KeepWindow", false);

	KbdMem1->Text      = ef->ReadString("ENV", "KbdMem1", "HR HR [BT] ");
	KbdMem2->Text      = ef->ReadString("ENV", "KbdMem2", "[AR]#");
	KbdMem3->Text      = ef->ReadString("ENV", "KbdMem3", "HR HR [ﾎﾚ] ");
	KbdMem4->Text      = ef->ReadString("ENV", "KbdMem4", "[ﾗﾀ]#");
	KbdMem5->Text      = ef->ReadString("ENV", "KbdMem5", "CQ CQ CQ ");
	KbdMem6->Text      = ef->ReadString("ENV", "KbdMem6", "DE ?????? ");
	KbdMem7->Text      = ef->ReadString("ENV", "KbdMem7", "");
	KbdMem8->Text      = ef->ReadString("ENV", "KbdMem8", "");
	KbdMem9->Text      = ef->ReadString("ENV", "KbdMem9", "");
	KbdMem10->Text     = ef->ReadString("ENV", "KbdMem10", "");
	delete ef;

	// デフォルト処理
	switch (SndSel->ItemIndex) {
	case 1:
    	break;
    case 2:
    	if (MidiDevCnt == 0)
        	SndSel->ItemIndex = 1;
        break;
    case 3:
		if (DSndDevCnt == 0)
	    	SndSel->ItemIndex = 1;
		break;
    default:
    	SndSel->ItemIndex = 0;
	}

	if (JoyDevCnt == 0)
		JoyDev->ItemIndex = 0;

	if (DataDir->Text == "DATA\\")
		DataDir->Text = ExtractFilePath(Application->ExeName) + "DATA\\";

	try {
		DirectoryListBox1->Directory = DataDir->Text;
	}
	catch (...) {
		;
	}

	RndRandomClick(NULL);

	return 0;
}
//---------------------------------------------------------------------------
bool __fastcall TEnv::Check_giveio_sys(void)
{
	GiveioUsable = false;
	
	//---( GIVEIO.SYSドライバーのチェック )---
	HANDLE h;
	h = CreateFile(L"\\\\.\\giveio", GENERIC_READ, 0, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h != INVALID_HANDLE_VALUE) {
		GiveioUsable = true;
		CloseHandle(h);
	}

	return GiveioUsable;
}
//---------------------------------------------------------------------------
#if DSound_Use
static BOOL CALLBACK DSEnumProc(LPGUID lpGUID,
		LPCTSTR lpszDesc, LPCTSTR lpszDevName, UINT *lpCount)
{
	Char buff[80];

	if (lpGUID != NULL) {
	    (*lpCount)++;
		if (*lpCount >= DSndMax)
	    	return FALSE;

///		swprintf(buff, L"%2d:%s(%08X)", *lpCount, lpszDesc, (long)lpGUID);
		swprintf(buff, L"%2d:%s", *lpCount, lpszDesc);
		Env->DSndDev->Items->Add(buff);
		memcpy(&DSndGUID[*lpCount], lpGUID, sizeof(GUID));
	}

	return TRUE;
}
#endif

void __fastcall TEnv::FormCreate(TObject *Sender)
{
	DWORD ret;
    UINT nDevId;
    MIDIOUTCAPS moc;
	JOYCAPS jc;
	JOYINFO ji;
	Char buff[80];
    String OS;           // OS情報

	//***( WindowのOS情報を取得する )***
	GiveioUsable = false;
	BeepUsable = false;
#ifdef __clang__
	int majorVersion = TOSVersion::Major;	// Windowsメジャーバージョン
	int minorVersion = TOSVersion::Minor;	// Windowsマイナーバージョン

	OS = String().sprintf(L"Windows %d.%d", majorVersion, minorVersion);

	if (majorVersion > 4) {				// Windows2000以降 ?
		if (Check_giveio_sys()) {		// GIVEIO.SYSドライバーのチェック
			BeepUsable = true;
			OS =+ " (giveio.sys)";
		}			
	}
	else {
		/* WindowsNTも含まれるがもう考えない */
		BeepUsable = true;				// Beepは利用可能(WindowsME,98,95)
	}
#else	
	OSVERSIONINFO ovi;	 // バージョン情報を格納する構造体
    ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    GetVersionEx( (LPOSVERSIONINFO)&ovi );

    if ( ovi.dwPlatformId == VER_PLATFORM_WIN32_NT ){
        OS = "WindowsNT";

		if (Check_giveio_sys()) {		// GIVEIO.SYSドライバーのチェック
			BeepUsable = true;
			OS = "WindowsNT(giveio.sys)";
		}			
    }
    else if ( ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ){
        double Ver =  ovi.dwMajorVersion + double(ovi.dwMinorVersion) / 100;
        if ( Ver <= 4.00 ){
            OS = "Windows 95";
        }
        else if ( Ver > 4.00 && Ver < 4.90 ){
            OS = "Windows 98";
        }
        else if ( Ver >= 4.90 ){
            OS = "Windows Me";
        }
        else{
            OS = "Windows ?";
        }

		BeepUsable = true;				// Beepは利用可能
    }
    else if ( ovi.dwPlatformId == VER_PLATFORM_WIN32s ){
        OS = "Windows3.1上のWin32s";
    }
    else{
		OS = "OS不明";
	}
#endif	

	OS_Label->Caption = "OS Type: " + OS;

	//***( WAVEデバイス情報の獲得 )***
	DSndDevCnt = 0;
#if DSound_Use
	DSndDev->Items->Add("(Default)");

	ret = DirectSoundEnumerate((LPDSENUMCALLBACK)DSEnumProc, &DSndDevCnt);
	if (ret != DS_OK) {
		DSndDevCnt = 0;
    }
#endif
	if (!DSndDevCnt) {
		Env->DSndDev->Items->Clear();
		DSndDev->Items->Add("（デバイス未検出）");
		DSndDev->ItemIndex = 0;
    }

	//***( MIDI出力デバイス情報の獲得 )***
	MidiDevCnt = midiOutGetNumDevs();
	for (nDevId=0; nDevId<MidiDevCnt; nDevId++) {
	    midiOutGetDevCaps(nDevId, &moc, sizeof(moc));
		swprintf(buff, L"%2d:%s", nDevId, moc.szPname);
		MidiDev->Items->Add(buff);
	}
	if (!MidiDevCnt)
		MidiDev->Items->Add("（デバイス未検出）");

	//***( ジョイスティックデバイス情報の獲得 )***
	JoyDevCnt = 0;
	for (nDevId=0; nDevId<joyGetNumDevs(); nDevId++) {
		if (joyGetPos(nDevId, &ji))		// 接続？
			continue;

	    joyGetDevCaps(nDevId, &jc, sizeof(jc));
		swprintf(buff, L"%2d:%s", nDevId, jc.szPname);
		JoyDev->Items->Add(buff);
		JoyDevCnt++;
	}
	if (!JoyDevCnt)
		JoyDev->Items->Add("（デバイス未検出）");

	//***( 初期設定とするデバイスの検索 )***
	MidiDevDefault = 0;
	for (nDevId=0; nDevId<MidiDevCnt; nDevId++) {
	    midiOutGetDevCaps(nDevId, &moc, sizeof(moc));

		if (moc.wTechnology != MOD_MIDIPORT) {
			if (moc.wTechnology == MOD_FMSYNTH) {
				MidiDevDefault = nDevId;
				break;					// FMｼﾝｾｻｲｻﾞｰを最優先とする
			}
			else {
				MidiDevDefault = nDevId;
			}
		}
	}

	//***( 機種判別 )***
	BeepModeDefault = 0;
	if (BeepUsable) {
    	SndSelDefault = 1;

		int i = GetKeyboardType(1);
		if ((i>=0) && (i<=4))
			BeepModeDefault = 1;			// DOS/V
    	if ((i>=0x0D01) && (i<=0x0D04))
			BeepModeDefault = 2;			// PC-98
    }
    else {
		SndSelDefault = 2;
    }

	//***( 環境ファイルの読み込み )***
	EnvFileRd();

	if (SndSel->ItemIndex == 1) {		// Beep ?
		if (!BeepUsable) {
			BeepMod->ItemIndex = 0;		// Beepは使用できないのでOFFする
			BeepMod->Enabled = false;
			SndSel->ItemIndex = 2;		// Midi音に変更
		}
    }

	ToneName->Caption = GMToneName[ToneNo->Position];	// 音色名の表示
	EnvPage->ActivePage = TabSheet1;	// 先頭ページの表示
}
//---------------------------------------------------------------------------
void __fastcall TEnv::TestBtnClick(TObject *Sender)
{
	Main->ToneSpdExe->Position = ToneSpd->Position;

	ToneThd = new ToneThread(false, Test_mode, Env->Handle);
	ToneThd->OnTerminate = ThreadDone;
	TestBtn->Enabled = False;
}
//---------------------------------------------------------------------------
void __fastcall TEnv::ThreadDone(TObject *Sender)
{
	Char buff[32];
	int i = ToneThd->TestTime;

	swprintf(buff, L"%d.%d秒（%d字/分）",
		i/1000, i%1000, (600000 + i) / (i * 2));
	RealSpd->Caption = buff;			// 実測表示

	TestBtn->Enabled = True;
}
//---------------------------------------------------------------------------
void __fastcall TEnv::CanBtnClick(TObject *Sender)
{
	if (CompareEnv() == 0)				// 変更されたか？
		return;

	int RetValue = Application->MessageBox(L"今回の設定をキャンセルしますか？",
						L"確認", MB_ICONEXCLAMATION | MB_YESNO);
	if (RetValue == IDYES) {
		RestoreEnv();					// 待避設定の復帰
	}
	else {
		ModalResult = mrNone;			// フォームを閉じない
	}
}
//---------------------------------------------------------------------------
void __fastcall TEnv::FormShow(TObject *Sender)
{
	SaveEnv();							// 現在設定の待避
}
//---------------------------------------------------------------------------
void __fastcall TEnv::SaveBtnClick(TObject *Sender)
{
	int RetValue = Application->MessageBox(L"設定を保存しますか？",
						L"確認", MB_ICONQUESTION | MB_YESNO);
	if (RetValue == IDYES) {
		EnvFileWr();
		SaveEnv();						// 現在設定の待避
	}
}
//---------------------------------------------------------------------------
void __fastcall TEnv::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (CompareEnv() == 0)				// 変更されたか？
		return;

	int RetValue = Application->MessageBox(L"変更されています。保存しますか？",
						L"確認", MB_ICONQUESTION | MB_YESNOCANCEL);
	if (RetValue == IDYES) {
		EnvFileWr();
	}
	else if (RetValue == IDCANCEL) {
		ModalResult = mrNone;			// フォームを閉じない
	}
	else
		;
}
//---------------------------------------------------------------------------
void __fastcall TEnv::ToneKeyChange(TObject *Sender)
{
	Char buff[20];

	swprintf(buff, L"%5dHz", keytable[ToneKey->Position].cyc);
	KeyCyc->Caption = buff;
}
//---------------------------------------------------------------------------
void __fastcall TEnv::DataDirExit(TObject *Sender)
{
	int len = DataDir->Text.Length();
	if (DataDir->Text.SubString(len, 1) != "\\")
		DataDir->Text = DataDir->Text + "\\";	// \の付加

	if (_waccess(DataDir->Text.c_str(), 0)) {
		ShowMessage(DataDir->Text + " \nは不正です");
	}
}
//---------------------------------------------------------------------------
void __fastcall TEnv::ToneNoTxtChange(TObject *Sender)
{
	if (MidiDevCnt == 0) {
		SndSel->ItemIndex = 1;			// MIDIがない時はBeepのみ
    }

	ToneName->Caption = GMToneName[ToneNo->Position];	// 音色名の表示
}
//---------------------------------------------------------------------------
void __fastcall TEnv::BeepModClick(TObject *Sender)
{
	if (BeepModeDefault != 0) {
		if (BeepModeDefault != BeepMod->ItemIndex) {
			if (BeepMod->ItemIndex != 0)
				ShowMessage("不正な機種を選択すると暴走します");
		}
	}
}
//---------------------------------------------------------------------------
int __fastcall TEnv::GetJoyDevNo(void)
{
	String s;

	if (!JoyDevCnt)
		return -1;						// ｼﾞｮｲｽﾃｨｯｸ未検出

	s = JoyDev->Items->Strings[JoyDev->ItemIndex].SubString(1,2);
	return s.ToInt();
}
//---------------------------------------------------------------------------
HANDLE __fastcall TEnv::GetRSHandle(void)
{
	HANDLE handle = CreateFile(RSDev->Items->Strings[RSDev->ItemIndex].c_str(),
					GENERIC_READ | GENERIC_WRITE,
                    0,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
					0);

	if (handle != INVALID_HANDLE_VALUE) {
		EscapeCommFunction(handle, CLRRTS);	// RTS=Off
    }

	return handle;
}
//---------------------------------------------------------------------------
void __fastcall TEnv::KIModeClick(TObject *Sender)
{
	switch (KIMode->ItemIndex) {
	case 1:
		RSDev->Enabled = true;
		JoyDev->Enabled = false;
		break;

	case 2:
		RSDev->Enabled = false;
		JoyDev->Enabled = true;
		break;

	default:
		RSDev->Enabled = false;
		JoyDev->Enabled = false;
        break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEnv::FonChgBtnClick(TObject *Sender)
{
	Graphics::TFont* theFont = new Graphics::TFont;
	theFont->Assign(FonSmpl->Font);

	// ダイアログが開かれた時のフォント指定を合わせる
	FontDialog1->Font->Assign(FonSmpl->Font);
	if (FontDialog1->Execute() == true) {
		FonSmpl->Font->Assign(FontDialog1->Font);
		Main->Memo->Font = FontDialog1->Font;
	}
	else {
		FonSmpl->Font->Assign(theFont);
	}

	delete theFont;
}
//---------------------------------------------------------------------------

void __fastcall TEnv::SndSelClick(TObject *Sender)
{
	BeepMod->Enabled = false;

	MidiDev->Enabled = false;
	ToneEco->Enabled = false;
	ToneNo->Enabled = false;
	ToneNoTxt->Enabled = false;

	DSndDev->Enabled = false;
    DSndWave->Enabled = false;

	switch (SndSel->ItemIndex) {
    case 1:                                 // BEEP
		if (BeepUsable) {
			BeepMod->Enabled = true;
		}
        else {
			BeepMod->ItemIndex = 0;			// Beepは使用できないのでOFF
			BeepMod->Enabled = false;
			ShowMessage("Windows2000以降でBEEP音を使うにはgiveio.sysが必要です。");
		}
		break;

    case 2:                                 // MIDI
		MidiDev->Enabled = true;
		ToneEco->Enabled = true;
		ToneNo->Enabled = true;
		ToneNoTxt->Enabled = true;
		break;

    case 3:                                 // WAVE
		DSndDev->Enabled = true;
		DSndWave->Enabled = true;
		break;
    }
}
//---------------------------------------------------------------------------


void __fastcall TEnv::RndRandomClick(TObject *Sender)
{
    if (RndRandom->Checked) {
        RndCntMin->Enabled = true;
        RndCntMinTxt->Enabled = true;
        Label25->Enabled = true;
    }
    else {
        RndCntMin->Enabled = false;
        RndCntMinTxt->Enabled = false;
        Label25->Enabled = false;
    }

}
//---------------------------------------------------------------------------
void __fastcall TEnv::RndCntMinChangingEx(TObject *Sender,
      bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
    if ((Direction == updUp) && (NewValue > RndCount->Position)) {
        AllowChange = false;
    }

}
//---------------------------------------------------------------------------
void __fastcall TEnv::DirectoryListBox1Change(TObject *Sender)
{
    DataDir->Text = DirectoryListBox1->Directory;

	int len = DataDir->Text.Length();
	if (DataDir->Text.SubString(len, 1) != "\\")
		DataDir->Text = DataDir->Text + "\\";	// \の付加
}
//---------------------------------------------------------------------------

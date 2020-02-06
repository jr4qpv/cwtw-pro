//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>

#include "UReg.h"
#include "UEnv.h"
#include "CWTW-Pro_def.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TRegist *Regist;
//---------------------------------------------------------------------------
__fastcall TRegist::TRegist(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegist::FormShow(TObject *Sender)
{
	Memo->Lines->LoadFromFile(Env->ExePath + LICENSE_FNAME);

	//=====( 登録者名の獲得 )======
	TRegistry *Reg = new TRegistry();

	Reg->RootKey = HKEY_LOCAL_MACHINE;
	if (Reg->OpenKey("\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion",false)) {
		if (Reg->ValueExists("RegisteredOwner")) {
			RegUser->Text = Reg->ReadString("RegisteredOwner");
			RegKey->SetFocus();
		}
	}

	Reg->CloseKey();
	Reg->Free();
}
//---------------------------------------------------------------------------
bool __fastcall TRegist::KeywordCheck(String Key)
{
	Key = Key.Trim();						// 前後の空白文字の削除
	Key = Key.UpperCase();					// 大文字へ変換

	if (Key == PassWord)
		return true;

	return false;
}
//---------------------------------------------------------------------------
void __fastcall TRegist::RegOkBtnClick(TObject *Sender)
{
	//=====( 登録キーワードのチェック )======
	if (KeywordCheck(RegKey->Text) != true) {
		Application->MessageBox(L"登録キーワードが不正です。",
			L"確認", MB_ICONSTOP | MB_OK);
        ModalResult = mrNone;
		RegKey->SetFocus();
		return;
	}

	//=====( ライセンス情報の記録 )=====
	int RetValue = Application->MessageBox(
		L"使用許諾契約書に同意しますか？\n同意する場合は「はい」を押してください。",
		L"確認", MB_ICONQUESTION | MB_YESNOCANCEL);
	if (RetValue == IDYES) {
#ifdef REGISTORY_BASE_KEY
		TRegIniFile *ef;
		ef = new TRegIniFile(REGISTORY_BASE_KEY);
		ef->WriteString("CWTW", "USER", RegUser->Text);
		ef->WriteString("CWTW", "KEY",  License);
		delete ef;
#endif
	}
	else {
		ModalResult = mrNone;				// フォームを閉じない
	}
}
//---------------------------------------------------------------------------

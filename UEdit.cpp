//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <io.h>

#include "UEdit.h"
#include "UEnv.h"
#include "Constant.h"
#include "debug.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TEditFile *EditFile;
//---------------------------------------------------------------------------
__fastcall TEditFile::TEditFile(TComponent* Owner)
	: TForm(Owner)
{
	Mode = 0;
}
//---------------------------------------------------------------------------
int __fastcall TEditFile::LoadFile(String fname)
{
	Mode = 0;
	String fn = fname.UpperCase();		// 大文字変換

	if (fn.Length() == 0)
		return -1;
	if (ExtractFilePath(fn).IsEmpty())
		fn = Env->DataDir->Text + fn;

	if (_waccess(fn.c_str(), 4))		// 読み込み可？
		return -1;						// 異常

	Memo->Lines->LoadFromFile(fn);      // ファイルの読み込み
	StatusBar->SimpleText = fn;         // フルファイル名の表示

	return 0;							// 正常
}
//---------------------------------------------------------------------------
int __fastcall TEditFile::PackMemo(void)
{
	String tmp;
	PChar s;
	int i = 0;

	while (i < Memo->Lines->Count) {
		tmp = Memo->Lines->Strings[i].UpperCase();	// 大文字に変換

		// 空行及びコメント行の削除
		if (tmp.Length() == 0) {
			Memo->Lines->Delete(i);
			continue;
		}
		s = tmp.c_str();
		if (wcsncmp(s, L";", 1) == 0) {
			Memo->Lines->Delete(i);
			continue;
		}
		if (wcsncmp(s, L"//", 2) == 0) {
			Memo->Lines->Delete(i);
			continue;
		}

		// 受信モードの解析
		if (s[0] == '#') {
			if (wcsncmp(s, L"#FILETEXT", 9) == 0)
				Mode = FileText_mode;
			if (wcsncmp(s, L"#FILERAND", 9) == 0)
				Mode = FileRand_mode;
			if (wcsncmp(s, L"#FILEWORD", 9) == 0)
				Mode = FileWord_mode;
			if (wcsncmp(s, L"#FILECHAR", 9) == 0)
				Mode = FileChar_mode;
			if (wcsncmp(s, L"#FILELINE", 9) == 0)
				Mode = FileLine_mode;

			Memo->Lines->Delete(i);
			continue;
		}

		i++;
	}

	return Mode;
}
//---------------------------------------------------------------------------
int __fastcall TEditFile::TrimMemo(void)
{
	int i;

	// 先頭の空白行を削除
	i = 0;
	while (i < Memo->Lines->Count) {
		if (Memo->Lines->Strings[i].Length() == 0) {
			Memo->Lines->Delete(i);
			continue;
		}
        else {
			i++;
        	break;
		}
	}

	// 後ろの空白行の削除
    for (i=Memo->Lines->Count-1; i>=0; i--) {
		if (Memo->Lines->Strings[i].Length() != 0) {
        	break;
		}
        else {
			Memo->Lines->Delete(i);
			continue;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
int __fastcall TEditFile::LoadFileCwt(String fname)
{
	if (LoadFile(fname) < 0) {
		ShowMessage("データファイルがオープン出来ません！");
		return -1;
	}
	PackMemo();					// 不要行の削除

	return Mode;
}
//---------------------------------------------------------------------------

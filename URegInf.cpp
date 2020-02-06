//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "URegInf.h"
#include "UReg.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TRegInf *RegInf;
//---------------------------------------------------------------------------
__fastcall TRegInf::TRegInf(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegInf::BitBtn2Click(TObject *Sender)
{
	int RetValue = Regist->ShowModal();		// 送金登録
	if (RetValue == mrOk) {
		ModalResult = mrYes; 				// 登録完了
	}
}
//---------------------------------------------------------------------------

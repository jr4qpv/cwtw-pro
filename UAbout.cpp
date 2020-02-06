//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>

#include "UAbout.h"
#include "UMain.h"
#include "CWTW-Pro_def.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
    if (Main->PRO_Mode) {
	    Version->Caption = ProTitle + " " + Revcode + " " + Revdate;
	}
    else {
	    Version->Caption = FreeTitle + " " + Revcode + " " + Revdate;
    }

	Copyright->Caption = Cpymsg;
	Comments->Caption = Progname;
	Homepage->Caption = "HomePage:\n " HomePageURL;
	
	if (Main->License_Ok) {
		RegBtn->Enabled = false;
		RegOk->Caption = "登録済：";

#ifdef REGISTORY_BASE_KEY
		TRegIniFile *ef;
		ef = new TRegIniFile(REGISTORY_BASE_KEY);
		LicenseUser->Caption = ef->ReadString("CWTW", "USER", "");
		delete ef;
#else
		LicenseUser->Caption = "Open License";
#endif
	}
}
//---------------------------------------------------------------------------


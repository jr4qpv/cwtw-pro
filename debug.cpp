//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "debug.h"
//---------------------------------------------------------------------------
#ifdef _DEBUG
static TForm* DebugForm = NULL;
static TMemo* Memo = NULL;

static void __fastcall CreateForm()
{
	DebugForm = new TForm(Application);
	Memo = new TMemo(DebugForm);
	Memo->Parent = DebugForm;
	Memo->Clear();
	Memo->ScrollBars = ssBoth;
}

void __fastcall DebugPrint(String s)
{
	if (DebugForm == NULL)
		CreateForm();
	DebugForm->Visible = true;
	Memo->Lines->Add(s);
}
#endif
//---------------------------------------------------------------------------

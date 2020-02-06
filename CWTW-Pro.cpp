//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UAbout.cpp", AboutBox);
USEFORM("UEdit.cpp", EditFile);
USEFORM("URegInf.cpp", RegInf);
USEFORM("UReg.cpp", Regist);
USEFORM("UMain.cpp", Main);
USEFORM("UEnv.cpp", Env);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	HANDLE mx = CreateMutex(NULL, true, L"SingleInstanceProgram");
	if (GetLastError()) {
		ShowMessage("CWTW-Proは既に実行されています！");
		return -1;
	}
	
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMain), &Main);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TEditFile), &EditFile);
		Application->CreateForm(__classid(TRegist), &Regist);
		Application->CreateForm(__classid(TRegInf), &RegInf);
		Application->CreateForm(__classid(TEnv), &Env);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	
	ReleaseMutex(mx);
	return 0;
}
//---------------------------------------------------------------------------

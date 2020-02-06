//----------------------------------------------------------------------------
#ifndef UAboutH
#define UAboutH
//----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
	TLabel *Comments;
	TButton *OKButton;
	TPanel *LicenseUser;
	TLabel *Label1;
	TLabel *RegOk;
	TLabel *Homepage;
	TLabel *Label4;
	TButton *RegBtn;
	void __fastcall FormShow(TObject *Sender);
private:
public:
	virtual __fastcall TAboutBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern TAboutBox *AboutBox;
//----------------------------------------------------------------------------
#endif

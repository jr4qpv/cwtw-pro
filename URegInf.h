//---------------------------------------------------------------------------
#ifndef URegInfH
#define URegInfH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRegInf : public TForm
{
__published:	// IDE 管理のコンポーネント
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TPanel *InstDay;
	TLabel *Label5;
	void __fastcall BitBtn2Click(TObject *Sender);

private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TRegInf(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TRegInf *RegInf;
//---------------------------------------------------------------------------
#endif

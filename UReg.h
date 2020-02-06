//---------------------------------------------------------------------------
#ifndef URegH
#define URegH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TRegist : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMemo *Memo;
	TBitBtn *RegOkBtn;
	TBitBtn *RegCanBtn;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TEdit *RegUser;
	TLabel *Label2;
	TEdit *RegKey;
	TLabel *Label3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RegOkBtnClick(TObject *Sender);
private:	// ユーザー宣言
	bool __fastcall KeywordCheck(String Key);
public:		// ユーザー宣言
	__fastcall TRegist(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TRegist *Regist;
//---------------------------------------------------------------------------
#endif

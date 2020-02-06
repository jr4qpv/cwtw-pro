//---------------------------------------------------------------------------
#ifndef UEditH
#define UEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TEditFile : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMemo *Memo;
	TBitBtn *BitBtn1;
	TStatusBar *StatusBar;
private:	// ユーザー宣言
public:		// ユーザー宣言
	int Mode;
	__fastcall TEditFile(TComponent* Owner);
	int __fastcall LoadFileCwt(String fname);
	int __fastcall LoadFile(String fname);
	int __fastcall PackMemo(void);
	int __fastcall TrimMemo(void);
};
//---------------------------------------------------------------------------
extern TEditFile *EditFile;
//---------------------------------------------------------------------------
#endif

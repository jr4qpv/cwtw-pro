//---------------------------------------------------------------------------
#ifndef UMainH
#define UMainH
//---------------------------------------------------------------------------
#include <System.ImageList.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
#define	USR_TONEMSG		(WM_USER+400)
//---------------------------------------------------------------------------
class TMain : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *MNFileSel;
	TMenuItem *MNFileView;
	TMenuItem *N2;
	TMenuItem *MNExit;
	TMenuItem *MNEnv;
	TStatusBar *Status;
	TMenuItem *Help1;
	TMenuItem *MNRevDisp;
	TOpenDialog *OpenDialog;
	TComboBox *DataFile;
	TMenuItem *MNDocDisp;
	TMenuItem *MNExec;
	TMenuItem *MNStart;
	TMenuItem *MNStop;
	TMenuItem *MNRegist;
	TMenuItem *N1;
	TMenuItem *MNLicDisp;
	TTimer *Timer;
	TToolBar *ToolBar1;
	TToolButton *ToolFileOpen;
	TToolButton *ToolFileView;
	TImageList *ImageList1;
	TToolButton *ToolButton1;
	TToolButton *ToolStart;
	TToolButton *ToolStop;
	TMenuItem *MNFileEdit;
	TToolButton *ToolEdit;
	TCoolBar *CoolBar1;
	TPanel *Panel1;
	TLabel *Label4;
	TEdit *ToneSpdExeTxt;
	TUpDown *ToneSpdExe;
	TPanel *Panel2;
	TImage *Logo;
	TRadioGroup *SMode;
	TRadioGroup *RMode;
	TRadioGroup *JMode;
	TCheckBox *NMode;
	TCheckBox *LMode;
	TBitBtn *StartBtn;
	TBitBtn *StopBtn;
	TCheckBox *AMode;
	TMenuItem *N3;
	TMenuItem *HomePageDisp;
    TMenuItem *N4;
    TMenuItem *MNFileSave;
    TMemo *Memo;
    TSaveDialog *SaveDialog;
    TMenuItem *MNDispClear;
    TCheckBox *XMode;
    TCheckBox *YMode;
    TToolButton *ToolPause;
    TMenuItem *MNPause;
	TToolButton *ToolRetry;
	TMenuItem *MNRetry;
    void __fastcall StartBtnClick(TObject *Sender);
	void __fastcall MNEnvClick(TObject *Sender);
	void __fastcall StopBtnClick(TObject *Sender);
	void __fastcall ToolFileOpenClick(TObject *Sender);
	void __fastcall ToolFileViewClick(TObject *Sender);
	void __fastcall RModeClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall MNRevDispClick(TObject *Sender);
	void __fastcall MNExitClick(TObject *Sender);
	void __fastcall MNDocDispClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall DataFileKeyPress(TObject *Sender, char &Key);
	void __fastcall DataFileClick(TObject *Sender);
	void __fastcall MemoKeyPress(TObject *Sender, char &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MNRegistClick(TObject *Sender);
	void __fastcall SModeClick(TObject *Sender);
	void __fastcall MNLicDispClick(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall ToneSpdExeTxtKeyPress(TObject *Sender, char &Key);
	void __fastcall MNFileEditClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall ToneSpdExeTxtChange(TObject *Sender);
	void __fastcall HomePageDispClick(TObject *Sender);
    void __fastcall MNFileSaveClick(TObject *Sender);
    void __fastcall MNDispClearClick(TObject *Sender);
    void __fastcall ToolPauseClick(TObject *Sender);
private:	// ユーザー宣言
	int cwt_mode;
    int charcnt;
    int Repeat_Start;                       // リピート開始行
    int Repeat_End;                         // リピート最終行
///	ToneThread* ToneThd;
	TDateTime StartTime;
	void __fastcall ThreadDone(TObject *Sender);
	void __fastcall SetDataFile(String fn);
protected:
	void __fastcall UsrToneMsgEvent(TMessage &Message);
	void __fastcall WMGetMinMaxInfo(TWMGetMinMaxInfo &Msg);
	void __fastcall WMDropFiles(TWMDropFiles &Msg);
BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(USR_TONEMSG, TMessage, UsrToneMsgEvent)
///	MESSAGE_HANDLER(WM_GETMINMAXINFO, TWMGetMinMaxInfo, WMGetMinMaxInfo)
	MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
END_MESSAGE_MAP(TForm)
public:		// ユーザー宣言
    bool RegInf_show;			            // ﾗｲｾﾝｽの警告表示
	bool License_Ok;						// 登録完了フラグ
    bool PRO_Mode;                          // シェア版フラグ
    bool Pause_Flag;                        // 一時中断フラグ
   __fastcall TMain(TComponent* Owner);
	int __fastcall get_random(int num);
};
//---------------------------------------------------------------------------
extern TMain *Main;
//---------------------------------------------------------------------------
#endif

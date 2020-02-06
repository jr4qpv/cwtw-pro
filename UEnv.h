//---------------------------------------------------------------------------
#ifndef UEnvH
#define UEnvH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <Outline.hpp>
#include <Vcl.FileCtrl.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TEnv : public TForm
{
__published:	// IDE 管理のコンポーネント
	TBitBtn *CanBtn;
	TButton *SaveBtn;
	TBitBtn *OkBtn;
	TPageControl *EnvPage;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label4;
	TLabel *Label7;
	TComboBox *MidiDev;
	TEdit *ToneNoTxt;
	TTrackBar *ToneEco;
	TUpDown *ToneNo;
	TPanel *ToneName;
	TRadioGroup *BeepMod;
	TLabel *Label5;
	TEdit *ToneSpdTxt;
	TUpDown *ToneSpd;
	TLabel *Label9;
	TButton *TestBtn;
	TLabel *Label8;
	TPanel *RealSpd;
	TRadioGroup *SndSel;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *KeyCyc;
	TTrackBar *ToneKey;
	TTrackBar *ToneVol;
	TGroupBox *GroupBox7;
	TComboBox *DSndDev;
	TLabel *Label6;
	TRadioGroup *DSndWave;
	TLabel *Label10;
	TLabel *Label11;
	TTabSheet *TabSheet4;
	TGroupBox *GroupBox8;
	TEdit *KbdMem1;
	TEdit *KbdMem2;
	TEdit *KbdMem3;
	TEdit *KbdMem4;
	TEdit *KbdMem5;
	TEdit *KbdMem6;
	TEdit *KbdMem7;
	TEdit *KbdMem8;
	TEdit *KbdMem9;
	TEdit *KbdMem10;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *OS_Label;
    TTabSheet *TabSheet5;
    TGroupBox *GroupBox4;
    TEdit *DataDir;
    TGroupBox *GroupBox9;
    TLabel *Label22;
    TEdit *RndCountTxt;
    TUpDown *RndCount;
    TRadioGroup *KIMode;
    TGroupBox *GroupBox3;
    TComboBox *RSDev;
    TGroupBox *GroupBox2;
    TComboBox *JoyDev;
    TRadioGroup *DKMode;
    TFontDialog *FontDialog1;
    TGroupBox *GroupBox10;
    TLabel *Label23;
    TEdit *RndTimeTxt;
    TUpDown *RndTime;
    TLabel *Label24;
    TLabel *Label27;
    TEdit *CrTimeTxt;
    TUpDown *CrTime;
    TLabel *Label28;
    TGroupBox *GroupBox11;
    TLabel *Label26;
    TComboBox *RndSeparator;
    TPanel *Panel1;
    TLabel *Label25;
    TEdit *RndCntMinTxt;
    TUpDown *RndCntMin;
    TCheckBox *RndRandom;
    TTabSheet *TabSheet6;
    TRadioGroup *DispMod;
    TGroupBox *GroupBox6;
    TEdit *FonSmpl;
    TButton *FonChgBtn;
    TGroupBox *GroupBox5;
    TCheckBox *KeepMod;
    TCheckBox *CM_Mode;
    TDriveComboBox *DriveComboBox1;
    TDirectoryListBox *DirectoryListBox1;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TCheckBox *KeepWindow;
	TGroupBox *GroupBox12;
	TComboBox *SendAutoCR;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TestBtnClick(TObject *Sender);
	void __fastcall CanBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SaveBtnClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ToneKeyChange(TObject *Sender);
	void __fastcall DataDirExit(TObject *Sender);
	void __fastcall ToneNoTxtChange(TObject *Sender);
	void __fastcall BeepModClick(TObject *Sender);
	void __fastcall KIModeClick(TObject *Sender);
	void __fastcall FonChgBtnClick(TObject *Sender);
	void __fastcall SndSelClick(TObject *Sender);
    void __fastcall RndRandomClick(TObject *Sender);
    void __fastcall RndCntMinChangingEx(TObject *Sender, bool &AllowChange,
          short NewValue, TUpDownDirection Direction);
    void __fastcall DirectoryListBox1Change(TObject *Sender);
private:	// ユーザー宣言
	UINT DSndDevCnt;
	UINT MidiDevCnt;
	UINT MidiDevDefault;
	bool GiveioUsable;
	bool BeepUsable;
	int BeepModeDefault;
	int SndSelDefault;
	int Save_SndSel;
	int Save_DSndDev;
	int Save_DSndWave;
	int Save_MidiDev;
	int Save_RSDev;
	int Save_SendAutoCR;
	int Save_JoyDev;
	short Save_ToneSpd;
	short Save_ToneNo;
    short Save_RndTime;
    short Save_CrTime;
	short Save_RndCount;
    short Save_RndCntMin;
    bool Save_RndRandom;
	int Save_RndSeparator;
	int Save_ToneKey;
	int Save_ToneVol;
	int Save_ToneEco;
	int Save_BeepMod;
	int Save_KIMode;
	int Save_DKMode;
	int Save_DispMod;
	int Save_FonSize;
    bool Save_CM_Mode;
	bool Save_KeepMod;
   	bool Save_KeepWindow;
    bool Save_BoldMode;
    bool Save_ItalicMode;
	String Save_FonName;
	String Save_DataDir;
	bool __fastcall Check_giveio_sys(void);
	void __fastcall ThreadDone(TObject *Sender);
	void __fastcall SaveEnv(void);
	void __fastcall RestoreEnv(void);
	int  __fastcall CompareEnv(void);
	int __fastcall EnvFileWr(void);
	int __fastcall EnvFileRd(void);
public:		// ユーザー宣言
	UINT JoyDevCnt;
	String ExePath;
	int __fastcall GetJoyDevNo(void);
	HANDLE __fastcall GetRSHandle(void);
	__fastcall TEnv(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TEnv *Env;
//---------------------------------------------------------------------------
#endif

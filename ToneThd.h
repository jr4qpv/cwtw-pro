//---------------------------------------------------------------------------
#ifndef ToneThdH
#define ToneThdH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class ToneThread : public TThread
{
private:
	int cw_mode;
    int kana_judge;						// [ﾎﾚ]/[ﾗﾀ]判定ﾌﾗｸﾞ
	bool kana_flag;
	HWND whandle;
	String ToneFileName;
	void __fastcall OutputStatus(void);
    void __fastcall DisplayMessage(void);
	void __fastcall DisplayStart(void);
	void __fastcall DisplayComplete(void);
	void __fastcall DisplayAbort(void);
	int __fastcall ToneChar(Char c);
	int __fastcall ToneStr(String str);
	int __fastcall ToneFile(String fname);
	int __fastcall ToneMemo(void);
	int __fastcall ToneEmpty(void);
protected:
	void __fastcall Execute();
public:
	DWORD TestTime;
    Char _msgbuff[130];
	__fastcall ToneThread(bool CreateSuspended, int mode, HWND hwin);
	int __fastcall KbdWr(int key);
};
//---------------------------------------------------------------------------
#endif

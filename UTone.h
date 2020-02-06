//---------------------------------------------------------------------------
#ifndef UToneH
#define UToneH

#include "LingBuf.h"
#include "CwTable.h"

//---------------------------------------------------------------------------
struct TONESPEC {
	int		SoundSel;						/* サウンド選択 */
	int		BeepMode;						/* BEEPﾓｰﾄﾞ(機種) */
	UINT	MidiNo;							/* MIDIﾄﾞﾗｲﾊﾞ番号 */
	int		DSndNo;							/* WAVEﾄﾞﾗｲﾊﾞ番号 */
	int		DSndWave;						/* 波形番号 */
	UINT	ToneSpd;						/* 発信ｽﾋﾟｰﾄﾞ */
    UINT    CrTime;                         /* 改行間隔時間[ms] */
	BYTE	ToneNo;							/* 音色番号 */
	BYTE	ToneKey;						/* 音程番号 */
	BYTE	ToneVol;						/* ボリューム */
	BYTE	ToneEco;						/* 残響 */
	int		AutoCR;							/* 送信練習時の自動改行 */
};
//---------------------------------------------------------------------------
class TONESTM : public LINGBUF
{
private:
	UINT		unittime;
    UINT        crtime;
	UINT 		nTimerId;
	UINT		nTimerRes;
	int			timerflag;
	WORD		key_ch;						// 送信ﾓｰﾄﾞのWAVEｻｳﾝﾄﾞ
	WORD		dot_ch;						// 短点のWAVEｻｳﾝﾄﾞ
	WORD		bar_ch;						// 長点のWAVEｻｳﾝﾄﾞ
	int			JoyId;
	HANDLE		RSHandle;
	UINT		renzoku;
	UINT		unitcount;
	UINT		lapcount;
	DWORD		laptime;
	int			SoundFlag;
	int			j_mode;						// 和文指定
	int			a_mode;						// 自動切替指定
	int			CwDisp_mode;				// 符号表示指定
	int			elkey_mode;					// エレキーモード
	int			autocr_mode;				// 送信練習時の自動改行
	int ToneWr(DECODE* datap) { return (bufwr((DWORD)datap)); }
	DECODE* ToneRd(void) { return (DECODE *)bufrd(); }
	BOOL __fastcall Post_ToneMsg(Char c);
	void __fastcall cw_on(int mode);
	void __fastcall cw_off(int mode);
	int __fastcall get_btn(void);
	
public:
	__fastcall TONESTM(int Mod, TONESPEC *tsp, int JId, HANDLE RSh,
					   int Opt, HWND hwin);
	__fastcall ~TONESTM();
	DWORD __fastcall Terminate(void);
	void __fastcall OutOption(BYTE TNo, BYTE Key, BYTE Vol, BYTE Eco);
	int __fastcall ToneWrChar(Char c);
	int  __fastcall LapStart(void);
	int  __fastcall LapStop(void);
	UINT LapGet() { return lapcount; }
	DWORD LapGetTime() { return laptime; }
	void ToneTime(void);
	void KeyTime(void);
};
//---------------------------------------------------------------------------
#endif

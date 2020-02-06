//---------------------------------------------------------------------------
#ifndef DSndH
#define DSndH
//---------------------------------------------------------------------------
#include <mmsystem.h>
#include <dsound.h>
//---------------------------------------------------------------------------
#define ds_NUMCHANNELS      8
#define	ds_DEFAULT	        1
//#define	ds_DEFAULT	        0       /////

#define ds_CHANSPERSAMPLE   2

#if (ds_DEFAULT == 1)
#define ds_BITSPERSAMPLE    8
#define ds_SAMPLERATE       22050
#else
#define ds_BITSPERSAMPLE    16
#define ds_SAMPLERATE       44100
#endif
//---------------------------------------------------------------------------
typedef class DIRSOUND
{
public:
	__fastcall DIRSOUND(int DSno, HWND hwnd);
	__fastcall ~DIRSOUND();

	WORD __fastcall MakeWave(UINT playtime, short cycle, int no);	// ﾁｬﾝﾈﾙ番号を返す
	void __fastcall UnLoadWave(WORD channel);

	void __fastcall Option(Byte Vol);
	void __fastcall MainStart(void);
	void __fastcall MainStop(void);
	void __fastcall Start(WORD channel, BOOL looping);
	void __fastcall Stop(WORD channel);
private:
	/* ---------- メンバ関数 ---------- */
	WORD __fastcall AllocChannel(void);

	void __fastcall CreateBuffer(WORD channel);
	void __fastcall DestroyBuffer(WORD channel);

	void __fastcall FillBuffer(WORD channel);

	/* ---------- データ ---------- */
	HWND          hwnd; 			// アプリケーションのウィンドウハンドル
	LPDIRECTSOUND lpdirsnd;
	LPDIRECTSOUNDBUFFER pDSPrimary;	// プライマリバッファー

	/* これらのフィールドで各サウンドを管理します */
#if (ds_BITSPERSAMPLE == 8)
	BYTE                 *sound[ds_NUMCHANNELS];
#else
	short                *sound[ds_NUMCHANNELS];
#endif
	DWORD				 len[ds_NUMCHANNELS];
	LPDIRECTSOUNDBUFFER  lpdsb[ds_NUMCHANNELS];	// セカンダリバッファー

} DIRSOUND;
//---------------------------------------------------------------------------
#endif

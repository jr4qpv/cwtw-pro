//---------------------------------------------------------------------------
#ifndef MSndH
#define MSndH
#include <mmsystem.h>
//---------------------------------------------------------------------------
union SHORTMSG {
	DWORD	dwData;
	BYTE	bData[4];
};
//---------------------------------------------------------------------------
typedef class MIDISOUND
{
public:
	__fastcall MIDISOUND(UINT MNo);
	__fastcall ~MIDISOUND();
	void __fastcall Start(void);
	void __fastcall Stop(void);
	void __fastcall Option(BYTE TNo,BYTE Key,Byte Vol,BYTE Eco);
private:
	/* ---------- メンバ関数 ---------- */
	void __fastcall OutShortMsg(BYTE sts, BYTE dt1, BYTE dt2);

	/* ---------- データ ---------- */
	HMIDIOUT 	hmo;
	SHORTMSG	tone_on;
	SHORTMSG	tone_off;

} MIDISOUND;
//---------------------------------------------------------------------------
#endif

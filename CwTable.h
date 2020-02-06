//---------------------------------------------------------------------------
#ifndef CwTableH
#define CwTableH
//---------------------------------------------------------------------------
#define	DOT_UNIT			1			/* 短点 */
#define	BAR_UNIT			3			/* 長点 */
#define	OFF_UNIT			1
#define	CHAR_SPAN			3			/* 字間 */
#define	WORD_SPAN			7			/* 語間 */
//---------------------------------------------------------------------------
struct DECODE {
	DWORD	data;
	Char	code;
	Char	cod2;
};
//---------------------------------------------------------------------------
typedef class CWTABLE
{
private:
	/* ---------- データ ---------- */
	
	/* ---------- メンバ関数 ---------- */

public:
	__fastcall CWTABLE();
	__fastcall ~CWTABLE();
	
	DECODE* __fastcall search_code(int jmode, Char c);
	DECODE* __fastcall search_data(int jmode, DWORD data);
	int __fastcall ask_cwunit(DECODE *p);

} CWTABLE;
//---------------------------------------------------------------------------
#endif

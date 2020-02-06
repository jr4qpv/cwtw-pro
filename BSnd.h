//---------------------------------------------------------------------------
#ifndef BSndH
#define BSndH
//---------------------------------------------------------------------------
typedef class BEEPSOUND
{
public:
	__fastcall BEEPSOUND(int BMd);
	__fastcall ~BEEPSOUND();
	void __fastcall Start(void);
	void __fastcall Stop(void);
	void __fastcall Option(short cycle);
private:
	/* ---------- メンバ関数 ---------- */

	/* ---------- データ ---------- */
	int BeepMode;							// 機種判別番号

} BEEPSOUND;
//---------------------------------------------------------------------------
#endif

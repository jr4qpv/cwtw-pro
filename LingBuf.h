//---------------------------------------------------------------------------
#ifndef LingBufH
#define LingBufH
//---------------------------------------------------------------------------
class LINGBUF
{
private:
	DWORD* buf;
	int size;
	int cnt;
	int wrp;
	int rdp;
public:
	__fastcall LINGBUF(int size = 256);
	__fastcall ~LINGBUF();
	int __fastcall bufwr(DWORD data);
	DWORD __fastcall bufrd(void);
	int bufchk() { return (cnt < size-16) ? cnt : -1; }
	void bufclr() { cnt=0; wrp=rdp; };
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef MyRandomH
#define MyRandomH

#define	MYRANDMAX	1024

//---------------------------------------------------------------------------
class TMYRANDOM
{
private:
	short rndbuf[MYRANDMAX];
	int maxcnt;
	int curcnt;
    int old;
public:
	__fastcall TMYRANDOM(void);
	int __fastcall initialize(int reqnum);
   	int __fastcall get_random(int reqnum);
};
//---------------------------------------------------------------------------
#endif

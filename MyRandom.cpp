//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MyRandom.h"
//---------------------------------------------------------------------------
__fastcall TMYRANDOM::TMYRANDOM(void)
{
	int i;
    randomize();

    for (i=0; i<MYRANDMAX; i++)
    	rndbuf[i] = i;

	maxcnt = 0;
	curcnt = 0;
	old = -1;
}
//---------------------------------------------------------------------------
int __fastcall TMYRANDOM::initialize(int reqnum)
{
	int i;

	if (reqnum <= MYRANDMAX) {
		for (i=0; i<reqnum; i++)
			rndbuf[i] = i;
		maxcnt = reqnum;
	}
	else {
		maxcnt = 0;
	}

	curcnt = maxcnt;

	return 0;
}
//---------------------------------------------------------------------------
int __fastcall TMYRANDOM::get_random(int reqnum)
{
	int i, n, rc;

    if (reqnum <= 1)
    	return 0;

	if (reqnum > MYRANDMAX) {
		for (i=0; i<5; i++) {			// 最大５回までリトライ
			rc = random(reqnum);
			if (rc != old)
            	break;            		// 前回と重複しないように
		}
		old = rc;						// 前回値を記憶
		return rc;
    }

	if (reqnum != maxcnt) {
    	initialize(reqnum);
	}

	if (curcnt > 1) {
		for (i=0; i<5; i++) {			// 最大５回までリトライ
	    	n = random(curcnt);
			rc = rndbuf[n];
			if (rc != old)
            	break;            		// 前回と重複しないように
		}
		rndbuf[n] = rndbuf[curcnt-1];
		curcnt--;
    }
	else {
		rc = rndbuf[0];
		initialize(reqnum);
	}

	old = rc;							// 前回値を記憶
	return rc;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

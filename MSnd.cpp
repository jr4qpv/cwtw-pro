//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <mmsystem.h>

#include "MSnd.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall MIDISOUND::MIDISOUND(UINT MNo)
{
    MMRESULT	rc;
	MIDIOUTCAPS moc;

	hmo = NULL;

    //*****( MIDI出力デバイスを再生用にオープンする )*****
    rc = midiOutOpen(&hmo, MNo, (DWORD)NULL, (DWORD)NULL, CALLBACK_NULL);
    if (rc != MMSYSERR_NOERROR) {
		hmo = NULL;
	    MessageBox(NULL, L"MiditSoundオブジェクトの作成に失敗", L"Error", MB_OK);
		return;
    }

	//*****( Key,Volデータをセット )*****
	tone_on.dwData  = 0x603c90;
	tone_off.dwData = 0x003c90;

	//*****( ボリューム設定 )*****
	if (hmo) {
		midiOutGetDevCaps(MNo, &moc, sizeof(MIDIOUTCAPS));
		if (moc.dwSupport & MIDICAPS_VOLUME) {
////			midiOutSetVolume(hmo, (DWORD)MAKELONG(0xFFFF, 0xFFFF));
				midiOutSetVolume(hmo, (DWORD)0xffffffff);		}
		OutShortMsg(0xb0, 0x07, 0x7f);		// ボリューム
		OutShortMsg(0xb0, 0x0b, 0x7f);		// エクスプレッション
	}
}
//---------------------------------------------------------------------------
__fastcall MIDISOUND::~MIDISOUND()
{
	// MIDI出力デバイスをクローズする
	if (hmo) {
	    midiOutReset(hmo);
	    midiOutClose(hmo);
	}
}
//---------------------------------------------------------------------------
void __fastcall MIDISOUND::OutShortMsg(BYTE sts, BYTE dt1, BYTE dt2)
{
	SHORTMSG data;

	data.bData[0] = sts;
	data.bData[1] = dt1;
	data.bData[2] = dt2;
	if (hmo) {
		midiOutShortMsg(hmo, data.dwData);
	}

//	static DECODE mididata = {data, 0};
//	ToneWr(&mididata);
}
//---------------------------------------------------------------------------
void __fastcall MIDISOUND::Option(BYTE TNo, BYTE Key, Byte Vol, BYTE Eco)
{
	tone_on.bData[1] = Key;
	tone_off.bData[1] = Key;
	tone_on.bData[2] = Vol;
	OutShortMsg(0xb0, 0x5b, Eco);		// 残響設定（リバーブ）

	if (TNo) {
		TNo--;
		OutShortMsg(0xc0, TNo, 0);		// 音色選択（プログラムチェンジ）
	}
}
//---------------------------------------------------------------------------
void __fastcall MIDISOUND::Start(void)
{
	if (hmo) {
		midiOutShortMsg(hmo, tone_on.dwData);
	}
}
//---------------------------------------------------------------------------
void __fastcall MIDISOUND::Stop(void)
{
	if (hmo) {
		midiOutShortMsg(hmo, tone_off.dwData);
	}
}
//---------------------------------------------------------------------------

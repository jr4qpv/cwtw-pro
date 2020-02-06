//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <math.h>
#include <stdio.h>

#include "DSnd.h"
extern GUID DSndGUID[];
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dsound.lib"
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::FillBuffer(WORD channel)
{
	LPVOID write1;
	DWORD length1;
	LPVOID write2;
	DWORD length2;
	HRESULT hr;

	hr = lpdsb[channel]->Lock(0, len[channel], &write1, &length1,
										&write2, &length2, 0);

	if (hr == DSERR_BUFFERLOST)	{
		lpdsb[channel]->Restore();

		hr = lpdsb[channel]->Lock(0, len[channel], &write1, &length1,
										&write2, &length2, 0);
	}
	if (hr != DS_OK) {
	    MessageBox(NULL, L"バッファーのロックに失敗", L"Error", MB_OK);
		exit(-1);
	}

	CopyMemory(write1, sound[channel], length1);

	if (write2 != NULL)	{
		CopyMemory(write2, sound[channel] + length1, length2);
	}

	hr = lpdsb[channel]->Unlock(write1, length1, write2, length2);
	if (hr != DS_OK) {
	    MessageBox(NULL, L"バッファーのアンロックに失敗", L"Error", MB_OK);
		exit(-1);
	}
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::DestroyBuffer(WORD channel)
{
	DWORD hr;

	hr = lpdsb[channel]->Release();
	if (hr != DS_OK) {
	    MessageBox(NULL, L"セカンダリバッファーの削除に失敗", L"Error", MB_OK);
		exit(-1);
	}

	lpdsb[channel] = NULL;
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::CreateBuffer(WORD channel)
{
	DSBUFFERDESC dsbdesc;
	PCMWAVEFORMAT pcmwf;
	HRESULT hr;

	// セカンダリ・バッファを作成する
	memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
	pcmwf.wf.wFormatTag		= WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels 		= ds_CHANSPERSAMPLE;
	pcmwf.wf.nSamplesPerSec	= ds_SAMPLERATE;
	pcmwf.wf.nBlockAlign	= ds_CHANSPERSAMPLE * ds_BITSPERSAMPLE / 8;
	pcmwf.wf.nAvgBytesPerSec= pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample	= ds_BITSPERSAMPLE;

	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize			= sizeof(DSBUFFERDESC);
///	dsbdesc.dwFlags			= DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC;
/// dsbdesc.dwFlags			= DSBCAPS_STATIC;
    dsbdesc.dwFlags			= DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
                              DSBCAPS_LOCDEFER;
	dsbdesc.dwBufferBytes	= len[channel];
	dsbdesc.lpwfxFormat		= (LPWAVEFORMATEX)&pcmwf;

	hr = lpdirsnd->CreateSoundBuffer(&dsbdesc, &lpdsb[channel], NULL);
	if (hr != DS_OK) {
		MessageBox(NULL, L"セカンダリ・バッファの作成に失敗", L"Error", MB_OK);
		lpdsb[channel] = NULL;
		return;
	}
}
//---------------------------------------------------------------------------
WORD __fastcall DIRSOUND::AllocChannel(void)
{
	WORD x;

	for (x=0;x<ds_NUMCHANNELS;x++) {
		if (lpdsb[x] == NULL) {
			break;
		}
	}

	if (x == ds_NUMCHANNELS) {
		MessageBox(NULL, L"使用できるチャンネルがありません", L"Error", MB_OK);
		exit(-1);
	}

	return (x);
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::Option(Byte Vol)
{
											/* Vol = 0～127 */
	long volume;							/* -10,000～0 */
    double temp;

	if (pDSPrimary == NULL)
		return;

	temp = double(Vol) * 787.4;
	if (temp < 1.0)
		temp = 0.0;
	else
		temp = 100 * 20.0 * log10(temp);
	volume = long(temp) - 10000;
    if (volume > 0)
    	volume = 0;

	pDSPrimary->SetVolume(volume);
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::MainStart(void)
{
	HRESULT hr;

	if (pDSPrimary == NULL)
		return;

	hr = pDSPrimary->Play(0, 0, DSBPLAY_LOOPING);
	if (hr != DS_OK) {
		MessageBox(NULL, L"プライマリバッファのPlayに失敗", L"Error", MB_OK);
		exit(-1);
	}
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::MainStop(void)
{
	if (pDSPrimary == NULL)
		return;

	pDSPrimary->Stop();
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::Start(WORD channel, BOOL looping)
{
	HRESULT hr;

	if (lpdsb[channel] == NULL)
		return;

	lpdsb[channel]->SetCurrentPosition(0);
	hr = lpdsb[channel]->Play(0, 0, (looping == TRUE) ? DSBPLAY_LOOPING : 0);
	if (hr != DS_OK) {
		MessageBox(NULL, L"セカンダリバッファのPlayに失敗", L"Error", MB_OK);
		exit(-1);
	}
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::Stop(WORD channel)
{
	if (lpdsb[channel] == NULL)
		return;

	lpdsb[channel]->Stop();
}
//---------------------------------------------------------------------------
void __fastcall DIRSOUND::UnLoadWave(WORD channel)
{
	DestroyBuffer(channel);

	if (sound[channel] != NULL)	{
		delete sound[channel];
	}
}
//---------------------------------------------------------------------------
WORD __fastcall DIRSOUND::MakeWave(UINT playtime, short cycle, int no)
{
	DWORD i, size, esize;
	WORD channel;
	double data, pi2, f, a;
#if (ds_BITSPERSAMPLE == 8)
	BYTE* p;
	BYTE d, d0;
#else
	short* p;
	short d, d0;
#endif

	size = (ds_SAMPLERATE * playtime) / 1000;	// playtimeあたりのｻﾝﾌﾟﾙ数

	pi2 = 2.0 * 3.14159265359;
    f = double(cycle);						// 周波数

	channel = AllocChannel();

#if (ds_BITSPERSAMPLE == 8)
	a = 100.0;								// 振幅
	sound[channel] = new BYTE[size * ds_CHANSPERSAMPLE];	// *ﾁｬﾝﾈﾙ数
#else
	a = 30000.0;							// 振幅
	sound[channel] = new short[size * ds_CHANSPERSAMPLE];	// *ﾁｬﾝﾈﾙ数
#endif
	if (sound[channel] == NULL) {
		exit(-1);
	}

	/* wave データを作成する */
	p = sound[channel];
	d0 = 0;
	esize = 0;
	for (i=0; i<size; i++) {
   	    data = a * sin(pi2 * f * double(i) / double(ds_SAMPLERATE));

#if (ds_BITSPERSAMPLE == 8)
		d = (BYTE)(128 + BYTE(data));
		if ((d0 < 128) && (d >= 128))		// 零ｸﾛｽ点を検出
			esize = i;

		if (no) {							// 矩形波へ変換処理
			if (d < 128)
	        	d = 56;
			else
	        	d = 200;
        }
#else
		d = short(data);
		if ((d0 < 0) && (d >= 0))		// 零ｸﾛｽ点を検出
			esize = i;

		if (no) {							// 矩形波へ変換処理
			if (d < 0)
	        	d = -10000;
			else
	        	d = 10000;
        }
#endif

		*(p++) = d;
		*(p++) = d;
		d0 = d;
    }
	esize ++;

#if (ds_BITSPERSAMPLE == 8)
	len[channel]   = esize * ds_CHANSPERSAMPLE;
#else
	len[channel]   = esize * ds_CHANSPERSAMPLE * 2;
#endif

	CreateBuffer(channel);
	FillBuffer(channel);

	return (channel);
}
//---------------------------------------------------------------------------
__fastcall DIRSOUND::~DIRSOUND()
{
	WORD x;

	for (x=0;x<ds_NUMCHANNELS;x++) {
		if (lpdsb[x] != NULL) {
			DestroyBuffer(x);

			if (sound[x] != NULL) {
				delete sound[x];
			}
		}
	}

	pDSPrimary->Release();
	(void)lpdirsnd->Release();	// ここでエラーになってもすることはない
}
//---------------------------------------------------------------------------
__fastcall DIRSOUND::DIRSOUND(int DSno, HWND hwnd)
{
	DWORD ret;
	WORD x;

	lpdirsnd = NULL;
	for (x=0;x<ds_NUMCHANNELS;x++) 	{
		lpdsb[x] = NULL;
	}

	if (DSno == 0)
		ret = DirectSoundCreate(NULL, &lpdirsnd, NULL);
	else
		ret = DirectSoundCreate(&DSndGUID[DSno], &lpdirsnd, NULL);
	if (ret != DS_OK) {
		MessageBox(NULL, L"DirectSoundオブジェクトの作成に失敗", L"Error", MB_OK);
		lpdirsnd = NULL;
		return;
	}

	// 協調レベルを設定
#if ds_DEFAULT
	ret = lpdirsnd->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
#else
	ret = lpdirsnd->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
#endif
	if (ret != DS_OK) {
		MessageBox(NULL, L"協調レベルの設定に失敗", L"Error", MB_OK);
		return;
	}

	// プライマリ・バッファの作成
	// DSBUFFERDESC構造体を設定
	DSBUFFERDESC dsbdesc;
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	// プライマリ・バッファを指定
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;

	// バッファを作る
	ret = lpdirsnd->CreateSoundBuffer(&dsbdesc, &pDSPrimary, NULL);
	if(ret != DS_OK) {
	    // バッファの作成に失敗
		pDSPrimary = NULL;
	    MessageBox(NULL, L"プライマリ・バッファの作成に失敗", L"Error", MB_OK);
		return;
	}

#if ds_DEFAULT
#else
	// プライマリ・バッファのWaveフォマットを設定
	//		優先協調レベル以上の設定がされている必要がある
	WAVEFORMATEX pcmwf;
	memset(&pcmwf, 0, sizeof(WAVEFORMATEX));
	pcmwf.wFormatTag		= WAVE_FORMAT_PCM;
	pcmwf.nChannels 		= ds_CHANSPERSAMPLE;
	pcmwf.nSamplesPerSec	= ds_SAMPLERATE;
	pcmwf.nBlockAlign		= ds_CHANSPERSAMPLE * ds_BITSPERSAMPLE / 8;
	pcmwf.nAvgBytesPerSec	= pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
	pcmwf.wBitsPerSample	= ds_BITSPERSAMPLE;
	ret = pDSPrimary->SetFormat(&pcmwf);
	if(ret != DS_OK) {
	    // バッファの作成に失敗
		pDSPrimary = NULL;
	    MessageBox(NULL, "プライマリ・バッファの設定に失敗", "Error", MB_OK);
		return;
	}
#endif
}
//---------------------------------------------------------------------------

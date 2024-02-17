/*
   Class Name:

      CAuDev

   Description:

      Play Sound
*/

#pragma once
class CAuDev
{
// variable

private:
	HWND hWnd;
	HWAVEOUT hWaveOut;
	WAVEHDR whdr;
	BYTE* buffer[3];
	int buffersize[3];
	int i;

public:
	wchar_t error[MAXERRORLENGTH];

// function

private:

public:
	CAuDev();
	~CAuDev();

	void OnWaveOutOpen(HWND hWnd, HWAVEOUT hwo, LPWAVEHDR pwavehdr);
	void OnWaveOutClose(HWND hWnd, HWAVEOUT hwo, LPWAVEHDR pwavehdr);
	void OnWaveOutDone(HWND hWnd, HWAVEOUT hwo, LPWAVEHDR pwavehdr);

	int Load(HWND hWnd, wchar_t *szFile);
	int Load(wchar_t *szFile);
	void Play(int i);
	void Close();
};

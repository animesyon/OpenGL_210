/*
   Class Name:

      CAuDev

   Description:

      Play Sound
*/

#include "stdafx.h"
#include "audev.h"

// constructor
CAuDev::CAuDev()
{
	buffer[0] = NULL;
	buffer[1] = NULL;
	buffer[2] = NULL;
}

// destructor
CAuDev::~CAuDev()
{
	if(buffer[0] != NULL) delete[] buffer[0];
	if(buffer[1] != NULL) delete[] buffer[1];
	if(buffer[2] != NULL) delete[] buffer[2];
}

// message MM_WOM_OPEN handler
void CAuDev::OnWaveOutOpen(HWND hWnd, HWAVEOUT hwo, LPWAVEHDR pwavehdr)
{
	//OutputDebugString(L"----- MM_WOM_OPEN\n");
}

// message MM_WOM_CLOSE handler
void CAuDev::OnWaveOutClose(HWND hWnd, HWAVEOUT hwo, LPWAVEHDR pwavehdr)
{
	//OutputDebugString(L"----- MM_WOM_CLOSE\n");
}

// message MM_WOM_DONE handler
void CAuDev::OnWaveOutDone(HWND hWnd, HWAVEOUT hwo, LPWAVEHDR pwavehdr)
{
	//OutputDebugString(L"----- MM_WOM_DONE\n");

	MMRESULT mmresult;

	// The waveOutUnprepareHeader function cleans up the preparation performed by the waveOutPrepareHeader function.
	// This function must be called after the device driver is finished with a data block.
	// You must call this function before freeing the buffer.
	mmresult = waveOutUnprepareHeader(hwo, pwavehdr, sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR)
	{
		waveOutGetErrorText(mmresult, error, MAXERRORLENGTH);

		OutputDebugString(L"waveOutUnprepareHeader error: ");
		OutputDebugString(error);
		OutputDebugString(L"\n");
	}
}

//
int CAuDev::Load(HWND hWnd, wchar_t *szFile)
{
   FILE *fp;
   errno_t err;
   char chunkid[4];
   int chunksize;

   this->hWnd = hWnd;

	// open file
	if( (err = _wfopen_s(&fp,szFile,L"rb")) != 0 )
		return -1;
 	
	// read chunk RIFF
	fread(chunkid, sizeof(char), 4, fp);
	fread(&chunksize, sizeof(int), 1, fp);

	// error checking
	if(chunkid[0] != 'R' && chunkid[1] != 'I' && chunkid[2] != 'F' && chunkid[3] != 'F') 
	{
		fclose(fp);
		return -1;
	}

	// read chunk WAVE
	fread(chunkid, sizeof(char), 4, fp);

	// error checking
	if(chunkid[0] != 'W' && chunkid[1] != 'A' && chunkid[2] != 'V' && chunkid[3] != 'E') 
	{
		fclose(fp);
		return -1;
	}

	// read chunk fmt
	fread(chunkid, sizeof(char), 4, fp);
	fread(&chunksize, sizeof(int), 1, fp);

	// error checking
	if(chunkid[0] != 'f' && chunkid[1] != 'm' && chunkid[2] != 't' && chunkid[3] != ' ') 
	{
		fclose(fp);
		return -1;
	}

	// read audio format
	WAVEFORMATEX wfx;

	wfx.cbSize          = 0;
	fread(&wfx.wFormatTag,      sizeof(WORD),  1, fp);
	fread(&wfx.nChannels,       sizeof(WORD),  1, fp);
	fread(&wfx.nSamplesPerSec,  sizeof(DWORD), 1, fp);
	fread(&wfx.nAvgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&wfx.nBlockAlign,     sizeof(WORD),  1, fp);
	fread(&wfx.wBitsPerSample,  sizeof(WORD),  1, fp);


	// read chunk audio data
	fread(chunkid, sizeof(char), 4, fp);
	fread(&chunksize, sizeof(int), 1, fp);


	// error checking
	if(chunkid[0] != 'd' && chunkid[1] != 'a' && chunkid[2] != 't' && chunkid[3] != 'a') 
	{
		fclose(fp);
		return -1;
	}

   // allocate memory for audio data
	i = 0;

	if(buffer[i] != NULL)
		delete[] buffer[i];

   buffer[i] = new BYTE[chunksize];
   fread(buffer[i], sizeof(BYTE), chunksize, fp);

   buffersize[i] = chunksize;

	// close file
	fclose(fp);

	// The waveOutOpen function opens the given waveform-audio output device for playback.
	MMRESULT mmresult = waveOutOpen(&hWaveOut, 0, &wfx, (DWORD)hWnd, NULL, CALLBACK_WINDOW);
	if(mmresult != MMSYSERR_NOERROR)
	{
		waveOutGetErrorText(mmresult, error, MAXERRORLENGTH);

		OutputDebugString(L"waveOutOpen error: ");
		OutputDebugString(error);
		OutputDebugString(L"\n");
	}

	return i;
}

//
int CAuDev::Load(wchar_t *szFile)
{
   FILE *fp;
   errno_t err;
   char chunkid[4];
   int chunksize;

	// open file
	if( (err = _wfopen_s(&fp,szFile,L"rb")) != 0 )
		return -1;
 	
	// read chunk RIFF
	fread(chunkid, sizeof(char), 4, fp);
	fread(&chunksize, sizeof(int), 1, fp);

	// error checking
	if(chunkid[0] != 'R' && chunkid[1] != 'I' && chunkid[2] != 'F' && chunkid[3] != 'F') 
	{
		fclose(fp);
		return -1;
	}

	// read chunk WAVE
	fread(chunkid, sizeof(char), 4, fp);

	// error checking
	if(chunkid[0] != 'W' && chunkid[1] != 'A' && chunkid[2] != 'V' && chunkid[3] != 'E') 
	{
		fclose(fp);
		return -1;
	}

	// read chunk fmt
	fread(chunkid, sizeof(char), 4, fp);
	fread(&chunksize, sizeof(int), 1, fp);

	// error checking
	if(chunkid[0] != 'f' && chunkid[1] != 'm' && chunkid[2] != 't' && chunkid[3] != ' ') 
	{
		fclose(fp);
		return -1;
	}

	// read audio format
	WAVEFORMATEX wfx;

	wfx.cbSize          = 0;
	fread(&wfx.wFormatTag,      sizeof(WORD),  1, fp);
	fread(&wfx.nChannels,       sizeof(WORD),  1, fp);
	fread(&wfx.nSamplesPerSec,  sizeof(DWORD), 1, fp);
	fread(&wfx.nAvgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&wfx.nBlockAlign,     sizeof(WORD),  1, fp);
	fread(&wfx.wBitsPerSample,  sizeof(WORD),  1, fp); 

	// read chunk audio data
	fread(chunkid, sizeof(char), 4, fp);
	fread(&chunksize, sizeof(int), 1, fp);

	// error checking
	if(chunkid[0] != 'd' && chunkid[1] != 'a' && chunkid[2] != 't' && chunkid[3] != 'a') 
	{
		fclose(fp);
		return -1;
	}

   // allocate memory for audio data
   ++i;

	if(buffer[i] != NULL)
		delete[] buffer[i];

   buffer[i] = new BYTE[chunksize];
   fread(buffer[i], sizeof(BYTE), chunksize, fp);

   buffersize[i] = chunksize;

	// close file
	fclose(fp);

	return i;
}

//
void CAuDev::Play(int i)
{
	whdr.dwBufferLength  = buffersize[i];
	whdr.dwBytesRecorded = 0;
	whdr.dwFlags         = 0;
	whdr.dwLoops         = 0;
	whdr.dwUser          = 0;
	whdr.lpData          = (LPSTR)buffer[i];
	whdr.lpNext          = NULL;
	whdr.reserved        = 0;

	MMRESULT mmresult;

	// The waveOutPrepareHeader function prepares a waveform-audio data block for playback.
	mmresult = waveOutPrepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR)
	{
		waveOutGetErrorText(mmresult, error, MAXERRORLENGTH);

		OutputDebugString(L"waveOutPrepareHeader error: ");
		OutputDebugString(error);
		OutputDebugString(L"\n");
	}

	// The waveOutWrite function sends a data block to the given waveform-audio output device.
	mmresult = waveOutWrite(hWaveOut, &whdr, sizeof(WAVEHDR));
	if(mmresult != MMSYSERR_NOERROR)
	{
		waveOutGetErrorText(mmresult, error, MAXERRORLENGTH);

		OutputDebugString(L"waveOutWrite error: ");
		OutputDebugString(error);
		OutputDebugString(L"\n");
	}
}

//
void CAuDev::Close()
{
	MMRESULT mmresult;

	// The waveOutClose function closes the given waveform-audio output device.
	mmresult = waveOutClose(hWaveOut);
	if(mmresult != MMSYSERR_NOERROR)
	{
		waveOutGetErrorText(mmresult, error, MAXERRORLENGTH);

		OutputDebugString(L"OnWaveOutClose error: ");
		OutputDebugString(error);
		OutputDebugString(L"\n");
	}
}

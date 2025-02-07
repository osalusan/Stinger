#include "main/main.h"
#include "audioManager.h"


IXAudio2*				AudioManager::m_Xaudio = nullptr;
IXAudio2MasteringVoice*	AudioManager::m_MasteringVoice = nullptr;
std::unordered_map<AUDIO, AUDIO_DATA*> AudioManager::m_LoadAudioPool = {};
std::unordered_map<AUDIO, AUDIO_RESERVE_DATA> AudioManager::m_ReservAudioPool = {};

constexpr float MASTER_VOLUME = 0.6f;

void AudioManager::InitMaster()
{
	// COM初期化
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio生成
	if (m_Xaudio == nullptr)
	{
		XAudio2Create(&m_Xaudio, 0);
	}
	
	// マスタリングボイス生成
	if (m_Xaudio != nullptr && m_MasteringVoice == nullptr)
	{
		m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
	}
}


void AudioManager::UninitMaster()
{
	for (std::pair<const AUDIO, AUDIO_DATA*>& audio : m_LoadAudioPool)
	{
		AUDIO_DATA* audioData = audio.second;
		if (audioData == nullptr) continue;
		IXAudio2SourceVoice* sourceVoice = audioData->s_SourceVoice;
		if (sourceVoice == nullptr) continue;

		sourceVoice->Stop();
		sourceVoice->DestroyVoice();

		delete[] audioData->s_SoundData;
		delete audio.second;
		audio.second = nullptr;
	}

	m_LoadAudioPool.clear();

	m_MasteringVoice->DestroyVoice();
	m_Xaudio->Release();
	CoUninitialize();
}

void AudioManager::LoadAudio(const AUDIO& audio,const char *FileName)
{
	// 保存済みがある場合
	if (m_LoadAudioPool.count(audio) > 0) return;

	AUDIO_DATA* audioData = new AUDIO_DATA;

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = nullptr;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, nullptr, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			BYTE* pFormatData = new BYTE[mmckinfo.cksize];
			if (pFormatData == nullptr) return;
			mmioRead(hmmio, (HPSTR)pFormatData, mmckinfo.cksize);
			memcpy(&wfx, pFormatData, sizeof(WAVEFORMATEX)); 
			delete[] pFormatData;
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}

		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		buflen = datachunkinfo.cksize;
		audioData->s_SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)audioData->s_SoundData, buflen);


		audioData->s_Length = readlen;
		audioData->s_PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	m_Xaudio->CreateSourceVoice(&audioData->s_SourceVoice, &wfx);
	assert(audioData->s_SourceVoice);


	m_LoadAudioPool.emplace(audio, audioData);
}

void AudioManager::Init()
{
	for (const std::pair<const AUDIO, AUDIO_RESERVE_DATA>& reservAudio : m_ReservAudioPool)
	{
		LoadAudio(reservAudio.first,reservAudio.second.s_FileName);
		if (reservAudio.second.s_StartSound)
		{
			Play(reservAudio.first, reservAudio.second.s_Loop);
		}
	}

	m_ReservAudioPool.clear();
}

void AudioManager::Uninit()
{
	for (std::pair<const AUDIO, AUDIO_DATA*>& loadAudio : m_LoadAudioPool)
	{
		AUDIO_DATA* audioData = loadAudio.second;
		if (audioData == nullptr) continue;
		IXAudio2SourceVoice* sourceVoice = audioData->s_SourceVoice;
		if (sourceVoice == nullptr) continue;

		sourceVoice->Stop();
		sourceVoice->FlushSourceBuffers();
	}
}

void AudioManager::ReservAudio(const AUDIO& audio, const char* fileName, const bool& startSound, const bool& loopSound)
{
	const AUDIO_RESERVE_DATA& audioReserveData = { fileName ,startSound ,loopSound };
	m_ReservAudioPool.emplace(audio, audioReserveData);
}

void AudioManager::Play(const AUDIO& audio, const bool& Loop, const float& volume)
{
	if (m_LoadAudioPool.count(audio) <= 0) return;
	AUDIO_DATA* audioData = m_LoadAudioPool.at(audio);
	if (audioData == nullptr) return;
	IXAudio2SourceVoice* sourceVoice = audioData->s_SourceVoice;
	if (sourceVoice == nullptr) return;
	BYTE* soundData = audioData->s_SoundData;
	if (soundData == nullptr) return;

	sourceVoice->Stop();
	sourceVoice->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = audioData->s_Length;
	bufinfo.pAudioData = soundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = audioData->s_PlayLength;

	// ループ設定
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = audioData->s_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	sourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	float outputMatrix[4] = { 1.0f , 0.0f, 0.0f , 1.0f };
	sourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
	sourceVoice->SetVolume(MASTER_VOLUME * volume);

	// 再生
	sourceVoice->Start();

}

void AudioManager::Finish(const AUDIO& audio)
{
	if (m_LoadAudioPool.count(audio) <= 0) return;
	AUDIO_DATA* audioData = m_LoadAudioPool.at(audio);
	if (audioData == nullptr) return;
	IXAudio2SourceVoice* sourceVoice = audioData->s_SourceVoice;
	if (sourceVoice == nullptr) return;
	BYTE* soundData = audioData->s_SoundData;
	if (soundData == nullptr) return;

	sourceVoice->Stop();
	sourceVoice->FlushSourceBuffers();
}

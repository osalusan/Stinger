#pragma once
#include <xaudio2.h>
#include <unordered_map>

enum class AUDIO
{
	MAWJ_BATTLE_BGM = 0,
	MAX
};

struct AUDIO_DATA
{
	IXAudio2SourceVoice* s_SourceVoice = nullptr;
	BYTE* s_SoundData = nullptr;
	int s_Length = 0;
	int s_PlayLength = 0;
};
struct AUDIO_RESERVE_DATA
{
	const char* s_FileName = {};
	bool s_StartSound = false;			// 読み込んですぐに音を鳴らす
	bool s_Loop = false;
};

class AudioManager final
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	static std::unordered_map<AUDIO, AUDIO_DATA*> m_LoadAudioPool;
	static std::unordered_map<AUDIO, AUDIO_RESERVE_DATA> m_ReservAudioPool;

	static void LoadAudio(const AUDIO& audio,const char* FileName);
public:
	static void InitMaster();		// ゲームを起動してから一度のみ呼ぶ
	static void UninitMaster();		// ゲームを起動してから一度のみ呼ぶ
	static void Init();
	static void Uninit();
	static void ReservAudio(const AUDIO& audio, const char* fileName,const bool& startSound = false,const bool& loopSound = false);

	static void Play(const AUDIO& audio,const bool& Loop = false,const float& volume = 1.0f);
};


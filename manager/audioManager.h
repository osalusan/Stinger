#pragma once
#include <xaudio2.h>
#include <unordered_map>

enum class AUDIO
{
	MAWJ_BATTLE_BGM = 0,
	TITLE_BGM,
	PARRY_SE,
	SLASH1_SE,
	SLASH2_SE,
	SLASH3_SE,
	SOWRD_CHAGE,
	HIT_ATTACK,
	MAX
};
// 再生するのに必要なデータ
struct AUDIO_DATA
{
	IXAudio2SourceVoice* s_SourceVoice = nullptr;
	BYTE* s_SoundData = nullptr;
	int s_Length = 0;
	int s_PlayLength = 0;
};
// 読み込み時に必要なデータ
struct AUDIO_RESERVE_DATA
{
	const char* s_FileName = {};
	bool m_PlaySoundOnStart = false;			// 読み込んですぐに音を鳴らす
	bool s_Loop = false;
};
// ゲーム内すべての音を管理するクラス
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
	static void Init();				// 予約された音をSceneの最後に一括で読み込む
	static void Uninit();
	static void ReservAudio(const AUDIO& audio, const char* fileName,const bool& startSound = false,const bool& loopSound = false);

	static void Play(const AUDIO& audio,const bool& Loop = false,const float& volume = 1.0f);
	static void Finish(const AUDIO& audio);
};


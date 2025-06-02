#pragma once
#include <unordered_map>
#include <string>

// 読み込んでいるfbxモデル一覧
enum class ANIMETION_MODEL
{
	PLAYER = 0,
	MAWJLAYGO,
	MAX
};

// 前方宣言
class FbxModelRenderer;
// ゲーム内すべてのfbxモデルを管理するクラス
class FbxModelManager final
{
private:
	static std::unordered_map<ANIMETION_MODEL, FbxModelRenderer*> m_LoadModelPool;
	static std::unordered_map<ANIMETION_MODEL, std::string> m_ReservModelPool;
	static std::unordered_map<std::string, ANIMETION_MODEL> m_ReservAnimationPool;
	static std::unordered_map<std::string, std::string> m_ReservAnimationNamePool;		// これはシーン更新時に初期化しない

	static void LoadModel(const ANIMETION_MODEL& model, const std::string& fileName);
	static void LoadAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName);
public:
	static void Init();				// 予約されたモデルをSceneの最後に一括で読み込む
	static void Uninit();
	static void ReservModel(const ANIMETION_MODEL& model, const std::string& fileName);
	// 先にモデル本体を予約していないと読み込み時にアニメーションを読み込めない
	static void ReservAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName);
	static FbxModelRenderer* GetAnimationModel(const ANIMETION_MODEL& model);
};
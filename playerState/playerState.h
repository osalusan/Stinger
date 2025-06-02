#pragma once
#include "playerState/playerStateMachine.h"

class Player;
class GameObject;
// プレイヤーステートの基底クラス
class PlayerState
{
protected:
	PlayerStateMachine* m_PlayerMachine = nullptr;		// ステートマシンに情報を伝えられるように
	const Player* m_PlayerCache = nullptr;				// プレイヤーのデータを直接変更できないように / 変更はステートマシンで行う

	bool m_IsLoad = false;				// 一度だけ読み込むため
	std::string m_AnimName = {};
	bool m_IsLoadAnimation = false;		// アニメーションを一度だけ読み込むため
	float m_CurrentTime = 0.0f;			// 経過時間
	float m_MaxAnimTime = 0.0f;			// アニメーション最大時間
	float m_BlendTime = 0.0f;			// モーションブレンド速度
	float m_AnimSpeedValue = 1.0f;		// アニメーション速度 / 1.0が等倍

	// 2回目以降は内部で弾く
	void LoadAnimation(const std::string& fileName, const std::string& animationName);
	// カメラの向きに回転
	void RotToCameraDirection(const float& deltaTime);
	// カメラから見て、キーの入力方向に向く
	void RotToInputKeyDirection(const float& deltaTime,const bool& rotAway = false);
	// ターゲットの向きに回転
	void RotToTarget(const GameObject* obj,const float& deltaTime);
	// プレイヤークラスで読み込んだデータを取得
	float FindStateData(const std::unordered_map<std::string, float>& stateData,const std::string& dataName);
public:
	virtual ~PlayerState();
	PlayerState(PlayerStateMachine* machine);
	void ChangePlayerState(const PLAYER_STATE& playerState);

	virtual void Init() = 0;
	virtual void Unit() = 0;
	virtual void Update(const float& deltaTime);
	virtual void ChangeStateControl() = 0;		// ステートの切り替え処理
};
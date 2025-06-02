#pragma once
#include "staticMeshObject.h"

// 前方宣言
enum class ANIMETION_MODEL;
class ShaderComponent;
// 装備品用のクラス
class EquipmentObject final: public StaticMeshObject
{
private:
	const GameObject* m_ParentObject = nullptr;		// プレイヤーの現在の回転を取得
	ShaderComponent* m_ShaderCache = nullptr;		// コンポーネント側が自動取得する回転と異なる値を格納する為
	ANIMETION_MODEL m_ParentModel;					// コンストラクタで初期設定
	const char* m_BoneName = "";
	// 初期設定した数値を保存しておく用
	XMFLOAT3 m_OffsetScale = {};					
	XMFLOAT3 m_OffsetRot = {};
	XMFLOAT3 m_OffsetPos = {};

	virtual void MoveControl(const float& deltaTime)override;
public:
	~EquipmentObject();
	EquipmentObject() = delete;
	EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel,const std::string& fileName,const ANIMETION_MODEL& parentModel,const char* boneName, const XMFLOAT3& scale);
	EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel,const std::string& fileName,const ANIMETION_MODEL& parentModel,const char* boneName, const XMFLOAT3& scale, const XMFLOAT3& rot);
	EquipmentObject(const GameObject* gameObj, const STATICMESH_MODEL& staticModel,const std::string& fileName,const ANIMETION_MODEL& parentModel,const char* boneName, const XMFLOAT3& scale, const XMFLOAT3& rot, const XMFLOAT3& pos);
};
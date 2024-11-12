#include "collisionComponent.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "object/gameObject.h"
#include "staticMeshObject/staticmeshObject.h"
#include "character/character.h"

// ------------------------ protected ------------------------
bool CollisionComponent::HitOBB(const OBB& obb1, const OBB& obb2)
{
	// 各方向ベクトルの確保
	const XMVECTOR& NAe1 = obb1.GetDirect(0), Ae1 = XMVectorScale(NAe1, obb1.GetSize(0));
	const XMVECTOR& NAe2 = obb1.GetDirect(1), Ae2 = XMVectorScale(NAe2, obb1.GetSize(1));
	const XMVECTOR& NAe3 = obb1.GetDirect(2), Ae3 = XMVectorScale(NAe3, obb1.GetSize(2));
	const XMVECTOR& NBe1 = obb2.GetDirect(0), Be1 = XMVectorScale(NBe1, obb2.GetSize(0));
	const XMVECTOR& NBe2 = obb2.GetDirect(1), Be2 = XMVectorScale(NBe2, obb2.GetSize(1));
	const XMVECTOR& NBe3 = obb2.GetDirect(2), Be3 = XMVectorScale(NBe3, obb2.GetSize(2));
	const XMVECTOR& Interval = XMVectorSubtract(obb1.Center, obb2.Center);

	// 計算用の関数の初期化
	float rA, rB, L = 0.0f;

	// 分離軸 : Ae1
	rA = XMVectorGetX(XMVector3Length(Ae1));
	rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NAe1)));
	if (L > rA + rB) return false;

	// 分離軸 : Ae2
	rA = XMVectorGetX(XMVector3Length(Ae2));
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NAe2)));
	if (L > rA + rB) return false;

	// 分離軸 : Ae3
	rA = XMVectorGetX(XMVector3Length(Ae3));
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NAe3)));
	if (L > rA + rB) return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = XMVectorGetX(XMVector3Length(Be1));
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NBe1)));
	if (L > rA + rB) return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = XMVectorGetX(XMVector3Length(Be2));
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NBe2)));
	if (L > rA + rB) return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = XMVectorGetX(XMVector3Length(Be3));
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NBe3)));
	if (L > rA + rB) return false;

	// 分離軸 : C11-C33
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			const XMVECTOR& Cross = XMVector3Cross(obb1.Axis[i], obb2.Axis[j]);
			rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, Ae3);
			rB = LenSegOnSeparateAxis(Cross, Be1, Be2, Be3);
			L = fabsf(XMVectorGetX(XMVector3Dot(Interval, Cross)));
			if (L > rA + rB) return false;
		}
	}

	// 分離平面が存在しないので「衝突している」
	return true;
}

// 分離軸に投射された軸成分から投射線分長を算出
float CollisionComponent::LenSegOnSeparateAxis(const XMVECTOR& Sep, const XMVECTOR& e1, const XMVECTOR& e2, const XMVECTOR& e3)
{
	const float& r1 = fabsf(XMVectorGetX(XMVector3Dot(Sep, e1)));
	const float& r2 = fabsf(XMVectorGetX(XMVector3Dot(Sep, e2)));
	const float& r3 = fabsf(XMVectorGetX(XMVector3Dot(Sep, e3)));
	return r1 + r2 + r3;
}

void CollisionComponent::SetHitObject(GameObject* hitObj)
{
	if (hitObj != nullptr)
	{
		m_HitGameObjectsCache.emplace_back(hitObj);
	}
}

bool CollisionComponent::CheckHitObject()
{
	// オブジェクト一覧の取得
	if (m_GameObjectsCache->empty())
	{
		GameScene* gameScene = SceneManager::GetScene<GameScene>();
		if (gameScene == nullptr) return false;
		ObjectManager* objectManager = gameScene->GetObjectManager();
		if (objectManager == nullptr) return false;

		objectManager->GetGameObjects(m_GameObjectsCache);

		if (m_GameObjectsCache->empty()) return false;
	}

	// TODO : Box以外の当たり判定を作成したら、このコメント以下の処理をBoxComponentに移動
	
	if (m_GameObject == nullptr) return false;

	OBB myObb = {};

	// オブジェクトの位置、スケール、回転行列を取得
	const XMFLOAT3& myPosition = m_GameObject->GetPos();
	const XMFLOAT3& myScale = m_GameObject->GetScale();
	const XMMATRIX& myRotationMatrix = m_GameObject->GetRotationMatrix();

	// モデルの原点とサイズを取得
	const XMFLOAT3& myModelCenter = m_GameObject->GetModelCenter();
	const XMFLOAT3& myModelScale = m_GameObject->GetModelScale();

	// 補正された原点を計算
	XMVECTOR correctedOrigin = XMVectorSet(
		myModelCenter.x * myScale.x,
		myModelCenter.y * myScale.y,
		myModelCenter.z * myScale.z,
		0.0f);

	// オブジェクトの回転を補正された原点に適用
	correctedOrigin = XMVector3TransformCoord(correctedOrigin, myRotationMatrix);

	// OBBの中心を計算
	const XMVECTOR& myCenter = XMVectorSet(
		myPosition.x,
		myPosition.y,
		myPosition.z,
		0.0f) + correctedOrigin;

	// OBBのサイズ（ハーフサイズ）を計算
	const XMVECTOR& mySize = XMVectorSet(
		(myModelScale.x * myScale.x) * 0.5f,
		(myModelScale.y * myScale.y) * 0.5f,
		(myModelScale.z * myScale.z) * 0.5f,
		0.0f);

	// OBBの軸を設定
	const XMFLOAT3& axisX = m_GameObject->GetRight();
	const XMFLOAT3& axisY = m_GameObject->GetUp();
	const XMFLOAT3& axisZ = m_GameObject->GetForward();

	myObb.Axis[0] = XMLoadFloat3(&axisX);
	myObb.Axis[1] = XMLoadFloat3(&axisY);
	myObb.Axis[2] = XMLoadFloat3(&axisZ);

	// OBBに値を設定
	myObb.Center = myCenter;
	myObb.Size = mySize;

	for (GameObject* object : m_GameObjectsCache[static_cast<int>(OBJECT::BOX)])
	{
		OBB boxObb = {};


		const XMFLOAT3& boxPosition = object->GetPos();
		const XMFLOAT3& boxScale = object->GetScale();
		const XMMATRIX& boxRotationMatrix = object->GetRotationMatrix();

		const XMFLOAT3& boxModelCenter = object->GetModelCenter();
		const XMFLOAT3& boxModelScale = object->GetModelScale();	

		// 補正された原点を計算
		XMVECTOR boxCorrectedOrigin = XMVectorSet(
			boxModelCenter.x * boxScale.x,
			boxModelCenter.y * boxScale.y,
			boxModelCenter.z * boxScale.z,
			0.0f);

		// ボックスの回転を補正された原点に適用
		boxCorrectedOrigin = XMVector3TransformCoord(boxCorrectedOrigin, boxRotationMatrix);

		// OBBの中心を計算
		const XMVECTOR& boxCenter = XMVectorSet(
			boxPosition.x,
			boxPosition.y,
			boxPosition.z,
			0.0f) + boxCorrectedOrigin;

		// OBBのサイズ（ハーフサイズ）を計算
		const XMVECTOR& boxSize = XMVectorSet(
			(boxModelScale.x * boxScale.x) * 0.5f,
			(boxModelScale.y * boxScale.y) * 0.5f,
			(boxModelScale.z * boxScale.z) * 0.5f,
			0.0f);

		// OBBの軸を設定
		const XMFLOAT3& boxAxisX = object->GetRight();
		const XMFLOAT3& boxAxisY = object->GetUp();
		const XMFLOAT3& boxAxisZ = object->GetForward();

		boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
		boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
		boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

		// OBBに値を設定
		boxObb.Center = boxCenter;
		boxObb.Size = boxSize;

		// 衝突判定
		if (HitOBB(myObb, boxObb))
		{

			SetHitObject(object);
			return true;
		}
	}

	return false;
}

void CollisionComponent::Init()
{
	if (m_GameObject != nullptr)
	{
		m_Scale = m_GameObject->GetScale();
	}
}

void CollisionComponent::Uninit()
{
	m_GameObjectsCache->clear();
}

void CollisionComponent::Draw()
{
	if (m_GameObject == nullptr) return;

	m_Pos = m_GameObject->GetPos();
}

bool CollisionComponent::CheckHitCollision(const COLLISION_TAG& tag)
{
	m_HitGameObjectsCache.clear();

	switch (tag)
	{
	case COLLISION_TAG::OBJECT:
		if (CheckHitObject())
		{

			return true;
		}
		break;
	default:
		break;
	}

	return false;
}
#include "collisionComponent.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "object/gameObject.h"
#include "staticMeshObject/staticmeshObject.h"
#include "character/character.h"

// ------------------------ protected ------------------------
bool CollisionComponent::HitOBB(const OBB& obb1, const OBB& obb2)
{
	// �e�����x�N�g���̊m��
	const XMVECTOR& NAe1 = obb1.GetDirect(0), Ae1 = XMVectorScale(NAe1, obb1.GetSize(0));
	const XMVECTOR& NAe2 = obb1.GetDirect(1), Ae2 = XMVectorScale(NAe2, obb1.GetSize(1));
	const XMVECTOR& NAe3 = obb1.GetDirect(2), Ae3 = XMVectorScale(NAe3, obb1.GetSize(2));
	const XMVECTOR& NBe1 = obb2.GetDirect(0), Be1 = XMVectorScale(NBe1, obb2.GetSize(0));
	const XMVECTOR& NBe2 = obb2.GetDirect(1), Be2 = XMVectorScale(NBe2, obb2.GetSize(1));
	const XMVECTOR& NBe3 = obb2.GetDirect(2), Be3 = XMVectorScale(NBe3, obb2.GetSize(2));
	const XMVECTOR& Interval = XMVectorSubtract(obb1.Center, obb2.Center);

	// �v�Z�p�̊֐��̏�����
	float rA, rB, L = 0.0f;

	// ������ : Ae1
	rA = XMVectorGetX(XMVector3Length(Ae1));
	rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NAe1)));
	if (L > rA + rB) return false;

	// ������ : Ae2
	rA = XMVectorGetX(XMVector3Length(Ae2));
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NAe2)));
	if (L > rA + rB) return false;

	// ������ : Ae3
	rA = XMVectorGetX(XMVector3Length(Ae3));
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NAe3)));
	if (L > rA + rB) return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = XMVectorGetX(XMVector3Length(Be1));
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NBe1)));
	if (L > rA + rB) return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = XMVectorGetX(XMVector3Length(Be2));
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NBe2)));
	if (L > rA + rB) return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = XMVectorGetX(XMVector3Length(Be3));
	L = fabsf(XMVectorGetX(XMVector3Dot(Interval, NBe3)));
	if (L > rA + rB) return false;

	// ������ : C11-C33
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			const XMVECTOR& Cross = XMVector3Cross(obb1.Axis[i], obb2.Axis[j]);
			rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, Ae3);
			rB = LenSegOnSeparateAxis(Cross, Be1, Be2, Be3);
			L = fabsf(XMVectorGetX(XMVector3Dot(Interval, Cross)));
			if (L > rA + rB) return false;
		}
	}

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

// �������ɓ��˂��ꂽ���������瓊�ː��������Z�o
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
	// �I�u�W�F�N�g�ꗗ�̎擾
	if (m_GameObjectsCache->empty())
	{
		GameScene* gameScene = SceneManager::GetScene<GameScene>();
		if (gameScene == nullptr) return false;
		ObjectManager* objectManager = gameScene->GetObjectManager();
		if (objectManager == nullptr) return false;

		objectManager->GetGameObjects(m_GameObjectsCache);

		if (m_GameObjectsCache->empty()) return false;
	}

	// TODO : Box�ȊO�̓����蔻����쐬������A���̃R�����g�ȉ��̏�����BoxComponent�Ɉړ�
	
	if (m_GameObject == nullptr) return false;

	OBB myObb = {};

	// �I�u�W�F�N�g�̈ʒu�A�X�P�[���A��]�s����擾
	const XMFLOAT3& myPosition = m_GameObject->GetPos();
	const XMFLOAT3& myScale = m_GameObject->GetScale();
	const XMMATRIX& myRotationMatrix = m_GameObject->GetRotationMatrix();

	// ���f���̌��_�ƃT�C�Y���擾
	const XMFLOAT3& myModelCenter = m_GameObject->GetModelCenter();
	const XMFLOAT3& myModelScale = m_GameObject->GetModelScale();

	// �␳���ꂽ���_���v�Z
	XMVECTOR correctedOrigin = XMVectorSet(
		myModelCenter.x * myScale.x,
		myModelCenter.y * myScale.y,
		myModelCenter.z * myScale.z,
		0.0f);

	// �I�u�W�F�N�g�̉�]��␳���ꂽ���_�ɓK�p
	correctedOrigin = XMVector3TransformCoord(correctedOrigin, myRotationMatrix);

	// OBB�̒��S���v�Z
	const XMVECTOR& myCenter = XMVectorSet(
		myPosition.x,
		myPosition.y,
		myPosition.z,
		0.0f) + correctedOrigin;

	// OBB�̃T�C�Y�i�n�[�t�T�C�Y�j���v�Z
	const XMVECTOR& mySize = XMVectorSet(
		(myModelScale.x * myScale.x) * 0.5f,
		(myModelScale.y * myScale.y) * 0.5f,
		(myModelScale.z * myScale.z) * 0.5f,
		0.0f);

	// OBB�̎���ݒ�
	const XMFLOAT3& axisX = m_GameObject->GetRight();
	const XMFLOAT3& axisY = m_GameObject->GetUp();
	const XMFLOAT3& axisZ = m_GameObject->GetForward();

	myObb.Axis[0] = XMLoadFloat3(&axisX);
	myObb.Axis[1] = XMLoadFloat3(&axisY);
	myObb.Axis[2] = XMLoadFloat3(&axisZ);

	// OBB�ɒl��ݒ�
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

		// �␳���ꂽ���_���v�Z
		XMVECTOR boxCorrectedOrigin = XMVectorSet(
			boxModelCenter.x * boxScale.x,
			boxModelCenter.y * boxScale.y,
			boxModelCenter.z * boxScale.z,
			0.0f);

		// �{�b�N�X�̉�]��␳���ꂽ���_�ɓK�p
		boxCorrectedOrigin = XMVector3TransformCoord(boxCorrectedOrigin, boxRotationMatrix);

		// OBB�̒��S���v�Z
		const XMVECTOR& boxCenter = XMVectorSet(
			boxPosition.x,
			boxPosition.y,
			boxPosition.z,
			0.0f) + boxCorrectedOrigin;

		// OBB�̃T�C�Y�i�n�[�t�T�C�Y�j���v�Z
		const XMVECTOR& boxSize = XMVectorSet(
			(boxModelScale.x * boxScale.x) * 0.5f,
			(boxModelScale.y * boxScale.y) * 0.5f,
			(boxModelScale.z * boxScale.z) * 0.5f,
			0.0f);

		// OBB�̎���ݒ�
		const XMFLOAT3& boxAxisX = object->GetRight();
		const XMFLOAT3& boxAxisY = object->GetUp();
		const XMFLOAT3& boxAxisZ = object->GetForward();

		boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
		boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
		boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

		// OBB�ɒl��ݒ�
		boxObb.Center = boxCenter;
		boxObb.Size = boxSize;

		// �Փ˔���
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
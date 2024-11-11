#include "collisionComponent.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "object/gameObject.h"
#include "staticMeshObject/staticmeshObject.h"

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
bool CollisionComponent::CheckHitObject()
{
	// �I�u�W�F�N�g�̎擾
	if (m_GameObjectsCache->empty())
	{
		GameScene* gameScene = SceneManager::GetScene<GameScene>();
		if (gameScene == nullptr) return false;
		ObjectManager* objectManager = gameScene->GetObjectManager();
		if (objectManager == nullptr) return false;

		objectManager->GetGameObjects(m_GameObjectsCache);

		if (m_GameObjectsCache->empty()) return false;
	}

	OBB myObb = {};

	{

		// OBB�̒��S�ʒu���v�Z
		const XMVECTOR& myCenter = XMVectorSet(
			m_Pos.x + m_Scale.x,
			m_Pos.y + m_Scale.y,
			m_Pos.z + m_Scale.z,
			0.0f);

		// OBB�̃T�C�Y���v�Z
		const XMVECTOR& mySize = XMVectorSet(
			m_Scale.x * 10.0f,
			m_Scale.y * 10.0f,
			m_Scale.z * 10.0f,
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
	}


	for (GameObject* object : m_GameObjectsCache[static_cast<int>(OBJECT::BOX)])
	{
		OBB boxObb = {};


		const XMFLOAT3& boxPosition = object->GetPos();
		const XMFLOAT3& boxScale = object->GetScale();

		// OBB�̒��S�ʒu���v�Z
		const XMVECTOR& boxCenter = XMVectorSet(
			boxPosition.x + boxScale.x,
			boxPosition.y + boxScale.y,
			boxPosition.z + boxScale.z,
			0.0f);

		// OBB�̃T�C�Y���v�Z�i�n�[�t�T�C�Y�j
		const XMVECTOR& boxSize = XMVectorSet(
			boxScale.x,
			boxScale.y,
			boxScale.z,
			0.0f);

		// OBB�̎���ݒ�
		XMFLOAT3 boxAxisX = object->GetRight();
		XMFLOAT3 boxAxisY = object->GetUp();
		XMFLOAT3 boxAxisZ = object->GetForward();

		boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
		boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
		boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

		boxObb.Center = boxCenter;
		boxObb.Size = boxSize;

		if (HitOBB(myObb, boxObb))
		{
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
//void Player::GetOBBVertices(const OBB& obb, XMFLOAT3 vertices[8]) 
//{
//	// OBB�̒��S�ʒu�Ɗe�������̔����T�C�Y���擾
//	XMVECTOR center = obb.Center;
//	XMVECTOR axisX = XMVectorScale(obb.Axis[0], XMVectorGetX(obb.Size));
//	XMVECTOR axisY = XMVectorScale(obb.Axis[1], XMVectorGetY(obb.Size));
//	XMVECTOR axisZ = XMVectorScale(obb.Axis[2], XMVectorGetZ(obb.Size));
//
//	// 8�̒��_���v�Z
//	XMVECTOR corners[8];
//	corners[0] = center - axisX - axisY - axisZ;
//	corners[1] = center + axisX - axisY - axisZ;
//	corners[2] = center + axisX + axisY - axisZ;
//	corners[3] = center - axisX + axisY - axisZ;
//	corners[4] = center - axisX - axisY + axisZ;
//	corners[5] = center + axisX - axisY + axisZ;
//	corners[6] = center + axisX + axisY + axisZ;
//	corners[7] = center - axisX + axisY + axisZ;
//
//	// XMVECTOR����XMFLOAT3�֕ϊ�
//	for (int i = 0; i < 8; ++i) 
//	{
//		XMStoreFloat3(&vertices[i], corners[i]);
//	}
//}
//
//void Player::DrawOBBWireframe(const XMFLOAT3 vertices[8])
//{
//	// ���_�f�[�^�����C�����X�g�p�ɒ�`
//	XMFLOAT3 lineVertices[24] = {
//		vertices[0], vertices[1], // Edge 0-1
//		vertices[1], vertices[2], // Edge 1-2
//		vertices[2], vertices[3], // Edge 2-3
//		vertices[3], vertices[0], // Edge 3-0
//
//		vertices[4], vertices[5], // Edge 4-5
//		vertices[5], vertices[6], // Edge 5-6
//		vertices[6], vertices[7], // Edge 6-7
//		vertices[7], vertices[4], // Edge 7-4
//
//		vertices[0], vertices[4], // Edge 0-4
//		vertices[1], vertices[5], // Edge 1-5
//		vertices[2], vertices[6], // Edge 2-6
//		vertices[3], vertices[7]  // Edge 3-7
//	};
//
//	// ���_�o�b�t�@�̐ݒ�
//	D3D11_BUFFER_DESC bufferDesc = {};
//	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	bufferDesc.ByteWidth = sizeof(lineVertices);
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA initData = {};
//	initData.pSysMem = lineVertices;
//
//	ID3D11Buffer* vertexBuffer;
//	Renderer::GetDevice()->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
//
//	// ���_�o�b�t�@���o�C���h
//	UINT stride = sizeof(XMFLOAT3);
//	UINT offset = 0;
//	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
//
//	// �v���~�e�B�u�g�|���W��ݒ�
//	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//	// �`��
//	Renderer::GetDeviceContext()->Draw(24, 0);
//
//	// ���
//	vertexBuffer->Release();
//}
#include "boxCollisionComponent.h"
#include "object/gameObject.h"

void BoxCollisionComponent::Init()
{
	CollisionComponent::Init();
}

void BoxCollisionComponent::Uninit()
{
	CollisionComponent::Uninit();
}

void BoxCollisionComponent::Update()
{
	//OBB myObb = {};

	//{

	//	// OBBの中心位置を計算
	//	XMVECTOR myCenter = XMVectorSet(
	//		m_Pos.x + m_Scale.x,
	//		m_Pos.y + m_Scale.y,
	//		m_Pos.z + m_Scale.z,
	//		0.0f);

	//	// OBBのサイズを計算
	//	XMVECTOR mySize = XMVectorSet(
	//		m_Scale.x * 0.5f,
	//		m_Scale.y * 0.5f,
	//		m_Scale.z * 0.5f,
	//		0.0f);

	//	// OBBの軸を設定
	//	const XMFLOAT3& axisX = m_GameObject->GetRight();
	//	const XMFLOAT3& axisY = m_GameObject->GetUp();
	//	const XMFLOAT3& axisZ = m_GameObject->GetForward();

	//	myObb.Axis[0] = XMLoadFloat3(&axisX);
	//	myObb.Axis[1] = XMLoadFloat3(&axisY);
	//	myObb.Axis[2] = XMLoadFloat3(&axisZ);

	//	// OBBに値を設定
	//	myObb.Center = myCenter;
	//	myObb.Size = mySize;
	//}


	//for (Box* box : BoxList)
	//{
	//	OBB boxObb = {};


	//	XMFLOAT3 boxPosition = box->GetPosition();
	//	XMFLOAT3 boxScale = box->GetScale();

	//	// OBBの中心位置を計算
	//	XMVECTOR boxCenter = XMVectorSet(
	//		boxPosition.x + boxScale.x,
	//		boxPosition.y + boxScale.y,
	//		boxPosition.z + boxScale.z,
	//		0.0f);

	//	// OBBのサイズを計算（ハーフサイズ）
	//	XMVECTOR boxSize = XMVectorSet(
	//		boxScale.x * 0.5f,
	//		boxScale.y * 0.5f,
	//		boxScale.z * 0.5f,
	//		0.0f);

	//	// OBBの軸を設定
	//	XMFLOAT3 boxAxisX = box->GetRight();
	//	XMFLOAT3 boxAxisY = box->GetUp();
	//	XMFLOAT3 boxAxisZ = box->GetForward();

	//	boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
	//	boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
	//	boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

	//	boxObb.Center = boxCenter;
	//	boxObb.Size = boxSize;
	//}
}

void BoxCollisionComponent::Draw()
{
	CollisionComponent::Draw();

	OBB myObb = {};

	{

		// OBBの中心位置を計算
		const XMVECTOR& myCenter = XMVectorSet(
			m_Pos.x + m_Scale.x,
			m_Pos.y + m_Scale.y,
			m_Pos.z + m_Scale.z,
			0.0f);

		// OBBのサイズを計算
		const XMVECTOR& mySize = XMVectorSet(
			m_Scale.x * 0.5f,
			m_Scale.y * 0.5f,
			m_Scale.z * 0.5f,
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
	}


	//for (Box* box : BoxList)
	//{
	//	OBB boxObb = {};


	//	XMFLOAT3 boxPosition = box->GetPosition();
	//	XMFLOAT3 boxScale = box->GetScale();

	//	// OBBの中心位置を計算
	//	XMVECTOR boxCenter = XMVectorSet(
	//		boxPosition.x + boxScale.x,
	//		boxPosition.y + boxScale.y,
	//		boxPosition.z + boxScale.z,
	//		0.0f);

	//	// OBBのサイズを計算（ハーフサイズ）
	//	XMVECTOR boxSize = XMVectorSet(
	//		boxScale.x * 0.5f,
	//		boxScale.y * 0.5f,
	//		boxScale.z * 0.5f,
	//		0.0f);

	//	// OBBの軸を設定
	//	XMFLOAT3 boxAxisX = box->GetRight();
	//	XMFLOAT3 boxAxisY = box->GetUp();
	//	XMFLOAT3 boxAxisZ = box->GetForward();

	//	boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
	//	boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
	//	boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

	//	boxObb.Center = boxCenter;
	//	boxObb.Size = boxSize;
	//}
}

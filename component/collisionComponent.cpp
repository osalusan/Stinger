#include "collisionComponent.h"
#include "manager/sceneManager.h"
#include "manager/objModelManager.h"
#include "scene/gameScene.h"
#include "object/gameObject.h"
#include "staticMeshObject/staticmeshObject.h"
#include "character/character.h"
#include "renderer/objModelRenderer.h"

// ------------------------ protected ------------------------
bool CollisionComponent::HitOBB(const OBB& obb1, const OBB& obb2)
{
	// ローカル変数として初期化
	float minOverlap = FLT_MAX;
	XMVECTOR mtvAxis = XMVectorZero();

	const XMVECTOR& Interval = XMVectorSubtract(obb2.Center, obb1.Center);

	// 軸のリスト
	XMVECTOR axes[15] = {};
	int axisCount = 0;

	// OBB1の軸
	axes[axisCount++] = obb1.Axis[0];
	axes[axisCount++] = obb1.Axis[1];
	axes[axisCount++] = obb1.Axis[2];

	// OBB2の軸
	axes[axisCount++] = obb2.Axis[0];
	axes[axisCount++] = obb2.Axis[1];
	axes[axisCount++] = obb2.Axis[2];

	// 各軸での判定
	for (int i = 0; i < axisCount; ++i)
	{
		XMVECTOR axis = axes[i];

		float rA = LenSegOnSeparateAxis(axis, obb1);
		float rB = LenSegOnSeparateAxis(axis, obb2);
		float L = fabsf(XMVectorGetX(XMVector3Dot(Interval, axis)));
		float overlap = (rA + rB) - L;
		if (overlap < 0.0f) return false;

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			mtvAxis = axis;
		}
	}

	// クロス軸
	for (int i = 0; i < 3; ++i) 
	{
		XMVECTOR axisA = obb1.Axis[i];

		for (int j = 0; j < 3; ++j) 
		{
			XMVECTOR axisB = obb2.Axis[j];
			XMVECTOR cross = XMVector3Cross(axisA, axisB);
			// 数値が小さすぎたら無視
			if (XMVector3LengthSq(cross).m128_f32[0] > 1e-6f) 
			{
				axes[axisCount++] = XMVector3Normalize(cross);
			}
		}
	}

	// MTVの方向を調整
	XMVECTOR direction = XMVectorSubtract(obb1.Center, obb2.Center);
	if (XMVectorGetX(XMVector3Dot(direction, mtvAxis)) < 0) 
	{
		mtvAxis = XMVectorNegate(mtvAxis);
	}

	// 計算結果を保存
	m_MinOverlap = minOverlap;
	m_MtvAxis = mtvAxis;

	return true;
}

// 分離軸に投射された軸成分から投射線分長を算出
float CollisionComponent::LenSegOnSeparateAxis(const XMVECTOR& Sep, const OBB& obb)
{
	float r = 0.0f;
	for (int i = 0; i < 3; ++i) 
	{
		float e = obb.GetSize(i); 
		XMVECTOR axis = obb.Axis[i];
		float dot = fabsf(XMVectorGetX(XMVector3Dot(Sep, axis)));
		r += e * dot;
	}
	return r;
}
void CollisionComponent::SetHitObject(GameObject* hitObj)
{
	if (hitObj != nullptr)
	{
		m_HitGameObjectsCache.emplace_back(hitObj);
	}
}

bool CollisionComponent::CheckHitObject(const OBJECT& object)
{
	m_HitGameObjectsCache.clear();
	m_MinOverlap = FLT_MAX;
	m_MtvAxis = {};

	// オブジェクト一覧の取得
	if (m_GameObjectsCache->empty())
	{
		Scene* gameScene = SceneManager::GetScene();
		if (gameScene == nullptr) return false;
		ObjectManager* objectManager = gameScene->GetObjectManager();
		if (objectManager == nullptr) return false;

		objectManager->GetAllGameObjects(m_GameObjectsCache);

		if (m_GameObjectsCache->empty()) return false;
	}
	return true;
}

bool CollisionComponent::CheckHitObject(const COLLISION_TAG& tag)
{
	m_HitGameObjectsCache.clear();
	m_MinOverlap = FLT_MAX;
	m_MtvAxis = {};

	// オブジェクト一覧の取得
	if (m_GameObjectsCache->empty())
	{
		Scene* gameScene = SceneManager::GetScene();
		if (gameScene == nullptr) return false;
		ObjectManager* objectManager = gameScene->GetObjectManager();
		if (objectManager == nullptr) return false;

		objectManager->GetAllGameObjects(m_GameObjectsCache);

		if (m_GameObjectsCache->empty()) return false;
	}
	return true;
}

CollisionComponent::CollisionComponent(GameObject* gameObject,const COLLISION_TAG& tag)
	:Component(gameObject)
{
	m_CollisionTag = tag;
	m_Model = STATICMESH_MODEL::BOX_CENTER;
}

CollisionComponent::CollisionComponent(GameObject* gameObject, const COLLISION_TAG& tag, const std::string& name)
	:CollisionComponent(gameObject,tag)
{
	m_CollisionName = name;
}

CollisionComponent::~CollisionComponent()
{
#if _DEBUG

	// シェーダーの削除
	if (m_VertexLayout != nullptr)
	{
		m_VertexLayout->Release();
		m_VertexLayout = nullptr;
	}
	if (m_VertexShader != nullptr)
	{
		m_VertexShader->Release();
		m_VertexLayout = nullptr;
	}
	if (m_PixelShader != nullptr)
	{
		m_PixelShader->Release();
		m_VertexLayout = nullptr;
	}
#endif
}

void CollisionComponent::Init()
{
	// 初期値
	if (m_GameObject != nullptr)
	{
		m_Scale = m_GameObject->GetScale();
	}
#if _DEBUG
	if (m_VertexShader == nullptr && m_VertexLayout == nullptr)
	{
		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "cso\\unlitTextureVS.cso");
	}
	if (m_PixelShader == nullptr)
	{
		Renderer::CreatePixelShader(&m_PixelShader, "cso\\unlitTexturePS.cso");
	}
#endif
}

void CollisionComponent::Uninit()
{
	m_GameObjectsCache->clear();
}


void CollisionComponent::Draw()
{
#if _DEBUG
	if (m_VertexLayout != nullptr && m_VertexShader != nullptr && m_PixelShader != nullptr)
	{
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
	}

	XMMATRIX world, scl, rot, trans;

	scl = XMMatrixScaling((m_Scale.x * m_ModelScale.x) * 0.5f, (m_Scale.y * m_ModelScale.y) * 0.5f, (m_Scale.z * m_ModelScale.z) * 0.5f);
	rot = m_RotationMatrix;
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	
	world = scl * rot * trans;

	if (m_CollisionName != "")
	{
		// TODO :修正予定 / バグってる
		//world = XMMatrixMultiply(m_OffsetMatrix,world);
	}
	
	Renderer::SetWorldMatrix(world);

	if (ObjModelRenderer* model = ObjModelManager::GetModel(m_Model))
	{
		if (model == nullptr) return;

		model->DrawCollision();
	}
	
#endif // _DEBUG
}

void CollisionComponent::SetCollisionInfo(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& modelCenterPos, const XMFLOAT3& modelScale, const XMMATRIX& rotateMatrix)
{
	m_Position = pos;
	m_Scale = scale;
	m_ModelCenter = modelCenterPos;
	m_ModelScale = modelScale;
	m_RotationMatrix = rotateMatrix;
}

void CollisionComponent::SetCollisionInfo(const XMFLOAT3& pos, const XMFLOAT3& modelScale, const XMMATRIX& rotateMatrix, const XMMATRIX& worldMatrix)
{
	m_Position = pos;
	m_ModelScale = modelScale;
	m_RotationMatrix = rotateMatrix;
	m_OffsetMatrix = worldMatrix;
}


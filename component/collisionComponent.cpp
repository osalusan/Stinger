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
	// ���[�J���ϐ��Ƃ��ď�����
	float minOverlap = FLT_MAX;
	XMVECTOR mtvAxis = XMVectorZero();

	const XMVECTOR& Interval = XMVectorSubtract(obb2.Center, obb1.Center);

	// ���̃��X�g
	XMVECTOR axes[15];
	int axisCount = 0;

	// OBB1�̎�
	axes[axisCount++] = obb1.Axis[0];
	axes[axisCount++] = obb1.Axis[1];
	axes[axisCount++] = obb1.Axis[2];

	// OBB2�̎�
	axes[axisCount++] = obb2.Axis[0];
	axes[axisCount++] = obb2.Axis[1];
	axes[axisCount++] = obb2.Axis[2];

	// �e���ł̔���
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

	// �N���X��
	for (int i = 0; i < 3; ++i) 
	{
		XMVECTOR axisA = obb1.Axis[i];

		for (int j = 0; j < 3; ++j) 
		{
			XMVECTOR axisB = obb2.Axis[j];
			XMVECTOR cross = XMVector3Cross(axisA, axisB);
			// ���l�������������疳��
			if (XMVector3LengthSq(cross).m128_f32[0] > 1e-6f) 
			{
				axes[axisCount++] = XMVector3Normalize(cross);
			}
		}
	}

	// MTV�̕����𒲐�
	XMVECTOR direction = XMVectorSubtract(obb1.Center, obb2.Center);
	if (XMVectorGetX(XMVector3Dot(direction, mtvAxis)) < 0) 
	{
		mtvAxis = XMVectorNegate(mtvAxis);
	}

	// �v�Z���ʂ�ۑ�
	m_MinOverlap = minOverlap;
	m_MtvAxis = mtvAxis;

	return true;
}

// �������ɓ��˂��ꂽ���������瓊�ː��������Z�o
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

bool CollisionComponent::CheckHitObject()
{
	// �I�u�W�F�N�g�ꗗ�̎擾
	if (m_GameObjectsCache->empty())
	{
		GameScene* gameScene = SceneManager::GetScene<GameScene>();
		if (gameScene == nullptr) return false;
		ObjectManager* objectManager = gameScene->GetObjectManager();
		if (objectManager == nullptr) return false;

		objectManager->GetAllGameObjects(m_GameObjectsCache);

		if (m_GameObjectsCache->empty()) return false;
	}
	return true;
}

CollisionComponent::~CollisionComponent()
{
#if _DEBUG
	delete m_ModelRenderer;
	m_ModelRenderer = nullptr;

	// �V�F�[�_�[�̍폜
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
	// �����l
	if (m_GameObject != nullptr)
	{
		m_Scale = m_GameObject->GetScale();
	}
#if _DEBUG
	if (m_ModelRenderer == nullptr)
	{
		m_ModelRenderer = new ObjModelRenderer;
	}
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

// �Ăяo�����Ɂu#if _DEBUG�v��
void CollisionComponent::Draw()
{
	// �G���[�h�~�p
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
	Renderer::SetWorldMatrix(world);

	if (m_ModelRenderer != nullptr)
	{
		if (const MODEL* model = ObjModelManager::GetModel(m_Model))
		{
			m_ModelRenderer->DrawCollision(model);
		}
	}
#endif // _DEBUG
}

void CollisionComponent::SetBoxCollisionInfo(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& modelCenterPos, const XMFLOAT3& modelScale, const XMMATRIX& rotateMatrix)
{
	m_Position = pos;
	m_Scale = scale;
	m_ModelCenter = modelCenterPos;
	m_ModelScale = modelScale;
	m_RotationMatrix = rotateMatrix;
}

bool CollisionComponent::CheckHitCollision(const COLLISION_TAG& tag)
{
	m_HitGameObjectsCache.clear();
	m_MinOverlap = FLT_MAX;
	m_MtvAxis = {};

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
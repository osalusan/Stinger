#pragma once
#include "main/main.h"
#include <string>

// 前方宣言
class BoxCollisionComponent;
enum class COLLISION_TAG;

class GameObject {
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };

	// 当たり判定用
	XMFLOAT3 m_ColliPosition = {};
	XMFLOAT3 m_ColliRotation = {};
	XMFLOAT3 m_ColliScale = { 1.0f, 1.0f, 1.0f };

	// Model関係
	XMFLOAT3 m_ModelCenter = {};
	XMFLOAT3 m_ModelScale = {};

	bool m_Enable = false;		// 有効、無効

	// 描画関連
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	BoxCollisionComponent* m_BoxCollision = nullptr;

	// シェーダーを変更したい時にコンストラクタで呼ぶ
	void LoadShader(const std::string& vsFileName, const std::string& psFileName);
	// コリジョンを追加したい時に呼ぶ
	void AddBoxCollisionComponent(const COLLISION_TAG& tag);
	// Updateに書かないとModelCenterなどが取得できない
	void UpdateBoxCollisionInfo();

public:
	virtual ~GameObject();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	BoxCollisionComponent* GetBoxCollision()
	{
		return m_BoxCollision;
	}

	const bool& GetEnable()const
	{
		return m_Enable;
	}
	const XMFLOAT3& GetPos()const
	{
		return m_Position;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}

	const XMFLOAT3& GetModelCenter()const
	{
		return m_ModelCenter;
	}
	const XMFLOAT3& GetModelScale()const
	{
		return m_ModelScale;
	}

	//前方ベクトルの取得
	XMFLOAT3 GetForward()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	//右方向ベクトルの取得
	XMFLOAT3 GetRight()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	//上方向ベクトルの取得
	XMFLOAT3 GetUp()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 up;
		XMStoreFloat3(&up, rotationMatrix.r[1]);
		return up;
	}

	// 回転マトリックスを取得
	XMMATRIX GetRotationMatrix()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		return rotationMatrix;
	}

	//当たり判定の回転マトリックスを取得
	XMMATRIX GetColliRotationMatrix()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_ColliRotation.x, m_ColliRotation.y, m_ColliRotation.z);

		return rotationMatrix;
	}
	// ターゲットへのベクトルを取得
	XMFLOAT3 GetTargetDirection(const XMFLOAT3& targetpos) {
		
		XMVECTOR vector = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetpos),XMLoadFloat3(&m_Position)));

		XMFLOAT3 direction;
		XMStoreFloat3(&direction, vector);
		return direction;
	}

};
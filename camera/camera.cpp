#include "camera.h"
#include "renderer/renderer.h"

void Camera::Draw()
{
	GameObject::Draw();
	// ターゲットとカメラ座標が同じだとプロジェクションマトリクス設定でエラーが起きる
	if (m_Position.x == m_Target.x && m_Position.y == m_Target.y && m_Position.z == m_Target.z) return;

	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	XMFLOAT3 position = m_Position;

	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);
}

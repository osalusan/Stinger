#include "main/main.h"
#include "objModelRenderer.h"

void ObjModelRenderer::Draw(const MODEL* model)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < model->SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(model->SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		if (model->SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &model->SubsetArray[i].Material.Texture);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(model->SubsetArray[i].IndexNum, model->SubsetArray[i].StartIndex, 0);
	}
}


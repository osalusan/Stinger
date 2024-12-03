#include "common.hlsl"
StructuredBuffer<AnimationVertex> inputVertexBuffer : register(t0); // �ϊ��O�̒��_�o�b�t�@

RWStructuredBuffer<VS_IN> outPutVertexBuffer : register(u0); // �ϊ����ꂽ���_���i�[����o�b�t�@

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x; // �X���b�hid�𒸓_id�Ƃ���
    
    if (index >= maxVertexCount)
        return;
    
    AnimationVertex inputVertex = inputVertexBuffer[index];
    
    // �X�L�j���O�v�Z����
    float4x4 skinMatrix;
    skinMatrix = mul(boneMatrix[inputVertex.boneIndex.x], inputVertex.boneWeight.x);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex.y], inputVertex.boneWeight.y);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex.z], inputVertex.boneWeight.z);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex.w], inputVertex.boneWeight.w);

    // �o��
    VS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.pos, skinMatrix);
    
    // �@���p�Ɉړ��������폜
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    skinMatrix[3][0] = 0.0f;
    skinMatrix[3][1] = 0.0f;
    skinMatrix[3][2] = 0.0f;
    skinMatrix[3][3] = 1.0f;

    outputVertex.Normal = mul(inputVertex.normal, skinMatrix);
    
    outputVertex.Diffuse = inputVertex.diffuse;
    outputVertex.TexCoord = inputVertex.uv;

    // �i�[�p�o�b�t�@�ɓ���Ă���
    outPutVertexBuffer[index] = outputVertex;
}
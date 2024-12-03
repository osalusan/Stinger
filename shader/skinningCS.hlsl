#include "common.hlsl"
StructuredBuffer<AnimationVertex> inputVertexBuffer : register(t0); // 変換前の頂点バッファ

RWStructuredBuffer<VS_IN> outPutVertexBuffer : register(u0); // 変換された頂点を格納するバッファ

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.x; // スレッドidを頂点idとする
    
    if (index >= maxVertexCount)
        return;
    
    AnimationVertex inputVertex = inputVertexBuffer[index];
    
    // スキニング計算処理
    float4x4 skinMatrix;
    skinMatrix = mul(boneMatrix[inputVertex.boneIndex.x], inputVertex.boneWeight.x);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex.y], inputVertex.boneWeight.y);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex.z], inputVertex.boneWeight.z);
    skinMatrix += mul(boneMatrix[inputVertex.boneIndex.w], inputVertex.boneWeight.w);

    // 出力
    VS_IN outputVertex;
    outputVertex.Position = mul(inputVertex.pos, skinMatrix);
    
    // 法線用に移動成分を削除
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

    // 格納用バッファに入れていく
    outPutVertexBuffer[index] = outputVertex;
}
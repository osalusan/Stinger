#include "common.hlsl"

VS_OUTPUT main(VS_IN input)
{
    VS_OUTPUT output;

    // スキニング計算処理
    float4x4 skinMatrix;
    skinMatrix = mul(gBoneMatrices[input.BoneIndices.x], input.BoneWeights.x);
    skinMatrix += mul(gBoneMatrices[input.BoneIndices.y], input.BoneWeights.y);
    skinMatrix += mul(gBoneMatrices[input.BoneIndices.z], input.BoneWeights.z);
    skinMatrix += mul(gBoneMatrices[input.BoneIndices.w], input.BoneWeights.w);
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    // 頂点位置の変換
    float4 pos = mul(input.Position, skinMatrix);
    output.Position = mul(pos, wvp);

    // 法線用に移動を消す
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    // 法線の変換
    float3 normal = mul(input.Normal.xyz, (float3x3) skinMatrix);
    output.Normal = normalize(normal);

    output.TexCoord = input.TexCoord;
    
    output.Diffuse = input.Diffuse * Material.Diffuse;

    return output;
}
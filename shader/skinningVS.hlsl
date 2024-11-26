#include "common.hlsl"

VS_OUTPUT main(VS_IN input)
{
    VS_OUTPUT output;

    // �X�L�j���O�}�g���N�X�̌v�Z
    matrix skinMatrix =
        input.BoneWeights.x * gBoneMatrices[input.BoneIndices.x] +
        input.BoneWeights.y * gBoneMatrices[input.BoneIndices.y] +
        input.BoneWeights.z * gBoneMatrices[input.BoneIndices.z] +
        input.BoneWeights.w * gBoneMatrices[input.BoneIndices.w];

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    // ���_�ʒu�̕ϊ�
    float4 pos = mul(input.Position, skinMatrix);
    output.Position = mul(pos, wvp);

    // �@���̕ϊ�
    float3 normal = mul(input.Normal.xyz, (float3x3) skinMatrix);
    output.Normal = normalize(normal);

    output.TexCoord = input.TexCoord;
    
    output.Diffuse = input.Diffuse * Material.Diffuse;

    return output;
}
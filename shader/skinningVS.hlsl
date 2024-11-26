#include "common.hlsl"

VS_OUTPUT main(VS_IN input)
{
    VS_OUTPUT output;

    // �X�L�j���O�}�g���N�X�̌v�Z
    float4x4 boneMatrix0 = gBoneMatrices[input.BoneIndices.x];
    float4x4 boneMatrix1 = gBoneMatrices[input.BoneIndices.y];
    float4x4 boneMatrix2 = gBoneMatrices[input.BoneIndices.z];
    float4x4 boneMatrix3 = gBoneMatrices[input.BoneIndices.w];

    float weight0 = input.BoneWeights.x;
    float weight1 = input.BoneWeights.y;
    float weight2 = input.BoneWeights.z;
    float weight3 = input.BoneWeights.w;

    float4x4 skinMatrix = boneMatrix0 * weight0 + boneMatrix1 * weight1 + boneMatrix2 * weight2 + boneMatrix3 * weight3;

    // ���_�ʒu�̕ϊ��i�X�L�j���O�j
    float4 pos = float4(input.Position.xyz, 1.0f);
    float4 skinnedPos = mul(pos, skinMatrix);

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    output.Position = mul(wvp, skinnedPos);

    // �@���̕ϊ��i�ړ������������j
    float3x3 normalMatrix = (float3x3) skinMatrix;
    output.Normal.xyz = normalize(mul(normalMatrix, input.Normal.xyz));

    // ���̑����̃R�s�[
    output.TexCoord = input.TexCoord;
    output.Diffuse = input.Diffuse * Material.Diffuse;
    
    return output;
}
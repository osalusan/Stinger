#pragma once
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    //頂点の変換
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp); //頂点変換
    
    //頂点法線をワールド行列で変換
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0f);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    Out.Normal = worldNormal;
    
    //頂点のデフューズを出力
    Out.Diffuse = In.Diffuse; //明るさの値を頂点色として出力  
    //テクスチャ座標を出力
    Out.TexCoord = In.TexCoord;
    //ワールド変換した頂点座標を入力
    Out.WorldPosition = mul(In.Position, World);
    
    //タンジェントをワールド変換
    float4 worldTangent, tangent;
    tangent = float4(In.Normal.xyz, 0.0f);
    worldTangent = mul(tangent, World);
    worldTangent = normalize(worldTangent);
    Out.Tangent = worldTangent;
    
    //バイノーマル
    float4 worldBinormal;
    worldBinormal = 0;
    worldBinormal.xyz = cross(worldTangent.xyz, worldNormal.xyz);
    worldBinormal = normalize(worldBinormal);
    Out.Binormal = worldBinormal;

}
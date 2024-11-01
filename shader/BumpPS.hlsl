
#include "common.hlsl"

Texture2D g_Texture : register(t0); //0番テクスチャの情報
Texture2D g_TextureNormal : register(t1); //1番テクスチャの情報
SamplerState g_SamplerState : register(s0); //サンプラー０番

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
//物体から光源へのベクトル
    float4 lv = In.WorldPosition - Light.Position;
    //物体と光源の距離
    float4 ld = length(lv);
    //ベクトルの正規化
    lv = normalize(lv);
    
    //光の減衰
    float ofs = 1.0f - (1.0f / Light.PointLightParam) * ld;
    //0未満は０にする
    ofs = max(0, ofs);

    //法線マップ
    float4 normalMap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    //RGB=>ベクトルに戻す
    normalMap = (normalMap * 2.0f) - 1.0f;
    //法線として変数へ格納する
    float4 normal;
    normal.x = -normalMap.x;
    normal.y = normalMap.y;
    normal.z = normalMap.y;
    normal.w = 0.0f;
    
    //ピクセルの法線を正規化
    normal = normalize(normal);
    //光源計算
    float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
    //減衰率を乗算
    light *= ofs;
    
    //テクスチャのピクセル処理
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //色に明るさを乗算
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    //αの処理
    outDiffuse.a *= In.Diffuse.a;
    //環境光の加算
    outDiffuse.rgb += Light.Ambient.rgb;
    
    //カメラからピクセルに向かうベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    //正規化
    eyev = normalize(eyev);
    
    //ハーフベクトルの計算
    float3 halfv = eyev + lv.xyz;
    //正規化
    halfv = normalize(halfv);
    
    //スペキュラーの計算
    float specular = -dot(halfv, normal.xyz); //鏡面反射の計算
    specular = saturate(specular); 
    specular = pow(specular, 10);
    
    outDiffuse.rgb += specular; //スペキュラ値をデフューズとして足す

}
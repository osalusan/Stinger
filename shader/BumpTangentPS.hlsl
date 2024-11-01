
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
    float ofs = 1.0f - lv;
    //0未満は０にする
    ofs = max(0.0f, ofs);

    //法線マップのデータを取得
    float4 localnormal = g_TextureNormal.Sample(g_SamplerState, In.TexCoord);
    //RGB値をベクトルへ戻す
    localnormal = (localnormal * 2.0f) - 1.0f;
    localnormal.w = 1.0f;
    //正規化
    localnormal = normalize(localnormal);
    
    //タンジェント
    float4 tangent = normalize(-In.Tangent);
    //バイノーマル
    float4 binomal = normalize(In.Binormal);
    //ノーマル
    float4 normal = normalize(In.Normal);
    
    //接空間を表す行列を作成する
//    matrix mat = matrix(
//    tangent,//x軸
//    binomal,//y軸
//    normal,//z軸
//    float4(0, 0, 0, 0)//平行移動要素はダミー
    
//);
    matrix mat = matrix(
    float4(-1, 0, 0, 0), //x軸
    float4(0, 0, 1, 0), //y軸
    float4(0, 1, 0, 0), //z軸
    float4(0, 0, 0, 0) //平行移動要素はダミー
    
);
       
    //法線マップ内の法線を接空間へ変換する
    normal = mul(localnormal, mat);
    normal = normalize(normal);
    
    //光源計算
    float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
    //減衰率を乗算
    light *= ofs;
    
    //テクスチャのピクセルを取得
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //頂点色を合成
    outDiffuse.rgb *= In.Diffuse.rgb;
    //明るさの合成
    outDiffuse.rgb *= light;
    
    //αの処理
    outDiffuse.a *= In.Diffuse.a;
    //環境光の加算
    outDiffuse.rgb += Light.Ambient.rgb;
    
    //カメラからピクセルへ向かうベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    //正規化
    eyev = normalize(eyev);
    //ハーフベクトル
    float3 halfv = eyev + lv.xyz;
    //正規化
    halfv = normalize(halfv);
    
    
    //スペキュラーの計算
    float specular = -dot(halfv, normal.xyz);
    specular = saturate(specular);
    specular = pow(specular, 10);
     //スペキュラーの加算
    outDiffuse.rgb += (specular * ofs);
    
    

}
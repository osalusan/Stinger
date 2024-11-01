
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
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
    
    Out.WorldPosition = mul(In.Position, World);

}
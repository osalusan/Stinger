//このファイルは他のシェーダーファイルへインクルードされます
// 各種マトリクスやベクトルを受け取る変数を用意
cbuffer WorldBuffer : register(b0) //定数バッファ 0 番
{
    matrix World;
}
cbuffer ViewBuffer : register(b1) //定数バッファ 1 番
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2) //定数バッファ 2 番
{
    matrix Projection;
}

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

//頂点シェーダーへ入力されるデータを構造体の形で表現
//これは頂点バッファの内容そのもの
struct VS_IN
{
    float4 Position : POSITION0; //セマンティクスは頂点レイアウトに合わせる
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    float4 Tangent : TANGENT;
}; //表示するポリゴンのレイアウトと同じ
//ピクセルシェーダーへ入力されるデータを構造体の形で表現
struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    float4 Tangent : TANGENT0;
    float4 Binormal : BINORMAL0;
};
//ライト構造体の定義　C言語側のLIGHT構造体と同じ内容
struct LIGHT
{
    bool Enable;
    bool3 Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
    float4 SkyColor; //天球色
    float4 GroundColor; //地面色
    float4 GroundNormal; //地面法線
    
    float4 Position; //光の座標
    float4 PointLightParam;
    float4 Angle; //スポットライトのコーン
};
cbuffer LightBuffer : register(b4) //コンスタントバッファ４番で受け取る
{
    LIGHT Light; //ライト型構造体として利用する
}

cbuffer CameraBuffer : register(b5) //バッファの５番とする
{
    float4 CameraPosition; //カメラ座標を受け取る変数
}

//---------------------------------
//汎用パラメータ受け取り用
//---------------------------------
cbuffer ParameterBuffer : register(b6)
{
    float4 Parameter; //C言語側から受け取る
    
   
}

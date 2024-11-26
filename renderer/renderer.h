#pragma once
#include "main/main.h"

struct VERTEX_3D
{
	XMFLOAT3 Position = {};
	XMFLOAT3 Normal = {};
	XMFLOAT4 Diffuse = {};
	XMFLOAT2 TexCoord = {};

	UINT BoneIndices[4]; // ボーンインデックス
	float BoneWeights[4]; // ボーンウェイト
};

struct MATERIAL
{
	XMFLOAT4	Ambient = {};
	XMFLOAT4	Diffuse = {};
	XMFLOAT4	Specular = {};
	XMFLOAT4	Emission = {};
	float		Shininess = 0.0f;
	BOOL		TextureEnable = { 0 };
	float		Dummy[2] = { 0.0f };
};

struct LIGHT
{
	BOOL		Enable = { 0 };
	BOOL		Dummy[3] = { 0 };//16byte境界用
	XMFLOAT4	Direction = {};
	XMFLOAT4	Diffuse = {};
	XMFLOAT4	Ambient = {};
	XMFLOAT4 SkyColor = {};//空の色
	XMFLOAT4 GroundColor = {};//地面の色
	XMFLOAT4 GroundNormal = {};//地面の法線

	XMFLOAT4 Position = {};//光の座標
	XMFLOAT4 PointLightParam = {};
	XMFLOAT4 Angle = {};//スポットライトの範囲
};



class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_BoneBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateAdd; //加算合成ブレンドステート
	static ID3D11BlendState*		m_BlendStateATC;

	static ID3D11RasterizerState* m_WireframeRasterState; // ワイヤーフレーム用

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(const bool& enable);
	static void SetATCEnable(const bool& enable);
	static void SetWireframeEnable(const bool& enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(const XMMATRIX& WorldMatrix);
	static void SetViewMatrix(const XMMATRIX& ViewMatrix);
	static void SetProjectionMatrix(const XMMATRIX& ProjectionMatrix);
	static void SetMaterial(const MATERIAL& Material);
	static void SetBoneMatrix(const std::vector<XMFLOAT4X4>& Matrix);
	static void SetLight(const LIGHT& Light);
	static void SetBlendAddEnable(const bool& enable);

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
};

#pragma once
#include <vector>
#include "main/main.h"

// 前方宣言
class UDPServer;
class UDPClient;
// ネットワーク接続をを管理するクラス
class MultiPlayManager final
{
private:
	static UDPServer* m_UDPServer;
	static UDPClient* m_UDPClient;
	static int m_PlayerCnt;

	// クライアントとサーバーを同時に作れないようにする
	static void CreateServer();
	static void RemoveServer();
	static void CreateClient();
	static void RemoveClient();
public:
	static void Init();
	static void Uninit();
	static void Update(const float& deltaTime);
};
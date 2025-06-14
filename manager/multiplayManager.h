#pragma once
#include <vector>
#include "main/main.h"

// �O���錾
class UDPServer;
class UDPClient;
// �l�b�g���[�N�ڑ������Ǘ�����N���X
class MultiPlayManager final
{
private:
	static UDPServer* m_UDPServer;
	static UDPClient* m_UDPClient;
	static int m_PlayerCnt;

	// �N���C�A���g�ƃT�[�o�[�𓯎��ɍ��Ȃ��悤�ɂ���
	static void CreateServer();
	static void RemoveServer();
	static void CreateClient();
	static void RemoveClient();
public:
	static void Init();
	static void Uninit();
	static void Update(const float& deltaTime);
};
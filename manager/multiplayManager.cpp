#include "multiplayManager.h"
#include "online/udpClient.h"
#include "online/udpServer.h"
#include "manager/inputManager.h"

#define PORT (8888)
const std::string serverIP = "192.168.11.32";

int MultiPlayManager::m_PlayerCnt = 0;
UDPServer* MultiPlayManager::m_UDPServer = nullptr;
UDPClient* MultiPlayManager::m_UDPClient = nullptr;

void MultiPlayManager::CreateServer()
{
	if (m_UDPServer == nullptr)
	{
		m_UDPServer = new UDPServer;
	}
}

void MultiPlayManager::RemoveServer()
{
	if (m_UDPServer != nullptr) 
	{
		m_UDPServer->Stop(); 
		delete m_UDPServer; 
		m_UDPServer = nullptr;
	}
}

void MultiPlayManager::CreateClient()
{
	if (m_UDPClient == nullptr)
	{
		 m_UDPClient = new UDPClient(serverIP, PORT);
	}
}

void MultiPlayManager::RemoveClient()
{
	if (m_UDPClient != nullptr)
	{
		delete m_UDPServer;
		m_UDPServer = nullptr;
	}
}

void MultiPlayManager::Init()
{

}

void MultiPlayManager::Uninit()
{

}

void MultiPlayManager::Update(const float& deltaTime)
{

}


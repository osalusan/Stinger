#pragma once
#include "main/main.h"
#include <thread>
#include <atomic>
#include <unordered_map>
#include <string>
#include "playerState/playerStateMachine.h"
#include <winsock2.h>
#include <ws2tcpip.h> 

class UDPServer 
{
private:
    // サーバーループ
    void ServerLoop();

    SOCKET serverSocket;                                    // サーバーソケット
    struct sockaddr_in serverAddr;                          // サーバーのアドレス情報
    std::atomic<bool> running;                              // サーバーの実行状態
    std::thread serverThread;                               // サーバースレッド
    std::unordered_map<std::string, sockaddr_in> clients;   // クライアント情報の管理

    OtherPlayerData _AllPlayerData[9] = {};
public:
	UDPServer();
	~UDPServer();
	void Start();
	void Stop();
    //バイトストリームとして送信
    void SendInt_Check(const sockaddr_in& client, int value);
    void SendFloat(const sockaddr_in& client, float value);
    void SendXMFLOAT3(const sockaddr_in& client, const XMFLOAT3& value);
    void SendOtherPlayerData(const sockaddr_in& client, const OtherPlayerData &value);

    void ReturnOtherPlayers(sockaddr_in addsocket,int number);
    void ReturnHost(sockaddr_in addsocket);

    //他のプレイヤーの入れ物を作る
    void ReflectOtherPlayer();
};
#pragma once
#include "main/main.h"
#include <iostream>
#include "playerState/playerStateMachine.h"
#include <winsock2.h>
#include <ws2tcpip.h> 

class UDPClient 
{
private:
    SOCKET _Sock = {};                 // ソケット
    struct sockaddr_in _Server = {};   // サーバーのアドレス情報
    int _ServerLen = {};               // サーバーアドレスの長さ

    OtherPlayerData _MyPlayerData = {};
    OtherPlayerData _OtherPlayerData[MAX_PLAYER] = {};
public:
    bool _Success = false;
    //IPアドレスとポートを指定して初期化
    UDPClient(const std::string& serverIP, int port);
    ~UDPClient();

    void Success();
    void Check();

    //バイトストリームとして送信
    bool SendDisconnect();
    bool SendFloat(const float value);
    bool SendMyPlayerData(const XMFLOAT3 value, const PLAYER_STATE state);

    //バイトストリームとして受信
    int ReceiveInt_Check();
    void ReflectOtherPlayer();
};

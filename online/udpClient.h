#pragma once
#include "main/main.h"
#include <iostream>
#include "playerState/playerStateMachine.h"
#include <winsock2.h>
#include <ws2tcpip.h> 

class UDPClient 
{
private:
    SOCKET _Sock = {};                 // �\�P�b�g
    struct sockaddr_in _Server = {};   // �T�[�o�[�̃A�h���X���
    int _ServerLen = {};               // �T�[�o�[�A�h���X�̒���

    OtherPlayerData _MyPlayerData = {};
    OtherPlayerData _OtherPlayerData[MAX_PLAYER] = {};
public:
    bool _Success = false;
    //IP�A�h���X�ƃ|�[�g���w�肵�ď�����
    UDPClient(const std::string& serverIP, int port);
    ~UDPClient();

    void Success();
    void Check();

    //�o�C�g�X�g���[���Ƃ��đ��M
    bool SendDisconnect();
    bool SendFloat(const float value);
    bool SendMyPlayerData(const XMFLOAT3 value, const PLAYER_STATE state);

    //�o�C�g�X�g���[���Ƃ��Ď�M
    int ReceiveInt_Check();
    void ReflectOtherPlayer();
};

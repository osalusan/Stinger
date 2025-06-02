#include "udpClient.h"
#include "manager/sceneManager.h"
#include "character/otherplayer.h"

#define BUFLEN 1024 

const std::string coment = "Hello";

UDPClient::UDPClient(const std::string& serverIP, int port) 
{
    WSADATA wsa;
    _ServerLen = sizeof(_Server);

    //Winsockの初期化
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Winsockの初期化に失敗しました。エラーコード: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    //ソケットの作成
    if ((_Sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        std::cerr << "ソケット作成に失敗しました。エラーコード: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    //アドレスファミリ、IPアドレス、ポート番号を設定
    _Server.sin_family = AF_INET;
    _Server.sin_port = htons(port);
    inet_pton(AF_INET, serverIP.c_str(), &(_Server.sin_addr));
}

UDPClient::~UDPClient() 
{
    SendDisconnect();
    closesocket(_Sock); // ソケットを閉じる
    WSACleanup();      // Winsockのクリーンアップ
}

void UDPClient::Success()
{
    char buf[BUFLEN];
    int recvLen;



    if ((recvLen = recvfrom(_Sock, buf, BUFLEN - 1, 0, (struct sockaddr*)&_Server, &_ServerLen)) != SOCKET_ERROR) 
    {

        buf[recvLen] = '\0'; // 受信データの終端を設定

        if (recvLen == sizeof(OtherPlayerData))
        {
            OtherPlayerData receivedValue;
            std::memcpy(&receivedValue, buf, sizeof(OtherPlayerData));
            //他のプレイヤーの情報が初めて来たら
            if (_OtherPlayerData[receivedValue.playerID].playerID == 0)
            {
                _OtherPlayerData[receivedValue.playerID].playerID = receivedValue.playerID;
                // Manager::AddOtherClientPlayers(receivedValue.playerID);
            }
            _OtherPlayerData[receivedValue.playerID] = receivedValue;
            ReflectOtherPlayer();
        }
    }
}

void UDPClient::Check()
{
    if (SendFloat(1.0f)) {
        u_long blockmode = 1; //非ブロッキングモード
        ioctlsocket(_Sock, FIONBIO, &blockmode);
        //一定時間反応が無かったら接続待機を解除
        if (ReceiveInt_Check() != 0) {
            _Success = true;
            // Manager::ResetConnectionCound();
        }
    }
}

bool UDPClient::SendDisconnect()
{
    bool value = true;

    char buf[sizeof(bool)];
    std::memcpy(buf, &value, sizeof(bool));

    if (sendto(_Sock, buf, sizeof(bool), 0, (struct sockaddr*)&_Server, _ServerLen) == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool UDPClient::SendFloat(const float value)
{
    char buf[sizeof(float)];
    std::memcpy(buf, &value, sizeof(float));

    if (sendto(_Sock, buf, sizeof(float), 0, (struct sockaddr*)&_Server, _ServerLen) == SOCKET_ERROR) {
        return false;
    }
    return true;
}

//プレイヤーのデータを送る
bool UDPClient::SendMyPlayerData(const XMFLOAT3 value,const PLAYER_STATE state)
{//今はプレイヤーで呼んでる/後で変える
    _MyPlayerData.pos = value;
    _MyPlayerData.playerState = state;

    char buf[sizeof(OtherPlayerData)];
    std::memcpy(buf, &_MyPlayerData, sizeof(OtherPlayerData));

    if (sendto(_Sock, buf, sizeof(OtherPlayerData), 0, (struct sockaddr*)&_Server, _ServerLen) == SOCKET_ERROR) {
        return false;
    }
    return true;
}

int UDPClient::ReceiveInt_Check()
{
    char buf[sizeof(int)];
    int recvLen;

    if ((recvLen = recvfrom(_Sock, buf, sizeof(int), 0, (struct sockaddr*)&_Server, &_ServerLen)) == SOCKET_ERROR) {
        // Manager::AddConnectionCount();
        return 0;
    }

    int receivedValue;
    std::memcpy(&receivedValue, buf, sizeof(int));
    _MyPlayerData.playerID = receivedValue;
    return receivedValue;
}

//サーバーから受け取った他のプレイヤーのデータを設定
void UDPClient::ReflectOtherPlayer()
{
    //for (auto otherplayer : Manager::GetScene()->GetGameObjects<OtherPlayer>())
    //{
    //    otherplayer->m_TransForm->m_Position = _OtherPlayerData[otherplayer->playerID].pos;
    //    otherplayer->_state = _OtherPlayerData[otherplayer->playerID].playerState;
    //}
}

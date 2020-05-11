#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>

// MAX_CONNECT 
#define MAX_LISTEN 10000
#define MAX_CONNECT 16383
#define MAX_CONNECT_HEX 0x3FFF

// epoll user data
class EpollUserData
{
public:
    enum EDataType
    {
        et_ctrlor = 0,          // 控制指令
        et_acceptor,            // 监听处理
        et_connector,           // 连接处理
    };
    EpollUserData() = delete;
    EpollUserData(EDataType etype){m_etype = etype;};
    virtual ~EpollUserData() = default;

    EDataType GetType(){return m_etype;};
    virtual void Handle(int nEvent) = 0;
    virtual int GetFd() = 0;

private:
    EDataType m_etype;
};

typedef std::shared_ptr<EpollUserData> EpollUserDataPtr;

// 网络事件上层回调接口
class INetEvent
{
public:
    INetEvent() = default;
    virtual ~INetEvent() = default;
    virtual OnConnected(int index) = 0;
}

// 网络连接
class IConnector
{
public:
    IConnector() = default;
    virtual IConnector() = default;
    virtual Send() = 0;
}

#endif // __COMMON_H__
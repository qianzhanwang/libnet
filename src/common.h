#ifndef __COMMON_H__
#define __COMMON_H__

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

class INetEvent
{
public:
    INetEvent() = default;
    virtual ~INetEvent() = default;
    virtual OnAccept(int fd) = 0;
}

#endif // __COMMON_H__
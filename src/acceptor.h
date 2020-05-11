#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__
#include <functional>
#include "common.h"

class Acceptor : public EpollUserData
{
public:
    typedef std::function<void(int, INetEvent* net)> FunOnAccept;

    Acceptor(std::uint32_t index, FunOnAccept cb, INetEvent* net);
    virtual ~Acceptor();
    virtual void Handle(int nEvent);
    virtual int GetFd(){return m_fd;};

    bool Start(int port, int max);
    bool Close();

private:
    std::uint32_t m_index; // 唯一ID
    int m_fd;
    INetEvent* m_iNetOwner; // 上层逻辑接口
    FunOnAccept m_cb;  // 新连接到来回调
};



#endif //__ACCEPTER_H__ 
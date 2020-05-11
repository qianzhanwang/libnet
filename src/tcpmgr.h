#ifndef __TCPMGR_H__
#define __TCPMGR_H__
#include "common.h"
#include <cstdint>

/*
* tcp manager 
*/
class TcpMgr
{
public: 
    struct SNetNode
    {
        SNetNode()
        {
            m_spNode.reset();
            m_bUsing = false;
        }

        EpollUserDataPtr    m_spNode;
        bool                m_bUsing;
    };

    TcpMgr();
    ~TcpMgr() = default;
    TcpMgr(const TcpMgr& ) = delete;
    TcpMgr& operator = (const TcpMgr& ) = delete;

    bool Listen(int port, int max_listen, INetEvent* net);

private:
    std::uint32_t NewID();
    std::uint32_t GetIDSize(){return m_nIdUsed;};
    void OnAccept(int nFd, INetEvent* pOwner);

private:
    std::uint32_t   m_nIdUsed; 
    std::uint32_t   m_vctIdMagic[MAX_CONNECT];
    SNetNode        m_vctNetObj[MAX_CONNECT];

};

#endif //__TCPMGR_H__
#include "tcpmgr.h"
#include <atomic>
#include "acceptor.h"

TcpMgr::TcpMgr()
{
    memset(m_vctIdMagic, 0, sizeof(m_vctIdMagic));
    m_nIdUsed = 0;
}

std::uint32_t TcpMgr::NewID()
{
    // 仅保证累加原子性，循环一遍后再次获取目标索引会有概率竞争，设置MAX_CONNECT为最大监听数1.5倍，避免目标索引上节点的潜在竞争
    static std::atomic_uint32_t last_index = 0; 
    do
    {
        std::uint32_t index = (last_index++) & MAX_CONNECT_HEX;
        if (!m_vctNetObj[index].m_bUsing)
        {
            ++m_nIdUsed;
            m_vctNetObj[index].m_bUsing = true;
            return index << 32 | (++m_vctIdMagic[index]);
        }
    } while (true);
}

bool TcpMgr::Listen(int port, int max_listen, INetEvent* net)
{
    std::uint32_t id = NewID();
    std::function<void(int)> cb = std::bind(&TcpMgr::OnAccept, this, std::placeholders::_1, std::placeholders::_2);
    auto pNet = std::shared_ptr<EpollUserData>(new Acceptor(id, cb, net)); 
    m_vctNetObj[id>>32].m_spNode = pNet ;
    Acceptor* pAcc = dynamic_cast<Acceptor>(pNet.get());
    if (pAcc)
    {
        pAcc->Start(port, max_listen);
    }
    return true;
}

void TcpMgr::OnAccept(int nFd, INetEvent* pOwner);
{

}
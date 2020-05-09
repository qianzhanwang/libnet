#include "epollctrl.h"
#include <unistd.h>

EpollCtrl::EpollCtrl ()
    :EpollUserData(EpollUserData::et_ctrlor)
{
    m_nfds[0] = 0;
    m_nfds[1] = 0;
}

EpollCtrl::~EpollCtrl ()
{

}

bool EpollCtrl::Init(std::functioal<void()> cb)
{
    m_cbHandleCmd = cb;
    if (pipe(m_nfds) < 0)
    {
        return false;
    }
    
    return true;
}

bool EpollCtrl::Release()
{
    if (m_nfds[0] > 0)
    {
        close(m_nfds[0]);
    }

    if (m_nfds[1] > 0)
    {
        close(m_nfds[1]);
    }
    m_nfds[0] = 0;
    m_nfds[1] = 0;
    return true;
}

void EpollCtrl::Handle(int nEvent) 
{
    bool bRead = (nEvent & EPOLLIN);
    if (!bRead)
    {
        // log
        return;
    }

    int len = 0;
    char buf[4*1024];
    do
    {
        int ret = read(m_nfds[0], buf, sizeof(buf)); 
        if (ret <= 0)
        {
            break;
        }
        len += ret;
    } while (true);
     
    if (m_cbHandleCmd)
    {
        m_cbHandleCmd(len);
    }
}

int EpollCtrl::OnPush()
{
    do
    {
        int ret = write(m_nfds[1], "C", 1);
        if (ret < 0 && (errno == EINTR))
        {
            // 中断重试
            continue;
        }
        return ret;
    } while (true);
}
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
    if (m_cbHandleCmd)
    {
        m_cbHandleCmd(1);
    }
}
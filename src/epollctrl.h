#ifndef __EPOLLCTL_H__
#define __EPOLLCTL_H__
#include "common.h"
#include <functional>

class EpollCtrl : public EpollUserData
{
public:
    EpollCtrl ();
    virtual ~EpollCtrl ();

    bool Init(std::functioal<void(int)> cb);
    bool Release();
    int ReadFd(){return m_nfds[0];};
    int WriteFd(){return m_nfds[1];};
    virtual void Handle(int nEvent) ;

private:
    int m_nfds[2];
    std::functioal<void(int)> m_cbHandleCmd;
};

#endif //__EPOLLCTL_H__
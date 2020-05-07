#ifndef __EPOLLCTL_H__
#define __EPOLLCTL_H__
#include "common.h"

class EpollCtrl : public EpollUserData
{
public:
    EpollCtrl ();
    virtual ~EpollCtrl ();
private:
    /* data */
};

#endif //__EPOLLCTL_H__
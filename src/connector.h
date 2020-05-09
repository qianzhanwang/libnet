#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "common.h"

class Connector : public EpollUserData
{
public:
    Connector();
    virtual ~Connector();
    virtual void Handle(int nEvent);
    virtual int GetFd() {return m_fd;};

private:
    int m_fd;
};



#endif //__CONNECTOR_H__ 
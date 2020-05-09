#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__
#include "common.h"

class Acceptor : public EpollUserData
{
public:
    Acceptor();
    virtual ~Acceptor();
    virtual void Handle(int nEvent);
    virtual int GetFd(){return m_fd;};

    bool Start(int port, int max);
    bool Close();

private:
    int m_fd;
};



#endif //__ACCEPTER_H__ 
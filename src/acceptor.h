#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__
#include "common.h"

class Acceptor : public EpollUserData
{
public:
    Acceptor();
    virtual ~Acceptor();
    virtual void Handle(int nEvent);

private:
    /* data */
};



#endif //__ACCEPTER_H__ 
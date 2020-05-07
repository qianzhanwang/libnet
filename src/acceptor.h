#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__
#include "common.h"

class Acceptor : public EpollUserData
{
private:
    /* data */
public:
    Acceptor();
    virtual ~Acceptor();
};



#endif //__ACCEPTER_H__ 
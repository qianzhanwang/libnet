#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "common.h"

class Connector : public EpollUserData
{
public:
    Connector();
    virtual ~Connector();
    virtual void Handle(int nEvent);

private:
    /* data */
};



#endif //__CONNECTOR_H__ 
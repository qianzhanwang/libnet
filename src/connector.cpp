#include "connector.h"

Connector::Connector()
    : EpollUserData(EpollUserData::et_connector)
    , m_fd(0)
{

}

Connector::~Connector()
{

}

void Connector::Handle(int nEvent)
{

}
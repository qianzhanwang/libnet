#include "acceptor.h"
#include <sys/socket.h>

Acceptor::Acceptor(std::uint32_t index,FunOnAccept cb, INetEvent* net)
    : EpollUserData(EpollUserData::et_acceptor)
    , m_fd(0)
    , m_cb(cb)
    , m_iNetOwner(net)
    , m_index(index)
{
    assert(net != nullptr);
    assert(index > 0);
}

Acceptor::~Acceptor()
{
}

void Acceptor::Handle(int nEvent)
{
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    memset(&cli_addr, 0, sizeof(cli_addr));

    do
    {
        int fd = accept(m_fd, (struct sockaddr *) &cli_addr, &clilen);
        if (fd < 0)
        {
            int err = errno;  
            if (err & EAGAIN)
            {
                // nothing
                return;
            }

            if (err & EINTER)
            {
                // sigle inter, try again
                continue;
            }
            
            // error
            return; 
        }

        if (m_cb)
        {
            m_cb(fd, m_iNetOwner);
        }
        return;
    } while (true);
    
}
 
bool Acceptor::Start(int port, int max)
{
    if (port <= 0 || max <=0 || max > SOMAXCONN)
    {
        return false;
    }

    do
    {
        int m_fd = socket(AF_INET, SOCK_STREAM, 0))
        if (m_fd < 0)
        {
            return false;
        }

        // time_wait状态下监听， IP别名
        int reuse = 1;
        if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR,(const void *)&reuse, sizeof(int)) < 0)
        {
            break;
        }

        // 非阻塞
        if (fcntl(fd, F_SETFL, (fcntl(fd,F_GETFL)，0) | O_NONBLOCK) < 0)
        {
            break;
        } 

        // 心跳，发现不可到达网络、不可到达主机
        int keepalive = 1;
        if (setsockopt(rs, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive )) < 0)
        {
            break;
        }

        // 设置发送和接受缓冲区
        int send_buff =  64*1024;
        int recv_buff =  64*1024;
        if (setsockopt(rs, SOL_SOCKET, SO_SNDBUF, (void *)&send_buff, sizeof(send_buff)) < 0)
        {
            break;
        }

        if (setsockopt(rs, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buff, sizeof(recv_buff)) < 0)
        {
            break;
        }

        // 关闭延迟发送和ACK合并
        int nodelay = 1;
        if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)))
        {
            break;
        }

        // time_wait 对性能影响不大，并不消耗过多CPU和内存，客户端支持500连接/s 无需SO_LINGER

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
        if(bind(m_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
        {
            break;
        }

        if (listen(m_fd, max) < 0)
        {
            break;
        }
        return true;

    } while (false);

    Close();
    return false;
}

bool Acceptor::Close()
{
    if (m_fd > 0)
    {
        close(m_fd);
        m_fd = -1;
    }
    return true;
}
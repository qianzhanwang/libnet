#include "epollthread.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

EpollThrad::EpollThrad()
{
    m_bRunning = false;
    m_nEpollfd = -1;
}

EpollThrad::~EpollThrad()
{
}

bool EpollThrad::Init(int nMax)
{
    if (nMax <= 0)
    {
        return false;
    }

    // epoll
    m_nEpollfd = epoll_create(nMax);
    if (m_nEpollfd < 0)
    {
        return false;
    }

    // pipe control
    if (m_ctrl.Init(std::bind(&EpollThrad::HandleCmd, this, std::placeholders::_1)) == false)
    {
        return false;
    }
    SetNoBlock(m_ctrl.ReadFd());
    SetNoBlock(m_ctrl.WriteFd());

    epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd = m_ctrl.ReadFd();
    ev.data.ptr = &m_ctrl;
    if (epoll_ctl(m_nEpollfd, EPOLL_CTL_ADD, ev.data.fd, &ev) != 0)
    {
        return false;
    }

    m_bRunning = true;
    m_thread = std::thread(&EpollThrad::Run, this);
    return true;
}

bool EpollThrad::Release()
{
    m_bRunning = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }

    epoll_ctl(m_nEpollfd, EPOLL_CTL_DEL, m_ctrl.ReadFd(), 0);
    close(m_nEpollfd);
    m_nEpollfd = -1;
    return true;
}

void EpollThrad::Run()
{
    epoll_event ev[128];
    do
    {
        if (m_bRunning)
        {
            int count = epoll_wait(m_nEpollfd, ev, 128, 1000);
            if (count < 0) 
            {
                if (errno != EINTR) 
                {
                    break;
                }
                continue; 
            }

            for (size_t i = 0; i < count; i++)
            {
                EpollUserData* pData = (EpollUserData*)ev[i].data.ptr;
                pData->Handle(ev[i].event);
            }
        }
    } while (true);
}

void EpollThrad::SetNoBlock(int fd)
{
    fcntl(fd, F_SETFL, (fcntl(fd,F_GETFL)，0) | O_NONBLOCK);
}

void EpollThrad::HandleCmd(int num)
{
    std::vector<EpollCmdPtr> tmp;
    std::unique_lock<std::mutex> ul(m_mtCmds);
    for (size_t i = 0; i < num; i++)
    {
        if (m_cmds.empty()) 
        {
            break;
        }

        tmp.push_back(m_cmds.pop());
    }
    ul.unlock();

    for (size_t i = 0; i < tmp.size(); i++)
    {
        EpollCmdPtr& cmd = tmp[i];
        int fd = cmd->m_pData->GetFd();
        cmd->m_ev.data.ptr = (void*)cmd->m_pData.get();

        switch (cmd->m_etype )
        {
        case EpollCmd::et_listen_add: 
        case EpollCmd::et_connect_add:
        {
            int ret = epoll_ctl(m_nEpollfd,EPOLL_CTL_ADD,fd,&cmd->m_ev);
            if (ret == -1)
            {
                // errno
                assert(false);
            }
        } 
        break;
        
        case EpollCmd::et_listen_sub:
        case EpollCmd::et_connect_sub:
        {
            int ret = epoll_ctl(m_nEpollfd,EPOLL_CTL_DEL,fd,&cmd->m_ev);
            if (ret == -1 )
            {
                // errno
                assert(false);
            }
        } 
        break;

        default:
            assert(false);
            break;
        }
    }
}

bool EpollThrad::AddCmd(EpollCmd& cmd)
{
    std::unique_lock<std::mutex> ul(m_mtCmds);
    m_cmds.push(cmd);
    ul.unlock();

    m_ctrl.OnPush();
    return true;
}
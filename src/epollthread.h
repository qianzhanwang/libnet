#ifndef __EPOLLTHREAD_H__
#define __EPOLLTHREAD_H__
#include <thread>
#include <queue>
#include <mutex>
#include "epollctrl.h"
#include "common.h"

struct EpollCmd
{
    enum ECmdType
    {
        et_listen_add = 0,
        et_listen_sub,
        et_connect_add,
        et_connect_sub,
    };
    ECmdType m_etype;
    EpollUserDataPtr m_pData;
    epoll_event m_ev;
};
typedef std::shared_ptr<EpollCmd> EpollCmdPtr;

class EpollThrad
{
public:
    EpollThrad();
    ~EpollThrad();
    EpollThrad(const EpollThrad&) = delete;
    EpollThrad& operator = (const EpollThrad&) = delete;

    bool Init(int nMax);
    bool Release();
    bool AddCmd(EpollCmd& cmd);
    bool AddListen();

private:
    void Run();
    void SetNoBlock(int fd);
    void HandleCmd(int num);

private:
    bool m_bRunning;
    std::thread m_thread;
    int m_nEpollfd;
    EpollCtrl m_ctrl;               // pipe
    std::mutex m_mtCmds;            // 保证一读一写用kfifo替代
    std::queue<EpollCmdPtr> m_cmds; // 指令控制
};

#endif //__EPOLLTHREAD_H__ 
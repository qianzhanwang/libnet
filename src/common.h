#ifndef __COMMON_H__
#define __COMMON_H__

// epoll user data
class EpollUserData
{
public:
    enum EDataType
    {
        et_ctrlor = 0,          // 控制指令
        et_acceptor,            // 监听处理
        et_connector,           // 连接处理
    }
    EpollUserData() = delete;
    EpollUserData(EDataType etype){m_etype = etype;};
    virtual ~EpollUserData() = default;

    EDataType GetType(){return m_etype;};

private:
    EDataType m_etype;
};

#endif // __COMMON_H__
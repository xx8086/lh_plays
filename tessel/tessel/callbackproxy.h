//
//  callbackproxy.h
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/24.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef callbackproxy_h
#define callbackproxy_h

//Tobject:调用对象的类型，Tparam回调函数参数的类型
template<typename Tobject, typename Tparam>
class CCallbackProxy
{
    typedef void (Tobject::*CbFun)(Tparam*);
    
public:
    void Set(Tobject *pInstance, CbFun pFun);
    bool Exec(Tparam* pParam);
    
private:
    CbFun       pCbFun;     //回调函数指针
    Tobject*    m_pInstance;    //调用对象
};

//设置调用对象及其回调函数
template<typename Tobject, typename Tparam>
void CCallbackProxy<Tobject, Tparam>::Set(Tobject *pInstance , CbFun pFun)
{
    m_pInstance = pInstance;
    pCbFun = pFun;
};

//调用回调函数
template<typename Tobject, typename Tparam>
bool CCallbackProxy<Tobject, Tparam>::Exec(Tparam* pParam)
{
    (m_pInstance->*pCbFun)(pParam);
    return true;
}


#endif /* callbackproxy_h */

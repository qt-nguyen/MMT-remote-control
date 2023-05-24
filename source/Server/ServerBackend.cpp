#include "ServerBackend.h"

std::shared_ptr<DataObj> ServerBackend::handleClientRequest(DataObj data)
{
    
    switch (data.getFuncType())
    {
    case IAP:
        return _IAPfunc.HandleRequest(data);
        break;
    case RPC:
        return _RPCfunc.HandleRequest(data);
        break;
    case SCR:
        return _SCRfunc.HandleRequest(data);
        break;
    case KLG:
        return _KLGfunc.HandleRequest(data);
        break;
    case DIR:
        return _DIRfunc.HandleRequest(data);
        break;
    default:
        break;
    }
    return NULL;
}

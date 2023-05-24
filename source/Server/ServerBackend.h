#pragma once
#include "../DataObj/DataObj.h"
#include "../DIR_Func.h"
#include "../KLG_Func.h"
#include "../RPC_Func.h"
#include "../SCR_Func.h"
#include "../IAP_Func.h"


class ServerBackend
{
private:
    IAP_Func _IAPfunc;
    RPC_Func _RPCfunc;
    KLG_Func _KLGfunc;
    SCR_Func _SCRfunc;
    DIR_Func _DIRfunc;
public:
    std::shared_ptr<DataObj> ServerBackend::handleClientRequest(DataObj data);
    //DataObj deserialize(std::string& data);
    //std::string createResponseData(DataObj data);
};
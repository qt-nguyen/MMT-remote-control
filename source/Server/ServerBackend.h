#pragma once
#include "../DataObj/DataObj.h"
#include "../Func/DIR_Func.h"
#include "../Func/KLG_Func.h"
#include "../Func/RPC_Func.h"
#include "../Func/SCR_Func.h"
#include "../Func/IAP_Func.h"


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
};
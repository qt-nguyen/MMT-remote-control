#include "ServerBackend.h"

void ServerBackend::handleClientRequest(std::shared_ptr<DataObj> data)
{
    // Chuyển đổi dữ liệu thành DataObj
    //DataObj request = deserialize(buffer);
    // Xử lý yêu cầu từ client dựa trên thông tin trong DataObj
    // Gọi các phương thức tương ứng trong lớp con để thực hiện chức năng
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
            //data = _SCRfunc.startCapture();
        }
        else
        {
            //data = _SCRfunc.stopCapture();
        }
        break;
    case KLG:
        if (data->getCmdType() == START)
        {
            //data = _KLGfunc.startKeylog();
        }
        else
        {
            //data = _KLGfunc.stopKeylog();
        }
        break;
    case DIR:
        //data = _DIRfunc.HandleRequest();
        break;

    default:
        break;
    }
}
/*
DataObj ServerBackend::deserialize(std::string& data)
{
    std::size_t pos = 0;
    std::vector<std::string> tokens;

    // Phân tách dữ liệu thành các thành phần
    while ((pos = data.find(',')) != std::string::npos)
    {
        std::string token = data.substr(0, pos);
        tokens.push_back(token);
        data.erase(0, pos + 1);
    }

    // Cài đặt các thuộc tính của DataObj từ các thành phần tương ứng
    
    // Chuyển đổi dữ liệu từ string sang vector<char>
    std::string dataStr = tokens[4];
    std::vector<char> dataCh;
    dataCh.assign(dataStr.begin(), dataStr.end());

    DataObj request(tokens[0], static_cast<DataType>(std::stoi(tokens[1])), static_cast<FuncType>(std::stoi(tokens[2])), static_cast<CmdType>(std::stoi(tokens[3])), dataCh);
    return request;
}

std::string ServerBackend::createResponseData(DataObj data)
{

    std::string response;

    // Chuyển đổi các thuộc tính thành chuỗi
    response += data.getID() + ",";
    response += data.getDataType() + ",";
    response += data.getFuncType() + ",";
    response += data.getCmdType() + ",";
    response += std::string(data.getData().begin(), data.getData().end());

    return response;
}
*/
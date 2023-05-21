#include "../DataObj/DataObj.h"


class ServerBackend
{
public:
    void handleClientRequest(DataObj& request);
    DataObj deserialize(std::string& data);
    std::string createResponseData(DataObj data);
};
#include "../DataObj/DataObj.h"


class ServerBackend
{
public:
    DataObj handleClientRequest(std::string buffer);

    DataObj deserialize(std::string& data);
    std::string createResponseData(DataObj data);
};
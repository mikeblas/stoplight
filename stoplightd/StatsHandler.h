#pragma once

#include "CivetServer.h"
#include "lights.h"


class stoplightd;

class StatsHandler : public CivetHandler
{
    stoplightd& daemon;

    std::string GetOneLight(Lights::LightLine line);

public:
    StatsHandler(stoplightd& _daemon)
    :   daemon(_daemon)
    {
    }


    bool handleGet(CivetServer* /* server */, struct mg_connection* conn);
};


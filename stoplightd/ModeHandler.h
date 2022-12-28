#pragma once

#include "CivetServer.h"

class stoplightd;


class ModeHandler : public CivetHandler
{
    stoplightd& daemon;

public:
    ModeHandler(stoplightd& _daemon)
    :   daemon(_daemon)
    {
    }

    bool handleGet(CivetServer* server, struct mg_connection* conn);
    bool handlePost(CivetServer* server, struct mg_connection* conn);
};


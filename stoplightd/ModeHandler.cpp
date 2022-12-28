#include "CivetServer.h"

#include "modeinterface.h"
#include "ModeHandler.h"
#include "stoplightd.h"


bool ModeHandler::handleGet(CivetServer* /* server */, struct mg_connection* conn)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n"
        "Connection: close\r\n"
        "\r\n");

    mg_printf(conn,
        "{\r\n");
    mg_printf(conn,
        "    \"mode_id\": %d, \r\n", daemon.GetModeID());
    mg_printf(conn,
        "    \"mode_name\": %s \r\n", daemon.GetModeName().c_str());
    mg_printf(conn,
        "}\r\n");

    return true;
}

bool ModeHandler::handlePost(CivetServer* /* server */, struct mg_connection* conn)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\nContent-Type: text/json\r\n"
        "Connection: close\r\n"
        "\r\n");

    std::string s = "";
    if (CivetServer::getParam(conn, "mode", s))
    {
        try
        {
            int n = stoi(s);
            mg_printf(conn, "{ \"value\": \"%d\"\r\n", n);
            daemon.SetNewWebMode((ModeID) n);
        }
        catch (std::invalid_argument const& x)
        {
            mg_printf(conn, "{ invalid }");
        }
    } else {
        mg_printf(conn, "{ not found }");
    }



    mg_printf(conn,
        "\r\n");

    return true;
}

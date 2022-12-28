
#include <chrono>

#include "CivetServer.h"

#include "StatsHandler.h"
#include "stoplightd.h"
#include "lights.h"

std::string StatsHandler::GetOneLight(Lights::LightLine line)
{
    char sz[1000];

    const OnOffCounter& counts = daemon.GetLightCounter(line);

    double denominator = ((double) counts.timeOnMillis + counts.timeOffMillis);
    double duty_cycle = 0.0;
    if (denominator > 0)
        duty_cycle = counts.timeOnMillis / denominator;

    sprintf(sz, "{ time_on_millis: %ld, time_off_millis: %ld, duty_cycle: %.5f }", counts.timeOnMillis, counts.timeOffMillis, duty_cycle);

    return std::string(sz);
}

bool StatsHandler::handleGet(CivetServer* /* server */, struct mg_connection* conn)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n");

    long uptime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - daemon.GetStartTime()).count();

    mg_printf(conn, "{ uptime_milliseconds: %ld },\r\n", uptime);

    mg_printf(conn, "\"red\": %s\r\n", GetOneLight(Lights::LIGHT_RED).c_str());
    mg_printf(conn, "\"yellow\": %s\r\n", GetOneLight(Lights::LIGHT_YELLOW).c_str());
    mg_printf(conn, "\"green\": %s\r\n", GetOneLight(Lights::LIGHT_GREEN).c_str());
    mg_printf(conn, "\"ind1\": %s\r\n", GetOneLight(Lights::LIGHT_IND_1).c_str());
    mg_printf(conn, "\"ind2\": %s\r\n", GetOneLight(Lights::LIGHT_IND_2).c_str());

    return true;
}

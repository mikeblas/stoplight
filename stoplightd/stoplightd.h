
#pragma once

#include <thread>
#include <chrono>

#include <daemonize/daemonizer.hpp>
#include <daemonize/syslog.hpp>

#include <gpiod.h>

#include "modeinterface.h"

#include "buttons.h"
#include "lights.h"
#include "smartbuzzer.h"

#include <civetweb.h>
#include "CivetServer.h"

#include "ExampleHandler.h"
#include "StatsHandler.h"
#include "ModeHandler.h"

class stoplightd : public daemonize::daemon
{
    Lights* lights;
    Buttons* buttons;
    SmartBuzzer* buzzer;

    struct gpiod_chip* chip;

    ModeInterface* mode;
    bool selectorMode;
    bool needRelease;
    bool runner;
    std::thread* theThread;
    std::chrono::time_point<std::chrono::system_clock> startTime;

    void SetupRunner();

    // web server
    CivetServer* server;

    ModeID newWebMode;

    // handlers
    ExampleHandler h_ex;
    StatsHandler h_stats;
    ModeHandler h_mode;

public:
    stoplightd(daemonize::logger& log);

    std::chrono::time_point<std::chrono::system_clock> GetStartTime()
    {
        return startTime;
    }

    const OnOffCounter& GetLightCounter(Lights::LightLine line)
    {
        return lights->GetLightCounter(line);
    }

    std::string GetModeName()
    {
        if (mode == nullptr)
            return "none";
        else
            return mode->GetModeName();
    }

    ModeID GetModeID()
    {
        if (mode == nullptr)
            return NO_MODE;
        else
            return mode->GetModeID();
    }

    void SetNewWebMode(ModeID newMode)
    {
        newWebMode = newMode;
    }

    ~stoplightd()
    {
        if (!runner)
            log << log.notice << "stoplightd dtor: not the runner" << std::endl;
        else
        {
            log << log.notice << "stoplightd dtor: runner!" << std::endl;

            if (lights == nullptr)
                log << log.notice << "lights is null" << std::endl;
            else
                log << log.notice << "lights is not null" << std::endl;

            delete lights;
            delete buttons;
            delete buzzer;

            mg_exit_library();
        }
    }

    virtual void run() override;

private:
    void DispatchMessages(ModeInterface* mode, Buttons& b2);

    void StartSelectorMode();
};




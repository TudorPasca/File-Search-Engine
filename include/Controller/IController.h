#pragma once

#include <crow/app.h>
#include <crow/middlewares/cors.h>

struct IController {
    virtual void registerRoutes(crow::App<crow::CORSHandler> &app) = 0;
    virtual ~IController() = default;
};
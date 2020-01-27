/**
* @file game_server.cpp
* @brief GameServer定義ファイル
*/

#include "stdafx.h"
#include "game_server.hpp"

namespace app::game_server {

    GameServer::GameServer(net::io_context& ioc) :
        ioc         (ioc),
        endpoint    (tcp::v4(), GameServer::PORT),
        listener    (std::make_shared<Listener>(ioc, endpoint))
    {
    }

    GameServer::GameServer(net::io_context& ioc, unsigned short port) :
        ioc         (ioc),
        endpoint    (tcp::v4(), port),
        listener    (std::make_shared<Listener>(ioc, endpoint))
    {
    }

    void GameServer::Run() {
        this->listener->Run();
    }


    void GameServer::Stop() {
        this->listener->Stop();
    }
}
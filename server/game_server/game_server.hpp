/**
*  @file game_server.hpp
*  @brief GameServer定義ファイル
*/

#ifndef WEBGAMES_GAME_SERVER_HPP
#define WEBGAMES_GAME_SERVER_HPP

#include "stdafx.h"
#include "listener.hpp"

namespace app::game_server {

    namespace http = boost::beast::http;           
    namespace net = boost::asio;            
    using tcp = boost::asio::ip::tcp; 

    /**
    * @class GameServer
    * @brief Http通信サーバー
    */
    class GameServer {
    private:
        static constexpr unsigned char PORT = 32600;    //!< ポート番号

    public:
        GameServer() = delete;
        GameServer(const GameServer&) = default;
        GameServer(GameServer&&) = default;
        ~GameServer() = default;
        GameServer& operator=(const GameServer&) = default;
        GameServer& operator=(GameServer&&) = default;
        
        /**
        * @brief コンストラクタ
        * @param ioc コンテキスト
        * @exception runtime_error送出
        */
        GameServer(net::io_context& ioc);

        /**
        * @brief コンストラクタ
        * @param ioc コンテキスト
        * @param port ポート番号
        * @exception runtime_error送出
        */
        GameServer(net::io_context& ioc, unsigned char port);

    private:   
        net::io_context& ioc;
        const tcp::endpoint endpoint;
        std::shared_ptr<Listener> listener;
        
    public:
        /**
        * @brief Httpサーバーを起動する
        * @return none
        */
        void Run();

        /**
        * @brief Httpサーバーを停止する
        * @return none
        */
        void Stop();
    };

}

#endif
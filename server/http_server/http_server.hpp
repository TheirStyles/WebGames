/**
*  @file http_server.hpp
*  @brief HttpServer定義ファイル
*/

#ifndef WEBGAMES_HTTP_SERVER_HPP
#define WEBGAMES_HTTP_SERVER_HPP

#include <string>
#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "./listener.hpp"

namespace app::http {

    namespace http = boost::beast::http;           
    namespace net = boost::asio;            
    using tcp = boost::asio::ip::tcp; 

    /**
    * @class HttpServer
    * @brief Http通信サーバー
    */
    class HttpServer {
    private:
        static constexpr char* DocRoot = "./";      //!< デフォルトのドキュメントルート
        static constexpr unsigned char Port = 80;    //!< ポート番号

    public:
        HttpServer() = delete;
        HttpServer(const HttpServer&) = default;
        HttpServer(HttpServer&&) = default;
        ~HttpServer() = default;
        HttpServer& operator=(const HttpServer&) = default;
        HttpServer& operator=(HttpServer&&) = default;

        /**
        * @brief コンストラクタ
        * @param ioc コンテキスト
        * @exception runtime_error送出
        */
        HttpServer(net::io_context& ioc);

    private:   
        net::io_context& ioc;
        const tcp::endpoint endpoint;
        std::shared_ptr<std::string const> doc_root;
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
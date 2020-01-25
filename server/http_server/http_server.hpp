/**
*  @file http_server.hpp
*  @brief HttpServer定義ファイル
*/

#ifndef WEBGAMES_HTTP_SERVER_HPP
#define WEBGAMES_HTTP_SERVER_HPP

#include "stdafx.h"
#include "listener.hpp"

namespace app::http_server {

    namespace http = boost::beast::http;           
    namespace net = boost::asio;            
    using tcp = boost::asio::ip::tcp; 

    /**
    * @class HttpServer
    * @brief Http通信サーバー
    */
    class HttpServer {
    private:
        static constexpr char* DOC_ROOT = ".";		 //!< デフォルトのドキュメントルート
        static constexpr unsigned char PORT = 80;    //!< ポート番号

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

		/**
		* @brief コンストラクタ
		* @param ioc コンテキスト
		* @param doc_root ドキュメントルートのパス
		* @exception runtime_error送出
		*/
		HttpServer(net::io_context& ioc, const std::string& doc_root);

    private:   
        net::io_context& ioc;
        const tcp::endpoint endpoint;
        std::shared_ptr<const std::string> doc_root;
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
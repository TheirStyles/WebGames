/**
* @file http_server.cpp
* @brief HttpServer定義ファイル
*/

#include <string>
#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "./http_server.hpp"

namespace http = boost::beast::http;           
namespace net = boost::asio;            
using tcp = boost::asio::ip::tcp; 

namespace app::http {

    HttpServer::HttpServer(net::io_context& ioc) :
        ioc         (ioc),
        endpoint    (tcp::v4(), HttpServer::Port),
        doc_root    (std::make_shared<std::string const>(HttpServer::doc_root)),
        listener    (std::make_shared<Listener>(ioc, endpoint, doc_root))
    {
    }

    void HttpServer::Run() {
        this->listener->Run();
    }


    void HttpServer::Stop() {
        this->listener->Stop();
    }
}
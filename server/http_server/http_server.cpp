/**
* @file http_server.cpp
* @brief HttpServer定義ファイル
*/

#include "stdafx.h"
#include "http_server.hpp"

namespace app::http_server {

    HttpServer::HttpServer(net::io_context& ioc) :
        ioc         (ioc),
        endpoint    (tcp::v4(), HttpServer::PORT),
        doc_root    (std::make_shared<const std::string>(HttpServer::DOC_ROOT)),
        listener    (std::make_shared<Listener>(ioc, endpoint, this->doc_root))
    {
    }

	HttpServer::HttpServer(net::io_context& ioc, const std::string& doc_root) :
		ioc			(ioc),
		endpoint	(tcp::v4(), HttpServer::PORT),
		doc_root	(std::make_shared<const std::string>(doc_root)),
		listener	(std::make_shared<Listener>(ioc, endpoint, this->doc_root))
	{
	}

    void HttpServer::Run() {
        this->listener->Run();
    }


    void HttpServer::Stop() {
        this->listener->Stop();
    }
}
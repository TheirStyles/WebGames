/**
 * @file session.cpp
 * @brief Sessionクラス実装ファイル
*/

#include <string>
#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

namespace app::http {

    namespace beast = boost::beast;
    namespace http = boost::beast::http;           
    namespace net = boost::asio;            
    using tcp = boost::asio::ip::tcp; 

    beast::string_view //TODO

}
/**
* @file main.cpp
* @brief エントリーポイント定義ファイル
*/

#include <iostream>
#include <memory>
#include <exception>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "./http_server/http_server.hpp"

/**
* @brief エントリーポイント 
* @param args コマンドライン引数の数
* @param argv コマンドライン引数
* @return アプリケーション終了コード
*/
int main(int args, char** argv){
    namespace beast = boost::beast;       
    namespace http = beast::http;         
    namespace net = boost::asio;            
    using tcp = boost::asio::ip::tcp;
    using HttpServer = app::http::HttpServer;

    try {
        //コンテキスト生成
        net::io_context ioc(2);

        //サーバー作成
        auto http_server = std::make_unique<HttpServer>(ioc);

        //サーバー起動
        http_server->Run();

        //リクエスト処理
        std::thread th1([&ioc] { ioc.run(); });
        std::thread th2([&ioc] { ioc.run(); });
        
        //キー入力で終了
        std::getc(stdin);
        http_server->Stop();
        th1.join();
        th2.join();

        //終了
        return EXIT_SUCCESS;
    }
    catch(const std::runtime_error& e){
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception& e){
        std::cout << e.what() << std::endl;
    }
    catch(...){
        std::cout << "Happen Exception" << std::endl; 
    }
}
/**
* @file main.cpp
* @brief エントリーポイント定義ファイル
*/

/**
* @file stdafx.h
* @breif プリコンパイル済みヘッダー
*/

#pragma once


#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <vector>
#include <exception>


#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "http_server.hpp"
#include "game_server.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


/**
* @brief エントリーポイント 
* @param args コマンドライン引数の数
* @param argv コマンドライン引数
* @return アプリケーション終了コード
*/
int main(int argc, char** argv){

    try {
		//サーバー情報
		constexpr char* DOC_ROOT = "./";
		constexpr size_t DEFAULT_THREAD_NUM = 2;
		size_t thread_num = DEFAULT_THREAD_NUM;  //!< 将来の変更用に変数を用意
		const char* doc_root = DOC_ROOT;
		std::vector<std::thread> threads{};

        //コンテキスト生成
        net::io_context ioc(thread_num);

        //サーバー作成
        auto http_server = std::make_unique<app::http_server::HttpServer>(ioc, doc_root);
		auto game_server = std::make_unique<app::game_server::GameServer>(ioc);

        //サーバー起動
        http_server->Run();
		std::cout << "HTTP Serverを起動しました。" << std::endl;
		game_server->Run();
		std::cout << "Game Serverを起動しました。" << std::endl;

        //リクエスト処理用のスレッドを立てる
		threads.reserve(thread_num);
		for (size_t i = 0; i < thread_num; ++i) {
			threads.push_back(
				std::thread(
					[&ioc] { 
						ioc.run(); 
					}
				)
			);
		}
		std::cout << "メッセージ処理を開始しました。" << std::endl;
        
        //キー入力で終了
		std::cout << "キー入力でサーバー処理を終了します。" << std::endl;
        std::getc(stdin);

        http_server->Stop();
		std::cout << "HTTP Serverが終了しました。" << std::endl;
		game_server->Stop();
		std::cout << "Game Serverが終了しました。" << std::endl;

		ioc.stop();
		std::cout << "メッセージ処理が終了しました。" << std::endl;

		for (size_t i = 0; i < threads.size(); ++i) {
			threads[i].join();
			std::cout << "スレッド" << (i+1) << "が終了しました。" << std::endl;
		}
		std::cout << "すべてのスレッドが終了しました。" << std::endl;

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

	//異常終了
	return EXIT_FAILURE;
}
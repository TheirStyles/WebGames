/**
* @file main.cpp
* @brief エントリーポイント定義ファイル
*/

#include "stdafx.h"
#include "http_server.hpp"

/**
* @brief エントリーポイント 
* @param args コマンドライン引数の数
* @param argv コマンドライン引数
* @return アプリケーション終了コード
*/
int main(int args, char** argv){

    try {
        //コンテキスト生成
        net::io_context ioc(2);

        //サーバー作成
        auto http_server = std::make_unique<app::http_server::HttpServer>(ioc);

        //サーバー起動
        http_server->Run();
		std::cout << "HTTP Serverを起動しました。" << std::endl;

        //リクエスト処理
        std::thread th1([&ioc] { ioc.run(); });
        std::thread th2([&ioc] { ioc.run(); });
        
        //キー入力で終了
		std::cout << "キー入力でサーバー処理を終了します。" << std::endl;
        std::getc(stdin);
        http_server->Stop();
		std::cout << "HTTP Serverが終了しました。" << std::endl;
		ioc.stop();
		std::cout << "メッセージ処理が終了しました。" << std::endl;
        th1.join();
		std::cout << "スレッド1が終了しました。" << std::endl;
        th2.join();
		std::cout << "スレッド2が終了しました。" << std::endl;
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
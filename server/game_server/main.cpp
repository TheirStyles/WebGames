/**
* @file main.cpp
* @brief エントリーポイント定義ファイル
*/

#include "stdafx.h"
#include "game_server.hpp"

/**
* @brief エントリーポイント 
* @param args コマンドライン引数の数
* @param argv コマンドライン引数
* @return アプリケーション終了コード
*/
int main(int argc, char** argv){

    try {
		//サーバー情報
		constexpr size_t DEFAULT_THREAD_NUM = 2;
		constexpr unsigned char port = 32600;
		size_t thread_num = DEFAULT_THREAD_NUM;  //!< 将来の変更用に変数を用意
		std::vector<std::thread> threads{};

        //コンテキスト生成
        net::io_context ioc(thread_num);

        //サーバー作成
        auto http_server = std::make_unique<app::game_server::GameServer>(ioc, port);

        //サーバー起動
        http_server->Run();
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
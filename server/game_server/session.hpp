/**
 * @file session.hpp
 * @brief Sessionクラス定義ファイル
*/

#ifndef WEBGAMES_APP_GAME_SERVER_SESSION_HPP
#define WEBGAMES_APP_GAME_SERVER_SESSION_HPP

#include "stdafx.h"

namespace app::game_server {

    /**
     * @class Session
     * @brief クライアントとのやり取りを管理するクラス
    */
    class Session : public std::enable_shared_from_this<Session> {
	public:
        Session() = delete;
        Session(const Session&) = default;
        Session(Session&&) = default;
        ~Session() = default;
        Session& operator=(const Session&) = default;
        Session& operator=(Session&&) = default;

        /**
         * @brief コンストラクタ
         * @param socket ソケット
        */
        Session(tcp::socket&& socket);

    private:
        websocket::stream<beast::tcp_stream> ws;    //!< ストリーム
        beast::flat_buffer buffer;                  //!< 受信用バッファ

    private:
		/**
		 * @brief 実行時の処理
		 * @parma none
		 * @return none
		*/
		void OnRun();

        /**
         * @brief 接続時の処理
         * @param ec エラー情報
         * @return none
         */
        void OnAccept(beast::error_code ec);

        /**
         * @brief 読み込み処理を開始する
         * @param none
         * @return none
        */
        void DoRead();

        /**
         * @brief 読み込み時の処理
         * @param ec エラー情報
         * @param bytes_transferred 処理されたサイズ
         * @return none
        */
        void OnRead(beast::error_code ec, std::size_t bytes_transferred);

        /**
         * @brief 書き込み処理
         * @param ec エラー情報
         * @param bytes_transferred 処理されたサイズ
         * @return none
        */
       void OnWrite(beast::error_code ec, std::size_t bytes_transferred);

        /**
         * @brief セッションを閉じる
         * @param none
         * @return none
        */
        void DoClose();

    public:
        /**
        * @brief セッションを開始する
        * @return none
        */
        void Run();
    };

}

#endif
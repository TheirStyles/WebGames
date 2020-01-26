/**
 * @file listener.hpp
 * @brief Listenerクラス定義ファイル
*/

#ifndef WEBGAMES_APP_GAME_SERVER_LISTENER_HPP
#define WEBGAMES_APP_GAME_SERVER_LISTENER_HPP

#include "stdafx.h"


namespace app::game_server {

    /**
     *  @class Listener
     *  @brief 通信の待ち受けを行うクラス
     *  @attention std::shared_ptrでラップして構築すること
    */
    class Listener : public std::enable_shared_from_this<Listener> {
    public:
        Listener() = default;
        Listener(const Listener&) = default;
        Listener(Listener&&) = default;
        ~Listener() = default;
        Listener& operator=(const Listener&) = default;
        Listener& operator=(Listener&&) = default;

        /**
         * @brief コンストラクタ
         * @param ioc コンテキスト
         * @param endpoint ホスト情報
         * @exception エラー時にruntime_error
        */
        Listener(net::io_context& ioc, tcp::endpoint endpoint);

    private:
        net::io_context& ioc;
        tcp::acceptor acceptor;

    private:
        /**
         * @brief 非同期で通信相手の受信を行う
         * @param none
         * @return none
        */
        void DoAccept();

        /**
         * @brief 相手を受信したときの処理
         * @param ec エラー情報
         * @param socket ソケット
         * @return none
        */
        void OnAccept(beast::error_code ec, tcp::socket socket);

    public:
        /**
        * @brief リスナーを起動する
        * @param none
        * @return none
        */
        void Run();

        /**
        * @brief リスナーを停止する
        * @param none
        * @return none
        */
        void Stop();
    };

}

#endif

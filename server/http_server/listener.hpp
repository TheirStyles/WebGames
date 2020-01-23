/**
 * @file listener.hpp
 * @brief Listenerクラス定義ファイル
*/

#ifndef WEBGAMES_LISTENER_HPP
#define WEBGAMES_LISTENER_HPP

#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

namespace app::http {

    namespace http = boost::beast::http;           
    namespace net = boost::asio;            
    using tcp = boost::asio::ip::tcp; 

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
        Listener& operator=(const Listener&&) = default;

        /**
         * @brief コンストラクタ
         * @param ioc コンテキスト
         * @param endpoint ホスト情報
         * @param doc_root ドキュメントルートのパス 
         * @exception エラー時にruntime_error
        */
        Listener(net::io_context& ioc, tcp::endpoint endpoint, std::shared_ptr<std::string const> doc_root);

    private:
        net::io_context& ioc;
        tcp::acceptor acceptor;
        std::shared_ptr<std::string const> doc_root;

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

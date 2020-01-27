/**
 * @file listener.cpp
 * @brief Listenerクラス実装ファイル 
*/

#include "stdafx.h"
#include "listener.hpp"
#include "session.hpp"

namespace app::game_server {

    Listener::Listener(net::io_context& ioc, tcp::endpoint endpoint) :
        ioc         (ioc),
        acceptor    (net::make_strand(ioc))
    {
        
        beast::error_code ec{};

        //アクセプターを開く
        this->acceptor.open(endpoint.protocol(), ec);
        if (ec) {
            throw std::runtime_error("Listener Constructor : " + ec.message());
        }

        //アドレスの再利用を許可
        this->acceptor.set_option(net::socket_base::reuse_address(true), ec);
        if (ec) {
            throw std::runtime_error("Listener Constructor : "  + ec.message());
        }

        //アクセプターをアドレスに結びつける
        this->acceptor.bind(endpoint, ec);
        if (ec) {
            throw std::runtime_error("Listener Constructor : "  + ec.message());
        }

        //待ち受け開始
        this->acceptor.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            throw std::runtime_error("Listener Constructor : " + ec.message());
        }
    }

    void Listener::DoAccept(){

		//TODO
		std::cout << __func__ << std::endl;

        this->acceptor.async_accept(
            net::make_strand(ioc),
            beast::bind_front_handler(
                &Listener::OnAccept,
                this->shared_from_this()
            )
        );
    }

    void Listener::OnAccept(beast::error_code ec, tcp::socket socket){

		//TODO
		std::cout << __func__ << std::endl;

        //エラーは無視
        if(!ec){
            //セッションを生成・実行する
			std::make_shared<Session>(std::move(socket))->Run();
        }

        //再び受付開始
        this->DoAccept();
    }

    void Listener::Run(){
        this->DoAccept();
    }

    void Listener::Stop(){
        this->acceptor.cancel();
        this->acceptor.close();
    }

}
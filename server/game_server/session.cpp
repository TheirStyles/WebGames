/**
 * @file session.cpp
 * @brief Sessionクラス実装ファイル
*/

#include "stdafx.h"
#include "session.hpp"

namespace app::game_server {

	Session::Session(tcp::socket&& socket) :
		ws(std::move(socket))
	{
	}
	
	void Session::OnAccept(beast::error_code ec){
		this->DoRead();
	}

	void Session::DoRead() {
		//リクエストを非同期読み込み
		this->ws.async_read(
			this->buffer,
			beast::bind_front_handler(
				&Session::OnRead,
				this->shared_from_this()
			)
		);
	}

	void Session::OnRead(beast::error_code ec, std::size_t bytes_transferred) {
		//使用しない変数であることを通知
		boost::ignore_unused(bytes_transferred);

		//ストリームがエラーで閉じた場合
		if (ec == http::error::end_of_stream) {
			return this->DoClose();
		}

		//エコー処理 TODO UPDATE
		ws.text(ws.got_text());
		this->ws.async_write(
			this->buffer.data(),
			beast::bind_front_handler(
				&Session::OnWrite,
				this->shared_from_this()
			)
		);
	}

	void Session::OnWrite(beast::error_code ec, std::size_t bytes_transferred) {
		//使用しない変数であることを通知
		boost::ignore_unused(ec);
		boost::ignore_unused(bytes_transferred);

		//バッファクリア
		this->buffer.consume(this->buffer.size());

		//再び読み込み開始
		this->DoRead();
	}

	void Session::DoClose() {
		beast::error_code ec{};
		this->ws.close(beast::websocket::close_reason("shutdown"), ec);
	}

	void Session::Run() {
		//タイムアウト設定
		this->ws.set_option(
			websocket::stream_base::timeout::suggested(
				beast::role_type::server
			)
		);

		//ハンドシェイクのデコレーターせってい
		this->ws.set_option(websocket::stream_base::decorator(
			[](websocket::response_type& res){
				 res.set(
					http::field::server,
                	std::string(BOOST_BEAST_VERSION_STRING) +" websocket-server-async"
				);
			}
		));

		//メッセージ処理を実行する
		this->ws.async_accept(
			beast::bind_front_handler(
				&Session::OnAccept,
				this->shared_from_this()
			)
		);
	}
}
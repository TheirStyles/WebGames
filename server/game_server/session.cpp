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

	void Session::OnRun() {
		//タイムアウト設定
		this->ws.set_option(
			websocket::stream_base::timeout::suggested(
				beast::role_type::server
			)
		);

		//ハンドシェイクのデコレーター設定
		this->ws.set_option(websocket::stream_base::decorator(
			[](websocket::response_type& res) {
				res.set(
					http::field::server,
					std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async"
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
	
	void Session::OnAccept(beast::error_code ec){
		this->DoRead();
	}

	void Session::DoRead() {

		//バッファクリア
		this->buffer.consume(this->buffer.size());

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
		boost::ignore_unused(ec);
		boost::ignore_unused(bytes_transferred);

		//データをstring化
		std::string data = beast::buffers_to_string(this->buffer.data());

		//分割
		std::vector<std::string> split{};
		boost::algorithm::split(split, data, boost::is_any_of(":"));

		//命令解析
		if (split[0] == "NICKNAME") {
			this->name = split[1];
			this->DoRead();
		}
		else if (split[0] == "SELECT") {
			if (split[1] == "MakeRoom") {
				make_room_player = this->shared_from_this();
				if (join_room_player) {
					join_room_player->ws.async_write(
						net::buffer("PLAYER_LIST:" + this->name),
						beast::bind_front_handler(
							&Session::OnWrite,
							this->shared_from_this()
						)
					);
				}
			}
			else if (split[1] == "JoinRoom") {
				join_room_player = this->shared_from_this();
				if (make_room_player) {
					join_room_player->ws.async_write(
						net::buffer("PLAYER_LIST:" + make_room_player->name),
						beast::bind_front_handler(
							&Session::OnWrite,
							this->shared_from_this()
						)
					);
				}
			}
		}
		else if (split[0] == "TARGET_PLAYER") {
			make_room_player->ws.async_write(
				net::buffer(this->name),
				beast::bind_front_handler(
					&Session::OnWrite,
					this->shared_from_this()
				)
			);
		}
		else if (split[0] == "ENTRY") {
			this->ws.async_write(
				net::buffer("PREPARED:OK"),
				beast::bind_front_handler(
					&Session::OnWrite,
					this->shared_from_this()
				)
			);
		}
		else {
			this->DoRead();
		}
	}

	void Session::OnWrite(beast::error_code ec, std::size_t bytes_transferred) {
		//使用しない変数であることを通知
		boost::ignore_unused(ec);
		boost::ignore_unused(bytes_transferred);

		//再び読み込み開始
		this->DoRead();
	}

	void Session::DoClose() {
		beast::error_code ec{};
		this->ws.close(beast::websocket::close_reason("shutdown"), ec);
	}

	void Session::Run() {
		//メッセージをコンテキストに投機
		net::dispatch(
			this->ws.get_executor(),
			beast::bind_front_handler(
				&Session::OnRun,
				this->shared_from_this()
			)
		);
	}

	void Session::Stop() {
		this->DoClose();
	}
}
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
				this->first = true;
				make_room_player_turn = true;
				make_room_player = this->shared_from_this();
				if (join_room_player) {
					join_room_player->ws.async_write(
						net::buffer("PLAYER_LIST:" + this->name),
						beast::bind_front_handler(
							&Session::OnWrite,
							join_room_player->shared_from_this()
						)
					);
				}
			}
			else if (split[1] == "JoinRoom") {
				this->first = false;
				join_room_player = this->shared_from_this();
				if (make_room_player) {
					join_room_player->ws.async_write(
						net::buffer("PLAYER_LIST:" + make_room_player->name),
						beast::bind_front_handler(
							&Session::OnWrite,
							join_room_player->shared_from_this()
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
					make_room_player->shared_from_this()
				)
			);
		}
		else if (split[0] == "ENTER") {

			//ゲームのインスタンス化
			othello = std::make_unique<Othello>();

			//準備完了を通知
			join_room_player->ws.async_write(
				net::buffer("PREPARED:OK"),
				beast::bind_front_handler(
					&Session::OnWrite,
					join_room_player->shared_from_this()
				)
			);

			this->DoRead();

		}
		else if (split[0] == "ONGAME") {
			//盤面情報
			this->ws.async_write(
				net::buffer("BOARD:" + othello->GetBoardStateString()),
				beast::bind_front_handler(
					&Session::OnGame,
					this->shared_from_this()
				)
			);
		}
		else if (split[0] == "SET_STONE") {
			//x y 取得
			std::vector<std::string> pos{};
			boost::algorithm::split(pos, split[1], boost::is_any_of(","));

			//セット
			auto ans = othello->SetStone(std::atoi(pos[0].c_str()), std::atoi(pos[1].c_str()));

			//結果送信
			if (ans) {
				make_room_player_turn = !make_room_player_turn;

				//auto make_p = make_room_player;
				//make_p->ws.async_write(
				//	net::buffer("BOARD:" + othello->GetBoardStateString()),
				//	[&make_p](beast::error_code ec, std::size_t bytes_transferred) {
				//		make_p->ws.async_write(
				//			net::buffer("SWITCH:NONE"),
				//				make_room_player_turn ? 
				//				beast::bind_front_handler(
				//					&Session::OnWrite,
				//					make_p->shared_from_this()
				//				) :
				//				[](beast::error_code ec, std::size_t bytes_transferred) {}
				//		);
				//	}
				//);

				//auto join_p = join_room_player;
				//join_room_player->ws.async_write(
				//	net::buffer("BOARD:" + othello->GetBoardStateString()),
				//	[&join_p](beast::error_code ec, std::size_t bytes_transferred) {
				//		join_p->ws.async_write(
				//			net::buffer("SWITCH:NONE"),
				//			make_room_player_turn ?
				//			beast::bind_front_handler(
				//				&Session::OnWrite,
				//				join_room_player->shared_from_this()
				//			) :
				//			[](beast::error_code ec, std::size_t bytes_transferred) {}
				//		);
				//	}
				//);

				this->DoRead();
			}
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

	void Session::OnGame(beast::error_code ec, std::size_t bytes_transferred) {
		//使用しない変数であることを通知
		boost::ignore_unused(ec);
		boost::ignore_unused(bytes_transferred);

		if (this->first) {
			this->ws.async_write(
				net::buffer(std::string("START:") + (this->first ? "YOU" : "NO")),
				beast::bind_front_handler(
					&Session::OnWrite,
					this->shared_from_this()
				)
			);
		}
		else {
			this->ws.async_write(
				net::buffer(std::string("START:") + (this->first ? "YOU" : "NO")),
				[](beast::error_code ec, std::size_t bytes_transferred) {}
			);
		}
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
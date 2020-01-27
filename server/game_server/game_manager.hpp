/**
* @file game_manager.hpp
* @brief ゲームマネージャークラス
*/

#pragma once

#include "stdafx.h"
#include "session.hpp"

namespace app::game_server {

	/**
	* @class GameManager
	* @brief マッチングやゲームの生成などゲーム全体を管理する
	*/
	class GameManager {
	public:
		/**
		* @typedef Player
		* @brief プレイヤー情報
		*/
		using Player = std::pair<std::string, std::shared_ptr<Session>>;
	
	private:
		static std::mutex mtx;
		static inline std::list<Player> make_room_player;	//!< 部屋を作成したプレイヤー
		static inline std::list<Player> join_room_player;	//!< 部屋に参加したいプレイヤー

	public:
		/**
		* @brief 部屋を作成したプレイヤーを追加
		* @param player プレイヤー情報
		*/
		static void AddMakeRoomPlayer(Player player) {
			constexpr int MAX_PLAYER = 3;
			
			std::lock_guard<std::mutex> lock(mtx);

			//管理プレイヤー追加
			GameManager::make_room_player.push_back(player);
			
			//プレイヤー数が3人より多ければデータの追加送信は無し
			if (MAX_PLAYER < make_room_player.size()) {
				return;
			}
			
			//送信データの作成
			std::string send_msg = {"PLAYER_LIST:"};
			int count = 0;
			for (auto& it : make_room_player) {
				send_msg += it.first;

				++count;

				if (count == 3) {
					break;
				}
				else {
					send_msg += ",";
				}
			}

			//データ送信
			for (auto& it : join_room_player) {
				it.second->DoWrite(send_msg);
			}
		};

		/**
		* @brief 部屋に参加したいプレイヤーを追加
		* @param player プレイヤー情報
		*/
		static void AddJoinRoomPlayer(Player player) {
			std::lock_guard<std::mutex> lock(mtx);

			//管理プレイヤー追加
			GameManager::join_room_player.push_back(player);

			//送信データの作成
			std::string send_msg = { "PLAYER_LIST:" };
			int count = 0;
			for (auto& it : make_room_player) {
				send_msg += it.first;

				++count;

				if (count == 3) {
					break;
				}
				else {
					send_msg += ",";
				}
			}

			//データ送信
			for (auto& it : join_room_player) {
				it.second->DoWrite(send_msg);
			}
		}


	};

}
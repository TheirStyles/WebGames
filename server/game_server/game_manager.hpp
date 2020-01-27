/**
* @file game_manager.hpp
* @brief �Q�[���}�l�[�W���[�N���X
*/

#pragma once

#include "stdafx.h"
#include "session.hpp"

namespace app::game_server {

	/**
	* @class GameManager
	* @brief �}�b�`���O��Q�[���̐����ȂǃQ�[���S�̂��Ǘ�����
	*/
	class GameManager {
	public:
		/**
		* @typedef Player
		* @brief �v���C���[���
		*/
		using Player = std::pair<std::string, std::shared_ptr<Session>>;
	
	private:
		static std::mutex mtx;
		static inline std::list<Player> make_room_player;	//!< �������쐬�����v���C���[
		static inline std::list<Player> join_room_player;	//!< �����ɎQ���������v���C���[

	public:
		/**
		* @brief �������쐬�����v���C���[��ǉ�
		* @param player �v���C���[���
		*/
		static void AddMakeRoomPlayer(Player player) {
			constexpr int MAX_PLAYER = 3;
			
			std::lock_guard<std::mutex> lock(mtx);

			//�Ǘ��v���C���[�ǉ�
			GameManager::make_room_player.push_back(player);
			
			//�v���C���[����3�l��葽����΃f�[�^�̒ǉ����M�͖���
			if (MAX_PLAYER < make_room_player.size()) {
				return;
			}
			
			//���M�f�[�^�̍쐬
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

			//�f�[�^���M
			for (auto& it : join_room_player) {
				it.second->DoWrite(send_msg);
			}
		};

		/**
		* @brief �����ɎQ���������v���C���[��ǉ�
		* @param player �v���C���[���
		*/
		static void AddJoinRoomPlayer(Player player) {
			std::lock_guard<std::mutex> lock(mtx);

			//�Ǘ��v���C���[�ǉ�
			GameManager::join_room_player.push_back(player);

			//���M�f�[�^�̍쐬
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

			//�f�[�^���M
			for (auto& it : join_room_player) {
				it.second->DoWrite(send_msg);
			}
		}


	};

}
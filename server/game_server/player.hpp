/**
* @file player.hpp
* @brief Player�N���X��`�t�@�C��
*/

#ifndef WEBGAMES_APP_GAME_SERVER_PLAYER_HPP
#define WEBGAMES_APP_GAME_SERVER_PLAYER_HPP

#include "stdafx.h"

namespace app::game_server {

	/**
	* @class Player
	* @breif �Q�[���v���C���[�̃N���X
	*/
	class Player {
	public:

	private:
		websocket::stream<beast::tcp_stream>& ws;   //!< �X�g���[��

	public:

	};
}

#endif
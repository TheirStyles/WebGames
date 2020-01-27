/**
* @file player.hpp
* @brief Playerクラス定義ファイル
*/

#ifndef WEBGAMES_APP_GAME_SERVER_PLAYER_HPP
#define WEBGAMES_APP_GAME_SERVER_PLAYER_HPP

#include "stdafx.h"

namespace app::game_server {

	/**
	* @class Player
	* @breif ゲームプレイヤーのクラス
	*/
	class Player {
	public:

	private:
		websocket::stream<beast::tcp_stream>& ws;   //!< ストリーム

	public:

	};
}

#endif
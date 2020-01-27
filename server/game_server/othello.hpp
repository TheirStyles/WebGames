/**
* @file othello.hpp
* @brief Othello実装ファイル
*/

#pragma once

namespace app::game_server {

	/**
	* @enum OthelloState
	* @brief 盤面の状態
	*/
	enum OthelloState : char {
		BLANK = 'O',
		BLACK = 'B',
		WHITE = 'W'
	};

	/**
	* @class Othello
	* @brief オセロクラス
	*/
	class Othello{
	public:
		/**
		* @brief コンストラクタ
		*/
		Othello() {
			for (int i = 0; i < 8; ++i)
				for (int j = 0; j < 8; ++j)
					board[i][j] = BLANK;

			board[3][3] = board[4][4] = WHITE;
			board[3][4] = board[4][3] = BLACK;
		}

	private:
		OthelloState board[8][8];	//!< 盤面

	private:
		/**
		* @brief 石を返せるか確認する
		* @param y y位置
		* @param x x位置
		*/
		void CheckTurn(int y, int x) {

		}

	public:
		/**
		* @brief 
		* @return trueで成功 falseで失敗
		*/
		bool SetStone(int y, int x) {
			return true;
		}

		/**
		* @brief 盤面情報を文字列で取得
		* @return 盤面情報の文字列
		*/
		std::string GetBoardStateString()const {
			std::string ans{};
			for (int i = 0; i < 8; ++i)
				for (int j = 0; j < 8; ++j)
					ans += board[i][j];

			return ans;
		}
	};


}
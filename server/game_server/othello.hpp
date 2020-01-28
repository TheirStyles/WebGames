/**
* @file othello.hpp
* @brief Othello�����t�@�C��
*/

#pragma once

namespace app::game_server {

	/**
	* @enum OthelloState
	* @brief �Ֆʂ̏��
	*/
	enum OthelloState : char {
		BLANK = 'O',
		BLACK = 'B',
		WHITE = 'W'
	};

	/**
	* @class Othello
	* @brief �I�Z���N���X
	*/
	class Othello{
	public:
		/**
		* @brief �R���X�g���N�^
		*/
		Othello() :
			white_turn(true)
		{
			for (int i = 0; i < 8; ++i)
				for (int j = 0; j < 8; ++j)
					board[i][j] = BLANK;

			//board[3][3] = board[4][4] = WHITE;
			//board[3][4] = board[4][3] = BLACK;
			board[0][0] = WHITE;
		}

	private:
		OthelloState board[8][8];	//!< �Ֆ�
		bool white_turn;

	private:
		/**
		* @brief �΂�Ԃ��邩�m�F����
		* @param y y�ʒu
		* @param x x�ʒu
		*/
		void CheckTurn(int y, int x) {

		}

	public:
		/**
		* @brief 
		* @return true�Ő��� false�Ŏ��s
		*/
		bool SetStone(int y, int x) {
			if (white_turn) {
				board[y][x] = WHITE;
			}
			else {
				board[y][x] = BLACK;
			}
			white_turn = !white_turn;
			return true;
		}

		/**
		* @brief �Ֆʏ��𕶎���Ŏ擾
		* @return �Ֆʏ��̕�����
		*/
		std::string GetBoardStateString()const {
			std::string ans{};
			ans.reserve(8 * 8);
			for (int i = 0; i < 8; ++i)
				for (int j = 0; j < 8; ++j)
					ans += board[i][j];

			std::cout << ans << std::endl;

			return ans;
		}
	};


}
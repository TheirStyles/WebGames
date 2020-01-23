/**
* @file application.hpp
* @brief アプリケーション全体に関する定義を参照するためのヘッダーファイル
* @attention プリコンパイル済みヘッダ―(stdafx.h)を通して参照すること
*/

#pragma once

namespace app {

/*******************************************************************************************************************
*
*	定数・列挙体
*
*******************************************************************************************************************/
	/**
	* @enum RESULT
	* @brief 実行結果を表す列挙体
	*/
	enum RESULT : int {
		FAILED,		//!< 処理は失敗
		SUCCEEDED,	//!< 処理は問題なく行われた
	};

/*******************************************************************************************************************
*
*	グローバル変数
*
*******************************************************************************************************************/
	/**
	* @var hinstance
	* @brief インスタンスハンドル
	*/
	inline HINSTANCE hInstance;




/*******************************************************************************************************************
*
*	前方宣言
*
*******************************************************************************************************************/


}
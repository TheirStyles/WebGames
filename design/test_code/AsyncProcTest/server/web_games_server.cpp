/**
* @file web_games_server.cpp
* @brief エントリポイント定義ファイル
*/


/*******************************************************************************************************************
*
*	インクルード
*
*******************************************************************************************************************/
#include "async_server.hpp"

/*******************************************************************************************************************
*
*	定数
*
*******************************************************************************************************************/


/*******************************************************************************************************************
*
*	前方宣言
*
*******************************************************************************************************************/

/**
* @brief アプリケーションの初期化関数
* @param hInstance インスタンスハンドル
* @param nCmdShow 指定された表示方法
* @return FAILEDの場合失敗、SUCCEEDEDの場合成功
*/
app::RESULT InitApp(HINSTANCE hInstance, int nCmdShow)noexcept;


/*******************************************************************************************************************
*
*	関数定義
*
*******************************************************************************************************************/

/**
* @brief エントリポイント
* @param hInstance アプリのインスタンスハンドル
* @param hPrevInstance win16互換用
* @param lpCmdLine コマンドライン引数
* @param nCmdShow 指定された表示形式
* @return 終了コード
*/
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	namespace beast = boost::beast;         // from <boost/beast.hpp>
	namespace http = beast::http;           // from <boost/beast/http.hpp>
	namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
	namespace net = boost::asio;            // from <boost/asio.hpp>
	using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

	MSG msg{};

    // アプリケーション初期化の実行:
    if (!InitApp(hInstance, nCmdShow)){
        return FALSE;
    }

	// サーバー設定
	auto const address = tcp::v4();
	auto const port = static_cast<unsigned short>(std::atoi("31400"));

	//サーバーの受信処理開始
	std::make_shared<app::listener>(app::ioc, tcp::endpoint(address, port))->run();

	// サーバーのメッセージ処理を別スレッドで行わせる
	std::thread([&] { app::ioc.run(); }).detach();

    // メイン メッセージ ループ
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	app::ioc.stop();

    return static_cast<int>(msg.wParam);
}


/**
* @brief アプリケーションの初期化関数
* @param hInstance インスタンスハンドル
* @param nCmdShow 指定された表示方法
* @return FAILEDの場合失敗、SUCCEEDEDの場合成功
*/
app::RESULT InitApp(HINSTANCE hInstance, int nCmdShow)noexcept {

	//グローバル変数初期化
	app::hinstance = hInstance;

	//ウィンドウクラス設定
	WNDCLASS winc{};
	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("KITTY");
	winc.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
		switch (msg) {
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
		}
		return DefWindowProc(hwnd, msg, wp, lp);
	};
	if (!RegisterClass(&winc)) {
		return app::RESULT::FAILED;
	}

	//ウィンドウ生成
	app::hwnd = CreateWindow(
		TEXT("KITTY"), 
		TEXT("Async Server"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 
		100, 
		200, 
		200, 
		NULL, 
		NULL,
		hInstance, 
		NULL
	);
	if (app::hwnd == NULL) {
		return app::RESULT::FAILED;
	}

	//表示指定に従い表示状態を変更
	ShowWindow(app::hwnd, nCmdShow);

	return app::RESULT::SUCCEEDED;
}
/**
* @file web_games_server.cpp
* @brief エントリポイント定義ファイル
*/


/*******************************************************************************************************************
*
*	インクルード
*
*******************************************************************************************************************/


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

	MSG msg{};

    // アプリケーション初期化の実行:
    if (!InitApp(hInstance, nCmdShow)){
        return FALSE;
    }

    // メイン メッセージ ループ:
    while (true){
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			[]() {
				using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
				namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

				try
				{
					auto const address = tcp::v4();
					auto const port = static_cast<unsigned short>(std::atoi("31400"));

					// The io_context is required for all I/O
					boost::asio::io_context ioc{ 1 };

					// The acceptor receives incoming connections
					tcp::acceptor acceptor{ ioc, {address, port} };

					for (;;){
						// This will receive the new connection
						tcp::socket socket{ ioc };

						// Block until we get a connection
						acceptor.accept(socket);

						// Launch the session, transferring ownership of the socket
						std::thread{ std::bind(
							[](tcp::socket& socket)->void{
								try {

									// Construct the stream by moving in the socket
									websocket::stream<tcp::socket> ws{ std::move(socket) };

									// Accept the websocket handshake
									ws.accept();

									for (;;) {
										// This buffer will hold the incoming message
										boost::beast::multi_buffer buffer;

										// Read a message
										ws.read(buffer);

										// Echo the message back
										ws.text(ws.got_text());
										ws.write(buffer.data());
									}
								}
								catch (boost::system::system_error const& se) {
									// This indicates that the session was closed
									if (se.code() != websocket::error::closed)
										MessageBoxA(NULL, se.code().message().c_str(), "Error", MB_OK);
								}
								catch (std::exception const& e) {
									MessageBoxA(NULL, e.what(), "Error", MB_OK);
								}
							},
							std::move(socket)) 
						}.detach();
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << "Error: " << e.what() << std::endl;
					return EXIT_FAILURE;
				}
			}();
			break;
		}
    }

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
	app::hInstance = hInstance;

	return app::RESULT::SUCCEEDED;
}
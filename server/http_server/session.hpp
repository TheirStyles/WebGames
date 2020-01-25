/**
 * @file session.hpp
 * @brief Sessionクラス定義ファイル
*/

#include "stdafx.h"

namespace app::http_server {

    /**
     * @class Session
     * @brief 通信の一連処理
    */
    class Session : public std::enable_shared_from_this<Session> {
	private:
		/**
		* @struct SendLamda
		* @brief 送信用のラムダ
		*/
		struct SendLamda {
			Session& self;	//!< セッション

			/**
			* @brief コンストラクタ
			* @param self セッションクラス
			*/
			SendLamda(Session& self);

			/**
			* @brief メッセージを送信する
			* @tparam メッセージがリクエストであるか
			* @tparam メッセージのボディーの型
			* @tparam メッセージのフィールドの型
			* @param msg メッセージ
			* @return none
			*/
			template<bool isRequest, class Body, class Fields>
			void operator()(http::message<isRequest, Body, Fields>&& msg) const;
		};

    public:
        Session() = delete;
        Session(const Session&) = default;
        Session(Session&&) = default;
        ~Session() = default;
        Session& operator=(const Session&) = default;
        Session& operator=(Session&&) = default;

        /**
         * @brief コンストラクタ
         * @param socket ソケット
         * @param doc_root ドキュメントルートのパス
        */
        Session(tcp::socket&& socket, const std::shared_ptr<std::string const>& doc_root);

    private:
        beast::tcp_stream stream;                       //!< ストリーム
        beast::flat_buffer buffer;                      //!< 受信用バッファー
        std::shared_ptr<std::string const> doc_root;    //!< ドキュメントルートへのパス
        http::request<http::string_body> req;           //!< リクエスト情報
        std::shared_ptr<void> res;                      //!< 送信するリソース情報
		SendLamda send_lamda;							//!< 送信に利用するラムダ

    private:
        /**
         * @brief Httpヘッダに載せるMimeを取得する
         * @param path リクエストパス情報
         * @return Mime情報
        */
        beast::string_view GetMimeType(beast::string_view path);

        /**
         * @brief パスの結合を行う
         * @param base ベース
         * @param path 追加パス
         * @return 結合結果
        */
        std::string PathCat(beast::string_view base, beast::string_view path);

        /**
         * @brief リクエストハンドラ
         * @tparam Body リクエストBodyの型
         * @tparam Allocator リクエストのアロケーター
         * @tparam send 送信用コールバック関数の型
         * @param doc_root ドキュメントルートのパス
         * @param req リクエスト情報
         * @param send 送信用コールバック関数
         * @return none
        */
		template<typename Body, typename Allocator, typename Send>
		void HandleRequest(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send);

        /**
         * @brief 読み込み処理を開始する
         * @param none
         * @return none
        */
        void DoRead();

        /**
         * @brief 読み込み時の処理
         * @param ec エラー情報
         * @param bytes_transferred 処理されたサイズ
         * @return none
        */
        void OnRead(beast::error_code ec, std::size_t bytes_transferred);

        /**
         * @brief 書き込み処理
         * @param close 終了フラグ
         * @param ec エラー情報
         * @param bytes_transferred 処理されたサイズ
         * @return none
        */
       void OnWrite(bool close, beast::error_code ec, std::size_t bytes_transferred);

        /**
         * @brief セッションを閉じる
         * @param none
         * @return none
        */
        void DoClose();

    public:
        /**
        * @brief セッションを開始する
        * @return none
        */
        void Run();
    };

}
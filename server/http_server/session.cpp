/**
 * @file session.cpp
 * @brief Sessionクラス実装ファイル
*/


#include "stdafx.h"
#include "session.hpp"

namespace app::http_server {

	Session::SendLamda::SendLamda(Session& self) :
		self(self)
	{
	}

	template<bool isRequest, class Body, class Fields>
	void Session::SendLamda::operator()(http::message<isRequest, Body, Fields>&& msg) const {
		//メッセージの寿命を延長
		auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));
		this->self.res = sp;

		//レスポンスを書き込む
		http::async_write(
			this->self.stream,
			*sp,
			beast::bind_front_handler(
				&Session::OnWrite,
				self.shared_from_this(),
				sp->need_eof()
			)
		);
	}

	Session::Session(tcp::socket&& socket, const std::shared_ptr<std::string const>& doc_root) :
		stream		(std::move(socket)),
		doc_root	(doc_root),
		send_lamda	(*this)
	{
	}
	
	beast::string_view Session::GetMimeType(beast::string_view path) {
		using beast::iequals;

		//拡張子を取得
		auto const ext = [&path]{
			auto const pos = path.rfind(".");
			if (pos == beast::string_view::npos)
				return beast::string_view{};
			return path.substr(pos);
		}();

		//拡張子でMimeを判別
		if (iequals(ext, ".htm"))  return "text/html";
		if (iequals(ext, ".html")) return "text/html";
		if (iequals(ext, ".php"))  return "text/html";
		if (iequals(ext, ".css"))  return "text/css";
		if (iequals(ext, ".txt"))  return "text/plain";
		if (iequals(ext, ".js"))   return "application/javascript";
		if (iequals(ext, ".json")) return "application/json";
		if (iequals(ext, ".xml"))  return "application/xml";
		if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
		if (iequals(ext, ".flv"))  return "video/x-flv";
		if (iequals(ext, ".png"))  return "image/png";
		if (iequals(ext, ".jpe"))  return "image/jpeg";
		if (iequals(ext, ".jpeg")) return "image/jpeg";
		if (iequals(ext, ".jpg"))  return "image/jpeg";
		if (iequals(ext, ".gif"))  return "image/gif";
		if (iequals(ext, ".bmp"))  return "image/bmp";
		if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
		if (iequals(ext, ".tiff")) return "image/tiff";
		if (iequals(ext, ".tif"))  return "image/tiff";
		if (iequals(ext, ".svg"))  return "image/svg+xml";
		if (iequals(ext, ".svgz")) return "image/svg+xml";
		return "application/text";
	}

	std::string Session::PathCat(beast::string_view base, beast::string_view path) {
		//baseがない場合、そのままpathを返す
		if (base.empty())
			return std::string(path);

		//結果格納用
		std::string result(base);

		//msvcでのセパレーター
		char constexpr path_separator = '\\';

		//セパレーターで終了している場合、セパレーターを削除
		if (result.back() == path_separator) {
			result.resize(result.size() - 1);
		}

		//パスを追加
		result.append(path.data(), path.size());

		//スラッシュをセパレーターに変換
		for (auto& c : result) {
			if (c == '/') {
				c = path_separator;
			}
		}

		return result;
	}

	template<typename Body, typename Allocator, typename Send>
	void Session::HandleRequest(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {

		//BadRequestを生成する
		auto const bad_request = [&req](beast::string_view why){
			http::response<http::string_body> res{ http::status::bad_request, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = std::string(why);
			res.prepare_payload();
			return res;
		};

		//NotFoundを生成する
		auto const not_found = [&req](beast::string_view target){
			http::response<http::string_body> res{ http::status::not_found, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "The resource '" + std::string(target) + "' was not found.";
			res.prepare_payload();
			return res;
		};

		//InternalServerErrorを生成する
		auto const server_error = [&req](beast::string_view what){
			http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "An error occurred: '" + std::string(what) + "'";
			res.prepare_payload();
			return res;
		};

		//GetかHeadメソッドでない場合、BadRequestを送信
		if (req.method() != http::verb::get &&req.method() != http::verb::head) {
			return send(bad_request("Unknown HTTP-method"));
		}

		//リクエストが空、スラッシュから始まらない、".."を含む場合、BadRequestを送信
		if (req.target().empty() || req.target()[0] != '/' || req.target().find("..") != beast::string_view::npos) {
			return send(bad_request("Illegal request-target"));
		}

		//リクエストにファイルが指定されていない場合、index.htmlを追加
		std::string path = this->PathCat(doc_root, req.target());
		if (req.target().back() == '/') {
			path.append("index.html");
		}

		//ファイルオープン
		beast::error_code ec{};
		http::file_body::value_type body{};
		body.open(path.c_str(), beast::file_mode::scan, ec);

		//ファイルが見つからない場合、NotFoundを返す
		if (ec == beast::errc::no_such_file_or_directory) {
			return send(not_found(req.target()));
		}

		//NotFound以外でエラーが発生した場合、サーバーエラーを送信
		if (ec) {
			return send(server_error(ec.message()));
		}

		//ボディーのサイズを取得
		auto const size = body.size();

		//リクエストがHeadの場合
		if (req.method() == http::verb::head) {
			http::response<http::empty_body> res{ http::status::ok, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, this->GetMimeType(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());
			return send(std::move(res));
		}

		//リクエストがGetの場合
		if (req.method() == http::verb::get) {
			http::response<http::file_body> res(
				std::piecewise_construct,
				std::make_tuple(std::move(body)),
				std::make_tuple(http::status::ok, req.version())
			);
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, this->GetMimeType(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());
			return send(std::move(res));
		}
		
		//ここまで処理が到達することはない、到達した場合エラー
		assert(true);
		return send(server_error(ec.message()));
	}

	void Session::DoRead() {
		//リクエストを空にする
		this->req = {};

		//タイムアウトを30秒設定
		this->stream.expires_after(std::chrono::seconds(30));

		//リクエストを非同期読み込み
		http::async_read(
			this->stream,
			this->buffer,
			this->req,
			beast::bind_front_handler(
				&Session::OnRead,
				this->shared_from_this()
			)
		);
	}

	void Session::OnRead(beast::error_code ec, std::size_t bytes_transferred) {
		//使用しない変数であることを通知
		boost::ignore_unused(bytes_transferred);

		//ストリームがエラーで閉じた場合
		if (ec == http::error::end_of_stream) {
			return this->DoClose();
		}

		//レスポンスを返す
		this->HandleRequest(*this->doc_root, std::move(req), this->send_lamda);
	}

	void Session::OnWrite(bool close, beast::error_code ec, std::size_t bytes_transferred) {
		//使用しない変数であることを通知
		boost::ignore_unused(ec);
		boost::ignore_unused(bytes_transferred);

		//通信を終了する場合、ストリームを閉じる
		if (close) {
			return this->DoClose();
		}

		//リソースを空にする
		this->res = nullptr;

		//再び読み込み開始
		this->DoRead();
	}

	void Session::DoClose() {
		beast::error_code ec{};
		this->stream.socket().shutdown(tcp::socket::shutdown_send, ec);
	}

	void Session::Run() {
		//メッセージ処理を実行する
		net::dispatch(
			this->stream.get_executor(),
			beast::bind_front_handler(
				&Session::DoRead,
				this->shared_from_this()
			)
		);
	}
}
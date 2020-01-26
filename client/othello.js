/*****************************************************************************************************
 * 
 *  グローバル変数 
 * 
 ****************************************************************************************************/
/**
 * @var area
 * @breif ゲームエリアのDOM要素
 */
var area = document.getElementById("game_area");

 /**
 * @var canvas
 * @brief キャンバスのDOM要素
 */
var canvas = document.getElementById("canvas");

/**
 * @var ctx
 * @brief コンテキスト
 */
var ctx = canvas.getContext("2d");

/**
 * @var socket
 * @brief ソケット
 */
var socket = new WebSocket('wss://'+ "echo.websocket.org");
//const SERVER_URL = 'ws://' + location.host + ':8080';

/**
 * @var nowSceneDraw
 * @brief 現在のシーンの描画処理(再描画用)
 */
var nowSceneDraw = function(){};

/**
 * @var nickname
 * @brief ニックネーム
 */
var nickname = "no name";

/*****************************************************************************************************
 * 
 *  列挙体
 * 
 ****************************************************************************************************/
/**
 * @brief 盤面の状態情報
 */
 var OthelloState = {
    blank : 0,
    black : 1,
    white : 2
};


/*****************************************************************************************************
 * 
 *  サブルーチン
 * 
 ****************************************************************************************************/
/**
 * @brief キャンバスのコンテキストサイズを表示サイズに合わせる
 * @param canvas ターゲットキャンバス 
 */
 function canvasResize(canvas){
    // ブラウザがcanvasを表示しているサイズを調べる。
    let displayWidth  = canvas.clientWidth;
    let displayHeight = canvas.clientHeight;

    // canvasの「描画バッファーのサイズ」と「表示サイズ」が異なるかどうか確認する。
    if (canvas.width  != displayWidth ||canvas.height != displayHeight) {
    
        // サイズが違っていたら、描画バッファーのサイズを
        // 表示サイズと同じサイズに合わせる。
        canvas.width  = displayWidth;
        canvas.height = displayHeight;
    }
 }

/**
 * @brief ファイルパスから画像読み込み表示する
 * @param imagePath 画像へのパス
 * @param x 表示先のx座標
 * @param y 表示先のy座標
 * @param width 画像の横幅
 * @param height 画像の縦幅
 * @param onLoad 読み込み後に追加でしたい処理
 * @return 画像オブジェクト
 */
function drawImageFromPath(imagePath, x, y, width, height, onLoad=function(){}){
    //画像オブジェクト生成
    const image = new Image();

    //画像読み込みが完了したら開始するようにイベントを設定
    image.addEventListener("load", function(){
        //画像描画
        ctx.drawImage(image, x, y, width, height);

        //追加処理
        onLoad();
    });

    //画像読み込み
    image.src = imagePath;

    //画像オブジェクトを返す
    return image;
}

/**
 * @brief 画像オブジェクトで画像を表示する
 * @param image 画像
 * @param x 表示先のx座標
 * @param y 表示先のy座標
 * @param width 画像の横幅
 * @param height 画像の縦幅
 * @return 画像オブジェクト
 */
function drawImageFromObject(image, x, y, width, height){
    //画像描画
    ctx.drawImage(image, x, y, width, height);

    //画像オブジェクトを返す
    return image;
}

/**
 * @breif キャンバスの明暗を変更する
 * @param diff 加算する値
 * @return none 
 */
function setCanvasShade(diff){
    //キャンバスのピクセル取得
    var pixels = ctx.getImageData(0, 0, canvas.width, canvas.height);
    var d = pixels.data;
    
    //rgbを加算
    for (var i = 0; i < d.length; i+=4) {
        d[i]   += diff;  // r
        d[i+1] += diff;  // g
        d[i+2] += diff;  // b
    }

    //ピクセルデータを上書き
    ctx.putImageData(pixels, 0, 0);
}

/**
 * @breif キャンバスの一部分の明暗を変更する
 * @param diff 加算する値
 * @param x 左上のx座標
 * @param y 左上のy座標
 * @param width 変更する範囲の横幅
 * @param height 変更する範囲の縦幅
 * @return none 
 */
function setPartOfCanvasShade(diff, x, y, width, height){
    //キャンバスのピクセル取得
    var pixels = ctx.getImageData(x, y, width, height);
    var d = pixels.data;
    
    //rgbを加算
    for (var i = 0; i < d.length; i+=4) {
        d[i]   += diff;  // r
        d[i+1] += diff;  // g
        d[i+2] += diff;  // b
    }

    //ピクセルデータを上書き
    ctx.putImageData(pixels, x, y);
}

/**
 * @brief 盤面を表示する
 * @param board 盤面の情報
 */
function drawOthelloBoard(board){
    for(var i = 0; i < 8; ++i)
    for(var j = 0; j < 8; ++j) {
    }
}


/*****************************************************************************************************
* 
*  スタート画面処理
* 
****************************************************************************************************/
/**
 * @var error_back_button
 * @brief エラー時にトップに戻るボタン
 */
var error_back_button = null;

/**
 * @brief エラー画面を表示する
 * @param msg エラー内容
 * @return none
 */
function displayError(msg = "エラーが発生しました"){
    //再描画用の関数設定
    nowSceneDraw = displayError;

    //背景画像描画
    drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
        //描画完了後
        function() {
            //背景にフィルター
            setCanvasShade(-100);

            //タイトル描画
            ctx.font = 'bold 20pt sans-serif';
            ctx.fillStyle = "white";
            ctx.textAlign = "center";
            ctx.fillText(msg, canvas.width / 2, 150);
        }
    );

    //resize時に呼ばれたとき、前に配置したボタンが残っていれば
    if(error_back_button != null){
        error_back_button.style.display = "none";
    }

    //トップへのボタン配置
    error_back_button = document.createElement("BUTTON");
    error_back_button.innerText        = "トップに戻る";
    error_back_button.style.position   = "absolute";
    error_back_button.style.top        = canvas.height - 200 + "px";
    error_back_button.classList.add("btn");
    error_back_button.classList.add("btn-primary");
    error_back_button.classList.add("btn-lg");
    error_back_button.classList.add("active");
    error_back_button.addEventListener('click', function(){
        //ボタン削除
        error_back_button.style.display = "none";
        
        //次のシーンを呼び出す
        displayStartScene();
    })
    area.appendChild(error_back_button);

    //ボタンの横位置を調整 clientWidthを使うため、描画後に適用
    error_back_button.style.left  = canvas.width/2 - error_back_button.clientWidth/2 + 10 + "px"; 
}



/*****************************************************************************************************
* 
*  スタート画面処理
* 
****************************************************************************************************/
/**
 * @var start_button
 * @brief スタート画面のボタン
 */
var start_button = null;

/**
 * @var displayStartSceneDOMClear
 * @brief スタート画面のDOMを全て削除する
 */
function displayStartSceneClear() {
    if(start_button != null){
        start_button.style.display = "none";
        start_button = null;
    }
}

/**
 * @brief スタート画面を表示する
 * @param none
 * @return none
 */
function displayStartScene(){
    //再描画用の関数設定
    nowSceneDraw = function() {
        //再描画時に残る、DOMをクリアしておく
        displayStartSceneClear();

        //再描画
        displayStartScene();
    }

    //背景画像描画
    drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
        //描画完了後
        function() {
            //背景にフィルター
            setCanvasShade(-100);

            //タイトル描画
            ctx.font = 'bold 30pt sans-serif';
            ctx.fillStyle = "white";
            ctx.textAlign = "center";
            ctx.fillText("オセロゲーム", canvas.width / 2, 150);
        }
    );

    //開始ボタン配置
    start_button = document.createElement("BUTTON");
    start_button.innerText        = "Start Game";
    start_button.style.position   = "absolute";
    start_button.style.top        = canvas.height - 200 + "px";
    start_button.classList.add("btn");
    start_button.classList.add("btn-primary");
    start_button.classList.add("btn-lg");
    start_button.classList.add("active");
    start_button.addEventListener('click', function(){
        //DOM削除
        displayStartSceneClear();
        
        //次のシーンを呼び出す
        displayNicknameScene();
    })
    area.appendChild(start_button);

    //ボタンの横位置を調整 clientWidthを使うため、描画後に適用
    start_button.style.left  = canvas.width/2 - start_button.clientWidth/2 + 10 + "px"; 
}



/*****************************************************************************************************
* 
*  ニックネーム入力処理
* 
****************************************************************************************************/
/**
 * @var nickname_input_form
 * @brief ニックネーム入力フォーム
 */
var nickname_input_form = null;

/**
 * @var nickname_submit
 * @brief ニックネーム決定ボタン
 */
var nickname_submit = null;

/**
 * @brief ニックネーム入力画面のDOMをすべて削除する
 * @param none
 * @return none
 */
function displayNicknameSceneDOMClear() {
    if(nickname_input_form != null){
        nickname_input_form.style.display = "none";
        nickname_input_form = null;
    }
    if(nickname_submit != null){
        nickname_submit.style.display = "none";
        nickname_submit = null;
    }
}

/**
 * @brief ニックネーム入力画面を表示する
 * @param none
 * @return none
 */
function displayNicknameScene(){
    //再描画用の関数設定
    nowSceneDraw = function() {
        displayNicknameSceneDOMClear();
        displayNicknameScene();
    }

    //背景画像描画
    drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
        //描画完了後
        function() {
            //背景にフィルター
            setCanvasShade(-100);

            //タイトル描画
            ctx.font = 'bold 20pt sans-serif';
            ctx.fillStyle = "white";
            ctx.textAlign = "center";
            ctx.fillText("ニックネームを入力してください", canvas.width / 2, 150);
        }
    );

    //入力フォーム配置
    nickname_input_form = document.createElement("INPUT");
    nickname_input_form.style.position   = "absolute";
    nickname_input_form.style.top        = canvas.height - 280 + "px";
    nickname_input_form.style.width      = canvas.clientWidth * 4/5 + "px";
    nickname_input_form.setAttribute("type", "text");
    nickname_input_form.setAttribute("value", nickname);
    nickname_input_form.classList.add("form-control");
    area.appendChild(nickname_input_form);
    nickname_input_form.style.left  = canvas.width*1/8 + "px"; 

    //ボタン配置
    nickname_submit = document.createElement("BUTTON");
    nickname_submit.innerText        = "決定";
    nickname_submit.style.position   = "absolute";
    nickname_submit.style.top        = canvas.height - 200 + "px";
    nickname_submit.style.width      = canvas.clientWidth/5 + "px";
    nickname_submit.classList.add("btn");
    nickname_submit.classList.add("btn-primary");
    nickname_submit.classList.add("btn-lg");
    nickname_submit.classList.add("active");
    nickname_submit.addEventListener('click', function(){
        //入力チェック
        if(nickname_input_form.value == ""){
            alert("ニックネームが入力されていません。");
            return;
        }

        //ニックネーム保存
        nickname = nickname_input_form.value;

        //DOM削除
        displayNicknameSceneDOMClear();

        //ニックネームデータをサーバーに送信
        socket.send("NICKNAME : " + nickname);
        
        //次のシーンを呼び出す
        displaySelectScene();
    })
    area.appendChild(nickname_submit);
    nickname_submit.style.left  = canvas.width/2 - nickname_submit.clientWidth/2 + 10 + "px"; 
}


/*****************************************************************************************************
* 
*  選択画面処理
* 
****************************************************************************************************/
/**
 * @var select_make_room
 * @brief 選択ボタン1 部屋の作成
 */
var select_make_room = null;

/**
 * @var select_join_room
 * @brief 選択ボタン2 部屋に参加
 */
var select_join_room = null;

/**
 * @var select_back_button
 * @brief 戻るボタン
 */
var select_back_button = null;

/**
 * @brief 選択画面のDOMをすべて削除する
 * @param none
 * @return none
 */
function displaySelectSceneDOMClear(){
    if(select_make_room != null){
        select_make_room.style.display = "none";
        select_make_room = null;
    }
    if(select_join_room != null){
        select_join_room.style.display = "none";
        select_join_room = null;
    }
    if(select_back_button != null){
        select_back_button.style.display = "none";
        select_back_button = null;
    }
}

/**
 * @brief 選択画面を表示する
 * @param none
 * @return none
 */
function displaySelectScene(){
    //再描画用の関数設定
    nowSceneDraw = function() {
        displaySelectSceneDOMClear();
        displaySelectScene();
    }

    //背景画像描画
    drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
        //描画完了後
        function() {
            //背景にフィルター
            setCanvasShade(-100);

            //タイトル描画
            ctx.font = 'bold 30pt sans-serif';
            ctx.fillStyle = "white";
            ctx.textAlign = "center";
            ctx.fillText("参加方法", canvas.width / 2, 150);
        }
    );

    //ボタンサイズ
    let width = canvas.width * 8/10;

    //部屋の作成ボタンを配置
    select_make_room = document.createElement("BUTTON");
    select_make_room.innerText        = "部屋を作成する";
    select_make_room.style.position   = "absolute";
    select_make_room.style.top        = canvas.height - 300 + "px";
    select_make_room.style.width      = width + "px";
    select_make_room.classList.add("btn");
    select_make_room.classList.add("btn-primary");
    select_make_room.classList.add("btn-lg");
    select_make_room.classList.add("active");
    select_make_room.addEventListener('click', function(){
        //ボタン削除
        displaySelectSceneDOMClear();
        
        //次のシーンを呼び出す
        displayMakeRoomScene();
    })
    area.appendChild(select_make_room);
    select_make_room.style.left  = canvas.width/2 - select_make_room.clientWidth/2 + 10 + "px"; 
    
    //部屋への参加ボタンを配置
    select_join_room = document.createElement("BUTTON");
    select_join_room.innerText        = "部屋を探す";
    select_join_room.style.position   = "absolute";
    select_join_room.style.top        = canvas.height - 200 + "px";
    select_join_room.style.width      = width + "px";
    select_join_room.classList.add("btn");
    select_join_room.classList.add("btn-primary");
    select_join_room.classList.add("btn-lg");
    select_join_room.classList.add("active");
    select_join_room.addEventListener('click', function(){
        //ボタン削除
        displaySelectSceneDOMClear();
        
        //次のシーンを呼び出す
        displaySearchRoomScene()
    })
    area.appendChild(select_join_room);
    select_join_room.style.left  = canvas.width/2 - select_join_room.clientWidth/2 + 10 + "px"; 

    //戻るボタンを配置
    select_back_button = document.createElement("BUTTON");
    select_back_button.innerText        = "戻る";
    select_back_button.style.position   = "absolute";
    select_back_button.style.top        = canvas.height - 100 + "px";
    select_back_button.classList.add("btn");
    select_back_button.classList.add("btn-warning");
    select_back_button.classList.add("btn-lg");
    select_back_button.classList.add("active");
    select_back_button.addEventListener('click', function(){
        //ボタン削除
        displaySelectSceneDOMClear();
        
        //次のシーンを呼び出す
        displayNicknameScene();
    })
    area.appendChild(select_back_button);
    select_back_button.style.left  = canvas.width/2 - select_back_button.clientWidth/2 + 10 + "px"; 
}



/*****************************************************************************************************
* 
*  部屋の作成処理
* 
****************************************************************************************************/
/**
 * @var make_room_back_button
 * @brief 戻るボタン
 */
var make_room_back_button = null;

/**
 * @var make_room_join_button
 * @brief 決定ボタン
 */
var make_room_join_button = null;

/**
 * @var match_player_message
 * @brief 再描画時にメッセージを再利用するための変数
 */
var match_player_message = null;

/**
 * @brief 部屋の作成画面のDOMを全て削除する
 * @param none
 * @return none
 */
function displayMakeRoomDOMClear() {
    if(make_room_back_button != null) {
        make_room_back_button.style.display = "none";
        make_room_back_button = null;
    }
    if(make_room_join_button != null){
        make_room_join_button.style.display = "none";
        make_room_join_button = null;
    }
}

/**
 * @brief 部屋の作成画面を表示する
 * @param none
 * @return none
 */
function displayMakeRoomScene(){
    //再描画用に描画処理を関数化
    let drawBackground = function() {
        //背景画像描画
        drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
            //描画完了後
            function() {
                //背景にフィルター
                setCanvasShade(-100);

                //タイトル描画
                ctx.font = 'bold 20pt sans-serif';
                ctx.fillStyle = "white";
                ctx.textAlign = "center";
                ctx.fillText("相手の参加待ちを行っています", canvas.width / 2, 150);
            }
        );
    };
    //再描画用に戻るボタンの作成を関数化
    let createBackButton = function(){
        //戻るボタンを配置
        make_room_back_button = document.createElement("BUTTON");
        make_room_back_button.innerText        = "戻る";
        make_room_back_button.style.position   = "absolute";
        make_room_back_button.style.top        = canvas.height - 100 + "px";
        make_room_back_button.classList.add("btn");
        make_room_back_button.classList.add("btn-warning");
        make_room_back_button.classList.add("btn-lg");
        make_room_back_button.classList.add("active");
        make_room_back_button.addEventListener('click', function(){
            //ボタン削除
            displayMakeRoomDOMClear();
            
            //次のシーンを呼び出す
            displaySelectScene();
        })
        area.appendChild(make_room_back_button);
        make_room_back_button.style.left  = canvas.width/2 - make_room_back_button.clientWidth/2 + 10 + "px"; 
    };

    //描画
    drawBackground();
    createBackButton();

    //再描画用の関数設定
    nowSceneDraw = function() {
        displayMakeRoomDOMClear();
        drawBackground();
        createBackButton();
    }

    //ソケットの受信時設定
    let onMessage = function(e=null){
        //再描画用に登録
        nowSceneDraw = function() {    
            displayMakeRoomDOMClear();
            createBackButton();
            onMessage();
        };

        //再描画用の記録変数
        match_player_message = (e == null ? match_player_message : e.data);

        //参加相手が見つかったことを通知
        drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
            //描画完了後
            function() {
                //背景にフィルター
                setCanvasShade(-100);

                //タイトル描画
                ctx.font = 'bold 20pt sans-serif';
                ctx.fillStyle = "white";
                ctx.textAlign = "center";
                ctx.fillText("対戦相手が見つかりました", canvas.width / 2, 150);

                //テキスト表示部分を暗くする
                setPartOfCanvasShade(-50, canvas.width/10, canvas.height/3 + canvas.height/10, canvas.width * 8/10, canvas.height/5);

                //相手情報表示
                ctx.fillText(match_player_message, canvas.width / 2, 260);
            }
        );

        //戻るボタン位置変更
        make_room_back_button.style.left = canvas.width/4 - make_room_back_button.clientWidth/2 + 10 + "px"; 
        
        //決定ボタン追加
        make_room_join_button = document.createElement("BUTTON");
        make_room_join_button.innerText        = "決定";
        make_room_join_button.style.position   = "absolute";
        make_room_join_button.style.top        = canvas.height - 100 + "px";
        make_room_join_button.classList.add("btn");
        make_room_join_button.classList.add("btn-info");
        make_room_join_button.classList.add("btn-lg");
        make_room_join_button.classList.add("active");
        make_room_join_button.addEventListener('click', function(){
            //ボタン削除
            displayMakeRoomDOMClear();
            
            //次のシーンを呼び出す
            //TODO
        })
        area.appendChild(make_room_join_button);
        make_room_join_button.style.left  = canvas.width*3/4 - make_room_join_button.clientWidth/2 + 10 + "px"; 
    }
    socket.onmessage = onMessage;

    //切断時
    socket.onclose = function(e){
        //ボタン削除
        displayMakeRoomDOMClear();

        //エラー表示へ
        displayError("サーバーとの接続が途切れました");
    };

    //エラー時
    socket.onerror = function(error){
        //ボタン削除
        displayMakeRoomDOMClear();

        //エラー表示へ
        displayError("サーバーとの接続中にエラーが発生しました");
    };

    //部屋作成をサーバーに通知
    socket.send("SELECT : MakeRoom");
}



/*****************************************************************************************************
* 
*  部屋の参加処理
* 
****************************************************************************************************/
/**
 * @var search_room_back_button
 * @brief 戻るボタン
 */
var search_room_back_button = null;

/**
 * @var search_room_players_list
 * @brief 各プレイヤー情報を表示するリスト
 */
var search_room_players_list = null;

/**
 * @var serach_player_message
 * @brief 再描画時にメッセージを再利用するための変数
 */
var serach_player_message = null;

/**
 * @brief 部屋の参加画面のDOMを全て削除する
 * @param none
 * @return none
 */
function displaySearchRoomDOMClear() {
    if(search_room_back_button != null) {
        search_room_back_button.style.display = "none";
        search_room_back_button = null;
    }
    if(search_room_players_list != null){
        search_room_players_list.style.display = "none";
        search_room_players_list = null;
    }
}

/**
 * @brief 部屋の参加画面を表示する
 * @param none
 * @return none
 */
function displaySearchRoomScene(){
     //再描画用に描画処理を関数化
     let drawBackground = function() {
        //背景画像描画
        drawImageFromPath("./img/start_background.jpg", 0, 0, canvas.width, canvas.height,
            //描画完了後
            function() {
                //背景にフィルター
                setCanvasShade(-100);

                //タイトル描画
                ctx.font = 'bold 20pt sans-serif';
                ctx.fillStyle = "white";
                ctx.textAlign = "center";
                ctx.fillText("現在の部屋リスト", canvas.width / 2, 150);
            }
        );
    };
    //再描画用に戻るボタンの作成を関数化
    let createBackButton = function(){
        //戻るボタンを配置
        search_room_back_button = document.createElement("BUTTON");
        search_room_back_button.innerText        = "戻る";
        search_room_back_button.style.position   = "absolute";
        search_room_back_button.style.top        = canvas.height - 100 + "px";
        search_room_back_button.classList.add("btn");
        search_room_back_button.classList.add("btn-warning");
        search_room_back_button.classList.add("btn-lg");
        search_room_back_button.classList.add("active");
        search_room_back_button.addEventListener('click', function(){
            //ボタン削除
            displaySearchRoomDOMClear();
            
            //次のシーンを呼び出す
            displaySelectScene();
        })
        area.appendChild(search_room_back_button);
        search_room_back_button.style.left  = canvas.width/2 - search_room_back_button.clientWidth/2 + 10 + "px"; 
    };

    //描画
    drawBackground();
    createBackButton();

    //再描画用の関数設定
    nowSceneDraw = function() {
        displaySearchRoomDOMClear();
        drawBackground();
        createBackButton();
    }

    //ソケットの受信時設定
    let onMessage = function(e=null){
        //再描画用に登録
        nowSceneDraw = function() {    
            displaySearchRoomDOMClear();
            drawBackground();
            createBackButton();
            onMessage();
        };

        //リストを更新
        if(e!=null){
            //メッセージの種類を切り出し
            let temp = e.data.split(':');
            if(temp[1]==undefined){
                return;
            }

            //リストに分割
            serach_player_message = temp[1].split(',');
        }

        //リストのDOM作成
        search_room_players_list = document.createElement("UL");
        search_room_players_list.style.position     = "absolute";
        search_room_players_list.style.width        = canvas.width * 8/10 + "px";
        search_room_players_list.style.top          = "200px";
        search_room_players_list.classList.add("list-group");
        area.appendChild(search_room_players_list);

        //作成したリストDOMに要素を追加する
        serach_player_message.forEach(function(value){
            //リストの要素DOM作成
            let item = document.createElement("LI");
            item.classList.add("list-group-item");

            //名前追加
            item.innerText = value;

            //決定ボタン追加
            let made_button = document.createElement("BUTTON");
            made_button.innerText = "決定";
            made_button.classList.add("btn");
            made_button.classList.add("btn-info");
            made_button.classList.add("btn-sm");
            made_button.classList.add("active");
            made_button.classList.add("float-right");
            made_button.setAttribute("value", value);
            made_button.addEventListener(
                'click', 
                {
                    send_value: value, 
                    handleEvent : function(event){
                        //ボタン削除
                        displaySearchRoomDOMClear();
                        
                        //結果送信
                        socket.send("TargetPlayer : " + this.send_value);

                        //次のシーンを呼び出す
                        waitMatchPlayerScene();
                }
            });
            item.appendChild(made_button);

            //リスト要素をリストに追加
            search_room_players_list.appendChild(item);
        });

        //要素が追加された後で位置調整する
        search_room_players_list.style.left = canvas.width/2 - search_room_players_list.clientWidth/2 + 10 + "px";
    }
    socket.onmessage = onMessage;

    //切断時
    socket.onclose = function(e){
        //ボタン削除
        displaySearchRoomDOMClear();

        //エラー表示へ
        displayError("サーバーとの接続が途切れました");
    };

    //エラー時
    socket.onerror = function(error){
        //ボタン削除
        displaySearchRoomDOMClear();

        //エラー表示へ
        displayError("サーバーとの接続中にエラーが発生しました");
    };

    //部屋作成をサーバーに通知
    socket.send("SELECT : JoinRoom");
}



/*****************************************************************************************************
* 
*  相手プレイヤーの処理待機
* 
****************************************************************************************************/
/**
 * @brief  相手プレイヤーの入力待機画面を表示する
 * @param none
 * @return none
 */
function waitMatchPlayerScene(){

}


/*****************************************************************************************************
* 
*  開始処理
* 
****************************************************************************************************/
//ウィンドウ変更時のイベント登録
window.addEventListener('resize', function(){
    //コンテキストサイズ調整
    canvasResize(canvas);

    //再描画
    nowSceneDraw();
});

//キャンバス内部サイズを調整
canvasResize(canvas);

//処理開始
displayStartScene();
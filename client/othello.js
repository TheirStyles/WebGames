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
var socket = new WebSocket('ws://'+ location.host + ':8080');

/**
 * @var nowSceneDraw
 * @brief 現在のシーンの描画処理(再描画用)
 */
var nowSceneDraw = function(){};

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
* シーン処理関数
* 
****************************************************************************************************/

/**
 * @brief 部屋の作成を行う
 */
function createRoom() {

}

/**
 * @brief 部屋情報を表示する
 */
function getRoom() {

}

/**
 * @brief メニュー画面を表示する
 */
function displaySelectMenu() {

    drawImage("img/menu_board.png", 0, 0, canvas.width, canvas.height);
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
 * @brief スタート画面を表示する
 * @param none
 * @return none
 */
function displayStart(){
    //再描画用の関数設定
    nowSceneDraw = displayStart;

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

    //resize時に呼ばれたとき、前に配置したボタンが残っていれば
    if(start_button != null){
        start_button.style.display = "none";
    }

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
        //ボタン削除
        start_button.style.display = "none";
        
        //次のシーンを呼び出す
        displaySelectScene();
    })
    area.appendChild(start_button);

    //ボタンの横位置を調整 clientWidthを使うため、描画後に適用
    start_button.style.left  = canvas.width/2 - start_button.clientWidth/2 + 10 + "px"; 
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
 * @brief 選択画面を表示する
 * @param none
 * @return none
 */
function displaySelectScene(){
    //再描画用の関数設定
    nowSceneDraw = displayStart;

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
    
    //resize時に呼ばれたとき、前に配置したボタンが残っていれば
    if(select_make_room != null){
        select_make_room.style.display = "none";
    }
    if(select_join_room != null){
        select_join_room.style.display = "none";
    }

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
        select_make_room.style.display = "none";
        select_join_room.style.display = "none";
        
        //次のシーンを呼び出す
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
        select_make_room.style.display = "none";
        select_join_room.style.display = "none";
        
        //次のシーンを呼び出す
    })
    area.appendChild(select_join_room);
    select_join_room.style.left  = canvas.width/2 - select_join_room.clientWidth/2 + 10 + "px"; 
}


/*****************************************************************************************************
* 
*  実行処理
* 
****************************************************************************************************/
//ウィンドウ変更時のイベント登録
window.addEventListener('resize', function(){
    canvasResize(canvas);
    nowSceneDraw();
});

//キャンバスサイズを調整
canvasResize(canvas);

//処理開始
displayStart();
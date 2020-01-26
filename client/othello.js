/*****************************************************************************************************
 * 
 *  グローバル変数 
 * 
 ****************************************************************************************************/
/**
 * @var canvas
 * @brief キャンバスのDOM要素
 */
const canvas = document.getElementById("canvas");

/**
 * @var socket
 * @brief ソケット
 */
const socket = new WebSocket('ws://'+ location.host + ':8080');


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
 * @brief ファイルパスから画像を表示する
 * @param canvas canvasのDOM(表示先)
 * @param imagePath 画像へのパス
 * @param x 表示先のx座標
 * @param y 表示先のy座標
 * @param width 画像の横幅
 * @param height 画像の縦幅
 * @return 画像オブジェクト
 */
function drawImage(canvas, imagePath, x, y, width, height){
    //画像オブジェクト生成
    const image = new Image();

    //画像読み込みが完了したら開始するようにイベントを設定
    image.addEventListener("load", function(){
        
        //コンテキスト取得
        const ctx = canvas.getContext("2d");

        //画像描画
        ctx.drawImage(image, x, y, width, height);
    });

    //画像読み込み
    image.src = imagePath;

    //画像オブジェクトを返す
    return image;
}

/**
 * @brief 盤面を表示する
 * @param board 盤面の情報
 */
function drawOthelloBoard(){

}

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

    drawImage(canvas, "img/menu_board.png", 0, 0, canvas.width, canvas.height);
}

 /*****************************************************************************************************
 * 
 *  実行処理
 * 
 ****************************************************************************************************/
displaySelectMenu();
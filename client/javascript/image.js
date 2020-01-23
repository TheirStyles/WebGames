/**
 * @file image.js
 * @brief 画像の取扱いに対する処理を定義する
 */



 /*****************************************************************************************************
 * 
 *  クラス
 * 
 ****************************************************************************************************/
/**
 * @class DrawImageInfo
 * @brief 画像の表示情報
 */
class DrawImageInfo {
    constructor(x, y) {
        this.sx = 0;
        this.sy = 0;
        this.sWidth = 0;
        this.sHeight = 0;
        this.dx = x;
        this.dy = y;
        this.dWidth = 0;
        this.dHeight = 0;
    }
    
    constructor(x, y, width, height){
        this.sx = 0;
        this.sy = 0;
        this.sWidth = 0;
        this.sHeight = 0;
        this.dx = x;
        this.dy = y;
        this.dWidth = width;
        this.dHeight = height;
    }

    constructor(sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight){
        this.sx = sx;
        this.sy = sy;
        this.sWidth = sWidth;
        this.sHeight = sHeight;
        this.dx = dx;
        this.dy = dy;
        this.dWidth = dWidth;
        this.dHeight = dHeight;
    }
}

 /*****************************************************************************************************
 * 
 *  関数
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
 * @return none
 */
export function drawImage(canvas, imagePath, x, y, width, height){
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
}
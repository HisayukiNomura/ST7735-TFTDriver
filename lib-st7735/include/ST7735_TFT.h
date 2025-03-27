// --------------------------------------------------------------------------
// ST7735-library (declaration)
//
// Author: Bernhard Bablok
//
// The code is based on work from Gavin Lyons, see
// https://github.com/gavinlyonsrepo/pic_16F18346_projects
//
// https://github.com/bablokb/pic-st7735
// --------------------------------------------------------------------------
#pragma once 
#ifndef __ST7735_TFT_H__
#define __ST7735_TFT_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#if defined _PTC
  #include "picconfig.h"
#endif

#include "ST7735_commands.h"
#include "ST7735_initcmd.h"
#include "ST7735_struct.h"
#include "TextFonts.h"

/// @brief フォント機能を有効にするかのフラグ。
/// @details このシンボルを定義すると、フォント拡張機能が有効になる。<br/>
/// 未定義の場合、デフォルトのフォントとして、TextFonts.hで定義されている5x7の字体が自動的に使用される。<br>
/// 有効の場合、フォントはGFXfont構造体へのポインタを、setFontメソッドで指定することができる。
#define TFT_ENABLE_FONTS


/// @brief ビットマップ描画機能を有効にするかのフラグ
/// @details このシンボルを定義すると、ビットマップ描画の関数を使用できる。
/// RGBベタ（565)の配列を用意し、それを任意の場所に表示できる。
#define TFT_ENABLE_BITMAP


/// @brief 漢字フォント機能を有効にするかのフラグ
/// @details このシンボルを定義すると、漢字機能が有効になり、drawKanjiメソッドが使用できるようになる。
/// このシンボルが有効になると、漢字フォントとしてKanjiHelper.h内で字体のデータがincludeされる。
/// 使用されるフォントサイズや、文字セットにより大きさが異なるが、80kBytes～250kBytes程度のメモリを
/// フラッシュ側（プログラムメモリ）で消費する
#define TFT_ENABLE_KANJI

/// @brief 半角カナ文字の扱いを決定する
/// @details 通常Unicodeのエディタ（VS Code)では、半角カナ文字をefbda1～efbdffまでの3バイト文字セットと
/// して格納し、コンパイル時もそれが使用される。多バイト文字での半角表現は、JISコードには含まれていないため、
/// UTF-8からJISへの変換で範囲外となってしまい、表示できない。
/// このフラグを有効にすると、半角カナ文字（UTF:efbda1～efbdff)を、１バイト文字コード(a1～ff)として扱う
/// ことで、UTF-8表現エディタで作成・コンパイルしたプログラムで半角カナを正しく表示できるようになる。
#define TFT_FORCE_HANKANA

// #define TFT_ENABLE_GENERIC

#define TFT_ENABLE_BLACK		// ?
#define TFT_ENABLE_RESET		// ?

/// @brief テキスト表示を有効にするかのフラグ。
/// @details このシンボルを定義すると、文字表示が有効になる。TFT_ENABLE_TEXTが定義されていないと、フォント
/// 表示も日本語表示も組み込まれないため使用できない。
#define TFT_ENABLE_TEXT

/// @brief 図形描画機能を有効にするかのフラグ
/// @details 直線、矩形、角丸矩形、円、三角形の表示関数が使用できるようになる。
#define TFT_ENABLE_SHAPES
/// @brief 画面の回転機能を有効にするかのフラグ
/// @details このシンボルを有効にすると、画面を90/180/270度回転できる。
#define TFT_ENABLE_ROTATE
/// @brief 画面のスクロール機能を有効にするかのフラグ
/// @details このシンボルを有効にすると、画面のスクロール関連関数が使用できるようになる。
#define TFT_ENABLE_SCROLL
/// @brief 図形表示をすべて有効にするかのフラグ
/// @details このシンボルを有効にすると、以下のすべての図形表示が有効になる。
/// （漢字表示 TFT_ENABLE_KANJI と、フォント機能　TFT_ENABLE_FONTSは有効にならない。個別に有効化が必要）<br/>
/// - TFT_ENABLE_SHAPES ... 直線、矩形、角丸矩形、円、三角形の表示
/// - TFT_ENABLE_SCROLL ... 画面のスクロール
/// - TFT_ENABLE_TEXT   ... 画面のスクロール
/// - TFT_ENABLE_TEXT   ... テキスト表示機能
/// - TFT_ENABLE_ROTATE ...　画面の回転
#define TFT_ENABLE_ALL

/*
#if defined TFT_ENABLE_FONTS
  #if !defined TFT_ENABLE_TEXT
    #define TFT_ENABLE_TEXT
  #endif
  #if !defined PROGMEM
    #define PROGMEM
  #endif
#endif
*/

#if defined TFT_ENABLE_ALL
  #if !defined TFT_ENABLE_SHAPES
    #define TFT_ENABLE_SHAPES
  #endif
  #if !defined TFT_ENABLE_SCROLL
    #define TFT_ENABLE_SCROLL
  #endif
  #if !defined TFT_ENABLE_TEXT
    #define TFT_ENABLE_TEXT
  #endif
  #if !defined TFT_ENABLE_ROTATE
    #define TFT_ENABLE_ROTATE
  #endif
#endif

// extern uint8_t tft_width, tft_height;


/// @brief 画面表示のクラス。インスタンス化して使用する。
class ST7735 {
   private:
	static const uint8_t ASCII_OFFSET = 0x20;  // ASCII フォントの字体テーブルの開始文字。0x20
	
	bool bTextWrap = true;

	bool isTransparentColor = false;
	/// @brief 透過色の設定。isTransparentColor がtrueの時に有効
	/// @details ここで指定された色は、ビットマップ描画のときに透明として扱われる。使用しない場合は、is
	uint16_t bmpTransparentColor;

   public:
	HW* pSpiHW;  // ハードウェア関連
	ST7735Init st7735Init;
	private:
	


   public:

	public:
#pragma region コンストラクタ
	/**
	 * @brief グラフィックライブラリのコンストラクタ
	 * @details グラフィックライブラリのクラスを作成する。使用するには、new してインスタンス化する必要がある。
	 */
	 ST7735();
	 /**
	  * @brief グラフィックライブラリのコンストラクタ
	  * @details グラフィックライブラリのクラスを作成する。使用するには、new してインスタンス化する必要がある。
	  * @param spiHW ハードウェアにアクセスするための情報を持っているクラスへのポインタ 
	  */
	 ST7735(HW *a_spiHW);
	 /**
	  * @brief グラフィックライブラリのコンストラクタ
	  * @details グラフィックライブラリのクラスを作成する。使用するには、new してインスタンス化する必要がある。
	  * @param spiHW ハードウェアにアクセスするための情報を持っているクラスへの参照
	  */
	 ST7735(HW& a_spiHW);
#pragma endregion

#pragma region 初期化メソッド
	 /**
	  * @brief ハードウェア情報を設定する。引数無しのコンストラクタで作成した場合に、後から設定するためのメソッド。
	  * @param a_spiHW ハードウェアにアクセスするための情報を持っているクラスへのポインタ
	  */
	 void SetSPIHW(HW *a_spiHW);
	 /**
	  * @brief 初期化処理を行う
	  * @details ハードウェア(HWクラスのインスタンス）の初期化処理を呼びだし、各LCDに応じた初期化コマンドを実行する。
	  */
	 void doInit();

#pragma endregion
	
#pragma region SPI通信関連メソッド
	/**
	  * @brief コマンドを送信する
	  * @details ST7735の手順に基づいて、コマンドを送信する。実際には次のような手順で信号が送信される。]
	  * + Data/Command信号をＬに設定後、CSn信号をＬに設定。
	  * + コマンドを送信、信号を　DC:L→CS:L→コマンド送信→CS:H
	  * + CSn信号をＨに設定
	  * 通常は、この後にwriteDataが続くか、次のコマンドが送られることになる。
	  * @param cmd_ 送信するコマンド
	  */
	 void writeCommand(uint8_t cmd_);

	/// @brief	データを送信する。
	/// @details ST7735の手順に基づいて、コマンドを送信する。実際には次のような手順で信号が送信される。
	/// + Data/Command信号をＨに設定後、CSn信号をＬに設定。
	/// + データをを送信
	/// + CSn信号をＨに設定
	/// @param data_ 送信するデータ
	 void writeData(uint8_t data_);
#pragma endregion


#pragma region クラス内情報へのアクセッサ
	 /// @brief 初期化情報に対するアクセッサ。画面の幅を取得する。
	 /// @details 画面の幅は、TFT_ENABLE_BLACK、TFT_ENABLE_GREEN, TFT_ENABLE_RED、TFT_ENABLE_GENERICの定義それぞれで決定される。
	 /// これらは、ST7735_TFT.hで定義されている。変更の際はヘッダファイルを変更する。Pythonや、Arduino IDEなどではMicroPythonやライブラリの変更が必要になる。
	 /// @return 画面の幅

	 int getWidth() { return st7735Init.width; };
	 /// @brief 初期化情報に対するアクセッサ。画面の高さを取得する。
	 /// @details 画面の幅は、TFT_ENABLE_BLACK、TFT_ENABLE_GREEN, TFT_ENABLE_RED、TFT_ENABLE_GENERICの定義それぞれで決定される。
	 /// これらは、ST7735_TFT.hで定義されている。変更の際はヘッダファイルを変更する。Pythonや、Arduino IDEなどではMicroPythonやライブラリの変更が必要になる。
	 /// @return 画面の高さ
	 int getHeight() { return st7735Init.height; };
#pragma endregion

#pragma region 設定メソッド
	/// @brief BitBlitのためのアドレスウインドウを設定する。
	/// @details BitBlitは、画像の一部を別の場所にコピーする機能で、この関数はそのためのアドレスウインドウを設定する。
	/// 画面に対する点描画は、このアドレスウインドウの中で行われ、毎回の座標指定は省略可能になる。
	/// @param x0 左上のx座標
	/// @param y0 左上のy座標
	/// @param x1 右下のx座標
	/// @param y1 右下のy座標
	 void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t);

	/// @brief 画面の回転を設定する。
	/// @details ウインドウに対して描画するオブジェクトを回転させる。ST7735_ROTATION::NORMALを指定すると、元に戻すことができる。
	/// 描画するオブジェクトが左右反転（ミラー）してしまったりするときには、ここを呼びだす。
	/// @param r 回転する方向
	 void SetRotation(ST7735_ROTATION rot);
#pragma endregion 



	// 常に使用できる、基本的な描画関数

#pragma region 基本描画関数
	/// @brief 矩形を塗りつぶす。（なるべくfillRectWHやFillRectを使ってほしい）
	/// @param x 左上のx座標
	/// @param y 左上のy座標
	/// @param w 横幅
	/// @param h 高さ
	/// @param color
	 void fillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	//	 void fillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) { fillRectantle(x, y, w, h, color); }

	/// @brief 画面を単色でクリアする。
	/// @param color
	 void fillScreen(uint16_t color);

	/// @brief 水平線を描画する。ななめの線で必要なDDA処理が不要なので直線の場合はこちらの方が良い
	/// @param x 始点のx座標
	/// @param y 始点のy座標
	/// @param w 線の長さ
	/// @param color 線の色
	 void drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

	/// @brief 垂直線を高速に描画する。ななめの線で必要なDDA処理が不要なので高速で描画できる。
	/// @param x 始点のx座標
	/// @param y 始点のy座標
	/// @param h 線の長さ
	/// @param color 線の色
	 void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
	 
	/// @brief 画面に点を描画する
	/// @param x 表示位置のX座標	 
	/// @param y 表示位置のy座標
	/// @param color 色
	 void drawPixel(uint16_t x, uint16_t y, uint16_t color);

	 /// @brief 画面に点を描画する
	 /// @param axis 表示位置の座標(8bitのXY座標) 
	 /// @param color 色 
	 void drawPixel(Axis8 axis, uint16_t color) { drawPixel(axis.x, axis.y, color); }

	 /// @brief 画面に点を描画する
	 /// @param axis 表示位置の座標(16bitのXY座標)
	 /// @param color 色 
	 void drawPixel(Axis16 axis, uint16_t color) { drawPixel(axis.x, axis.y, color); }
	/// @brief 		画面の表示を反転させる
	/// @param i	true:反転、false:通常表示 
	 void invertDisplay(bool i);
	 
	 /// @brief 画面の表示を通常に戻す
	 void NormalDisplay();
#pragma endregion









#pragma region 図形描画メソッド
	/// @brief 図形の描画関数
	#if defined TFT_ENABLE_SHAPES		// 画像の表示関数を有効にする
	private:
	/// @brief 画面に、1/4の円を描画する。四分円を描画するための補助関数
	/// @param x0 中心のx座標
	/// @param y0 中心のy座標 
	/// @param r 半径 
	/// @param cornername 四分円の位置。1:左上、2:右上、4:左下、8:右下のビットマスクになっている。
	/// @param color 円の色	
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);

	/// @brief 画面に、1/4の円を塗りつぶす。四分円を描画するための補助関数
	/// @param x0 		中心のx座標
	/// @param y0 		中心のy座標
	/// @param r 		半径
	/// @param cornername 		四分円の位置。1:左上、2:右上、4:左下、8:右下のビットマスクになっている。
	/// @param delta	塗りつぶしのための補正値 		
	/// @param color		円の色 	
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

	public:
	/// @brief 画面に円を描画する
	/// @param x0 中心のx座標
	/// @param y0 中心のy座標
	/// @param r 半径 
	/// @param color 円の色 	
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	/// @brief 画面に円を描画する
	/// @param axis 中心の座標 （16bit)
	/// @param r 半径
	/// @param color 円の色
	void drawCircle(Axis16 axis , int16_t r, uint16_t color) { drawCircle(axis.x, axis.y, r, color); }
	/// @brief 画面に円を描画する
	/// @param axis 中心の座標 (88it)
	/// @param r 半径
	/// @param color 円の色
	void drawCircle(Axis8 axis, int16_t r, uint16_t color) { drawCircle(axis.x, axis.y, r, color); }

	/// @brief 画面に円を塗りつぶす
	/// @param x0 中心のx座標
	/// @param y0 中心のy座標
	/// @param r 半径
	/// @param color 円の色
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	/// @brief 塗りつぶされた円を描画する
	/// @param axis	中心の座標
	/// @param r 半径
	/// @param color 円の色
	void fillCircle(Axis16 axis , int16_t r, uint16_t color) { fillCircle(axis.x, axis.y, r, color); }
	/// @brief 塗りつぶされた円を描画する
	/// @param axis	中心の座標
	/// @param r 半径
	/// @param color 円の色
	void fillCircle(Axis8 axis, int16_t r, uint16_t color) { fillCircle(axis.x, axis.y, r, color); }

	/// @brief 塗りつぶされた円を描画する
	/// @param x0	中心のx座標
	/// @param y0	中心のy座標 
	/// @param r	半径 
	/// @param color 		円の色
	void drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

	/// @brief 矩形を描画する
	/// @param region 矩形の左上座標とサイズ
	/// @param color 色
	void drawRectWH(Region8 region, uint16_t color) { drawRectWH(region.start.x, region.start.y, region.size.w, region.size.h, color); }
	/// @brief 矩形を描画する
	/// @param start 矩形の左上座標
	/// @param size 矩形のサイズ
	/// @param color
	void drawRectWH(Axis8 start, Size8 size, uint16_t color) { drawRectWH(start.x, start.y, size.w, size.h, color); }

	/// @brief 矩形を描画する。
	/// @param x0 左上のx座標
	/// @param y0 左上のy座標
	/// @param x1 右下のx座標
	/// @param y1 右下のy座標
	/// @param color 矩形の色
	void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

	/// @brief 矩形を描画する
	/// @param start 始点の座標
	/// @param end 終点の座標
	/// @param color 色
	void drawRect(Axis8 start, Axis8 end, uint16_t color) { drawRect(start.x, start.y, end.x, end.y, color); }

	/// @brief 矩形を描画する
	/// @param region 矩形の左上座標と右下座標
	/// @param color 色
	void drawRect(Region8 region, uint16_t color) { drawRect(region.start.x, region.start.y, region.end.x, region.end.y, color); }

	/// @brief 塗りつぶし矩形を幅と高さを指定して描画する。
	/// @param x 		左上のx座標
	/// @param y 		左上のy座標
	/// @param w 		幅
	/// @param h 		高さ
	/// @param color		矩形の色
	void fillRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

	/// @brief 塗りつぶし矩形を幅と高さを指定して描画する。
	/// @param region 	 表示位置の座標(8bitのXY座標) 
	/// @param color 色
	void fillRectWH(Region8 region, uint16_t color) { fillRectWH(region.start.x, region.start.y, region.size.w, region.size.h, color); }

	/// @brief 塗りつぶし矩形を幅と高さを指定して描画する。
	/// @param start 矩形の左上座標
	/// @param size 矩形のサイズ
	/// @param color 色
	void fillRectWH(Axis8 start, Size8 size, uint16_t color) { fillRectWH(start.x, start.y, size.w, size.h, color); }
	
	/// @brief 塗りつぶし矩形を左上、右下座標を指定して描画する。
	/// @param x0	左上のx座標
	/// @param y0	左上のy座標 
	/// @param x1	右下のx座標 
	/// @param y1	右下のy座標 
	/// @param color	矩形の色 
	void fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

	/// @brief ぬ矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
	/// @param start 矩形の左上座標
	/// @param end 矩形の右下座標
	/// @param color 色
	void fillRect(Axis8 start, Axis8 end, uint16_t color) { fillRect(start.x, start.y, end.x, end.y, color); }

	/// @brief 塗りつぶし矩形を左上、右下座標を指定して描画する。
	/// @param region 矩形の座標
	/// @param color 色
	void fillRect(Region8 region, uint16_t color) { fillRect(region.start.x, region.start.y, region.end.x, region.end.y, color); }

	/// @brief 直線を描画する
	/// @param x0 始点のx座標
	/// @param y0 始点のy座標
	/// @param x1 終点のx座標
	/// @param y1 終点のy座標
	/// @param color 直線の色
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

	/// @brief 直線を描画する
	/// @param start 始点の座標
	/// @param end 終点の座標
	/// @param color 色
	void drawLine(Axis16 start, Axis16 end, uint16_t color) { drawLine(start.x, start.y, end.x, end.y, color); }

	/// @brief 直線を描画する
	/// @param start 始点の座標
	/// @param end 終点の座標
	/// @param color 色
	void drawLine(Axis8 start, Axis8 end, uint16_t color) { drawLine(start.x, start.y, end.x, end.y, color); }	

	/// @brief 直線を描画する
	/// @param region 直線の左上と右下座標
	/// @param color 色
	void drawLine(Region16 region, uint16_t color) { drawLine(region.start.x, region.start.y, region.end.x, region.end.y, color); }
	
	/// @brief 直線を描画する
	/// @param region 直線の左上と右下座標
	/// @param color 色
	void drawLine(Region8 region, uint16_t color) { drawLine(region.start.x, region.start.y, region.end.x, region.end.y, color); }

	/// @brief 	角丸の矩形を左上座標と幅、高さを指定して描画する。角丸の矩形は、矩形の四隅が丸くなっている。角丸の半径はrで指定する。
	/// @details rの値が矩形の半分を超えるなど、矛盾する形になると無限ループしたりするのでやめてほしい
	/// @param x 		左上のx座標
	/// @param y		左上のy座標 
	/// @param w		幅 
	/// @param h 		高さ 
	/// @param r 		角の半径 
	/// @param color	矩形の色
	void drawRoundRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint16_t color);
	
	/// @brief 	角丸の矩形を左上座標と幅、高さを指定して描画する。角丸の矩形は、矩形の四隅が丸くなっている。角丸の半径はrで指定する。
	/// @details rの値が矩形の半分を超えるなど、矛盾する形になると無限ループしたりするのでやめてほしい
	/// @param region 左上右下領域の座標とサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRectWH(Region8 region, uint8_t r, uint16_t color) { drawRoundRectWH(region.start.x, region.start.y, region.size.w, region.size.h, r, color); }

	/// @brief 	角丸の矩形を左上座標と幅、高さを指定して描画する。角丸の矩形は、矩形の四隅が丸くなっている。角丸の半径はrで指定する。
	/// @details rの値が矩形の半分を超えるなど、矛盾する形になると無限ループしたりするのでやめてほしい
	/// @param region 左上右下領域の座標とサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRectWH(Region16 region, uint8_t r, uint16_t color) { drawRoundRectWH(region.start.x, region.start.y, region.size.w, region.size.h, r, color); }
	
	/// @brief 	角丸の矩形を左上座標と幅、高さを指定して描画する。角丸の矩形は、矩形の四隅が丸くなっている。角丸の半径はrで指定する。
	/// @details rの値が矩形の半分を超えるなど、矛盾する形になると無限ループしたりするのでやめてほしい
	/// @param start 矩形の左上座標
	/// @param size 矩形のサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRectWH(Axis8 start , Size8 size , uint8_t r, uint16_t color) { drawRoundRectWH(start.x, start.y,size.w, size.h, r, color); }

	/// @brief 	角丸の矩形を左上座標と右下座標を指定して描画する。角丸の矩形は、矩形の四隅が丸くなっている。角丸の半径はrで指定する。
	/// @details rの値が矩形の半分を超えるなど、矛盾する形になると無限ループしたりするのでやめてほしい
	/// @param x		左上のx座標
	/// @param y		左上のy座標
	/// @param x1		右下のx座標
	/// @param y1		右下のy座標
	/// @param r		角の半径
	/// @param color		矩形の色
	void drawRoundRect(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint8_t r, uint16_t color);

	/// @brief 	角丸の矩形を左上座標と右下座標を指定して描画する。角丸の矩形は、矩形の四隅が丸くなっている。角丸の半径はrで指定する。
	/// @details rの値が矩形の半分を超えるなど、矛盾する形になると無限ループしたりするのでやめてほしい
	/// @param region 左上と右下領域の座標
	/// @param r 角丸部分の半径
	/// @param color 色

	void drawRoundRect(Region8 region, uint8_t r, uint16_t color) { drawRoundRect(region.start.x, region.start.y, region.end.x, region.end.y, r, color); }
	/// @brief 角丸塗りつぶし
	/// @param start 左上座標
	/// @param end 右下座標
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRect(Axis8 start, Axis8 end, uint8_t r, uint16_t color) { drawRoundRect(start.x, start.y, end.x, end.y, r, color); }

	/// @brief 角丸塗りつぶし
	/// @param x 左上のx座標
	/// @param y 左上のy座標
	/// @param w 横幅
	/// @param h 高さ
	/// @param r 角丸部分の半径
	/// @param color 色
	void fillRoundRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint16_t color);

	/// @brief 角丸塗りつぶし
	/// @param region 塗りつぶし範囲の左上座標とサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void fillRoundRectWH(Region8 region, uint8_t r, uint16_t color) { fillRoundRectWH(region.start.x, region.start.y, region.size.w,region.size.h, r, color); }
	/// @brief 角丸塗りつぶし
	/// @param start 塗りつぶし範囲の左上座標
	/// @param size 塗りつぶし範囲のサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void fillRoundRectWH(Axis8 start, Size8 size, uint8_t r, uint16_t color) { fillRoundRectWH(start.x, start.y, size.w, size.h, r, color); }

	/// @brief 角丸図形の塗りつぶし
	/// @param x 左上のx座標
	/// @param y 左上のy座標
	/// @param x1 右下のx座標
	/// @param y1 右下のy座標
	/// @param r 角丸部分の半径
	/// @param color 色
	void fillRoundRect(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint8_t r, uint16_t color);
	/// @brief 角丸図形の塗りつぶし
	/// @param region 塗りつぶし範囲の左上座標と右下座標
	/// @param r 角丸部分の半径
	/// @param color 色
	void fillRoundRect(Region8 region, uint8_t r, uint16_t color) { fillRoundRect(region.start.x, region.start.y, region.end.x, region.end.y, r, color); }
	/// @brief 角丸図形の塗りつぶし
	/// @param start 左上座標
	/// @param end 右下座標
	/// @param r 角丸部分の半径
	/// @param color 色
	void fillRoundRect(Axis8 start, Axis8 end, uint8_t r, uint16_t color) { fillRoundRect(start.x, start.y, end.x, end.y, r, color); }

	/// @brief 三角形の描画
	/// @param y0　左上のx座標 
	/// @param x1　左上のy座標
	/// @param y1　右上のx座標
	/// @param x2　右上のy座標
	/// @param y2　左下のx座標 
	/// @param color 
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	/// @brief 三角形の描画
	/// @param a 点１の座標
	/// @param b 点２の座標
	/// @param c 点３の座標
	/// @param color 色
	void drawTriangle(Axis16 a, Axis16 b, Axis16 c, uint16_t color) { drawTriangle(a.x, a.y, b.x, b.y, c.x, c.y, color); }
	/// @brief 三角形の描画
	/// @param a 点１の座標
	/// @param b 点２の座標
	/// @param c 点３の座標
	/// @param color 色
	void drawTriangle(Axis8 a, Axis8 b, Axis8 c, uint16_t color) { drawTriangle(a.x, a.y, b.x, b.y, c.x, c.y, color); }

	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	/// @brief 三角形の塗りつぶし
	/// @param a 点１の座標
	/// @param b 点２の座標
	/// @param c 点３の座標
	/// @param color 色
	void fillTriangle(Axis16 a, Axis16 b, Axis16 c, uint16_t color) { fillTriangle(a.x, a.y, b.x, b.y, c.x, c.y, color); }
	/// @brief 三角形の塗りつぶし
	/// @param a 点１の座標
	/// @param b 点２の座標
	/// @param c 点３の座標
	/// @param color 色
	void fillTriangle(Axis8 a, Axis8 b, Axis8 c, uint16_t color) { fillTriangle(a.x, a.y, b.x, b.y, c.x, c.y, color); }
	#endif
#pragma endregion

#pragma region テキスト表示メソッド
	#if defined TFT_ENABLE_TEXT

	#if defined TFT_ENABLE_FONTS
	/// @brief 英文のフォント指定機能を有効にした場合に、フォントを指定するメソッド。
	/// @details このメソッドは、TFT_ENABLE_FONTSが有効な場合にのみ使用できる。
	/// @param f フォント構造体へのポインタ。通常、AdafruitのGFXライブラリで定義されているフォントを指定する。
	void setFont(const GFXfont *f);
	/// @brief 日本語フォントを指定する
	/// @param name フォント名
	void setFont(const char *name);
	/// @brief 登録済みのフォントを取得する。引数には充分な余裕（登録済みのフォント名の長さ＋１）xフォント数を確保しておくこと。最低でも128バイト以上とする。
	/// @details newして返すバッファにしたいのだが、Micropythonから呼びだすとnewでハングするのでやむなくこの構成にした。
	/// @param buf フォント名を格納するバッファ
	bool getFonts(char *buf, uint16_t size);
	#endif
	
	/// @brief 	テキストの改行を行うかのフラグを設定する
	/// @param w true:画面の左に来たらテキストを改行する(デフォルト）、false:改行しない
	void setTextWrap(bool w);

	/// @brief 		1文字を描画する
	/// @param x 		描画するx座標
	/// @param y 		描画するy座標
	/// @param c 		描画する文字
	/// @param color 		文字の色
	/// @param bg 		背景色
	/// @param size 		文字のサイズ。1がデフォルト。2で2倍の大きさになる。
	void drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);

	/// @brief １文字を表示する
	/// @param axis 表示する文字の座標。<br/>
	/// TFT_ENABLE_FONTSが指定されている場合は、文字ごとに決められているその文字の始点オフセット。ENABLE_FONTが無効の場合、文字の左上座標。
	/// @param c 表示する文字コード
	/// @param color 文字色
	/// @param bg 背景色
	/// @param size 大きさ。１のときは文字の１ドットは画面上の１ドット。２にすると文字の１ドットは２ｘ２の矩形になる。
	void drawChar(Axis8 axis, uint8_t c, uint16_t color, uint16_t bg, uint8_t size) { drawChar(axis.x, axis.y, c, color, bg, size); }
	/// @brief １文字を表示する
	/// @param axis 表示する文字の座標。<br/>
	/// TFT_ENABLE_FONTSが指定されている場合は、文字ごとに決められているその文字の始点オフセット。ENABLE_FONTが無効の場合、文字の左上座標。
	/// @param c 表示する文字コード
	/// @param color 文字色
	/// @param bg 背景色
	/// @param size 大きさ。１のときは文字の１ドットは画面上の１ドット。２にすると文字の１ドットは２ｘ２の矩形になる。
	void drawChar(Axis16 axis, uint8_t c, uint16_t color, uint16_t bg, uint8_t size) { drawChar(axis.x, axis.y, c, color, bg, size); }

	/// @brief 	文字列を描画する
	/// @param x 描画するx座標
	/// @param y 描画するy座標 		
	/// @param _text 描画する文字 
	/// @param color 文字の色 
	/// @param bg 背景色
	/// @param size 文字のサイズ。1がデフォルト。2で2倍の大きさになる。
	void drawText(uint16_t x, uint16_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size);

	/// @brief 文字列を描画する
	/// @param axis 表示する文字の座標。<br/>
	/// TFT_ENABLE_FONTSが指定されている場合は、文字ごとに決められているその文字の始点オフセット。ENABLE_FONTが無効の場合、文字の左上座標。
	/// @param _text 表示する文字列
	/// @param color 前景色
	/// @param bg 背景色
	/// @param size 大きさ。１のときは文字の１ドットは画面上の１ドット。２にすると文字の１ドットは２ｘ２の矩形になる。
	void drawText(Axis8 axis, const char *_text, uint16_t color, uint16_t bg, uint8_t size) { drawText(axis.x, axis.y, _text, color, bg, size); }

	/// @brief 文字列を描画する
	/// @param axis 表示する文字の座標。<br/>
	/// TFT_ENABLE_FONTSが指定されている場合は、文字ごとに決められているその文字の始点オフセット。ENABLE_FONTが無効の場合、文字の左上座標。
	/// @param _text 表示する文字列
	/// @param color 前景色
	/// @param bg 背景色
	/// @param size 大きさ。１のときは文字の１ドットは画面上の１ドット。２にすると文字の１ドットは２ｘ２の矩形になる。
	void drawText(Axis16 axis, const char *_text, uint16_t color, uint16_t bg, uint8_t size) { drawText(axis.x, axis.y, _text, color, bg, size); }

	#pragma region 漢字表示メソッド
	#ifdef TFT_ENABLE_KANJI				// 漢字表示が可能な場合


	/// @brief 指定された座標に、指定された漢字パターン（ビットマップ）を１文字表示する。
	/// @param x 表示位置のX座標 
	/// @param y 表示位置のY座標
	/// @param w 表示幅
	/// @param h 表示高さ
	/// @param bmpData ビットマップデータ
	/// @param color 文字の色
	/// @param bg	背景色
	void drawKanjiBlock(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bmpData, uint16_t color , uint16_t bg);
	
	/// @brief 漢字１文字を表示する
	/// @param x 表示するX座標
	/// @param y 表示するY座標
	/// @param utf8codes 表示するUTF8、もしくはASCIIコード
	/// @param color 表示色
	/// @param bg 背景色	 
	void drawKanji(uint16_t &x, uint16_t& y, uint32_t code, uint16_t color, uint16_t bg);

	/// @brief 漢字文字列を表示する
	/// @param x 		描画するx座標
	/// @param y	描画するy座標
	/// @param _text	描画する文字
	/// @param color	文字の色
	/// @param bg 		背景色
	/// @param size 		文字のサイズ。1がデフォルト。2で2倍の大きさになる。
	void drawTextKanji(uint16_t x, uint16_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size);
	#endif
	#pragma endregion	

	#endif
#pragma endregion	
	
	

#pragma region スクロール関連メソッド
	#ifdef TFT_ENABLE_SCROLL
	/// @brief 画面の縦スクロール領域を指定する。<br/>
	/// @details 画面のスクロールでは、実際に表示されているビットイメージのフレームバッファは書き換えられない。
	/// 画面表示の時の、読み出しアドレスが変更されるもの。スクロール時、画面は上下が繋がったようにスクロールする。
	/// @param top_fix_height 画面上部の固定領域の高さ
	/// @param bottom_fix_height 画面下部の固定領域の高さ
	/// @param _scroll_direction スクロール方向。trueの場合ロールアップ	
	void setScrollDefinition(uint8_t top_fix_height, uint8_t bottom_fix_height, bool _scroll_direction);

	/// @brief 画面のスクロールを行う。
	/// @param _vsp スクロールするドット数。１０と指定すると、１０ドットスクロールした画面が表示される。このとき、再度１０と指定して呼びだしても画面は変わらない。巻物のように動かすには、1、２、３・・・と数字を変えて複数回呼びだす必要がある。
	void verticalScroll(uint8_t _vsp);
	#endif
#pragma endregion	

#pragma region ビットマップ関連メソッド
	#ifdef TFT_ENABLE_BITMAP
	/// @brief 透過色を使用しない。
	void bmpUnuseTransColor(void) { isTransparentColor = false; }

	/// @brief 透過色を 565 の色で指定する。
	/// @details bitmapを画面に表示するとき、透過色が指定されているとビットマップのその色の部分は描画されず、地の色がそのまま残る。<br/>
	/// 透過色を指定すると、描画時に処理が増えるため遅くなるのでなるべく使用しないほうが良い。
	/// @param c 指定する色。
	void bmpUseTransColor(uint16_t c) {
		isTransparentColor = true;
		bmpTransparentColor = c;
	}
	/// @brief 画面にビットマップを表示する。ビットマップは、５６５形式の16bit値の配列へのポインタ。
	/// @details 具体的には次のようなデータになる。<br/>
	/// const uint16_t bmp1[] = {0x0821, 0x0821, 0x0821, 0x0821, 0x0000, 0xF223, 0xF223, … };
	/// @param x 表示する左上の座標
	/// @param y 表示する左上の座標
	/// @param w ビットマップの大きさ
	/// @param h ビットマップの大きさ
	/// @param p 表示するデータへのポインタ
	/// @param direction 表示する方向。0…標準 1…ミラー表示		
	void bmpDraw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *p , uint8_t direction);
	#endif
#pragma endregion

};






#if defined TFT_ENABLE_ROTATE
//void setRotation(uint8_t m);
#endif




// ------ not functional yet ----------------------------------------------
// needs fat-library

// BMP processing from SD-card
#if defined TFT_ENABLE_BMP
	 
	 /// @brief 24ビットカラーを16ビットカラーに変換する。もとになったAdafruitのコードにあったので残してあるが使用していない。
	 /// @details それぞれの変数の「上位」を切り出しており、用途としては特殊なものと思われる。
	 /// @param r 赤成分
	 /// @param g 緑成分
	 /// @param b 青成分
	 /// @return 565方式（R:5bit/G:6ビット/ビットカラー
	 int16_t Color565(int16_t r, int16_t g, int16_t b)
	 {
		 return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	 }
	 /// @brief 画面に点を表示する。もとになったAdafruitのコードにあったので残してあるが使用していない。
	 /// @param color	色 
	 void pushColor(uint16_t color);

	bool bmpDraw(int8_t x, int8_t y, int8_t *bmpname);
#endif

#endif

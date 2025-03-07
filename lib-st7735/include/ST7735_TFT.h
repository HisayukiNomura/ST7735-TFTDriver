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
// #define TFT_ENABLE_FONTS


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
	 ST7735();
	 ST7735(HW *a_spiHW);
	 ST7735(HW& a_spiHW);

	 void SetSPIHW(HW *a_spiHW);
	 void doInit();
	 void writeCommand(uint8_t cmd_);
	 void writeData(uint8_t data_);

	 // 初期化情報へのアクセッサ
	 int getWidth() { return st7735Init.width; };
	 int getHeight() { return st7735Init.height; };

	 // 常に使用できる、基本的な描画関数
	 void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	 void fillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
	 void fillScreen(uint16_t color);
	 void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);
	 void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
	 void drawPixel(uint8_t x, uint8_t y, uint16_t color);
	 void drawPixel(Axis8 axis, uint16_t color) { drawPixel(axis.x, axis.y, color); }

	 void drawKanjiBlock(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *bmpData, uint16_t color , uint16_t bg);

	 void invertDisplay(bool i);
	 void NormalDisplay();
	 void SetRotation(ST7735_ROTATION rot);

	 // Convert 24-abit color to 16-abit color

	 int16_t Color565(int16_t r, int16_t g, int16_t b)
	 {
		 return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	 }
	 void pushColor(uint16_t color);


	/// @brief 図形の描画関数
	#if defined TFT_ENABLE_SHAPES		// 画像の表示関数を有効にする
	   private :
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

   public:

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

	
	void drawRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
	/// @brief 矩形を描画する
	/// @param region 矩形の左上座標とサイズ
	/// @param color 色
	void drawRectWH(Region8 region, uint16_t color) { drawRectWH(region.start.x, region.start.y, region.size.w, region.size.h, color); }
	/// @brief 矩形を描画する
	/// @param start 矩形の左上座標
	/// @param size 矩形のサイズ
	/// @param color
	void drawRectWH(Axis8 start, Size8 size, uint16_t color) { drawRectWH(start.x, start.y, size.w, size.h, color); }

	void drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
	/// @brief 矩形を描画する
	/// @param start 始点の座標
	/// @param end 終点の座標
	/// @param color 色
	void drawRect(Axis8 start, Axis8 end, uint16_t color) { drawRect(start.x, start.y, end.x, end.y, color); }
	/// @brief 矩形を描画する
	/// @param region 矩形の左上座標と右下座標
	/// @param color 色
	void drawRect(Region8 region, uint16_t color) { drawRect(region.start.x, region.start.y, region.end.x, region.end.y, color); }

	void fillRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
	/// @brief 矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
	/// @param region 矩形の左上座標とサイズ
	/// @param color 色
	void fillRectWH(Region8 region, uint16_t color) { fillRectWH(region.start.x, region.start.y, region.size.w, region.size.h, color); }
	/// @brief 矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
	/// @param start 矩形の左上座標
	/// @param size 矩形のサイズ
	/// @param color 色
	void fillRectWH(Axis8 start, Size8 size, uint16_t color) { fillRectWH(start.x, start.y, size.w, size.h, color); }

	void fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
	/// @brief 矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
	/// @param start 矩形の左上座標
	/// @param end 矩形の右下座標
	/// @param color 色
	void fillRect(Axis8 start, Axis8 end, uint16_t color) { fillRect(start.x, start.y, end.x, end.y, color); }
	/// @brief 矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
	/// @param region 矩形の座標
	/// @param color 色
	void fillRect(Region8 region, uint16_t color) { fillRect(region.start.x, region.start.y, region.end.x, region.end.y, color); }

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

	void drawRoundRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);
	/// @brief 角丸矩形を描画する
	/// @param region 左上右下領域の座標とサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRectWH(Region8 region, uint8_t r, uint16_t color) { drawRoundRectWH(region.start.x, region.start.y, region.size.w, region.size.h, r, color); }
	/// @brief 角丸矩形を描画する
	/// @param region 左上右下領域の座標とサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRectWH(Region16 region, uint8_t r, uint16_t color) { drawRoundRectWH(region.start.x, region.start.y, region.size.w, region.size.h, r, color); }
	/// @brief 角丸図形を描画する
	/// @param start 矩形の左上座標
	/// @param size 矩形のサイズ
	/// @param r 角丸部分の半径
	/// @param color 色
	void drawRoundRectWH(Axis8 start , Size8 size , uint8_t r, uint16_t color) { drawRoundRectWH(start.x, start.y,size.w, size.h, r, color); }

	void drawRoundRect(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t r, uint16_t color);
	/// @brief 角丸塗りつぶし矩形を描画する
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

	void fillRoundRectWH(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color);
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
	
	void fillRoundRect(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t r, uint16_t color);
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

		#if defined TFT_ENABLE_TEXT
		void setTextWrap(bool w);
		void drawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
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


		void drawText(uint8_t x, uint8_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size);
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

	

		void drawKanji(uint8_t& x, uint8_t& y, uint32_t code, uint16_t color, uint16_t bg);
		void drawTextKanji(uint8_t x, uint8_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size);
		#endif
	
		#if defined TFT_ENABLE_FONTS
		void setFont(const GFXfont *f);
		#endif
	

	#ifdef TFT_ENABLE_SCROLL
		void setScrollDefinition(uint8_t top_fix_height, uint8_t bottom_fix_height, bool _scroll_direction);
		void verticalScroll(uint8_t _vsp);
	#endif

	#ifdef TFT_ENABLE_BITMAP
		/// @brief 透過色を使用しない。
		void bmpUnuseTransColor(void) { isTransparentColor = false; }

		/// @brief 透過色を 565 の色で指定する。
		/// @details bitmapを画面に表示するとき、透過色が指定されているとビットマップのその色の部分は描画されず、地の色がそのまま残る。<br/>
		/// 透過色を指定すると、描画時に処理が増えるため遅くなるのでなるべく使用しないほうが良い。
		/// @param c 指定する色。
		void bmpUseTrasColor(uint16_t c) {
			isTransparentColor = true;
			bmpTransparentColor = c;
		}
		
		void bmpDraw(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t *p , uint8_t direction);
	#endif
	};








#if defined TFT_ENABLE_ROTATE
//void setRotation(uint8_t m);
#endif




// ------ not functional yet ----------------------------------------------
// needs fat-library

// BMP processing from SD-card
#if defined TFT_ENABLE_BMP
bool bmpDraw(int8_t x, int8_t y, int8_t *bmpname);
#endif


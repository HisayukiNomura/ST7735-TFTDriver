
/// @file ST7735_TFT.cpp
/// @brief Raspberry pi pico 標準SDK用の画面ライブラリ<br/>
/// 次のソースコードをベースにしている。<br/>
/// pic_16F18346_projects / Gavin Lyons (https://github.com/gavinlyonsrepo/pic_16F18346_projects) <br/>
/// ST7735 TFT-Library for PIC-Microcontrollers / Bernhard Bablok (https://github.com/bablokb/pic-st7735) <br/>
///
/// @details このライブラリは、Microchip社のPIC用ライブラリと、adafruit用のライブラリから作成した、Raspberry PI PICO用の
/// ST7732ライブラリ。（大元のpic_16F18346_projects自体、adafruit用のライブラリと相当類似しており、どちらが本家かはわからない。<br/>
/// 主な改変は次の通り。<br/>
/// - Raspberry PI PICO SDK への適合（https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico　を使用) <br/>
/// - c++のクラス化 <br/>
/// - 漢字表示機能の追加 <br/>

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC optimize ("O0")


#include <stdint.h>
#include <stdlib.h>
#include <iconv.h>


#include "../include/ST7735_TFT.h"
#include "../include/ST7735_commands.h"
#include "../include/ST7735_initcmd.h"
#include "../include/hw.h"

#if defined(TFT_ENABLE_KANJI)
#include "../include/KanjiHelper.h"
#endif
#if defined(TFT_ENABLE_TEXT) && !defined(TFT_ENABLE_FONTS)
#include "../include/TextFonts.h"
#endif
#include <bits/move.h>

#pragma region コンストラクタ
ST7735::ST7735()
{

}
ST7735::ST7735(HW* a_pSpiHW)
{
	pSpiHW = a_pSpiHW;
}

ST7735::ST7735(HW &spiHW) 
{
	pSpiHW = &spiHW;
}
#pragma endregion

#pragma region 初期化メソッド
void ST7735::SetSPIHW(HW *a_spiHW)
{
	pSpiHW = a_spiHW;
}
#ifdef TFT_ENABLE_FONTS
	#include "../include/font/Font_Mono9p.h"
	#include "../include/font/Font_Mono18p.h"
	#include "../include/font/FreeMonoOblique12pt7b.h"
	#include "../include/font/FreeMonoOblique12pt_sub.h"
#endif

void ST7735::doInit()
{
	// フォントの初期化
#ifdef TFT_ENABLE_FONTS
		for (int i = 0; i < 16; i++) {
			registeredFonts[i].name = NULL;
			registeredFonts[i].font = NULL;
		}
		registeredFonts[0].font = &FreeMono9pt7b;
		registeredFonts[0].name = "FreeMono9pt7b";
		registeredFonts[1].font = &FreeMono18pt7b;
		registeredFonts[1].name = "FreeMono18pt7b";
		registeredFonts[2].font = &FreeMonoOblique12pt_sub;
		registeredFonts[2].name = "FreeMonoOblique12pt_sub";
		registeredFonts[3].font = &FreeMonoOblique12pt7b;
		registeredFonts[3].name = "FreeMonoOblique12pt7b";
#endif

	pSpiHW->init();
	st7735Init.SetSPIHW(pSpiHW);
#ifdef TFT_ENABLE_BLACK
	st7735Init.initR((uint8_t)ST7735Type.BLACKTAB);
#elif defined(TFT_ENABLE_GREEN)
	st7735Init.initR((uint8_t)ST7735Type.GREENTAB);
#elif defined(TFT_ENABLE_RED)
	st7735Init.initR((uint8_t)ST7735Type.REDTAB);
#elif defined(TFT_ENABLE_GENERIC)
#endif
	//st7735Init.setRotation(1);
}

#pragma endregion


#pragma region SPI通信関連メソッド
void ST7735::writeCommand(uint8_t cmd_) 
{
	pSpiHW->writeCommand(cmd_);
}

void ST7735::writeData(uint8_t data_)
{
	pSpiHW->writeData(data_);
}
#pragma endregion

#pragma region 設定メソッド
void ST7735::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  writeCommand(ST7735Cmd.CASET);
  writeData(0);
  writeData(x0 + st7735Init.xstart);
  writeData(0);
  writeData(x1 + st7735Init.xstart);
  writeCommand(ST7735Cmd.RASET);
  writeData(0);
  writeData(y0 + st7735Init.ystart);
  writeData(0);
  writeData(y1 + st7735Init.ystart);
  writeCommand(ST7735Cmd.RAMWR);  // Write to RAM
}
void ST7735::SetRotation(ST7735_ROTATION r)
{
	st7735Init.setRotation(r);
}
#pragma endregion

#pragma region 基本描画メソッド

/// @brief 矩形を塗りつぶす
/// @param x 左上のx座標
/// @param y 左上のy座標
/// @param w 横幅
/// @param h 高さ
/// @param color
void ST7735::fillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  uint8_t hi, lo;
  if ((x >= st7735Init.width) || (y >= st7735Init.height)) return;
  if ((x + w - 1) >= st7735Init.width) w = st7735Init.width - x;
  if ((y + h - 1) >= st7735Init.height) h = st7735Init.height - y;
  setAddrWindow(x, y, x + w - 1, y + h - 1);
  hi = color >> 8;
  lo = color;
  pSpiHW->DCHigh();
  pSpiHW->CSLow();
  for (y = h; y > 0; y--) {
    for (x = w; x > 0; x--) {
		pSpiHW->spiWrite(hi);
		pSpiHW->spiWrite(lo);
	}
  }
  pSpiHW->CSHigh();
}

/// @brief 画面を単色で塗りつぶす。初期設定された液晶サイズの矩形を表示させることで実装。
/// @param color
void ST7735::fillScreen(uint16_t color)
{
	fillRectangle(0, 0, st7735Init.width, st7735Init.height, color);
}

void ST7735::drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
  uint8_t hi, lo;
  if ((x >= st7735Init.width) || (y >= st7735Init.height)) return;
  if ((x + w - 1) >= st7735Init.width) w = st7735Init.width - x;
  hi = color >> 8;
  lo = color;
  setAddrWindow(x, y, x + w - 1, y);
  pSpiHW->DCHigh();
  pSpiHW->CSLow();
  while (w--) {
	  pSpiHW->spiWrite(hi);
	  pSpiHW->spiWrite(lo);
  }
  pSpiHW->CSHigh();
}

void ST7735::drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color)
{
	uint8_t hi, lo;
	if ((x >= st7735Init.width) || (y >= st7735Init.height))
		return;
	if ((y + h - 1) >= st7735Init.height)
		h = st7735Init.height - y;
	hi = color >> 8;
	lo = color;
	setAddrWindow(x, y, x, y + h - 1);
	pSpiHW->DCHigh();
	pSpiHW->CSLow();
	while (h--) {
		pSpiHW->spiWrite(hi);
		pSpiHW->spiWrite(lo);
	}
	pSpiHW->CSHigh();
}


void ST7735::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x >= st7735Init.width) || (y >= st7735Init.height))
		return;
	setAddrWindow(x, y, x + 1, y + 1);
	writeData(color >> 8);
	writeData(color & 0xFF);
}

void ST7735::invertDisplay(bool i)
{
	if (i)
		writeCommand(ST7735Cmd.INVON);
	else
		writeCommand(ST7735Cmd.INVOFF);
}


void ST7735::NormalDisplay()
{
	writeCommand(ST7735Cmd.NORON);
}
#pragma endregion



#pragma region 図形描画メソッド
#if defined TFT_ENABLE_SHAPES	// 画像の表示関数を有効にする


void ST7735::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void ST7735::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

void ST7735::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void ST7735::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}


void ST7735::drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

void ST7735::drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	if (x0 > x1) _swap(x0, x1);
	if (x0 > x1) _swap(y0, y1);

	drawFastHLine(x0, y0, x1 - x0, color);
	drawFastHLine(x0, y1, x1 - x0, color);
	drawFastVLine(x0, y0, y1 - y0, color);
	drawFastVLine(x1, y0, y1 - y0, color);
}

void ST7735::fillRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	int16_t i;
	for (i = x; i < x + w; i++) {
		drawFastVLine(i, y, h, color);
	}
}
void ST7735::fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	if (x0 > x1) _swap(x0, x1);
	if (x0 > x1) _swap(y0, y1);
	uint8_t w = x1 - x0;
	uint8_t h = y1 - y0;
	fillRectWH(x0, y0, w, h, color);
}


void ST7735::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	int16_t steep, dx, dy, err, ystep;
	steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		_swap(x0, y0);
		_swap(x1, y1);
	}
	if (x0 > x1) {
		_swap(x0, x1);
		_swap(y0, y1);
	}
	dx = x1 - x0;
	dy = abs(y1 - y0);

	err = dx / 2;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void ST7735::drawRoundRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint16_t color)
{
	drawFastHLine(x + r, y, w - 2 * r, color);
	drawFastHLine(x + r, y + h - 1, w - 2 * r, color);
	drawFastVLine(x, y + r, h - 2 * r, color);
	drawFastVLine(x + w - 1, y + r, h - 2 * r, color);
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}
/// @brief 各丸矩形を描画する。
/// @param x1 左上のx座標
/// @param y1 左上のx座標
/// @param x2 右下のX座標
/// @param y2 右下のy座標
/// @param r 角の半径
/// @param color 矩形の色
void ST7735::drawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t r, uint16_t color)
{
	if (x1 > x2) _swap(x1, x2);
	if (y1 > y2) _swap(y1, y2);
	uint8_t w = x2 - x1;
	uint8_t h = y2 - y1;
	drawRoundRectWH(x1, y1, w, h, r, color);
}

/// @brief 		角丸の矩形を塗りつぶす
/// @param x 		左上のx座標
/// @param y 		左上のy座標
/// @param w 		幅
/// @param h 		高さ
/// @param r 		角の半径
/// @param color 		矩形の色
void ST7735::fillRoundRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint16_t color)
{
	
	fillRectWH(x + r, y, w - 2 * r, h, color);
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}
/// @brief 角丸の矩形を塗りつぶす
/// @param x1 左上のx座標
/// @param y1 左上のx座標
/// @param x2 右下のX座標
/// @param y2 右下のy座標
/// @param r 角の半径
/// @param color 矩形の色
void ST7735::fillRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t r, uint16_t color)
{
	if (x1 > x2) _swap(x1, x2);
	if (y1 > y2) _swap(y1, y2);
	uint8_t w = x2 - x1;
	uint8_t h = y2 - y1;
	fillRoundRectWH(x1, y1, w, h, r, color);
}

/// @brief 		三角形を描画する
/// @param x0 			頂点1のx座標	
/// @param y0 			頂点1のy座標
/// @param x1 			頂点2のx座標
/// @param y1 			頂点2のy座標
/// @param x2			頂点3のx座標 
/// @param y2 			頂点3のy座標
/// @param color		三角形の色
void ST7735::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}
/// @brief 		三角形を塗りつぶす
/// @param x0 			頂点1のx座標
/// @param y0 			頂点1のy座標
/// @param x1 			頂点2のx座標
/// @param y1 			頂点2のy座標
/// @param x2 			頂点3のx座標
/// @param y2 			頂点3のy座標
/// @param color 		三角形の色
void ST7735::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12, sa, sb;
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		_swap(y0, y1);
		_swap(x0, x1);
	}
	if (y1 > y2) {
		_swap(y2, y1);
		_swap(x2, x1);
	}
	if (y0 > y1) {
		_swap(y0, y1);
		_swap(x0, x1);
	}
	if (y0 == y2) {
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;
    sa   = 0;
    sb   = 0;
  if(y1 == y2) last = y1;   
  else         last = y1-1; 
  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if(a > b) _swap(a,b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if(a > b) _swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}
#endif

#pragma endregion


#pragma region  テキスト表示関連メソッド

#if defined(TFT_ENABLE_TEXT)			// テキスト表示機能が有効な場合


void ST7735::setTextWrap(bool w)
{
	bTextWrap = w;
}

#if !defined TFT_ENABLE_FONTS		// フォント変更ができない、基本的な文字出力の場合

/// @brief 	1文字を描画する
/// @param x 		描画するx座標
/// @param y 		描画するy座標
/// @param c 		描画する文字
/// @param color 	文字の色
/// @param bg 		背景色
/// @param size 	文字のサイズ。1がデフォルト。2で2倍の大きさになる。
void ST7735::drawChar(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size)
{
	int8_t i, j;
	if ((x >= st7735Init.width) || (y >= st7735Init.height))
		return;
	if (size < 1) size = 1;
	if ((c < ' ') || (c > '~'))
		c = '?';
	for (i = 0; i < 5; i++) {
		uint8_t line;
		line = Font[(c - ASCII_OFFSET) * 5 + i];
		for (j = 0; j < 7; j++, line >>= 1) {
			if (line & 0x01) {
				if (size == 1)
					drawPixel(x + i, y + j, color);
				else
					fillRectWH(x + (i * size), y + (j * size), size, size, color);
			} else if (bg != color) {
				if (size == 1)
					drawPixel(x + i, y + j, bg);
				else
					fillRectWH(x + i * size, y + j * size, size, size, bg);
			}
		}
	}
}

/// @brief 		文字列を描画する
/// @param x		描画するx座標
/// @param y 		描画するy座標
/// @param _text 	描画する文字列
/// @param color 	文字の色
/// @param bg 		背景色
/// @param size 	文字のサイズ。1がデフォルト。2で2倍の大きさになる。
void ST7735::drawText(uint8_t x, uint8_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size)
{
	uint8_t cursor_x, cursor_y;
	uint16_t textsize, i;
	cursor_x = x, cursor_y = y;
	textsize = strlen(_text);
	for (i = 0; i < textsize; i++) {
		if (bTextWrap && ((cursor_x + size * 5) > st7735Init.width)) {
			cursor_x = 0;
			cursor_y = cursor_y + size * 7 + 3;
			if (cursor_y > st7735Init.height) cursor_y = st7735Init.height;
			if (_text[i] == ASCII_OFFSET) {
				continue;
			}
		}
		drawChar(cursor_x, cursor_y, _text[i], color, bg, size);
		cursor_x = cursor_x + size * 6;
		if (cursor_x > st7735Init.width) {
			cursor_x = st7735Init.width;
		}
	}
}
#else
GFXfont *_gfxFont;
void ST7735::setFont(const GFXfont *f)
{
	_gfxFont = (GFXfont *)f;
}

void ST7735::setFont(const char *name)
{
	for (int i = 0; i < 16; i++) {
		if (registeredFonts[i].name != NULL) {
			if (strcmp(name, registeredFonts[i].name) == 0) {
				_gfxFont = (GFXfont *)registeredFonts[i].font;
				return;
			}
		}
	}
}
bool ST7735::getFonts(char *buf, uint16_t len)
{
	buf[0] = 0;
	int curLen = 0;
	for (int i = 0; i < 16; i++) {
		if (registeredFonts[i].name != NULL) {
			if (curLen+strlen(registeredFonts[i].name) + 1 > len) {
				return false;
			}
			strcat(buf, registeredFonts[i].name);
			strcat(buf, ",");
			curLen += strlen(registeredFonts[i].name) + 1;
		}
	}
	buf[strlen(buf) - 1] = 0;
	return true;
}

void ST7735::drawText(uint16_t x, uint16_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size)
{
	uint8_t cursor_x, cursor_y, first_char, last_char;
	uint16_t textlen, i;

	cursor_x = x, cursor_y = y;
	first_char = _gfxFont->first;
	last_char = _gfxFont->last;
	textlen = strlen(_text);

	for (i = 0; i < textlen; i++) {
		uint8_t c = _text[i];
		if (c < first_char || c > last_char) {
			continue;
		}

		GFXglyph *glyph = &(_gfxFont->glyph[c - first_char]);
		uint8_t w = glyph->width, h = glyph->height;

		if ((w > 0) && (h > 0)) {  // bitmap available
			int16_t xo = glyph->xOffset;
			if (bTextWrap && ((cursor_x + size * (xo + w)) > st7735Init.width)) {
				cursor_x = 0;
				cursor_y += (int16_t)size * _gfxFont->yAdvance;
			}
			drawChar(cursor_x, cursor_y, c, color, bg, size);
		}
		cursor_x += glyph->xAdvance * (int16_t)size;
	}
}

void ST7735::drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size)
{
	c -= (uint8_t)(_gfxFont->first);
	GFXglyph *glyph = _gfxFont->glyph + c;
	uint8_t *bitmap = _gfxFont->bitmap;

	uint16_t bo = glyph->bitmapOffset;
	uint8_t w = glyph->width, h = glyph->height;
	int8_t xo = glyph->xOffset, yo = glyph->yOffset;
	uint8_t xx, yy, bits = 0, abit = 0;
	int16_t xo16 = 0, yo16 = 0;

	if (size > 1) {
		xo16 = xo;
		yo16 = yo;
	}

	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
			if (!(abit++ & 7)) {
				bits = bitmap[bo++];
			}
			if (bits & 0x80) {
				if (size == 1) {
					drawPixel(x + xo + xx, y + yo + yy, color);
				} else {
					fillRect(x + (xo16 + xx) * size, y + (yo16 + yy) * size, size, size, color);
				}
			} else {
				if (isTransparentColor && bg == bmpTransparentColor) {
					// 何もしない
				} else {
					if (size == 1) {
						drawPixel(x + xo + xx, y + yo + yy, bg);
					} else {
						fillRect(x + (xo16 + xx) * size, y + (yo16 + yy) * size, size, size, bg);
					}
				}
				
			}

			bits <<= 1;
		}
	}
}
#endif

#pragma region 漢字表示関連メソッド
#ifdef TFT_ENABLE_KANJI				// 漢字表示が可能な場合
/// @brief 漢字１文字を表示する
/// @param x 表示するX座標
/// @param y 表示するY座標
/// @param utf8codes 表示するUTF8、もしくはASCIIコード
/// @param color 表示色
/// @param bg 背景色
void ST7735::drawKanji(uint16_t &x, uint16_t &y, uint32_t utf8codes, uint16_t color, uint16_t bg)
{
	const uint8_t *bmpData;
	uint8_t w;
	uint8_t h;
	if (utf8codes <= 0xFF) {  // １バイト文字
		const AsciiFont *pFont = KanjiHelper::FindAscii(utf8codes);
		if (pFont == NULL) {
			x = x + AFont[0].width;
			return;
		}
		bmpData = pFont->bmpData;
		w = pFont->width;
		h = pFont->height;
	} else {
		const KanjiFont *pFont = KanjiHelper::FindKanji(utf8codes);
		if (pFont == NULL) {
			x = x + KFont[0].width;
			return;
		}
		bmpData = pFont->bmpData;
		w = pFont->width;
		h = pFont->height;
	}
	if (bTextWrap) {
		if ((x + w) > st7735Init.width) {
			x = 0;
			y = y + h;
		}
	}
	drawKanjiBlock(x, y, w, h, bmpData, color, bg);
	/*
	uint8_t w_bytes = (w + 8 - 1) / 8;  // 横方向のバイト数
	uint8_t h_bytes = h;                // 縦方向のバイト数
	int16_t bmpIdx = 0;                 // ビットマップ情報には、bmp + yy*w_bytes + xx でアクセスできるが、順番に並んでいるので最初から順に読むほうが速いのでは？
	for (int8_t yy = 0; yy < h_bytes; yy++) {
		for (int8_t xx = 0; xx < w_bytes; xx++) {
			int xxx = x + (xx * 8); 		// そのバイトの描画起点
			uint8_t bits = bmpData[bmpIdx];
			for (int8_t bb = 0; bb < 8; bb++) {
				if (bits & 0x80) {
					drawPixel(xxx, y + yy, color);
				} else {
					drawPixel(xxx, y + yy, bg);
				}
				xxx++;
				bits <<= 1;
			}
			bmpIdx++;
		}
	}
	*/
	x = x + w;
	return;
}


void ST7735::drawKanjiBlock(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *bmpData, uint16_t color,uint16_t bg)
{
	setAddrWindow(x, y, x + w -1 , y + h -1);  // 漢字ブロックの大きさでアドレスウインドウを設定
	uint8_t w_bytes = (w + 8 - 1) / 8;  // 横方向のバイト数
	uint8_t h_bytes = h;                // 縦方向のバイト数
	int16_t bmpIdx = 0;                 // ビットマップ情報には、bmp + yy*w_bytes + xx でアクセスできるが、順番に並んでいるので最初から順に読むほうが速いのでは？
	bool isByteMultiple = (w % 8 == 0); // 横幅が8の倍数かのフラグ 
	for (int8_t yy = 0; yy < h_bytes; yy++) {
		for (int8_t xx = 0; xx < w_bytes; xx++) {
			uint8_t bitCnt;
			if (isByteMultiple) {
				bitCnt = 8;
			} else {
				bitCnt = (xx != (w_bytes - 1)) ? 8 : (w % 8);
			}
			uint8_t bits = bmpData[bmpIdx];
			for (int8_t bb = 0; bb < bitCnt; bb++) {
				if (isTransparentColor && bg == bmpTransparentColor) {
					if (bits & 0x80) {
						drawPixel(x+(xx*8)+bb,y+yy,color);
					} 					
				} else {
					if (bits & 0x80) {
						writeData(color >> 8);
						writeData(color & 0xFF);
					} else {
						writeData(bg >> 8);
						writeData(bg & 0xFF);
					}
				}
				bits <<= 1;
			}
			bmpIdx++;
		}
	}
}


void ST7735::drawTextKanji(uint16_t x, uint16_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size)
{
	uint16_t cursor_x, cursor_y;
	//uint8_t first_char, last_char;
	uint16_t textlen, i;

	cursor_x = x, cursor_y = y;

	for (i = 0; i < strlen(_text);) {

		uint32_t utf8codes = 0;
		uint8_t step = 0;
		if ((_text[i] & 0x80) == 0x00) {
			step = 1;  			// 1バイト文字 (ASCII)
			utf8codes = _text[i];
		} else if ((_text[i] & 0xE0) == 0xC0) {
			step = 2;  // 2バイト文字
			utf8codes = (_text[i] << 8) | _text[i + 1];
		} else if ((_text[i] & 0xF0) == 0xE0) {
			step =  3;  // 3バイト文字
			utf8codes = (_text[i] << 16) | (_text[i + 1] << 8) | _text[i+2];
		} else if ((_text[i] & 0xF8) == 0xF0) {
			utf8codes = (_text[i] << 24) | (_text[i + 1] << 16) | (_text[i +2] << 8) | _text[i + 3];
			step = 4;  // 4バイト文字
		} else {
			return ;  // 不正なUTF-8バイト
		}
		#ifdef TFT_FORCE_HANKANA					// 半角カナを1バイト文字として処理
		uint16_t top2bytes = (uint16_t)(utf8codes >> 8);
		if (top2bytes == 0xefbd)  {
			utf8codes = utf8codes & 0x000000FF;
		} else if (top2bytes == 0xefbe) {
			utf8codes = (utf8codes & 0x000000FF) + 0x40;
		}
		#endif
		drawKanji(cursor_x, cursor_y, utf8codes, color, bg);

		if (utf8codes <= 0xFF ){  // １バイト文字
			const AsciiFont *pFont = KanjiHelper::FindAscii(utf8codes);
		} else {
			const KanjiFont *pFont = KanjiHelper::FindKanji(utf8codes);
		}
		i += step;
	}
}
#endif
#pragma endregion


#endif
#pragma endregion 



#pragma region スクロール機能関連メソッド

#ifdef TFT_ENABLE_SCROLL

void ST7735::setScrollDefinition(uint8_t top_fix_height, uint8_t bottom_fix_height, bool _scroll_direction)
{
	uint8_t scroll_height;
	scroll_height = st7735Init.height - top_fix_height - bottom_fix_height;
	writeCommand(ST7735Cmd.VSCRDEF);
	writeData(0x00);
	writeData(top_fix_height);
	writeData(0x00);
	writeData(scroll_height);
	writeData(0x00);
	writeData(bottom_fix_height);
	writeCommand(ST7735Cmd.MADCTL);
	if (_scroll_direction) {
		if (st7735Init.tft_type == 0) {
			writeData(0xD8);
		}
		if (st7735Init.tft_type == 1) {
			writeData(0xD0);
		}
		if (st7735Init.tft_type == 2) {
			writeData(0x18);
		}
	} else {
		if (st7735Init.tft_type == 0) {
			writeData(0xC8);
		}
		if (st7735Init.tft_type == 1) {
			writeData(0xC0);
		}
		if (st7735Init.tft_type == 2) {
			writeData(0x08);
		}
	}
}
void ST7735::verticalScroll(uint8_t _vsp)
{
	writeCommand(ST7735Cmd.VSCRSADD);
	writeData(0x00);
	writeData(_vsp);
}
#endif
#pragma endregion

#pragma region ビットマップ表示関連メソッド
#ifdef TFT_ENABLE_BITMAP

void ST7735::bmpDraw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *p,uint8_t direction)
{
	if (direction == 1) {
		writeCommand(ST7735Cmd.MADCTL);
		writeData(0b10000000);
		x = getWidth() - x - w;
	} 
	uint16_t* pBase = p;
	if (isTransparentColor) {
		for (uint16_t yy = 0; yy < h; yy++) {
			for (uint16_t xx = 0; xx < w; xx++) {
				uint16_t c = *p;
				if (isTransparentColor && c == bmpTransparentColor) {
					// 透過色なら何もしない
				} else {
					drawPixel(xx+x, yy+y, c);
				}
				p++;
			}
		}
	} else {								// 透過色処理をしないなら、高速で書き込める
		setAddrWindow(x, y, x + w - 1, y + h - 1);  // 漢字ブロックの大きさでアドレスウインドウを設定
		for (uint16_t yy = 0; yy < h; yy++) {
			for (uint16_t xx = 0; xx < w; xx++) {
				uint16_t c = *p;
				if (isTransparentColor && c == bmpTransparentColor) {
					// 透過色なら何もしない
					// と思ったらそういうことはできない。ウインドウ指定を一回にする場合、スキャンを飛ばす方法がないので。
				} else {
					writeData(c >> 8);
					writeData(c & 0xFF);
				}
					p++;
			}
		}
	}
	if (direction == 1) {
		writeCommand(ST7735Cmd.MADCTL);
		writeData(0b11000000);
	}
}
#endif
#pragma endregion

// ------ not functional yet ----------------------------------------------
// needs fat-library

#if defined TFT_ENABLE_BMP
#pragma region 未使用のメソッド
#ifndef pixel_buffer
  #define pixel_buffer  10
#endif



void ST7735::pushColor(uint16_t color)
{
	uint8_t hi, lo;
	hi = color >> 8;
	lo = color;
	pSpiHW->DCHigh();
	pSpiHW->CSLow();
	pSpiHW->spiWrite(hi);
	pSpiHW->spiWrite(lo);
	pSpiHW->CSHigh();
}


bool bmpDraw(int8_t x, int8_t y, int8_t *bmpname) {
  bool ec = 0, padding = 0;
  int8_t bmpdata[pixel_buffer * 3],
       planes, depth, r, g, b, col, row;
  int16 i, buffer = pixel_buffer * 3, format, bmpHeight, bmpWidth, color;
  int32 offset, compression, bmp_size, row_size, padding_factor;
  if((x >= st7735Init.width) || (y >= st7735Init.height))
    return false;
  if(fat16_open_file(bmpname) != 0)
    return false;
  ec |= sdcard_read_byte(address_pointer + 1, &format);
  format <<= 8;
  ec |= sdcard_read_byte(address_pointer, &format);
  if(format != 0x4D42)                           // BMP file format signature
    return false;                                // Return error
  ec |= sdcard_read_byte(address_pointer + 0x0D, &offset);
  offset <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x0C, &offset);
  offset <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x0B, &offset);
  offset <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x0A, &offset);
  ec |= sdcard_read_byte(address_pointer + 0x13, &bmpWidth);
  bmpWidth <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x12, &bmpWidth);
  ec |= sdcard_read_byte(address_pointer + 0x17, &bmpHeight);
  bmpHeight <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x16, &bmpHeight);
  ec |= sdcard_read_byte(address_pointer + 0x1A, &planes);
  ec |= sdcard_read_byte(address_pointer + 0x1C, &depth);
  ec |= sdcard_read_byte(address_pointer + 0x21, &compression);
  compression <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x20, &compression);
  compression <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x1f, &compression);
  compression <<= 8;
  ec |= sdcard_read_byte(address_pointer + 0x1e, &compression);
  if(ec != 0 || compression != 0 || depth != 24 || planes != 1)
    return false;
  bmp_size = file_size - offset;            // bmp_size: BMP image raw size
  row_size = bmp_size / bmpHeight;          // row_size: number of bytes per row
  if((x + bmpWidth  - 1) >=  st7735Init.width) {      // st7735Init.width is the TFT screen width
    bmpWidth = st7735Init.width  - x;
    padding = 1;           //Padding = 1 ==> only upper left part will be displayed
    padding_factor = bmpWidth / pixel_buffer;
    if(bmpWidth % pixel_buffer)
      padding_factor++;
    padding_factor *= buffer;
  }
  if((y + bmpHeight - 1) >= st7735Init.height) {       // st7735Init.height is the TFT screen height
    offset += row_size * (bmpHeight - st7735Init.height + y);  // Only upper part will be displayed
    bmpHeight = st7735Init.height - y;
  }
  file_pointer     = offset;
  address_pointer += offset;
  i = buffer;
  if(_tft_type != 2){
  // Change row address order
    write_command(ST7735_MADCTL);
    if(_tft_type == 0)
      write_data(0x48);
    if(_tft_type == 1)
      write_data(0x40);
  }
  setAddrWindow(x , y, x + bmpWidth - 1, y + bmpHeight - 1);
  for(row = 0; row < bmpHeight; row++){
    for(col = 0; col < bmpWidth; col++){
      if(i >= buffer){
        i = 0;
        fat16_read_data(buffer, bmpdata);
      }
      b = bmpdata[i++];
      g = bmpdata[i++];
      r = bmpdata[i++];
      color = Color565(r, g, b);
      pushColor(color);
    }
    if(padding == 1){
      i = buffer;
      file_pointer    += row_size - padding_factor;
      address_pointer += row_size - padding_factor;
    }
  }
  if(_tft_type != 2){
  // Return row address order as it was
    write_command(ST7735_MADCTL);
    if(_tft_type == 0)
      write_data(0xC8);
    if(_tft_type == 1)
      write_data(0xC0);
  }
  // Good BMP
  return true;
}

#endif
#pragma endregion
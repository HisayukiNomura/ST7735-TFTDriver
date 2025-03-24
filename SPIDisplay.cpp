#pragma GCC optimize ("O0")

#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#define TFT_ENABLE_BLACK
#include "hardware/spi.h"
#include "lib-st7735/include/ST7735_TFT.h"
#include "lib-st7735/include/hw.h"
#include "lib-st7735/include/font/Font_Mono9p.h"
#include "SPIDisplay_bmpdata.h"
// 1.8インチTFT液晶モジュール★128x160★ [M018C7735S541](https://www.aitendo.com/product/16367)

/*
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
*/
#define SPI_PORT SPI_TFT_PORT
#define PIN_MISO PIN_TFT_RX                 // 使用しない？
#define PIN_CS   PIN_TFT_CS         // CSn (Chip Select)
#define PIN_SCK  PIN_TFT_SCK         // SCK
#define PIN_MOSI PIN_TFT_TX        // TX
volatile uint16_t iRet;

ST7735* pST7735;

Axis8 randAxis(Axis8& axis)
{
	axis.x = rand() % pST7735->getWidth();
	axis.y = rand() % pST7735->getHeight();
	return axis;
}
Size8 randSize(Size8& size)
{
	size.w = rand() % pST7735->getWidth();
	size.h = rand() % pST7735->getHeight();
	return size;
}
void randAxisSize(Axis8& axis , Size8& size)
{
	int x0 = rand() % pST7735->getWidth();
	int y0 = rand() % pST7735->getHeight();
	int x1 = rand() % pST7735->getWidth();
	int y1 = rand() % pST7735->getHeight();
	if (x0 > x1) _swap(x0, x1);
	if (y0 > y1) _swap(y0, y1);
	int w = x1 - x0;
	int h = y1 - y0;

	axis.x = x0;
	axis.y = y0;
	size.w = w;
	size.h = h;
}

Region8 randRegion(Region8& region)
{
	int x0 = rand() % pST7735->getWidth();
	int y0 = rand() % pST7735->getHeight();
	int x1 = rand() % pST7735->getWidth();
	int y1 = rand() % pST7735->getHeight();
	if (x0 > x1) _swap(x0, x1);
	if (y0 > y1) _swap(y0, y1);
	region.start.x = x0;
	region.start.y = y0;
	region.end.x = x1;
	region.end.y = y1;
	return region;
}

/// @brief ビットマップ表示のデモ
/// @param st7735
#ifdef TFT_ENABLE_BITMAP 
void demo_bmp(ST7735& st7735)
{


	st7735.fillScreen(ST7735Color.BLACK);

    // 大きなビットマップの表示
	st7735.bmpUnuseTransColor();			// 透過無し
	st7735.bmpDraw(0,0, st7735.getWidth(),st7735.getHeight() ,(uint16_t*)hiroshige,0);
	for (int i = 0;i<10;i++) {
		st7735.bmpDraw(rand() % (st7735.getWidth() - 50), rand() % (st7735.getHeight() - 80), 48, 77, (uint16_t*)sharaku, 0);
	}
	sleep_ms(1000);

	st7735.bmpUseTrasColor(0b0000100000100001);	// 透過あり
	st7735.bmpDraw(0,0, st7735.getWidth(),st7735.getHeight() ,(uint16_t*)hiroshige,0);
	for (int i = 0;i<10;i++) {
		st7735.bmpDraw(rand() % (st7735.getWidth() - 50), rand() % (st7735.getHeight() - 80), 48, 77, (uint16_t*)sharaku, 0);
	}
	sleep_ms(1000);

	st7735.fillScreen(ST7735Color.GREEN);

	// 透過あり
	for (int i = 4; i < st7735.getWidth() - 20; i += 4) {
		st7735.bmpUseTrasColor(0b0000100000100001);
		const uint16_t* p;
		if (i % 3 == 0)
			p = bmp1;
		else if (i % 3 == 1)
			p = bmp2;
		else
			p = bmp3;
		st7735.fillRectWH(i - 4, 64, 16, 16, ST7735Color.GREEN);
		st7735.bmpDraw(i, 64, 16, 16, (uint16_t*)p, 0);

		sleep_ms(100);
	}
	for (int i = st7735.getWidth() - 20; i > 4; i -= 4) {
		st7735.bmpUseTrasColor(0b0000100000100001);
		const uint16_t* p;
		if (i % 3 == 0)
			p = bmp1;
		else if (i % 3 == 1)
			p = bmp2;
		else
			p = bmp3;
		st7735.fillRectWH(i + 4, 64, 16, 16, ST7735Color.GREEN);
		st7735.bmpDraw(i, 64, 16, 16, (uint16_t*)p, 1);

		sleep_ms(100);
	}
	//　透過無し
	st7735.fillScreen(ST7735Color.BLACK);
	for (int i = 4; i < st7735.getWidth() - 20; i += 4) {
		st7735.bmpUnuseTransColor();
		const uint16_t* p;
		if (i % 3 == 0)
			p = bmp1;
		else if (i % 3 == 1)
			p = bmp2;
		else
			p = bmp3;
		st7735.fillRectWH(i - 4, 64, 16, 16, ST7735Color.BLACK);
		st7735.bmpDraw(i, 64, 16, 16, (uint16_t*)p, 0);

		sleep_ms(100);
	}
	for (int i = st7735.getWidth() - 20; i > 4; i -= 4) {
		st7735.bmpUnuseTransColor();
		const uint16_t* p;
		if (i % 3 == 0)
			p = bmp1;
		else if (i % 3 == 1)
			p = bmp2;
		else
			p = bmp3;
		st7735.fillRectWH(i + 4, 64, 16, 16, ST7735Color.BLACK);
		st7735.bmpDraw(i, 64, 16, 16, (uint16_t*)p, 1);

		sleep_ms(100);
	}
	sleep_ms(1000);
}
#endif

/// @brief 漢字の表示とスクロールのデモ
/// @param st7735 画面表示のためのオブジェクト
#if defined(TFT_ENABLE_KANJI) && defined(TFT_ENABLE_TEXT)
void demo_Kanji(ST7735 &st7735)
{
	// 全角半角カナ混じり
	st7735.st7735Init.setRotation(ST7735_ROTATION::NORMAL);
	st7735.fillScreen(ST7735Color.BLACK);
	st7735.drawTextKanji(0, 15, "半角｢ﾊﾝｶｸ｣カナは日本におけるﾊﾟｰｿﾅﾙｺﾝﾋﾟｭｰﾀの黎明期から存在し、JISｺｰﾄﾞ等が利用可能になるまではｺﾝﾋﾟｭｰﾀ上で日本語を表現できる唯一の方法だった。", ST7735Color.WHITE, ST7735Color.BLACK, 1);
	sleep_ms(2000);
	// 日本語表示デモ
	st7735.st7735Init.setRotation(ST7735_ROTATION::NORMAL);
	st7735.fillScreen(ST7735Color.BLACK);
	st7735.drawTextKanji(0, 15, "祇園精舎の鐘の聲、諸行無常の響あり。沙羅双樹の花の色、盛者必衰の理をあらはす。驕れる人も久しからず、唯春の夜の夢の如し。猛き者もつひには滅びぬ、偏に風の前の塵に同じ。", ST7735Color.WHITE, ST7735Color.BLACK, 1);
	sleep_ms(2000);
	#ifdef TFT_ENABLE_SCROLL
	for (int i = 0; i < st7735.getHeight(); i++) {
		st7735.setScrollDefinition(0, 0, true);
		st7735.verticalScroll(i);
		sleep_ms(10);
	}
	st7735.verticalScroll(0);
	#endif
	st7735.st7735Init.setRotation(ST7735_ROTATION::DEGREE_90);
	st7735.fillScreen(ST7735Color.WHITE);
	st7735.drawTextKanji(0, 15, "春はあけぼの。やうやう白くなりゆく山ぎは少し明りて、紫だちたる雲の細くたなびきたる。夏は夜。月の頃はさらなり。闇もなほ。螢の多く飛び違ひたる。また、ただ一つ二つなど、ほのかにうち光りて行くもをかし。雨など降るもをかし。", ST7735Color.BLACK, ST7735Color.WHITE, 1);
	sleep_ms(2000);
	st7735.st7735Init.setRotation(ST7735_ROTATION::NORMAL);
	#ifdef TFT_ENABLE_SCROLL
	for (int i = 0; i < st7735.getHeight(); i++) {
		st7735.setScrollDefinition(0, 0, false);
		st7735.verticalScroll(i);
		sleep_ms(10);
	}
	st7735.verticalScroll(0);
	#endif
}
#endif

/// @brief フォント指定テキスト出力とスクロールのデモ
/// @param st7735
#if defined(TFT_ENABLE_FONTS) && defined(TFT_ENABLE_TEXT)
void demo_fonts(ST7735 &st7735)
{
	#ifdef TFT_ENABLE_FONTS
	st7735.setFont(&FreeMono9pt7b);
	#endif
	st7735.fillScreen(ST7735Color.BLACK);
	const char* txt = "The quick brown fox jumps over the lazy dog";
	st7735.drawText(0, 30, txt, ST7735Color.WHITE, ST7735Color.BLACK, 1);
	st7735.fillRectangle(0, st7735.getHeight() - 15, st7735.getWidth(), st7735.getHeight(), ST7735Color.WHITE);
	st7735.drawText(0, st7735.getHeight() - 3, "FIXED AREA", ST7735Color.BLACK, ST7735Color.WHITE, 1);
	st7735.fillRectangle(0, 0, st7735.getWidth(), 15, ST7735Color.WHITE);
	st7735.drawText(0, 13, "FIXED AREA", ST7735Color.BLACK, ST7735Color.WHITE, 1);

	sleep_ms(2000);
	#ifdef TFT_ENABLE_SCROLL
		st7735.setScrollDefinition(15, 15, true);
		for (int i = 15; i < st7735.getHeight() - 15; i++) {
			st7735.verticalScroll(i);
			sleep_ms(10);
		}
		st7735.verticalScroll(0);
	#endif
}
#endif

/// @brief 通常テキスト出力とスクロールのデモ
/// @param st7735
#ifdef TFT_ENABLE_TEXT
void demo_text(ST7735 &st7735)
{
	st7735.fillScreen(ST7735Color.BLACK);
	st7735.fillRectangle(0, 0, st7735.getWidth(), 15, ST7735Color.WHITE);
	st7735.drawText(0, 0, "<< FIXED AREA >>", ST7735Color.BLACK, ST7735Color.WHITE, 1);
	st7735.fillRectangle(0, st7735.getHeight() - 15, st7735.getWidth(), 15, ST7735Color.WHITE);

	st7735.drawText(0, st7735.getHeight() - 15, "FIXED AREA", ST7735Color.BLACK, ST7735Color.WHITE, 1);

	const char* txt = " hello world";
	st7735.drawText(0, 15, txt, ST7735Color.WHITE, ST7735Color.BLACK, 1);
	st7735.drawText(0, 30, txt, ST7735Color.BLUE, ST7735Color.BLACK, 1);
	st7735.drawText(0, 45, txt, ST7735Color.RED, ST7735Color.BLACK, 1);
	st7735.drawText(0, 60, txt, ST7735Color.GREEN, ST7735Color.BLACK, 1);
	st7735.drawText(0, 75, txt, ST7735Color.CYAN, ST7735Color.BLACK, 1);
	st7735.drawText(0, 90, txt, ST7735Color.MAGENTA, ST7735Color.BLACK, 1);
	st7735.drawText(0, 105, txt, ST7735Color.YELLOW, ST7735Color.BLACK, 1);
	st7735.drawText(0, 120, txt, ST7735Color.WHITE, ST7735Color.BLACK, 1);
#ifdef TFT_ENABLE_SCROLL
	st7735.setScrollDefinition(15, 15, true);
	for (int i = 15; i < st7735.getHeight() - 15; i++) {
		st7735.verticalScroll(i);
		sleep_ms(100);
	}
	st7735.verticalScroll(0);
#endif
}
#endif

/// @brief 画面のクリア、縦直線、横直線、矩形、点の描画など、基本描画機能のデモ。
/// @param st7735 
void demo_basic(ST7735 &st7735)
{
	st7735.fillScreen(ST7735Color.BLACK);
	// FastxDrawLine のテスト
	for (int i = 0; i < st7735.getHeight(); i += 10) {
		st7735.drawFastHLine(0, i, st7735.getWidth(), ST7735Color.RED);
	}
	for (int i = 0; i < st7735.getWidth(); i += 10) {
		st7735.drawFastVLine(i, 0, st7735.getHeight(), ST7735Color.GREEN);
	}
	for (int i = 0; i < st7735.getWidth(); i += 10) {
		for (int j = 0; j < st7735.getHeight(); j += 10) {
			uint16_t color = rand() % 0xFFFF;
			st7735.fillRectWH(i + 2, j + 2, 6, 6, color);
		}
	}
	st7735.fillScreen(ST7735Color.BLACK);
	for (int i = 0; i < 3000; i++) {
		uint16_t x0 = rand() % st7735.getWidth();
		uint16_t y0 = rand() % st7735.getHeight();
		uint16_t color = rand() % 0xFFFF;
		st7735.drawPixel(x0, y0, color);
	}

	// 画面斑点のデモ
	for (int i = 0; i < 10; i++) {
		st7735.invertDisplay(true);
		sleep_ms(100);
		st7735.invertDisplay(false);
		sleep_ms(100);
	}
	st7735.NormalDisplay();
	st7735.fillScreen(ST7735Color.BLACK);
}

/// @brief 図形描画のデモ
/// @param st7735 
#ifdef TFT_ENABLE_SHAPES
void demo_shapes(ST7735 st7735)
{
	for (int i = 0; i < 20; i++) {
		Axis8 center;
		randAxis(center);
		int r = rand() % st7735.getWidth() / 2;
		int c = rand() % 0xFFFF;
		if (i < 10) {
			st7735.drawCircle(center, r, c);
		} else {
			st7735.fillCircle(center, r, c);
		}
	}


	for (int i = 0; i < 30; i++) {
		Axis8 start;
		Size8 size;

		int c = rand() % 0xFFFF;
		uint8_t r = rand() % 6 + 3;
		while (true) {
			randAxisSize(start, size);
			if (size.w < 20 || size.h < 20) continue;
			break;
		}
		if (i < 10) {
				st7735.drawRectWH(start, size, c);
		} else if (i < 20) {
			st7735.fillRectWH(start, size, c);
		} else if (i<30) {
			st7735.drawRoundRectWH(start, size, r, c);
		} else if (i<40) {
			st7735.fillRoundRectWH(start, size, r, c);
		}
	}


	for (int i = 0; i < 30; i++) {
		Region8 region;

		uint8_t r = rand() % 6 + 3;
		while (true) {
			randRegion(region);
			if ((region.end.x - region.start.x) < 20) continue;
			if ((region.end.y - region.start.y) < 20) continue;
			break;
		}
		int c = rand() % 0xFFFF;
		if (i<10) {
			st7735.drawRect(region, c);
		} else if (i<20) {
			st7735.fillRect(region, c);
		} else if (i<30) {
			st7735.drawRoundRect(region, r, c);
		} else if (i<40) {
			st7735.fillRoundRect(region, r, c);
		}

		st7735.drawRoundRect(region, 5, c);
	}

	for (int i = 0; i < 100; i++) {
		Region8 region;
		randRegion(region);
		int c = rand() % 0xFFFF;
		st7735.drawLine(region, c);
	}
}
#endif
HW spiHW(0, 16, 17, 18, 19, 28, 15);



int main()
{
	stdio_init_all();

	// SPI initialisation. This example will use SPI at 1MHz.
     iRet = spi_init(spiHW.portSPI, 10*1000*1000);		// これくらいはいける。15Mhzくらいまでという話も。
	//iRet = spi_init(spiHW.portSPI, 1 * 1000 * 1000);

	gpio_set_function(spiHW._dc, GPIO_FUNC_SPI);
    //gpio_set_function(spiHW._cs,  GPIO_FUNC_SPI);
    gpio_set_function(spiHW._sck,   GPIO_FUNC_SPI);
    gpio_set_function(spiHW._tx, GPIO_FUNC_SPI);
    gpio_put(spiHW._cs,1);

    spiHW.init();      // ライブラリ初期化
	ST7735	st7735(spiHW);
	pST7735 = &st7735;
	
	st7735.doInit();


	iRet = spi_get_index (SPI_PORT);
    iRet =  spi_is_writable (SPI_PORT);


	while (1) {
		/*
		st7735.fillScreen(ST7735Color.BLACK);
		st7735.fillScreen(ST7735Color.BLUE);
		st7735.setScrollDefinition(15, 15, true);
		st7735.setScrollDefinition(0, 0, true);
		st7735.verticalScroll(0);
		st7735.SetRotation(DEGREE_90);
		st7735.invertDisplay(false);
		st7735.doInit();
		while (1) {
		}
		*/

#if defined(TFT_ENABLE_TEXT)
		st7735.setTextWrap(true);
#endif
		st7735.SetRotation(ST7735_ROTATION::NORMAL);
#ifdef TFT_ENABLE_BITMAP
		demo_bmp(st7735);
#endif
// テキスト表示のデモ
#if defined(TFT_ENABLE_KANJI) && defined(TFT_ENABLE_TEXT)
		demo_Kanji(st7735);
#endif


#if defined(TFT_ENABLE_FONTS) && defined(TFT_ENABLE_TEXT)
		demo_fonts(st7735);
#elif defined(TFT_ENABLE_TEXT) 
		demo_text(st7735);
#endif

		// 基本描画機能のデモ
		demo_basic(st7735);

#ifdef TFT_ENABLE_SHAPES
		// 図形描画のテスト
		demo_shapes(st7735);
#endif
		// void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
		// void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

		st7735.fillScreen(ST7735Color.BLACK);
		sleep_ms(1000);
	}
}


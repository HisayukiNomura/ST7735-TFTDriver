// --------------------------------------------------------------------------
// ST7735-library (hw-specific defines and interfaces)
//
// If you want to port the library to a different platform, change this
// include (pins and ports, includes, function-map at the end of the file).
//
// Author: Bernhard Bablok
// 02/21 ILI9341対応に向けて分岐
// https://github.com/bablokb/pico-st7735
// http :  // pan.jczn1688.com/directlink/1/SPI%20%20display%20module/2.4inch_SPI_Module_ILI9341.zip
// --------------------------------------------------------------------------

#ifndef _HW_H
#define _HW_H

#include "hardware/spi.h"
#include "pico/stdlib.h"

// ----------------------------------------------------------------
// necessary includes

#include "hardware/spi.h"

// ----------------------------------------------------------------
// function-map
#ifdef __delay_ms
#undef __delay_ms
#endif
#define __delay_ms(x) sleep_ms(x)

		/// @brief ST7735の待ち時間を設定するクラス。デバッグ時には短い時間で動作させるために、DBG_SHORT_WAITをオンにする。
		/// そうしないと、信号の監視をするときに信号幅に対して、待ち時間が長くなりすぎて信号の確認がやりにくくなるため。
		/// もちろん、このシンボルを有効にしていると実際のデバイスは動作しない。
		// #define DBG_SHORT_WAIT
		class {
   public:
	#ifndef DBG_SHORT_WAIT
	/// @brief SPI通信で、最長のウエイトが必要な時の待ち時間（ms)
	const uint16_t MAX = 500;
	/// @brief SPI通信で、長いウエイトが必要な時の待ち時間（ms)
	const uint8_t LONG = 255;
	/// @brief SPI通信で、やや長いウエイトが必要な時の待ち時間（ms)
	const uint8_t MIDLONG = 150;
	/// @brief SPI通信で、中くらいのウエイトが必要な時の待ち時間（ms)
	const uint8_t MIDDLE = 50;
	/// @brief SPI通信で、短いウエイトが必要な時の待ち時間（ms)
	const uint8_t SHORT = 10;
	#else
	const uint8_t MAX = 5;
	const uint8_t LONG = 5;
	const uint8_t MIDLONG = 5;
	const uint8_t MIDDLE = 5;
	const uint8_t SHORT = 5;
	#endif
	/// @brief 最大の待ち時間として500ms待つ
	inline void delay_max(void) { sleep_ms(MAX); };
	/// @brief 長めの待ち時間として255ms待つ
	inline void delay_long(void) { sleep_ms(LONG); };
	/// @brief 中間の待ち時間として150ms待つ
	inline void delay_midlong(void) { sleep_ms(MIDLONG); }
	/// @brief 中間の待ち時間として50ms待つ
	inline void delay_middle(void) { sleep_ms(MIDDLE); }
	/// @brief 短い待ち時間として10ms待つ
	inline void delay_short(void) { sleep_ms(SHORT); }
} ST7735Sleep;

/// @brief ハードウェア関連の処理や設定を行う為のクラス。使用するピンを変える場合などは、このクラスの変数でデフォルト値を変更するか、
/// このクラスのコンストラクタでポート番号を指定する。
/// このクラスは静的ではないので、インスタンス化してから使用する。無いとは思うが、２つのLCDを接続して制御するときは、２つのインスタンスをそれぞれ
/// 作成、初期化して初期化処理を行う。
class HW 
{
	
	/// SPIで使用するポートの定義（デフォルト値。コンストラクタで変更可能）
	/// @brief SPIで使用するデフォルトのピン番号。RXの信号線（GP16）
	const uint8_t PIN_TFT_RX = 16;
	/// @brief  ST7735のData/Commandで使用するピン番号。ST7735では、RX(MISO）は使用せず、Data/CommandとしてRXのピンを使用する。（自力で制御する）
	const uint8_t PIN_TFT_DC =PIN_TFT_RX;
	/// @brief SPIのCSnのデフォルトピン
	const uint8_t PIN_TFT_CS = 17;   // SPI CSn
	/// @brief SPIのTX(MOSI)信号ので使用するデフォルトのピン番号
	const uint8_t PIN_TFT_TX = 19;   // SPI . マスターからの出力情報
	/// @brief SPIのクロック(SCK)で使用するデフォルトのピン番号
  	const uint8_t PIN_TFT_SCK = 18;  // SCK

  	/// @brief リセットピンで使用するポートの番号。無くても動く（電源に直結）。０を指定するとリセットピンは使用しない
  	const uint8_t PIN_TFT_RST = 28;
	/// @brief デバッグピンの定義（SPIの信号出力時にストローブしたり、Highになる）。ゼロを指定するとデバッグピンは使用しない
  	const uint8_t PIN_TFT_DEBUG = 15;

  /// @brief 各種の設定を集めた構造体。オン/オフの為に１バイト使うのがアレなのでこうしたらしい。
  /// PICの時の癖。RasPicoはメモリが潤沢なのでこういうのはいらなかったか。
  	struct {
    	uint8_t isUseDebugPin : 1;
    	uint8_t isUseResetPint : 1;
    	uint8_t isDebugModeOnOff : 1;
  	} options;

 	public:
  		/// @brief SPIポートへのポインタ
  		spi_inst_t* portSPI;
		/// @brief Data/Commandで使用するぽーと番号
		uint8_t _dc;
		uint8_t _cs;
		uint8_t _sck;
		uint8_t _tx;
		uint8_t _reset;  // 0の場合リセットピンは使用しない
		uint8_t _debug;

	private:
	 void initPort(uint8_t a_RXDC, uint8_t a_CS, uint8_t a_SCK, uint8_t a_TX,
				   uint8_t a_reset, uint8_t a_debug)
	 {
		 _dc = a_RXDC;
		 _cs = a_CS;
		 _sck = a_SCK;
		 _tx = a_TX;
		 _reset = a_reset;
		 _debug = a_debug;
	 }

 public:
  HW();
  HW(uint8_t spiPortNo);
  HW(uint8_t spiPortNo, uint8_t a_RXDC, uint8_t a_CS, uint8_t a_SCK,
     uint8_t a_TX, uint8_t a_reset, uint8_t a_debug);

  void init(void);
  void init(uint8_t spiPortNo, uint8_t a_RXDC, uint8_t a_CS, uint8_t a_SCK, uint8_t a_TX, uint8_t a_reset, uint8_t a_debug);

  int spiWrite(const uint8_t* data)
  {
    int ret = spi_write_blocking(portSPI, data, 1);
    return ret;
  }

  int spiWrite(uint8_t data)
  {
    int ret = spi_write_blocking(portSPI, &data, 1);
    return ret;
  }

  volatile inline void CSLow()
  {
    asm volatile("nop \n nop \n nop");
    gpio_put(_cs, 0);
    asm volatile("nop \n nop \n nop");
  }
  volatile inline void CSHigh()
  {
    asm volatile("nop \n nop \n nop");
    gpio_put(_cs, 1);
    asm volatile("nop \n nop \n nop");
  }
  volatile inline void DCLow()
  {
    asm volatile("nop \n nop \n nop");
    gpio_put(_dc, 0);
    asm volatile("nop \n nop \n nop");
  }

  volatile inline void DCHigh()
  {
    asm volatile("nop \n nop \n nop");
    gpio_put(_dc, 1);
    asm volatile("nop \n nop \n nop");
  }
  volatile inline void RSTLow()
  {
    asm volatile("nop \n nop \n nop");
    gpio_put(_reset, 0);
    asm volatile("nop \n nop \n nop");
  }
  volatile inline void RstHigh()
  {
    asm volatile("nop \n nop \n nop");
    gpio_put(_reset, 1);
    asm volatile("nop \n nop \n nop");
  }

  volatile void debugStrobe();
  volatile void debugOn();
  volatile void debugOff();
  volatile void debugIn();
  volatile void debugOut();
  volatile void Reset();

  void writeCommand(uint8_t cmd_);
  void writeData(uint8_t data_);
};

#define TFT_OPTIONS



#ifndef SPI_TFT_PORT
#define SPI_TFT_PORT spi1
#endif

#endif
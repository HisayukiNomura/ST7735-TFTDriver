
#pragma GCC optimize("O0")
#include "../include/hw.h"

/// @brief デフォルトコンストラクタ。Pythonの拡張モジュールで newが使えなかったので、引数なしのコンストラクタを追加して静的オブジェクトに対応する。
/// 通常使用では推奨しない。
HW::HW()
{
	portSPI = spi0;
	_dc = PIN_TFT_DC;
	_cs = PIN_TFT_CS;
	_sck = PIN_TFT_SCK;
	_tx = PIN_TFT_TX;
	_reset = PIN_TFT_RST;
	_debug = PIN_TFT_DEBUG;

	options.isUseDebugPin = 1;
	options.isUseResetPint = 1;
	options.isDebugModeOnOff = 0;
}


/// @brief コンストラクタ。使用するポート番号はデフォルト。
/// @param spiPortNo 使用するSPIのポート番号。0または1
HW::HW(uint8_t spiPortNo)
{
    portSPI = (spiPortNo == 0) ? spi0 : spi1;
    _dc = PIN_TFT_DC;
    _cs = PIN_TFT_CS;
    _sck = PIN_TFT_SCK;
    _tx = PIN_TFT_TX;
    _reset = PIN_TFT_RST;
    _debug = PIN_TFT_DEBUG;

    options.isUseDebugPin = 1;
    options.isUseResetPint = 1;
    options.isDebugModeOnOff = 0;
}
/// @brief コンストラクタ。使用するポート番号などを指定する
/// @param spiPortNo	使用するspiのポート番号。0または1 
/// @param a_RXDC		Data/Commandのポート番号 
/// @param a_CS			CSnのポート番号
/// @param a_SCK		SCKのポート番号 
/// @param a_TX			TXのポート番号 
/// @param a_reset		リセット信号のポート番号 
/// @param a_debug		デバッグ用のポート番号
HW::HW(uint8_t spiPortNo, uint8_t a_RXDC, uint8_t a_CS, uint8_t a_SCK,uint8_t a_TX, uint8_t a_reset, uint8_t a_debug)
{
	portSPI = (spiPortNo == 0) ? spi0 : spi1;
	initPort(a_RXDC, a_CS, a_SCK, a_TX, a_reset, a_debug);
};


/// @brief GPIOの初期化を行う。コンストラクタで指定されたポートを出力に設定する。
void HW::init(void) 
{
	gpio_init(_cs);
    gpio_set_dir(_cs, GPIO_OUT);
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_put(_cs, 1);  // Chip select is active-low

    //  ST7735では、RX(MISO)をData/Commandとして使うのでこれは不要
    // 代わりに、gpio_initを行って、RXではなく普通のGPIOに変更してOUT指定する
    gpio_init(_dc);
    gpio_set_dir(_dc, GPIO_OUT);
    gpio_put(_dc, 0);

    gpio_set_dir(_tx, GPIO_OUT);
    gpio_put(_tx, 0);

    // RESET信号の設定。リセットはSPIの一部ではないので、普通にGPIOとして初期化。
    // リセット信号が指定されている場合のみ、リセット信号を使用する。
    if (_reset != 0) {
        gpio_init(_reset);
        gpio_set_dir(_reset, GPIO_OUT);
        gpio_put(_reset, 0);

  	}
	// デバッグ用に、GP15を出力に設定する。ロジアナのトリガに使用する
	// デバッグ信号が指定されている場合のみ、デバッグ信号を使用する。
  	if (_debug != 0) {
    	gpio_init(_debug);
    	gpio_set_dir(_debug,GPIO_OUT);
    	gpio_put(_debug,0);
  	}
}
/// @brief ポートを指定して、GPIOの初期化を行う
/// @param spiPortNo	使用するspiのポート番号。0または1
/// @param a_RXDC		Data/Commandのポート番号
/// @param a_CS			CSnのポート番号
/// @param a_SCK		SCKのポート番号
/// @param a_TX			TXのポート番号
/// @param a_reset		リセット信号のポート番号
/// @param a_debug		デバッグ用のポート番号
void HW::init(uint8_t spiPortNo, uint8_t a_RXDC, uint8_t a_CS, uint8_t a_SCK, uint8_t a_TX, uint8_t a_reset, uint8_t a_debug)
{
	portSPI = (spiPortNo == 0) ? spi0 : spi1;
	initPort(a_RXDC, a_CS, a_SCK, a_TX, a_reset, a_debug);
	init();
}
/// @brief デバッグピンをストローブする
volatile void HW::debugStrobe()
{
    gpio_put(_debug, 0);
    gpio_put(_debug, 1);
    sleep_us(2);
    gpio_put(_debug, 0);
}
/// @brief デバッグピンをHighにする
volatile void HW::debugOn()
{
	gpio_put(_debug,1);
}
/// @brief デバッグピンをLowにする
volatile void HW::debugOff()
{
	gpio_put(_debug, 0);
}
/// @brief spiの出力ルーチンに入ったことを示す信号をデバッグピンに出力する。デバッグがストローブモードのときにはストローブ、そうでないときにはHighにする。
volatile void HW::debugIn()
{
    if (options.isDebugModeOnOff) {
		debugOn();
    } else {
		debugStrobe();
    }
}
/// @brief spiの出力ルーチンから出たことを示す信号をデバッグピンに出力する。デバッグがストローブモードのときにはストローブ、そうでないときにはLowにする。  
volatile void HW::debugOut()
{
    if (options.isDebugModeOnOff) {
        debugOff();
    } else {
        debugStrobe();
    }
}
/// @brief  リセット信号をLowにする
volatile void HW::Reset()
{
	if (_reset != 0) {
		RstHigh();
    	__delay_ms(10);
    	RSTLow();
    	__delay_ms(10);
    	RstHigh();
    	__delay_ms(10);
	}
}

/// @brief コマンドを送信する。DC:L→CS:L→コマンド送信→CS:H
/// @param cmd_ 送信するコマンド
void HW::writeCommand(uint8_t cmd_)
{
	debugIn();
	DCLow();
	CSLow();
	spiWrite(cmd_);
	CSHigh();
	debugOut();
}

/// @brief	データを送信する。DC:H→CS:L→データ送信→CS:H
/// @param data_ 送信するデータ
void HW::writeData(uint8_t data_)
{
	debugIn();
	DCHigh();
	CSLow();
	spiWrite(data_);
	CSHigh();
	debugOut();
}
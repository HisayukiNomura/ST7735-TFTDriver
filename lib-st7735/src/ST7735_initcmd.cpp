#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC optimize("O0")
#include "../include/ST7735_initcmd.h"
#include <stdint.h>
#include <stdlib.h>
#include "../include/ST7735_TFT.h"
#include "../include/ST7735_commands.h"
#include "../include/hw.h"

/**
 * @file ST7735_initcmd.cpp
 * @brief ST7735の初期化処理を行う、ST7735Initクラスを定義する。
 *
 */

/// @brief デフォルトコンストラクタ。128x160ドットの液晶として初期化される。<br/>
/// pSPIHWが設定されていないので、使用する前には、SetSPIHW(HW* a_pSpiHW)メソッドを使用して、
/// ハードウェアへのアクセスを行うクラスインスタンスへのポインタを設定する。
ST7735Init::ST7735Init() 
{
	colstart = 0;
	rowstart = 0;
	tft_type = 0;
	rotation = 0;
	xstart = 0;
	ystart = 0;
	width = 128;
	height = 160;
	BaseWidth = width;
	BaseHeight = height;
}
/// @brief コンストラクタ。128x160ドットの液晶として初期化される。<br/>
/// @param a_pSpiHW ハードウェアにアクセスするための情報を持っているクラスインスタンスへのポインタ
ST7735Init::ST7735Init(HW*  a_pSpiHW) : pSpiHW(a_pSpiHW) 
{
	colstart = 0;
	rowstart = 0;
	tft_type = 0;
	rotation = 0;
	xstart = 0;
	ystart = 0;
	width = 128;
	height = 160;
	BaseWidth = width;
	BaseHeight = height;
}
/// @brief ハードウェアにアクセスするための情報を持っているクラスインスタンスを設定する。<br/>
/// デフォルトコンストラクタを使ってnewした場合、必ずこのメソッドの呼び出しが必要。
/// @param a_pSpiHW
void ST7735Init::SetSPIHW(HW* a_pSpiHW)
{
	pSpiHW = a_pSpiHW;
}

/// @brief データのポインタを受け取り、一連のコマンドを送信する
/// @param commandByte コマンド
/// @param dataBytes データへのポインタ
/// @param numDataBytes 送信するデータのバイト数
void ST7735Init::sendCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes)
{
	pSpiHW->debugIn();

	pSpiHW->CSLow();
	pSpiHW->DCLow();
	pSpiHW->spiWrite(commandByte);  // Send the command byte
	pSpiHW->DCHigh();

	for (int i = 0; i < numDataBytes; i++) {
		pSpiHW->spiWrite(*dataBytes);  // Send the data bytes
		dataBytes++;
	}
	pSpiHW->debugOut();
}

/// @brief 指定されたコマンドリストをSPI経由でディスプレイに送信し、初期化処理を行う
/// @param addr 実行するコマンドリストのポインタ
void ST7735Init::displayInit(const uint8_t *addr)
{
	uint8_t numCommands, cmd, numArgs;
	uint16_t ms;
	uint8_t bytesSent = 0;

	pSpiHW->debugIn();

	numCommands = *addr++;  // 実行するコマンドの数を最初に読む
	for (uint8_t i = 0; i < numCommands; i++) {
		cmd = *addr++;
		numArgs = *addr++;
		ms = numArgs & ST7735Cmd.DELAY;
		numArgs &= ~ST7735Cmd.DELAY;
		sendCommand(cmd, addr, numArgs);
		addr += numArgs;
		if (ms) {
			ms = *addr++;
			if (ms == ST7735Sleep.LONG) {
				ST7735Sleep.delay_max();
			} else {
#ifndef DBG_SHORT_WAIT
				sleep_ms(ms);
#else
				sleep_us(ms);
#endif
			}
		}
	}
	pSpiHW->debugOut();
}

/// @brief 指定されたコマンドリストをSPI経由でディスプレイに送信し、初期化処理を行う。<br/>
/// displayInitをそのまま呼びだす。なぜこのメソッドがあるのかは謎・・・
/// @param cmdList コマンドリスト
void ST7735Init::commonInit(const uint8_t *cmdList)
{
	if (cmdList) {
		displayInit(cmdList);
	}
}

/// @brief ST7735Bの初期化処理を行う
void ST7735Init::initB(void)
{
	commonInit(Bcmd);
	setRotation(ST7735_ROTATION::NORMAL);
}

/// @brief ST7735を使用したディスプレイの初期化処理を行う。 <br/>
/// 引数は、対象デバイスのタイプで、ST7735Typeで定義されている。まず、共通のRcmd1を呼びだし、その後にそれぞれにあったものを呼びだす。
/// @param options adafruit で購入したタブの色らしい。
void ST7735Init::initR(uint8_t options)
{
	commonInit(Rcmd1);
	if (options == ST7735Type.GREENTAB) {
		displayInit(Rcmd2green);
		colstart = 2;
		rowstart = 1;
	} else if ((options == ST7735Type.GREENTAB144) || (options == ST7735Type.HALLOWING)) {
		height = ST7735Size.HEIGHT_128;
		width = ST7735Size.WIDTH_128;
		BaseWidth = width;
		BaseHeight = height;

		displayInit(Rcmd2green144);
		colstart = 2;
		rowstart = 3;  // For default rotation 0
	} else if (options == ST7735Type.MINI160x80) {
		height = ST7735Size.WIDTH_80;
		width = ST7735Size.HEIGHT_160;
		BaseWidth = width;
		BaseHeight = height;

		displayInit(Rcmd2green160x80);
		colstart = 24;
		rowstart = 0;
	} else if (options == ST7735Type.MINI160x80_PLUGIN) {
		height = ST7735Size.WIDTH_80;
		width = ST7735Size.HEIGHT_160;
		BaseWidth = width;
		BaseHeight = height;

		displayInit(Rcmd2green160x80plugin);
		colstart = 26;
		rowstart = 1;
	} else {
		// colstart, rowstart left at default '0' values
		displayInit(Rcmd2red);
	}
	displayInit(Rcmd3);
	// Black tab, change MADCTL color filter
	if ((options == ST7735Type.BLACKTAB) || (options == ST7735Type.MINI160x80)) {
		uint8_t data = 0xC0;
		sendCommand(ST7735Cmd.MADCTL, &data, 1);
	}

	if (options == ST7735Type.HALLOWING) {
		// Hallowing is simply a 1.44" green tab upside-down:
		tabcolor = ST7735Type.GREENTAB144;
		setRotation(ST7735_ROTATION::DEGREE_270);
	} else {
		tabcolor = options;
		setRotation(ST7735_ROTATION::NORMAL);
	}
}

#if defined TFT_ENABLE_ROTATE
/// @brief 画面の横と縦を指定する。変更しても、現在表示中の画面は変更されない。この命令以後表示されるもののみが対象になる。
/// @param m 画面の方向。0...標準、1...90度左回転、2...180度左回転、3...270度左回転
void ST7735Init::setRotation(ST7735_ROTATION m)
{
	// m can be 0-3
	uint8_t madctl = 0;

	rotation = m % 4;

	switch (rotation) {
		case ST7735_ROTATION::NORMAL: {
			madctl = ST7735Cmd.MADCTL_MX | ST7735Cmd.MADCTL_MY | ST7735Cmd.MADCTL_RGB;
			xstart = colstart;
			ystart = rowstart;
			width = BaseWidth;
			height = BaseHeight;
			break;
		}
		case ST7735_ROTATION::DEGREE_90: {
			madctl = ST7735Cmd.MADCTL_MY | ST7735Cmd.MADCTL_MV | ST7735Cmd.MADCTL_RGB;
			ystart = colstart;
			xstart = rowstart;
			width = BaseHeight;
			height = BaseWidth;;

			break;
		}
		case ST7735_ROTATION::DEGREE_180: {
			madctl = ST7735Cmd.MADCTL_RGB;
			xstart = colstart;
			ystart = rowstart;
			width = BaseWidth;
			height = BaseHeight;
			break;
		}
		case ST7735_ROTATION::DEGREE_270: {
			madctl = ST7735Cmd.MADCTL_MX | ST7735Cmd.MADCTL_MV | ST7735Cmd.MADCTL_RGB;
			ystart = colstart;
			xstart = rowstart;
			width = BaseHeight;
			height = BaseWidth;
			break;
		}
	}
	pSpiHW->writeCommand(ST7735Cmd.MADCTL);
	pSpiHW->writeData(madctl);
}
#endif
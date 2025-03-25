#pragma once
#ifndef ST7735_COMMANDS_H
#define ST7735_COMMANDS_H

#include <stdbool.h>
#include <string.h>
#include <cstdint>


/// @brief 捜査対象のTFTの種類
class {
   public:
	const uint8_t GREENTAB = 0x00;
	const uint8_t REDTAB = 0x01;
	const uint8_t BLACKTAB = 0x02;
	const uint8_t GREENTAB18 = 0x00;
	const uint8_t REDTAB18 = 0x01;
	const uint8_t BLACKTAB18 = 0x02;
	const uint8_t GREENTAB144 = 0x00;
	const uint8_t MINI160x80 = 0x04;
	const uint8_t HALLOWING = 0x05;
	const uint8_t MINI160x80_PLUGIN = 0x06;
} ST7735Type;

/// @brief 捜査対象のTFTのサイズ
class {
   public:
	const uint8_t WIDTH_128 = 128;
	const uint8_t HEIGHT_128 = 128;
	const uint8_t WIDTH_80 = 80;
	const uint8_t HEIGHT_160 = 160;
} ST7735Size;

#define _swap(a, b) \
	{               \
		int16_t t;  \
		t = a;      \
		a = b;      \
		b = t;      \
	}

/// @brief ST7735に対するコマンドの定義
class {
   public:
	const uint8_t DELAY = 0x80;  // special signifier for command lists
	const uint8_t NOP = 0x00;
	const uint8_t SWRESET = 0x01;
	const uint8_t RDDID = 0x04;
	const uint8_t RDDST = 0x09;
	const uint8_t SLPIN = 0x10;
	const uint8_t SLPOUT = 0x11;
	const uint8_t PTLON = 0x12;
	const uint8_t NORON = 0x13;
	const uint8_t INVOFF = 0x20;
	const uint8_t INVON = 0x21;
	const uint8_t DISPOFF = 0x28;
	const uint8_t DISPON = 0x29;
	const uint8_t CASET = 0x2A;
	const uint8_t RASET = 0x2B;
	const uint8_t RAMWR = 0x2C;
	const uint8_t RAMRD = 0x2E;
	const uint8_t PTLAR = 0x30;
	const uint8_t VSCRDEF = 0x33;
	const uint8_t COLMOD = 0x3A;
	const uint8_t MADCTL = 0x36;
	const uint8_t MADCTL_MY = 0x80;
	const uint8_t MADCTL_MX = 0x40;
	const uint8_t MADCTL_MV = 0x20;
	const uint8_t MADCTL_ML = 0x10;
	const uint8_t MADCTL_RGB = 0x00;
	const uint8_t VSCRSADD = 0x37;
	const uint8_t FRMCTR1 = 0xB1;
	const uint8_t FRMCTR2 = 0xB2;
	const uint8_t FRMCTR3 = 0xB3;
	const uint8_t INVCTR = 0xB4;
	const uint8_t DISSET5 = 0xB6;
	const uint8_t PWCTR1 = 0xC0;
	const uint8_t PWCTR2 = 0xC1;
	const uint8_t PWCTR3 = 0xC2;
	const uint8_t PWCTR4 = 0xC3;
	const uint8_t PWCTR5 = 0xC4;
	const uint8_t VMCTR1 = 0xC5;
	const uint8_t RDID1 = 0xDA;
	const uint8_t RDID2 = 0xDB;
	const uint8_t RDID3 = 0xDC;
	const uint8_t RDID4 = 0xDD;
	const uint8_t PWCTR6 = 0xFC;
	const uint8_t GMCTRP1 = 0xE0;
	const uint8_t GMCTRN1 = 0xE1;
} ST7735Cmd;



#endif
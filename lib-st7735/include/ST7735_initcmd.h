#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "ST7735_TFT.h"
#include "pico/stdlib.h"
#include "ST7735_struct.h"
#include "ST7735_commands.h"
#include "hw.h"
#ifndef ST7735_INIT_H
#define ST7735_INIT_H

// スクリーンの初期化
// write_command(ST7735_COLMOD)と、write_dataを羅列せずに、データにしておく。

/// @brief ST7735 LCDの初期化処理を行うクラス。
/// ハードウェアごとに異なるのと、長い初期化コマンドを実行させるために、描画処理とは別のクラスを作成してある。<br/>
/// このクラスは静的ではないので、インスタンス化してから使用する。無いとは思うが、２つのLCDを接続して制御するときは、２つのインスタンスをそれぞれ
/// 作成、初期化して初期化処理を行う。
class ST7735Init {
   private:
	/// @brief ハードウェア操作のためのクラスへのポインタ
	HW* pSpiHW;

   public:
	/// @brief 液晶の左上のX座標。
	/// @details この座標を実際の原点する。ハードウェアにより、この値は調整される。
	uint8_t colstart;
	/// @brief 液晶の左上のY座標。この座標を実際の原点する。ハードウェアにより、この値は調整される。
	/// @details この座標を実際の原点する。ハードウェアにより、この値は調整される。
	uint8_t rowstart;
	/// @brief 接続されているTFTの種別
	/// @details この値により、送信されるコマンドの一部が変わる
	uint8_t tft_type;
	/// @brief 現在の表示画面の向き。0...標準、1...90° 2...180° 3...270°
	uint8_t rotation;
	/// @brief 描画の左上座標
	uint8_t xstart;
	/// @brief 描画の右上座標
	uint8_t ystart;

	// we keept this public
	/// @brief 現在接続されている液晶の横ドット数
	uint8_t width = 128;
	/// @brief 現在接続されている液用の縦ドット数
	uint8_t height = 160;

	/// @brief プログラム起動時に設定されていた液晶のドット数を保存しておく。
	/// @details このプログラムでは、widthとheightが画面の回転により入れ替わる。
	/// 元に戻すために、この変数に初期値を保存しておく。
	uint8_t BaseWidth = 128;

	/// @brief プログラム起動時に設定されていた液晶のドット数を保存しておく。
	/// @details このプログラムでは、widthとheightが画面の回転により入れ替わる。
	/// 元に戻すために、この変数に初期値を保存しておく。
	uint8_t BaseHeight = 160;

	/// @brief 接続されているTFTの製品種類を表すための変数
	uint8_t tabcolor;

  public:
  /// @brief デフォルトコンストラクタ。128x160ドットの液晶として初期化される。
  /// @details pSPIHWが設定されていないので、使用する前には、SetSPIHW(HW* a_pSpiHW)メソッドを使用してハードウェアへのアクセスを行うクラスインスタンスへのポインタを設定する。
  ST7735Init();
  /// @brief コンストラクタ。128x160ドットの液晶として初期化される。
  /// @details 引数にはハードウェアにアクセスするための情報を持っているクラスインスタンスへのポインタを渡す。
  /// @param a_pSpiHW ハードウェアにアクセスするための情報を持っているクラスインスタンスへのポインタ
	ST7735Init(HW* a_pSpiHW);

  /// @brief ハードウェアにアクセスするための情報を持っているクラスインスタンスを設定する。<br/>
  /// @details デフォルトコンストラクタを使ってnewした場合、必ずこのメソッドの呼び出しが必要。
  /// @param a_pSpiHW ハードウェアにアクセスするための情報を持っているクラスインスタンスへのポインタ
	void SetSPIHW(HW* a_pSpiHW);	

  /// @brief データのポインタを受け取り、一連のコマンドを送信する
  /// @param commandByte コマンド
  /// @param dataBytes データへのポインタ
  /// @param numDataBytes 送信するデータのバイト数
	void sendCommand(uint8_t commandByte, const uint8_t* dataBytes, uint8_t numDataBytes);

  /// @brief 指定されたコマンドリストをSPI経由でディスプレイに送信し、初期化処理を行う
  /// @param addr 実行するコマンドリストのポインタ
	void displayInit(const uint8_t* addr);

  /// @brief 指定されたコマンドリストをSPI経由でディスプレイに送信し、初期化処理を行う。<br/>
  /// displayInitをそのまま呼びだす。なぜこのメソッドがあるのかは謎・・・
  /// @param cmdList コマンドリスト
  void commonInit(const uint8_t* cmdList);
	
  /// @brief ST7735Bの初期化処理を行う
  void initB(void);
  
  /// @brief ST7735を使用したディスプレイの初期化処理を行う。
  /// @details 引数は、対象デバイスのタイプで、ST7735Typeで定義されている。まず、共通のRcmd1を呼びだし、その後にそれぞれにあったものを呼びだす。
  /// @param options adafruit で購入したタブの色らしい。  
	void initR(uint8_t options);
  /// @brief 画面の横と縦を指定する。変更しても、現在表示中の画面は変更されない。この命令以後表示されるもののみが対象になる。
  /// @param m 画面の方向。0...標準、1...90度左回転、2...180度左回転、3...270度左回転  
	void setRotation(enum ST7735_ROTATION m);
};

/// @brief ST7735Bの初期化コマンド
/// @details このコマンドは、ST7735Bの初期化処理を行うためのもの。ST7735Init::initB(void)で、このデータを送信する。
static const uint8_t Bcmd[] = {                        		// Init commands for 7735B screens
    18,                             		// 18 commands in list:
	ST7735Cmd.SWRESET	,ST7735Cmd.DELAY, 	//  1: Software reset, no args, w/delay
    ST7735Sleep.MIDDLE	,                   //     50 ms delay
    ST7735Cmd.SLPOUT	,ST7735Cmd.DELAY, 	//  2: Out of sleep mode, no args, w/delay
      ST7735Sleep.LONG,                     //     255 = max (500 ms) delay
    ST7735Cmd.COLMOD,  (uint8_t)(ST7735Cmd.DELAY+1), 	//  3: Set color mode, 1 arg + delay:
      0x05,                         		//     16-bit color
      ST7735Sleep.SHORT,                    //     10 ms delay
    ST7735Cmd.FRMCTR1, (uint8_t)(3+ST7735Cmd.DELAY), 	//  4: Frame rate control, 3 args + delay:
      0x00,                         		//     fastest refresh
      0x06,                         		//     6 lines front porch
      0x03,                         		//     3 lines back porch
      ST7735Sleep.SHORT,                    //     10 ms delay
    ST7735Cmd.MADCTL,  1,              		//  5: Mem access ctl (directions), 1 arg:
      0x08,                         		//     Row/col addr, bottom-top refresh
    ST7735Cmd.DISSET5, 2,              		//  6: Display settings #5, 2 args:
      0x15,                         		//     1 clk cycle nonoverlap, 2 cycle gate
                                    		//     rise, 3 cycle osc equalize
      0x02,                         		//     Fix on VTL
    ST7735Cmd.INVCTR,  1,              		//  7: Display inversion control, 1 arg:
      0x0,                          		//     Line inversion
    ST7735Cmd.PWCTR1,  (uint8_t)(2+ST7735Cmd.DELAY), 	//  8: Power control, 2 args + delay:
      0x02,                         		//     GVDD = 4.7V
      0x70,                         		//     1.0uA
      ST7735Sleep.SHORT,                    //     10 ms delay
    ST7735Cmd.PWCTR2,  1,              		//  9: Power control, 1 arg, no delay:
      0x05,                         		//     VGH = 14.7V, VGL = -7.35V
    ST7735Cmd.PWCTR3,  2,              		// 10: Power control, 2 args, no delay:
      0x01,                         		//     Opamp current small
      0x02,                         		//     Boost frequency
    ST7735Cmd.VMCTR1,  (uint8_t)(2+ST7735Cmd.DELAY), // 11: Power control, 2 args + delay:
      0x3C,                         		//     VCOMH = 4V
      0x38,                         		//     VCOML = -1.1V
      ST7735Sleep.SHORT,                    //     10 ms delay
    ST7735Cmd.PWCTR6,  2,              		// 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735Cmd.GMCTRP1,16,              		// 13: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x09, 0x16, 0x09, 0x20,       		//     (Not entirely necessary, but provides
      0x21, 0x1B, 0x13, 0x19,       		//      accurate colors)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735Cmd.GMCTRN1,(uint8_t)(16+ST7735Cmd.DELAY), // 14: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E,       //     (Not entirely necessary, but provides
      0x22, 0x1D, 0x18, 0x1E,       //      accurate colors)
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      ST7735Sleep.SHORT,                           //     10 ms delay
    ST7735Cmd.CASET,   4,              // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 2
      0x00, 0x81,                   //     XEND = 129
    ST7735Cmd.RASET,   4,              // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 1
      0x00, 0x81,                   //     XEND = 160
    ST7735Cmd.NORON,     ST7735Cmd.DELAY, // 17: Normal display on, no args, w/delay
      ST7735Sleep.SHORT,                           //     10 ms delay
    ST7735Cmd.DISPON,    ST7735Cmd.DELAY, // 18: Main screen turn on, no args, delay
      ST7735Sleep.LONG 
	  };                        //     255 = max (500 ms) delay
/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。
static const uint8_t 
  Rcmd1[] = {                       // 7735R init, part 1 (red or green tab)
    15,                             // 15 commands in list:
    ST7735Cmd.SWRESET,   ST7735Cmd.DELAY, //  1: Software reset, 0 args, w/delay
      ST7735Sleep.MIDLONG,                          //     150 ms delay
    ST7735Cmd.SLPOUT,    ST7735Cmd.DELAY, //  2: Out of sleep mode, 0 args, w/delay
      ST7735Sleep.LONG,                          //     500 ms delay
    ST7735Cmd.FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735Cmd.FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735Cmd.FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
      0x01, 0x2C, 0x2D,             //     Dot inversion mode
      0x01, 0x2C, 0x2D,             //     Line inversion mode
    ST7735Cmd.INVCTR,  1,              //  6: Display inversion ctrl, 1 arg:
      0x07,                         //     No inversion
    ST7735Cmd.PWCTR1,  3,              //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                         //     -4.6V
      0x84,                         //     AUTO mode
    ST7735Cmd.PWCTR2,  1,              //  8: Power control, 1 arg, no delay:
      0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735Cmd.PWCTR3,  2,              //  9: Power control, 2 args, no delay:
      0x0A,                         //     Opamp current small
      0x00,                         //     Boost frequency
    ST7735Cmd.PWCTR4,  2,              // 10: Power control, 2 args, no delay:
      0x8A,                         //     BCLK/2,
      0x2A,                         //     opamp current small & medium low
    ST7735Cmd.PWCTR5,  2,              // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735Cmd.VMCTR1,  1,              // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735Cmd.INVOFF,  0,              // 13: Don't invert display, no args
    ST7735Cmd.MADCTL,  1,              // 14: Mem access ctl (directions), 1 arg:
      0xC8,                         //     row/col addr, bottom-top refresh
    ST7735Cmd.COLMOD,  1,              // 15: set color mode, 1 arg, no delay:
      0x05 };                       //     16-bit color

/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。

static const uint8_t
    Rcmd2green[] = {         // 7735R init, part 2 (green tab only)
        2,                   //  2 commands in list:
        ST7735Cmd.CASET, 4,  //  1: Column addr set, 4 args, no delay:
        0x00, 0x02,          //     XSTART = 0
        0x00, 0x7F + 0x02,   //     XEND = 127
        ST7735Cmd.RASET, 4,  //  2: Row addr set, 4 args, no delay:
        0x00, 0x01,          //     XSTART = 0
        0x00, 0x9F + 0x01};  //     XEND = 159

/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。

static const uint8_t 
  Rcmd2red[] = {                    // 7735R init, part 2 (red tab only)
    2,                              //  2 commands in list:
    ST7735Cmd.CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST7735Cmd.RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F };                 //     XEND = 159
/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。

static const uint8_t 
  Rcmd2green144[] = {               // 7735R init, part 2 (green 1.44 tab)
    2,                              //  2 commands in list:
    ST7735Cmd.CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST7735Cmd.RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F };                 //     XEND = 127
/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。

static const uint8_t
    Rcmd2green160x80[] = {   // 7735R init, part 2 (mini 160x80)
        2,                   //  2 commands in list:
        ST7735Cmd.CASET, 4,  //  1: Column addr set, 4 args, no delay:
        0x00, 0x00,          //     XSTART = 0
        0x00, 0x4F,          //     XEND = 79
        ST7735Cmd.RASET, 4,  //  2: Row addr set, 4 args, no delay:
        0x00, 0x00,          //     XSTART = 0
        0x00, 0x9F};         //     XEND = 159
/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。
static const uint8_t
    Rcmd2green160x80plugin[] = {  // 7735R init, part 2 (mini 160x80 with plugin FPC)
        3,                        //  3 commands in list:
        ST7735Cmd.INVON, 0,       //   1: Display is inverted
        ST7735Cmd.CASET, 4,       //  2: Column addr set, 4 args, no delay:
        0x00, 0x00,               //     XSTART = 0
        0x00, 0x4F,               //     XEND = 79
        ST7735Cmd.RASET, 4,       //  3: Row addr set, 4 args, no delay:
        0x00, 0x00,               //     XSTART = 0
        0x00, 0x9F};  //     XEND = 159
/// @brief ST7735Rの初期化コマンド
/// @details このコマンドは、ST7735Rの初期化処理を行うためのもの。ST7735Init::initR(uint8_t options)で、渡される引数に従い選択された場合にこのデータを送信する。
static const uint8_t
    Rcmd3[] = {                  // 7735R init, part 3 (red or green tab)
        4,                       //  4 commands in list:
        ST7735Cmd.GMCTRP1, 16,   //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
        0x02, 0x1c, 0x07, 0x12,  //     (Not entirely necessary, but provides
        0x37, 0x32, 0x29, 0x2d,  //      accurate colors)
        0x29, 0x25, 0x2B, 0x39,
        0x00, 0x01, 0x03, 0x10,
        ST7735Cmd.GMCTRN1, 16,   //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
        0x03, 0x1d, 0x07, 0x06,  //     (Not entirely necessary, but provides
        0x2E, 0x2C, 0x29, 0x2D,  //      accurate colors)
        0x2E, 0x2E, 0x37, 0x3F,
        0x00, 0x00, 0x02, 0x10,
        ST7735Cmd.NORON, ST7735Cmd.DELAY,   //  3: Normal display on, no args, w/delay
        ST7735Sleep.SHORT,                  //     10 ms delay
        ST7735Cmd.DISPON, ST7735Cmd.DELAY,  //  4: Main screen turn on, no args w/delay
        ST7735Sleep.MIDLONG};  //     100 ms delay



#endif
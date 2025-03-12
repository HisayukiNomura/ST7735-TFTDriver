/**

@mainpage 


@section main 概要

# 概要


メインブランチ
https://github.com/HisayukiNomura/ST7735-TFTDriver/tree/main
を参照するか、
https://github.com/HisayukiNomura/ST7735-TFTDriver/releases/tag/1.0
からソースコードをダウンロードしてください。

<hr/>


# このライブラリについて

ST7735をコントローラにしたTFT液晶の使用例はよくあるが、ほとんどがArdiunoを使用したもので、Raspberry piRaspberry piのSDKを使用した例はほとんどありません。
また、Ardiuno版を含め、日本語（漢字）が表示可能なライブラリは見つかりません。

このライブラリは、[Raspberry PI SDK](https://www.raspberrypi.com/news/get-started-with-raspberry-pi-pico-series-and-vs-code/) を使用してST7735をコントロールする為のプログラムです。
Arduinoにくらべ、Raspberry PI PICOはメモリが潤沢にあるため、日本語表示などに対応しています。

開発と動作確認は、aitendoの1.8インチTFT液晶モジュール [M018C7735S541](https://www.aitendo.com/product/16367) で行いました。
この液晶モジュールは、シルクの印刷が表裏で異なり、加えてRS(RESET?)と D/C(MISO)の印刷も逆になっているように思います。M018C7735S541での配線時には注意してください。

# 動作例
次の動画は、このライブラリのサンプルプログラム（本体に同梱）です。

- ビットマップ表示…有名なヒゲのオジサンが左右に動きます。
- 漢字表示とスクロール…12x12ドットの漢字文字列を表示し、スクロールさせます。
- 英文字表示とスクロール…フォント定義された大きな文字を画面に表示させ、スクロールさせます。上下に固定領域を設定し、その中だけをスクロールさせています。
- 図形の表示　円、矩形、角丸図形とその塗りつぶし、点と線の描画を行います。

https://www.youtube.com/watch?v=v_BOcLKssuE


# ソースコード
ソースコードはgithubの[ST7735-TFTDriverページ](https://github.com/HisayukiNomura/ST7735-TFTDriver)で公開しています。

### フォルダ構成と用途
プロジェクトのフォルダ構成と主な用途は次のようになっています。

 ／　動作を確認するためのプログラムや、VSCodeのプロジェクト関連ファイル
 　｜
 　＋ー SRC　ライブラリ本体のソースコード
 　｜
 　＋ー include　ライブラリが使用するincludeファイル
　　　　｜
　　　　＋ー　font　ライブラリが使用するフォントファイル

ライブラリの本体は、SRCサブフォルダと、includeフォルダ以下に入っています。このまま、これらのフォルダを使用するプロジェクトに組み込んでください。

### フォント情報ファイル

fontサブフォルダは、テキスト表示の際に使用されるフォントのビットマップ情報です。

#### 英文フォント用ファイル
拡張子 .hは、[adafruitのgfxライブラリ](https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts)で使用されているものですが、ディレクトリ構成の調整に伴い、次のように若干変更されています。

- インクルードファイルの変更<br/>オリジナルでは、GFXfont構造体が TextFonts.hで定義されていました。これだと使用されないFont[]がメモリに読み込まれるため、GFXフォント構造体はST7735_struct.hに移動させました。これに伴い、インクルードされるファイルをTextFonts.hからST7735_struct.hに変更してあります。
- PROGMEM　修飾子の削除<br/> Ardiunoで使用する、PROGMEM修飾子は削除しました。PICO sdkでは、const キーワードを使用することでフォントデータのような大きなデータをフラッシュメモリに格納できます。


#### 日本語フォント用ファイル
拡張子 .incは、日本語フォント用のファイルです。複数のファイルがありますが、このうち一つだけがプログラムに読み込まれます。
ファイル名は、 **Font_Kanji<横ドット数><文字セット>.inc** の形になっており、文字セットを KanjiHelper.h 内で定義されている、TFT_KANJI_DOT マクロと、 TFT_KANJI_LEVELマクロで定義します。

現在のところ、TFT_KANJI_DOTに指定できるのは 8, 12,16で、それぞれ 8x8、12x12、16x16ドット（マルチバイト文字、いわゆる全角）のサイズです。１バイト文字は横幅が半分になっています。 8x8は、かなり読むのが難しくなっています。液晶のサイズから、12x12程度が視認性などの点から適しています。

文字セットは、使用されている文字種別で次のようになっています。

| 文字セット|設定値|含まれる文字 |文字数|サイズ(12x12)|
|:-:|:-:|:-:|:-:|:-:|
|All|0|ＪＩＳすべて|約6,800文字|約165Kbytes|
|Level1|1|第二水準を除いた文字|約3,500文字|約83Kbytes|
|Jyoyo|2|常用漢字。ギリシャやキリル、罫線を除く|約2,500文字|約60Kbytes|
|Kyoiku|3|教育漢字。ギリシャやキリル、罫線を除く|約800文字|約19Kbytes|

※JISすべてには、第一水準、第二水準、かな、カナ、英字、記号１、記号２、ギリシャ文字、キリル文字、罫線を含みます。

たとえば、Font_Kanji16Level1.incファイルを使用する場合、KanjiHelper.h 内で次のように定義します。

```
#define TFT_KANJI_DOT 16
#define TFT_KANJI_LEVEL 1
```

フォントファイルは使い捨てに近い自作のアプリケーションで東雲フォント等フリーフォントから構成しました。（希望があれば公開します。）


# 使用方法

## 実装にあわせた変更

実際の使用環境や要望に合わせて、いくつかのマクロやプログラム内の定数定義を変更してください。

### ハードウェアにあわせて変更するもの

hw.h内
ここでは、SPIで使用するポート定義のデフォルト値が変更できます。あくまでデフォルトなので、コンストラクタから値を変更できるため必須ではありません。

- PIN_TFT_RX<br/> SPIで使用するデフォルトのピン番号。RXの信号線（GP16）
- PIN_TFT_DC<br/>ST7735のData/Commandで使用するピン番号。PIN_TFT_RXと同じ。ST7735では、RX(MISO）は使用せず、Data/Commandとして使用するため、PIN_TFT_RXと同じになっています。この信号線はSDKのSPIライブラリは使用できないため、自力で操作が必要です。
- PIN_TFT_CS<br/>PIのCSnのデフォルトピン。GP17。
- PIN_TFT_TX<br/>SPIのTX(MOSI)信号ので使用するデフォルトのピン番号。GP19。
- PIN_TFT_SCK<br/>SPIのクロック(SCK)で使用するデフォルトのピン番号。GP18
- PIN_TFT_RST<br/>リセットピンで使用するポートの番号。無くても動く（＋５V電源に直結）。０を指定するとリセットピンは使用しない。GP28。このポートをGNDにすると、液晶はリセットされる。
- PIN_TFT_DEBUG<br/>デバッグピンの定義（SPIの信号出力時にストローブしたり、Highにすることができる）。ゼロを指定するとデバッグピンは使用しない。GP15。

### アプリケーションのニーズに合わせて変更するもの

マクロを定義するかどうかで、機能を組み込むかが決定されます。使用しない機能をオフにしておくとメモリが節約できます。とくに、日本語関連はフラッシュメモリへの圧迫が大きいので不要な場合はシンボルを削除してください。

ST7735＿TFT.ｈ内
- TFT_ENABLE_TEXT<br/>文字表示を使用する場合は定義します。これを定義しないと、日本語を含みすべての文字出力機能は組み込まれません。
- TFT_ENABLE_FONTS<br/>フォント機能を有効にするかのフラグ。
このシンボルを定義すると、フォント拡張機能が有効になります。未定義の場合、デフォルトのフォントとして、TextFonts.hで定義されている5x7の字体が自動的に使用されます。有効の場合、フォントはGFXfont構造体へのポインタを、setFontメソッドで指定することができます。事前に用意したフォントデータを指定してください。このプロジェクトでは、[Adafruit GFX Graphics Library](https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts)からダウンロードしたフォントの中から、FreeMono9pt7b.hなど、いくつかのフォントを含めてあります。必要なものをincludeして使用してください。
- TFT_ENABLE_BITMAP<br/>ビットマップ描画機能を有効にするかのフラグ
このシンボルを定義すると、ビットマップ描画の関数を使用できるようになります。RGBベタ（565)の配列を用意し、それを任意の場所に表示できます。このプログラムでは、メインプログラム（デモンストレーション用）内で３枚の画像を用意して切り替えて表示しています。RGBベタの作成は任意のツールを使用してください。[これ](https://github.com/HisayukiNomura/BmpConverter)を使うこともできます。<br/>

- TFT_ENABLE_KANJI<br/>漢字フォント機能を有効にするかのフラグ
このシンボルを定義すると、漢字機能が有効になり、drawKanjiメソッドが使用できるようになります。このシンボルが有効になると、漢字フォントとしてKanjiHelper.h内で字体のデータがincludeされます。使用されるフォントサイズや、文字セットにより大きさが異なりますが、80kBytes～250kBytes程度のメモリをフラッシュ側（プログラムメモリ）で消費します。<br/>

- TFT_FORCE_HANKANA<br/>半角カナ文字を１バイト文字として扱うかのフラグ
通常 Unicodeのエディタ（VS Codeなど)では、半角カナ文字をefbda1～efbdffまでの3バイト文字セットとして格納し、コンパイル時もそれが使用されます。多バイト文字での半角表現はJISコードには含まれていないため、UTF-8からJISへの変換で範囲外となり表示できません。<br/>このフラグを有効にすると、UTF-8の半角カナ文字（UTF:efbda1～など）を、１バイト文字コード(a1～ff)に変換します。その結果UTF-8表現エディタで作成・コンパイルしたプログラムで半角カナを正しく表示できるようになります。

- TFT_ENABLE_SHAPES<br/>図形描画機能を有効にするかのフラグ
直線、矩形、角丸矩形、円、三角形の表示関数が使用できるようになります。

- TFT_ENABLE_ROTATE<br/>画面の回転機能を有効にするかのフラグ
このシンボルを有効にすると、画面を90/180/270度回転する機能が使用できるようになります。

- TFT_ENABLE_SCROLL<br/>画面のスクロール機能を有効にするかのフラグ
このシンボルを有効にすると、画面のスクロール関連関数が使用できるようになります。

- TFT_ENABLE_ALL<br/>図形表示をすべて有効にするかのフラグ
このシンボルを有効にするとすべての図形表示が有効になります。漢字表示 TFT_ENABLE_KANJI と、フォント機能　TFT_ENABLE_FONTSは有効になりません。個別に有効化が必要です。

KanjiHelkper.h内
- TFT_KANJI_DOT<br/>使用する漢字データの大きさを指定する。
16...全角 16x16 半角8x16
12...全角 12x12 半角6x12
 8....全角  8x 8 半角4x8 (かなり視認性は低い）
- TFT_KANJI_LEVEL<br/>使用する漢字データの種類を指定する。
0...JISに定義されているすべてのコード（約6800文字、289KBytes/16dot) 
1...JIS第一水準+かな+カナ+記号1,2+ギリシャ+キリル+罫線（約3400文字、83KBytes/16dot) 
2...常用漢字+かな+カナ+記号1,2（約2500文字、79KBytes/16dot) 
3...教育漢字+かな+カナ+記号1,2（約840文字、 25KBytes/16dot) 


## 使用するまでの手順

### ハードウェア

#### 結線例
aitendoの1.8インチTFT液晶モジュール [M018C7735S541](https://www.aitendo.com/product/16367) での結線例は次のようになります。シルクが間違えているような気がするので注意が必要です。

<img width=60% alt="RaspTFT.png" src="https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/2096509/8918b453-7dc3-4e5e-9b7b-5a1461ea670a.png">

ほかのデバイスで使用する場合、CSn(GP17)、SCK(GP18)、TX/MOSI(GP19)、RX/MISO(GP16)を接続してください。TXはマイコン→デバイスへの信号、RXはデバイス→マイコンへの信号用に用意されています。

デバイス側の表記によっては、RXとTXの表記は逆になっている（デバイスから見て受信するピンがRX)場合があります。MOSI、MISOと表記されていることもあります。MISOは、Master(マイコン）In　Slave（液晶) Out なので、マイコン側にとってはRXに相当します。（多分）　この場合、液晶のMISOはマイコン側のRX(GP16)、液晶のMOSIはマイコン側のTX(GP19）に接続することになります。

### ソフトウェア

## Visual Studio CodeへのSDK導入とビルド
RaspBerry pi の公式ページ、[Get started with Raspberry Pi Pico-series and VS Code](https://www.raspberrypi.com/news/get-started-with-raspberry-pi-pico-series-and-vs-code/)などを参照し、VSCodeにSDKを導入します。日本語でも[これ](https://zenn.dev/usagi1975/articles/2024-08-29-000_pico_development) や、[これ](https://tetsufuku-blog.com/pico-vscode/)などで情報を見つけることができます。

## デモンストレーションプログラムの解説
プログラムは、SRC、includeのサブフォルダにあるライブラリ本体と、ルートディレクトリにあるデモ用のプログラムに別れています。
デモ用のプログラム spiDisplay.cpp は、このライブラリが持っているほとんどの機能を実行するようになっています。

### main()
最初に実行され、GPIOやTFTライブラリの初期化処理を実行した後に、個々の機能を呼びだします。


#### 初期化処理

最初に実行される初期化処理は次のようになっています。基本的に、このライブラリを使う場合にはこれらの提携処理が最初に行われることになります。

```
1. HW spiHW(0, 16, 17, 18, 19, 28, 15);
　　：
　　：
2. iRet = spi_init(spiHW.portSPI, 10*1000*1000);	

3. gpio_set_function(spiHW._dc, GPIO_FUNC_SPI);
    gpio_set_function(spiHW._cs,  GPIO_FUNC_SPI);
    gpio_set_function(spiHW._sck,   GPIO_FUNC_SPI);
    gpio_set_function(spiHW._tx, GPIO_FUNC_SPI);
4. gpio_put(spiHW._cs,1);

5. spiHW.init();      // ライブラリ初期化
6.	ST7735	st7735(spiHW);
7.	pST7735 = &st7735;
	
8.	st7735.doInit();
　　：
　　：
```
1. SPIのハードウェア関連の処理を行うクラスのインスタンスを作成します。SPIハードウェアの番号（
SDKの関数で、SPI機能の初期化を行います。１つ目の引数はSPIハードウェアの番号です。Raspberry PI PICOは、SPIのハードウェアを２つ持っており、そのどちらを使用するか０か１で選びます。それ以降の引数は、使用するポート番号です。デフォルトで良い場合はデフォルトコンストラクタ（引数なし）で初期化することもできます。

2. SPIの初期化を行います。使用するのはSPIハードウェアの番号と、ボーレートです。10MHz程度は使用できるようです。もう少し上まで行くかもしれません。

3. それぞれのピン番号に、SPI機能を割り当てます。

4. csを初期状態にします。

5. ST7735固有のハードウェアの初期化を行います。RXではなくData/Commandとして扱うため、RXポートをSPIかGPIOに戻す、リセットの実行等を行います。

6. グラフィックライブラリの作成を行います。ここで、ハードウェアのオブジェクトを引き渡します。グラフィックライブラリ側では、この情報を使ってハードウェアと通信します。

7. とくに意味はありません。昔、C言語だったころ、参照を扱えなかったのでポインタにしていた名残です。

8. グラフィックライブラリの初期化を行います。TFTに対して初期化コマンドを送るなどが行われます。

#### 各機能の実行

初期化処理が終わると、各機能を順番に呼びだしていきます。

それぞれの機能は、有効/無効がマクロ定義で決まっています。そのため、デモの機能を呼びだす部分は ``` #if defined ～ #endif``` で囲まれています。


```
	while (1) {
#if defined(TFT_ENABLE_TEXT)
1.		st7735.setTextWrap(true);
#endif
		st7735.SetRotation(ST7735_ROTATION::NORMAL);
#ifdef TFT_ENABLE_BITMAP
2.		demo_bmp(st7735);
#endif
// テキスト表示のデモ
#if defined(TFT_ENABLE_KANJI) && defined(TFT_ENABLE_TEXT)
3.		demo_Kanji(st7735);
#endif


#if defined(TFT_ENABLE_FONTS) && defined(TFT_ENABLE_TEXT)
4.		demo_fonts(st7735);
#elif defined(TFT_ENABLE_TEXT) 
5.		demo_text(st7735);
#endif

		// 基本描画機能のデモ
6.		demo_basic(st7735);

#ifdef TFT_ENABLE_SHAPES
		// 図形描画のテスト
7.		demo_shapes(st7735);
#endif
		// void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
		// void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

		st7735.fillScreen(ST7735Color.BLACK);
		sleep_ms(1000);
	}
```

1. テキストの自動改行を行うかを設定しておきます。
2. ビットマップ表示のデモを行います。<br/>有名なヒゲのおじさんが左右に動きます。ヒゲのおじさんは、右向きのビットマップしか用意していませんが、ST7732のスキャン方向を変更する機能を使用して左右を入れ替え、左向きの画像を表示させて左向きに動かしています。
3. 漢字表示と、スクロール機能のデモを行います。<br/>最初に標準状態で日本語テキストを表示させ、スクロールさせます。その後、画面を横回転させてスクロールさせます。
4. フォント定義テキストのデモを行います。<br/>フォント定義を行い大きな文字の表示とスクロール機能のデモを行います。上下にスクロール禁止領域を定義し、画面の一部分だけをスクロールさせています。
5. 基本テキストのデモを行います<br/>フォント機能を使用しない、基本テキストの場合のテキスト表示とスクロール機能のデモを行います。上下にスクロール禁止領域を定義し、画面の一部分だけをスクロールさせています。
6. 基本描画機能のデモを行います。<br/>基本描画機能は縦横の線、矩形と塗りつぶし矩形、点の描画です。これらの機能は常に使用できます。
7. 図形描画機能のデモを行います。<br/>丸、矩形、角丸矩形とそれらの塗りつぶし、直線の描画を行います。


## 機能のリファレンス

詳しい機能については、<a href="class_s_t7735.html">ST7735</a> を参照してください。

主なメソッドは次の通りです。メソッド名の最後がWHのものは、開始座標（X,ｙ）と、横幅、高さを指定する引数を持ちます。メソッド名にWHの突かないものは、２点の座標を指定します。
引数には、座標を使用することもできますし、ベクトルを表すクラスインスタンスを指定することもできます。


void 	fillRectangle (uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
 	矩形を塗りつぶす
void 	fillScreen (uint16_t color)
 	画面を単色で塗りつぶす。初期設定された液晶サイズの矩形を表示させることで実装。
void 	drawFastHLine (uint8_t x, uint8_t y, uint8_t w, uint16_t color)
 	水平線を高速に描画する。ななめの線で必要なDDA処理が不要なので高速で描画できる。
void 	drawFastVLine (uint8_t x, uint8_t y, uint8_t h, uint16_t color)
 	垂直線を高速に描画する。ななめの線で必要なDDA処理が不要なので高速で描画できる。

void 	drawPixel (uint8_t x, uint8_t y, uint16_t color)
void 	drawPixel (Axis8 axis, uint16_t color)
 	画面に点を描画する

void 	invertDisplay (bool i)
 	画面の表示を反転させる
void 	NormalDisplay ()
 	画面の表示を通常に戻す


void 	SetRotation (ST7735_ROTATION rot)
 	画面の回転を設定する。

void 	drawCircle (int16_t x0, int16_t y0, int16_t r, uint16_t color)
void 	drawCircle (Axis16 axis, int16_t r, uint16_t color)
void 	drawCircle (Axis8 axis, int16_t r, uint16_t color)
 
void 	fillCircle (int16_t x0, int16_t y0, int16_t r, uint16_t color)
void 	fillCircle (Axis16 axis, int16_t r, uint16_t color)
void 	fillCircle (Axis8 axis, int16_t r, uint16_t color)
 	塗りつぶされた円を描画する
 
void 	drawRectWH (uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
void 	drawRectWH (Region8 region, uint16_t color)
void 	drawRectWH (Axis8 start, Size8 size, uint16_t color)
 	矩形を描画する
 
void 	drawRect (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
void 	drawRect (Axis8 start, Axis8 end, uint16_t color)
void 	drawRect (Region8 region, uint16_t color)
 	矩形を描画する
 
void 	fillRectWH (uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
void 	fillRectWH (Region8 region, uint16_t color)
void 	fillRectWH (Axis8 start, Size8 size, uint16_t color)
 	矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
 
void 	fillRect (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
void 	fillRect (Axis8 start, Axis8 end, uint16_t color)
void 	fillRect (Region8 region, uint16_t color)
 	矩形を塗りつぶす。fillRectangleより、こちらの方が高速かもしれない。
 
void 	drawLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
void 	drawLine (Axis16 start, Axis16 end, uint16_t color)
void 	drawLine (Axis8 start, Axis8 end, uint16_t color)
void 	drawLine (Region16 region, uint16_t color)
void 	drawLine (Region8 region, uint16_t color)
 	直線を描画する
 
void 	drawRoundRectWH (uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color)
void 	drawRoundRectWH (Region8 region, uint8_t r, uint16_t color)
void 	drawRoundRectWH (Region16 region, uint8_t r, uint16_t color)
void 	drawRoundRectWH (Axis8 start, Size8 size, uint8_t r, uint16_t color)
 	角丸図形を描画する
 
void 	drawRoundRect (uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t r, uint16_t color)
void 	drawRoundRect (Region8 region, uint8_t r, uint16_t color)
void 	drawRoundRect (Axis8 start, Axis8 end, uint8_t r, uint16_t color)
 	角丸塗りつぶし矩形を描画する
 
void 	fillRoundRectWH (uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint16_t color)
void 	fillRoundRectWH (Region8 region, uint8_t r, uint16_t color)
void 	fillRoundRectWH (Axis8 start, Size8 size, uint8_t r, uint16_t color)
 	角丸の矩形を塗りつぶす
 
 
void 	fillRoundRect (uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t r, uint16_t color)
void 	fillRoundRect (Region8 region, uint8_t r, uint16_t color)
void 	fillRoundRect (Axis8 start, Axis8 end, uint8_t r, uint16_t color)
 	角丸図形の塗りつぶし
 
void 	drawTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
void 	drawTriangle (Axis16 a, Axis16 b, Axis16 c, uint16_t color)
void 	drawTriangle (Axis8 a, Axis8 b, Axis8 c, uint16_t color)
 	三角形の描画
 
void 	fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
void 	fillTriangle (Axis16 a, Axis16 b, Axis16 c, uint16_t color)
void 	fillTriangle (Axis8 a, Axis8 b, Axis8 c, uint16_t color)
 	三角形の塗りつぶし
 
void 	setTextWrap (bool w)
 	テキストの改行を行うかのフラグを設定する
 

void 	drawText (Axis8 axis, const char *_text, uint16_t color, uint16_t bg, uint8_t size)
void 	drawText (Axis16 axis, const char *_text, uint16_t color, uint16_t bg, uint8_t size)
	文字列を表示する

void 	drawKanji (uint8_t &x, uint8_t &y, uint32_t code, uint16_t color, uint16_t bg)
void 	drawTextKanji (uint8_t x, uint8_t y, const char *_text, uint16_t color, uint16_t bg, uint8_t size)
 	漢字文字列を表示する
 
void 	setFont (const GFXfont *f)
	フォントのポインタを指定して、drawTextで使用されるフォントを選択する。（TFT_ENABLE_FONTSがオンの場合のみ）

void 	setScrollDefinition (uint8_t top_fix_height, uint8_t bottom_fix_height, bool _scroll_direction)
 	画面の縦スクロール領域を指定する。
 
void 	VerticalScroll (uint8_t _vsp)
 	画面のスクロールを行う。
 
void 	bmpUnuseTransColor (void)
 	透過色を使用しない。
 
void 	bmpUseTrasColor (uint16_t c)
 	透過色を 565 の色で指定する。
 
void 	bmpDraw (uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t *p, uint8_t direction)
 	画面にビットマップを表示する。ビットマップは、５６５形式の16bit値の配列へのポインタ。

 
**/
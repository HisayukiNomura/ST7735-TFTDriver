/**  
@mainpage ST7735 TFT用 Rapberry PI SDKライブラリ

@section main 概要

このライブラリは、Raspberry PI PICO 上で、ST7735 TFT液晶をコントロールするためのライブラリです。日本語表示等に対応しています。



Strawberry Linux の[I2C低電圧キャラクタ液晶モジュール(SB1602B)](https://strawberry-linux.com/catalog/items?code=27001)をはじめとして、多くのI2C液晶モジュールは、液晶コントローラにST7032を使用している。
使用方法については多くの情報があるとはいえ、Raspberry pi Picoについては、Arduinoフレームワークや、Pythonのプログラム中心で、C/C++ SDKを使用したものは少ないようだった。
また、多くがコマンドを直接叩くようなサンプルなので、もう少し抽象化した関数を持ったものが欲しいと思い、ライブラリを作成した。PICなどと異なり、Raspberry pi PICOはフラッシュメモリ、RAMとも広大で、使いやすさのために多少のフットプリントを割いてもよいと考えた。
　
<hr/>
## このライブラリについて
ST7735をコントローラにしたTFT液晶の使用例はよくあるが、ほとんどがArdiunoを使用したもので、Raspberry piRaspberry piのSDKを使用した例はほとんどありません。
また、Ardiuno版を含め、日本語（漢字）が表示可能なライブラリは見つかりません。

このライブラリは、[Raspberry PI SDK](https://www.raspberrypi.com/news/get-started-with-raspberry-pi-pico-series-and-vs-code/) を使用してST7735をコントロールする為のプログラムです。
Arduinoにくらべ、Raspberry PI PICOはメモリが潤沢にあるため、日本語表示などに対応しています。

開発と動作確認は、aitendoの1.8インチTFT液晶モジュール [M018C7735S541](https://www.aitendo.com/product/16367) で行いました。
この液晶モジュールは、シルクの印刷が表裏で異なり、加えてRS(RESET?)と D/C(MISO)の印刷も逆になっているように思います。M018C7735S541での配線時には注意してください。


<hr/>
## ソースコード
ソースコードはgithubの[ST7735-TFTDriverページ](https://github.com/HisayukiNomura/ST7735-TFTDriver)で公開しています。



### mandatory ライブラリを使用するのに必須なファイル

### その他のファイル

<hr/>
## 特徴


<hr/>
## ST7032を使用した液晶ディスプレイの操作

<hr/>
## 実装にあわせた変更


### 変更頻度の高いと思われるマクロ 


### 変更頻度は低いが変更可能なマクロ 

<hr/>
## 使用するまでの手順

###　ハードウェア

#### Strawberry Linux SB1602Bでの結線例


#### 秋月 AE-AQM0802+PCA9515での結線例


### ソフトウェア

#### Visual Studioへの環境構築（初回のみ）とプロジェクト作成

#### ライブラリの組み込み


#### メインプログラムからライブラリを使用

#### 応用プログラム


## 主な関数

@section 外部情報

*/ 

  



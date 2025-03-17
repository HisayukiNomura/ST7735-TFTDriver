import bmpdata
import machine,utime
#import cppexample2
import random
import struct
import KNJGfx
import array

BLACK = 0x0000
BLUE = 0x001F
RED = 0xF800
GREEN = 0x07E0
CYAN = 0x07FF
MAGENTA = 0xF81F
YELLOW = 0xFFE0
WHITE = 0xFFFF


DisplayWidth = 0
DisplayHeight = 0

def randXY(xofs,yofs):
    global DisplayWidth
    global DisplayHeight
    x = random.randint(0,(DisplayWidth-1)-xofs*2)+xofs
    y = random.randint(0,(DisplayHeight-1)-yofs*2)+yofs
    return (x,y)

def randXYXY(xofs,yofs):
    global DisplayWidth
    global DisplayHeight
    
    xy0 = randXY(xofs,yofs)
    xy1 = randXY(xofs,yofs)
    
    return (xy0[0],xy0[1],xy1[0],xy1[1])
    
def randXYWH(xofs,yofs):
    global DisplayWidth
    global DisplayHeight    
    
    xyxy=randXYXY(xofs,yofs)
    w = abs(xyxy[0]-xyxy[2])
    h = abs(xyxy[1]-xyxy[3])
    x = xyxy[0] if xyxy[0] < xyxy[2] else xyxy[2]
    y = xyxy[1] if xyxy [1] < xyxy[3] else xyxy[3]
    assert (x + w) < DisplayWidth and (y + h) < DisplayHeight, "Overflow:{x},{y},{w},{h}"
    return (x,y,w,h)


def demo_text() :
    global DisplayWidth
    global DisplayHeight
    print(f"Globals: DisplayHeight={DisplayHeight}")
    print(f"Globals: DisplayWidth={DisplayWidth}")
    
    KNJGfx.fillScreen(BLACK)
    ret = KNJGfx.setTextWrap(True)
    print(f"SetTextWrap:{ret}")
    
    #画面を黒くする。スクロール機能を使用するとき、なぜか現在の画面色が変になってしまう。
    #原因不明。
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight-1),BLACK)

    # 上下部分にスクロールしない領域を確保し、文字をそこに書く
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,9),WHITE)
    ret = KNJGfx.fillRectangle((0,DisplayHeight-9,DisplayWidth,8),WHITE)
    print(f"fillRectangle:{ret}")
    ret = KNJGfx.drawText((0,1,BLACK,WHITE,False),"<< TOP FIXED AREA>>")
    ret = KNJGfx.drawText((0,DisplayHeight-8,BLACK,WHITE,False),"<< BOTTOM FIXED AREA>>")

    #スクロール部分に文字を書く
    ret = KNJGfx.drawText((0,10,WHITE,BLACK,False),"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ")
    ret = KNJGfx.drawText((0,70,BLUE,BLACK,False),"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. ")
    utime.sleep(3)
    
    # スクロール固定領域の設定。
    # 固定領域の設定後、その中に文字を書いてしまうと、スクロール領域解除後にその領域に文字を書くと
    # なぜか２行分書かれてしまう。原因不明（ライブラリ側の問題ではなさそう）。
    # そのため、スクロールする部分の前後だけ、この固定領域を指定し、終わったらすぐ解除する。
    
    ret = KNJGfx.setScrollDefinition(9,9,False)
    KNJGfx.setRotation(0)  # なぜか、これがないとダメ。scoroll regionの設定後に、これをしないと画面の色が変になる。
    #画面を一回転スクロールさせる
    for i in range(0,DisplayHeight):
        KNJGfx.verticalScroll(i)
        utime.sleep(0.01)

    #スクロール領域をリセットする
    ret = KNJGfx.setScrollDefinition(0,0,True)
    KNJGfx.setRotation(0)  # なぜか、これがないとダメ。scoroll regionの設定後に、これをしないと画面の色が変になる。
    
    #画面を黒く塗りつぶす。スクロール機能
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight),BLACK)



def demo_kanji_V() :
    global DisplayWidth
    global DisplayHeight

    KNJGfx.fillScreen(BLACK)
    
    ret = KNJGfx.setTextWrap(True)
    print(f"SetTextWrap:{ret}")
    

    #画面を黒くする。スクロール機能を使用するとき、なぜか現在の画面色が変になってしまう。
    #原因不明。
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight-1),BLACK)

    # 上下部分にスクロールしない領域を確保し、文字をそこに書く
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,14),WHITE)
    ret = KNJGfx.fillRectangle((0,DisplayHeight-13,DisplayWidth,13),WHITE)
    print(f"fillRectangle:{ret}")
    ret = KNJGfx.drawTextKanji((0,1,BLACK,WHITE,False),"<<上部固定エリア>>")
    ret = KNJGfx.drawTextKanji((0,DisplayHeight-13,BLACK,WHITE,False),"<<下部固定エリア>>")

    #スクロール部分に文字を書く
    ret = KNJGfx.drawTextKanji((0,15,WHITE,BLACK,False),"半角｢ﾊﾝｶｸ｣カナは日本におけるﾊﾟｰｿﾅﾙｺﾝﾋﾟｭｰﾀの黎明期から存在し、JISｺｰﾄﾞ等が利用可能になるまではｺﾝﾋﾟｭｰﾀ上で日本語を表現できる唯一の方法だった。")
    utime.sleep(3)



    # スクロール固定領域の設定。
    # 固定領域の設定後、その中に文字を書いてしまうと、スクロール領域解除後にその領域に文字を書くと
    # なぜか２行分書かれてしまう。原因不明（ライブラリ側の問題ではなさそう）。
    # そのため、スクロールする部分の前後だけ、この固定領域を指定し、終わったらすぐ解除する。
    ret = KNJGfx.setScrollDefinition(14,14,True)
    # KNJGfx.setRotation(0)  # なぜか、これがないとダメ。scoroll regionの設定後に、これをしないと画面の色が変になる。
    #画面を一回転スクロールさせる
    for i in range(0,DisplayHeight):
        KNJGfx.verticalScroll(i)
        utime.sleep(0.01)
    #スクロール領域をリセットする
    ret = KNJGfx.setScrollDefinition(0,0,True)
    KNJGfx.setRotation(0)  # なぜか、これがないとダメ。scoroll regionの設定後に、これをしないと画面の色が変になる。
    # 画面のスクロール機能をオフにする
    KNJGfx.normalDisplay()
    
    #画面を黒く塗りつぶす。
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight),BLACK)
 

# 横書きはスクロールのデモで、固定領域を作らない。
# 固定領域は縦書きだろうと横書きだろうと、画面のデフォルト（縦長？）に対して上下に設定される。
# そうなると使い道が結構限定されてしまうと思うのだが・・・
# 文字描画では、固定領域を考慮していないので、左右の固定領域を無視して文字が描画されてしまう。
# きちんとやるためには・・・
# ・漢字の縦書き表示機能を付ける。（左表示領域、などと左側に表示するため）
# ・漢字の描画領域を、スクロール禁止領域を避けるようにソフトウェアで対処する
# 等が必要だが、現在は実装し宇していない。
def demo_kanji_H() :
    global DisplayWidth
    global DisplayHeight

    KNJGfx.fillScreen(BLACK)
    
    ret = KNJGfx.setTextWrap(True)
    print(f"SetTextWrap:{ret}")
    KNJGfx.setRotation(1)
    
    ret = KNJGfx.fillScreen(WHITE)
    #スクロール部分に文字を書く
    ret = KNJGfx.drawTextKanji((0,0,BLACK,WHITE,False),"僕は怠惰の塊だ。澱のように積み重なった悔恨を背負いながら、蒼茫たる空を眺める。葡萄酒を呑み干し懶惰な日々に溺れていく。孤独の籠で咲いた芙蓉の花は既に凋落し灯の影に揺れていた。。孤独の籠で咲いた芙蓉の花は既に凋落し、灯の影に揺れていた。")
    utime.sleep(3)

    #画面を一回転スクロールさせる
    for i in range(0,DisplayHeight):
        KNJGfx.verticalScroll(i)
        utime.sleep(0.01)
    # 画面のスクロール機能をオフにする
    KNJGfx.normalDisplay()

    #画面を黒く塗りつぶす。スクロール機能
    ret = KNJGfx.fillScreen(BLACK)
    KNJGfx.setRotation(0)




def demo_fontText() :
    global DisplayWidth
    global DisplayHeight

    KNJGfx.fillScreen(BLACK)
    
    ret = KNJGfx.setTextWrap(True)
    print(f"SetTextWrap:{ret}")
    
    fontList = KNJGfx.getAvaiableAsciiFonts()
    for fn in fontList :
        print(f"{fn}")

    #画面を黒くする。スクロール機能を使用するとき、なぜか現在の画面色が変になってしまう。
    #原因不明。
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight-1),BLACK)

    # 上下部分にスクロールしない領域を確保し、文字をそこに書く
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,16),WHITE)
    ret = KNJGfx.fillRectangle((0,DisplayHeight-16,DisplayWidth,16),WHITE)
    print(f"fillRectangle:{ret}")
    KNJGfx.setAsciiFont("FreeMono9pt7b")

    ret = KNJGfx.drawText((0,14,BLACK,WHITE,False),"<<FIXED>>")
    ret = KNJGfx.drawText((0,DisplayHeight-1,BLACK,WHITE,False),"<<FIXED>>")

    KNJGfx.setAsciiFont("FreeMonoOblique12pt7b")
    #スクロール部分に文字を書く
    
    ret = KNJGfx.drawText((0,32,BLUE,BLACK,False),"The quick brown fox jumps over the lazy dog")
    utime.sleep(3)



    # スクロール固定領域の設定。
    # 固定領域の設定後、その中に文字を書いてしまうと、スクロール領域解除後にその領域に文字を書くと
    # なぜか２行分書かれてしまう。原因不明（ライブラリ側の問題ではなさそう）。
    # そのため、スクロールする部分の前後だけ、この固定領域を指定し、終わったらすぐ解除する。
    ret = KNJGfx.setScrollDefinition(16,16,True)
    KNJGfx.setRotation(0)  # なぜか、これがないとダメ。scoroll regionの設定後に、これをしないと画面の色が変になる。
    #画面を一回転スクロールさせる
    for i in range(0,DisplayHeight):
        KNJGfx.verticalScroll(i)
        utime.sleep(0.01)
    #スクロール領域をリセットする
    ret = KNJGfx.setScrollDefinition(0,0,True)
    KNJGfx.setRotation(0)  # なぜか、これがないとダメ。scoroll regionの設定後に、これをしないと画面の色が変になる。
    # 画面のスクロール機能をオフにする
    KNJGfx.normalDisplay()
    #画面を黒く塗りつぶす。
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight),BLACK)


def demo_basic():
    global DisplayWidth
    global DisplayHeight
    # 画面を黒で塗りつぶす
    KNJGfx.fillScreen(BLACK)

    # 横線を描画
    for i in range(0, DisplayHeight, 10):
        KNJGfx.drawFastHLine((0, i), DisplayWidth , RED)
    utime.sleep(1)
    # 縦線を描画
    for i in range(0, DisplayWidth, 10):
        KNJGfx.drawFastVLine((i,0),DisplayHeight,GREEN)
    utime.sleep(1)
    
    KNJGfx.fillScreen(BLACK)
    
    # ランダムな打点
    for i in range(0,30000) :
        x0 = random.randint(10,DisplayWidth-10)
        y0 = random.randint(10,DisplayHeight-10)
        color = random.randint(0, 0xFFFF)  # ランダムな16ビットカラーを生成
        KNJGfx.drawPixel((x0, y0), color)
    utime.sleep(1)
    
    
    #画面の反転
    for i in range(0,10):
        KNJGfx.invertDisplay(True)
        utime.sleep(0.1)
        KNJGfx.invertDisplay(False)
        utime.sleep(0.1)
    utime.sleep(1)
    KNJGfx.normalDisplay()
    
    utime.sleep(3)
    
    
def demo_shapes():
    global DisplayWidth
    global DisplayHeight
    # 画面を黒で塗りつぶす
    KNJGfx.fillScreen(BLACK)

    # 直線の描画
    
    for i in range(0,500) :
        KNJGfx.drawLine(randXYXY(1,1),random.randint(0,0xFFFF))
    utime.sleep(1)
    KNJGfx.fillScreen(BLACK)
    
    # ランダムな色で矩形を書く
    for i in range(0, DisplayWidth, 10):
        for j in range(0, DisplayHeight, 10):
            color = random.randint(0, 0xFFFF)  # ランダムな16ビットカラーを生成
            KNJGfx.drawRectWH((i + 2, j + 2, 6, 6), color)
    utime.sleep(1)
    for i in range(0, DisplayWidth, 10):
        for j in range(0, DisplayHeight, 10):
            color = random.randint(0, 0xFFFF)  # ランダムな16ビットカラーを生成
            KNJGfx.drawRect((i + 3, j + 3, i+3+5, j+3+5), color)
    utime.sleep(1)
    KNJGfx.fillScreen(BLACK)
    
    # ランダムな色で矩形を塗りつぶす
    for i in range(0, DisplayWidth, 10):
        for j in range(0, DisplayHeight, 10):
            color = random.randint(0, 0xFFFF)  # ランダムな16ビットカラーを生成
            KNJGfx.fillRectWH((i + 2, j + 2, 6, 6), color)
    utime.sleep(1)
    for i in range(0, DisplayWidth, 10):
        for j in range(0, DisplayHeight, 10):
            color = random.randint(0, 0xFFFF)  # ランダムな16ビットカラーを生成
            KNJGfx.fillRect((i + 3, j + 3, i+3+5, j+3+5), color)
    utime.sleep(1)
    KNJGfx.fillScreen(BLACK)
    
    #円
    for i in range(0,20) :
        KNJGfx.drawCircle(randXY(30,30),random.randint(5,30),random.randint(0,0xFFFF))
    for i in range(0,20) :
        KNJGfx.fillCircle(randXY(30,30),random.randint(5,30),random.randint(0,0xFFFF))
    utime.sleep(1)
    #角丸四角
    for i in range(0,20) :
        while True:
            xywh = randXYWH(0,0)
            r = random.randint(3,9)
            if (xywh[2] >(r * 2) and xywh[3] > (r*2)) :
                break
        KNJGfx.drawRoundRectWH(xywh,r,random.randint(0,0xFFFF))
    for i in range(0,20) :
        while True:
            xywh = randXYWH(0,0)
            r = random.randint(3,9)
            if (xywh[2] >(r * 2) and xywh[3] > (r*2)) :
                break
        KNJGfx.fillRoundRectWH(xywh,r,random.randint(0,0xFFFF))
    utime.sleep(1)
    KNJGfx.fillScreen(BLACK)
        
        
        
def demo_bmp() :
    global DisplayWidth
    global DisplayHeight
    # 画面を黒で塗りつぶす
    KNJGfx.fillScreen(BLACK)
    
    # 大きなビットマップの表示
    KNJGfx.bmpDraw((0,0,DisplayWidth,DisplayHeight),bmpdata.hiroshige,0)
    for i in range(0,10):
        KNJGfx.bmpDraw((random.randint(0,DisplayWidth-50),random.randint(0,DisplayHeight-80),48,77),bmpdata.sharaku,0)
    KNJGfx.bmpUnuseTransColor()
    utime.sleep(1)

    #透過ビットマップ（低速）
    KNJGfx.bmpUseTransColor(0b0000100000100001)
    KNJGfx.bmpDraw((0,0,DisplayWidth,DisplayHeight),bmpdata.hiroshige,0)
    for i in range(0,10):
        KNJGfx.bmpDraw((random.randint(0,DisplayWidth-50),random.randint(0,DisplayHeight-80),48,77),bmpdata.sharaku,0)
    KNJGfx.bmpUnuseTransColor()
    
    utime.sleep(1)
    
    #ビットマップの登録と表示
    pat1 = KNJGfx.registerBitmap(1,(16,16),bmpdata.bmpPat1);
    pat2 = KNJGfx.registerBitmap(2,(16,16),bmpdata.bmpPat2);
    pat3 = KNJGfx.registerBitmap(3,(16,16),bmpdata.bmpPat3);
    ret = KNJGfx.fillScreen(0)
    
    patIdx = 0
    for i in range(4,DisplayWidth-20,4):
        if patIdx % 3 == 0 :
            pat = 1;
        elif patIdx % 3 == 1:
            pat = 2;
        else :
            pat = 3

        KNJGfx.fillRectWH((i - 4,64,16,16),BLACK)
        KNJGfx.bmpRegDraw(pat,(i,64),0);
        utime.sleep(0.1)
        patIdx = patIdx+1
       
    for i in range(DisplayWidth - 20,4,-4) :
        if patIdx % 3 == 0:
            pat = 1
        elif patIdx % 3 == 1:
            pat = 2
        else :
            pat = 3
        KNJGfx.fillRectWH((i + 4,64,16,16),BLACK)
        KNJGfx.bmpRegDraw(pat,(i,64),1);
        utime.sleep(0.1)
        patIdx = patIdx+1
    

#val = cppexample2.cppfunc(3,3)

led=machine.Pin(25,machine.Pin.OUT)

spi = machine.SPI(0,baudrate=10000000, # 通信速度（ボーレート）
          polarity=0,  # クロックの極性
          phase=0,  # クロックの位相
          sck=machine.Pin(18),  # クロックピン
          mosi=machine.Pin(19),  # マスタ出力/スレーブ入力ピン
          miso=machine.Pin(16))

print("---start---")

utime.sleep(0.1)
ret = KNJGfx.InitHW((0,16, 17, 18, 19, 28, 15))
#print(f"InitHW:{ret}")
#utime.sleep(0.5)

KNJGfx.CreateGFX(1)
#print(f"CreateGFX:{ret}")
#utime.sleep(0.5)

ret = KNJGfx.doInit()
#print(f"doInit:{ret}")
#utime.sleep(0.5)

while True:
    ret = KNJGfx.fillScreen(0)
    DisplayWidth = KNJGfx.getWidth()
    DisplayHeight= KNJGfx.getHeight()
    print(f"ScreenSize:Width={DisplayWidth} , Height={DisplayHeight}")
    
    ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight-1),BLUE)
    #このデモは、ライブラリ側でTFT_ENABLE_FONTSをオフにする必要がある。
    #フォントを使用しないシンプルな文字表示は、demo_kanjiで使用している KNJGfx.drawTextKanji を使うことができる。
    #ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight-1),BLUE)
    #demo_text()
    demo_fontText()
    demo_kanji_V()
    demo_kanji_H()

    demo_bmp()
    demo_basic()
    demo_shapes()
#このデモは、ライブラリ側でTFT_ENABLE_FONTSをオフにする必要がある。
#フォントを使用しないシンプルな文字表示は、demo_kanjiで使用している KNJGfx.drawTextKanji を使うことができる。
#ret = KNJGfx.fillRectangle((0,0,DisplayWidth,DisplayHeight-1),BLUE)
#demo_text()




#print("step5")

while True:
    led.value(1)
    utime.sleep(1)
    led.value(0)
    utime.sleep(1)

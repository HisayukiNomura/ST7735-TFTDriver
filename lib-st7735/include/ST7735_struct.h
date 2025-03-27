#ifndef ST7735_STRUCT_H
#define ST7735_STRUCT_H



/// @brief 画面の回転方向を示す定義
enum ST7735_ROTATION {
	NORMAL = 0,
	DEGREE_90 = 1,
	DEGREE_180 = 2,
	DEGREE_270 = 3,
};

/// @brief ST7735に対する色の定義
class {
   public:
	const uint16_t BLACK = 0x0000;
	const uint16_t BLUE = 0x001F;
	const uint16_t RED = 0xF800;
	const uint16_t GREEN = 0x07E0;
	const uint16_t CYAN = 0x07FF;
	const uint16_t MAGENTA = 0xF81F;
	const uint16_t YELLOW = 0xFFE0;
	const uint16_t WHITE = 0xFFFF;
	const uint16_t GRAY        = 0x8410; // 灰色
	const uint16_t LIGHT_BLUE  = 0x03EF; // ライトブルー
	const uint16_t DARK_GREEN  = 0x03E0; // ダークグリーン
	const uint16_t DARK_BLUE   = 0x0010; // ダークブルー
	const uint16_t LIGHT_GREEN = 0x07E8; // ライトグリーン
	const uint16_t DARK_RED    = 0x7800; // ダークレッド
	const uint16_t ORANGE      = 0xFC00; // オレンジ
	const uint16_t PINK        = 0xF81F; // ピンク
} ST7735Color;

/// @brief 座標を示すクラス（16bit)。描画メソッドでは、座標として２つの引数を渡す代わりに、このクラスを使うことができる。
class Axis16 {
   public:
	uint16_t x;
	uint16_t y;
	/// @brief デフォルトコンストラクタ。0,0で初期化される
	Axis16(){x=0;y=0;}
	/// @brief コンストラクタ。
	/// @param x x座標
	/// @param y y座標
	Axis16(uint16_t x, uint16_t y)
	{
		this->x = x;
		this->y = y;
	}
};
/// @brief 座標を示すクラス(8bit)。描画メソッドでは、座標として２つの引数を渡す代わりに、このクラスを使うことができる。
class Axis8 {
   public:
	uint8_t x;
	uint8_t y;
	/// @brief デフォルトコンストラクタ。0,0で初期化される
	Axis8(){x=0;y=0;}
	/// @brief コンストラクタ。
	/// @param x x座標
	/// @param y y座標
	Axis8(uint8_t x, uint8_t y)
	{
		this->x = x;
		this->y = y;
	}
};
/// @brief 大きさを示すクラス(8bit)。描画メソッドでは、大きさとして２つの引数を渡す代わりに、このクラスを使うことができる。
class Size16 {
   public:
	uint16_t w;
	uint16_t h;
	/// @brief デフォルトコンストラクタ。0,0で初期化される
	Size16()
	{
		w = 0;
		h = 0;
	}
	/// @brief コンストラクタ
	/// @param w 横幅
	/// @param h 高さ
	Size16(uint16_t w, uint16_t h)
	{
		this->w = w;
		this->h = h;
	}
};
/// @brief 大きさを示すクラス(8bit)。描画メソッドでは、大きさとして２つの引数を渡す代わりに、このクラスを使うことができる。
class Size8 {
   public:
	uint8_t w;
	uint8_t h;
	/// @brief デフォルトコンストラクタ。0,0で初期化される
	Size8(){w=0;h=0;}
	/// @brief コンストラクタ
	/// @param w 横幅
	/// @param h 高さ
	Size8(uint8_t w, uint8_t h)
	{
		this->w = w;
		this->h = h;
	}
};

/// @brief 矩形の範囲をx1,y1 - x2,y2で示すクラス(16bit)。描画メソッドでは、このクラスを使うことができる。
class Region16 {
   public:
	Axis16 start;
	union {
		Axis16 end;
		Size16 size;
	};
	/// @brief デフォルトコンストラクタ。0,0で初期化される
	Region16() {}
	/// @brief コンストラクタ
	/// @param start 左上の座標
	/// @param end 右下の座標
	Region16(Axis16 start, Axis16 end)
	{
		this->start = start;
		this->end = end;
	}
	/// @brief コンストラクタ
	/// @param start 左上の座標
	/// @param size サイズ
	Region16(Axis16 start, Size16 size) {
		this->start = start;
		this->size = size;
	}

};


/// @brief 矩形の範囲をx1,y1 - x2,y2で示すクラス(8bit)。描画メソッドでは、このクラスを使うことができる。
class Region8 {
   public:
	Axis8 start;
	union {
		Axis8 end;
		Size8 size;
	};
		/// @brief デフォルトコンストラクタ。0,0で初期化される
	Region8() {}
	Region8(Axis8 start, Axis8 end)
	{
			this->start = start;
			this->end = end;
	}
	Region8(Axis8 start, Size8 size)
	{
		this->start = start;
		this->size = size;
	}
};

/// TFT_ENABLE_FONTSが有効な場合に使用される、ビットマップ情報を含むフォント構造体を格納するための構造
/// Font data stored PER GLYPH

/// @brief １文字のフォントの画像情報、大きさなどを定義するための構造体
/// @details 文字の形や大きさと文字のベースラインを定義する。漢字では使用されない。
typedef struct {
	uint16_t bitmapOffset;  ///< Pointer into GFXfont->bitmap
	uint8_t width;          ///< Bitmap dimensions in pixels
	uint8_t height;         ///< Bitmap dimensions in pixels
	uint8_t xAdvance;       ///< Distance to advance cursor (x axis)
	int8_t xOffset;         ///< X dist from cursor pos to UL corner
	int8_t yOffset;         ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// @brief フォントセットを格納している構造体。
/// @details フォントグリフ（GFXglyph構造体）を複数格納し、ひとまとめのフォントセットとして管理するためのデータ構造。
/// この構造体のデータへのポインタを指定することで、アプリケーションが使用するフォント（非漢字の１バイトフォント）を変更できる。

typedef struct {
	uint8_t *bitmap;     ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;     ///< Glyph array
	uint16_t first;      ///< ASCII extents (first char)
	uint16_t last;       ///< ASCII extents (last char)
	uint8_t yAdvance;    ///< Newline distance (y axis)
	const char *subset;  ///< subset of chars in the font
} GFXfont;


static struct {
	const char *name;
	const GFXfont* font;
} registeredFonts[16];
#endif

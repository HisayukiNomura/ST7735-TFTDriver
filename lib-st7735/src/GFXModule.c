/// @brief このファイルは、MicropythonのC拡張モジュールとして使用する場合に有効にする。

#include "../include/GFXModule.h"
#ifdef USE_MICROPYTHON_MODULE

// Define a Python reference to the function we'll make available.
// See example.cpp for the definition.
static MP_DEFINE_CONST_FUN_OBJ_2(cppfunc_obj, cppfunc);
static MP_DEFINE_CONST_FUN_OBJ_1(getTypeName_obj, getTypeName);
static MP_DEFINE_CONST_FUN_OBJ_1(InitHW_obj, InitHW);
static MP_DEFINE_CONST_FUN_OBJ_1(CreateGFX_obj, CreateGFX);
static MP_DEFINE_CONST_FUN_OBJ_0(doInit_obj, doInit);

static MP_DEFINE_CONST_FUN_OBJ_1(setTextWrap_obj, setTextWrap);
static MP_DEFINE_CONST_FUN_OBJ_1(setRotation_obj, setRotation);
static MP_DEFINE_CONST_FUN_OBJ_0(getWidth_obj, getWidth);
static MP_DEFINE_CONST_FUN_OBJ_0(getHeight_obj, getHeight);
// テキスト関連関数
static MP_DEFINE_CONST_FUN_OBJ_1(setAsciiFont_obj, setAsciiFont);
static MP_DEFINE_CONST_FUN_OBJ_0(getAvaiableAsciiFonts_obj, getAvaiableAsciiFonts);
static MP_DEFINE_CONST_FUN_OBJ_2(drawText_obj, drawText);
static MP_DEFINE_CONST_FUN_OBJ_2(drawTextKanji_obj, drawTextKanji);

// スクロール関連関数
static MP_DEFINE_CONST_FUN_OBJ_3(setScrollDefinition_obj, setScrollDefinition);
static MP_DEFINE_CONST_FUN_OBJ_1(verticalScroll_obj, verticalScroll);

// 基本描画関数 static MP_DEFINE_CONST_FUN_OBJ_2(_obj, );

static MP_DEFINE_CONST_FUN_OBJ_1(fillScreen_obj, fillScreen);
static MP_DEFINE_CONST_FUN_OBJ_2(fillRectangle_obj, fillRectangle);
static MP_DEFINE_CONST_FUN_OBJ_3(drawFastHLine_obj, drawFastHLine);
static MP_DEFINE_CONST_FUN_OBJ_3(drawFastVLine_obj, drawFastVLine);
static MP_DEFINE_CONST_FUN_OBJ_0(normalDisplay_obj, normalDisplay);
static MP_DEFINE_CONST_FUN_OBJ_2(drawPixel_obj, drawPixel);
static MP_DEFINE_CONST_FUN_OBJ_1(invertDisplay_obj, invertDisplay);

// 描画関数 static MP_DEFINE_CONST_FUN_OBJ_3(_obj, );
static MP_DEFINE_CONST_FUN_OBJ_2(drawRectWH_obj, drawRectWH);
static MP_DEFINE_CONST_FUN_OBJ_2(drawRect_obj, drawRect);
static MP_DEFINE_CONST_FUN_OBJ_2(fillRectWH_obj, fillRectWH);
static MP_DEFINE_CONST_FUN_OBJ_2(fillRect_obj, fillRect);
static MP_DEFINE_CONST_FUN_OBJ_3(drawCircle_obj, drawCircle);
static MP_DEFINE_CONST_FUN_OBJ_3(fillCircle_obj, fillCircle);
static MP_DEFINE_CONST_FUN_OBJ_3(drawRoundRectWH_obj, drawRoundRectWH);
static MP_DEFINE_CONST_FUN_OBJ_3(drawRountRect_obj, drawRountRect);
static MP_DEFINE_CONST_FUN_OBJ_3(fillRoundRectWH_obj, fillRoundRectWH);
static MP_DEFINE_CONST_FUN_OBJ_3(fillRoundRect_obj, fillRoundRect);
static MP_DEFINE_CONST_FUN_OBJ_2(drawLine_obj, drawLine);

// ビットマップ関数 static MP_DEFINE_CONST_FUN_OBJ_3(_obj, );
static MP_DEFINE_CONST_FUN_OBJ_3(bmpDraw_obj, bmpDraw);
static MP_DEFINE_CONST_FUN_OBJ_3(registerBitmap_obj, registerBitmap);
static MP_DEFINE_CONST_FUN_OBJ_3(bmpRegDraw_obj, bmpRegDraw);
static MP_DEFINE_CONST_FUN_OBJ_1(bmpUseTransColor_obj, bmpUseTransColor);
static MP_DEFINE_CONST_FUN_OBJ_0(bmpUnuseTransColor_obj, bmpUnuseTransColor);

// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t KNJGfx_globals_table[] = {
	{MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_PTR(&InitHW_obj)},
	// 初期化関数
	{MP_ROM_QSTR(MP_QSTR_InitHW), MP_ROM_PTR(&InitHW_obj)},
	{MP_ROM_QSTR(MP_QSTR_CreateGFX), MP_ROM_PTR(&CreateGFX_obj)},
	{MP_ROM_QSTR(MP_QSTR_doInit), MP_ROM_PTR(&doInit_obj)},

	// 設定や情報の取得関数
	{MP_ROM_QSTR(MP_QSTR_getTypeName), MP_ROM_PTR(&getTypeName_obj)},
	{MP_ROM_QSTR(MP_QSTR_setTextWrap), MP_ROM_PTR(&setTextWrap_obj)},
	{MP_ROM_QSTR(MP_QSTR_setRotation), MP_ROM_PTR(&setRotation_obj)},
	{MP_ROM_QSTR(MP_QSTR_getWidth), MP_ROM_PTR(&getWidth_obj)},
	{MP_ROM_QSTR(MP_QSTR_getHeight), MP_ROM_PTR(&getHeight_obj)},
	{MP_ROM_QSTR(MP_QSTR_normalDisplay), MP_ROM_PTR(&normalDisplay_obj)},

	// テキスト関連関数
	{MP_ROM_QSTR(MP_QSTR_setAsciiFont), MP_ROM_PTR(&setAsciiFont_obj)},
	{MP_ROM_QSTR(MP_QSTR_getAvaiableAsciiFonts), MP_ROM_PTR(&getAvaiableAsciiFonts_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawText), MP_ROM_PTR(&drawText_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawTextKanji), MP_ROM_PTR(&drawTextKanji_obj)},

	// スクロール関連関数
	{MP_ROM_QSTR(MP_QSTR_setScrollDefinition), MP_ROM_PTR(&setScrollDefinition_obj)},
	{MP_ROM_QSTR(MP_QSTR_verticalScroll), MP_ROM_PTR(&verticalScroll_obj)},

	// 基本描画関数　		{MP_ROM_QSTR(MP_QSTR_), MP_ROM_PTR(&_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillScreen), MP_ROM_PTR(&fillScreen_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillRectangle), MP_ROM_PTR(&fillRectangle_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawFastHLine), MP_ROM_PTR(&drawFastHLine_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawFastVLine), MP_ROM_PTR(&drawFastVLine_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawPixel), MP_ROM_PTR(&drawPixel_obj)},
	{MP_ROM_QSTR(MP_QSTR_invertDisplay), MP_ROM_PTR(&invertDisplay_obj)},

	// 描画関数
	{MP_ROM_QSTR(MP_QSTR_drawRectWH), MP_ROM_PTR(&drawRectWH_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawRect), MP_ROM_PTR(&drawRect_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillRectWH), MP_ROM_PTR(&fillRectWH_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillRect), MP_ROM_PTR(&fillRect_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawCircle), MP_ROM_PTR(&drawCircle_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillCircle), MP_ROM_PTR(&fillCircle_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawRoundRectWH), MP_ROM_PTR(&drawRoundRectWH_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawRountRect), MP_ROM_PTR(&drawRountRect_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillRoundRectWH), MP_ROM_PTR(&fillRoundRectWH_obj)},
	{MP_ROM_QSTR(MP_QSTR_fillRoundRect), MP_ROM_PTR(&fillRoundRect_obj)},
	{MP_ROM_QSTR(MP_QSTR_drawLine), MP_ROM_PTR(&drawLine_obj)},

	// ビットマップ関数　		{MP_ROM_QSTR(MP_QSTR_), MP_ROM_PTR(&_obj)},
	{MP_ROM_QSTR(MP_QSTR_bmpDraw), MP_ROM_PTR(&bmpDraw_obj)},
	{MP_ROM_QSTR(MP_QSTR_registerBitmap), MP_ROM_PTR(&registerBitmap_obj)},
	{MP_ROM_QSTR(MP_QSTR_bmpRegDraw), MP_ROM_PTR(&bmpRegDraw_obj)},
	{MP_ROM_QSTR(MP_QSTR_bmpUseTransColor), MP_ROM_PTR(&bmpUseTransColor_obj)},
	{MP_ROM_QSTR(MP_QSTR_bmpUnuseTransColor), MP_ROM_PTR(&bmpUnuseTransColor_obj)},
};

static MP_DEFINE_CONST_DICT(KNJGfx_module_globals, KNJGfx_globals_table);
const mp_obj_module_t mp_module_KNJGfx = {
	.base = {&mp_type_module},
	.globals = (mp_obj_dict_t *)&KNJGfx_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_KNJGfx, mp_module_KNJGfx);



static const mp_rom_map_elem_t cppexample_module_globals_table[] = {
	{MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_cppexample)},
	{MP_ROM_QSTR(MP_QSTR_cppfunc), MP_ROM_PTR(&cppfunc_obj)},
};

// モジュール定義

static MP_DEFINE_CONST_DICT(cppexample_module_globals, cppexample_module_globals_table);

// Define module object.
const mp_obj_module_t cppexample_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&cppexample_module_globals,
};


// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_cppexample2, cppexample_user_cmodule);
#endif

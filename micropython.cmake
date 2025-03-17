# Micropythonの拡張Cモジュールとしてビルドするときに使用する cmakeファイル。

# インタフェースライブラリを作成する。
add_library(KNJGfx INTERFACE)

# ビルド対象となるファイルをここに指定する
target_sources(KNJGfx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/GFX.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GFXModule.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ST7735_TFT.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ST7735_initcmd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/KanjiHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/hw.cpp
 )

# インクルードディレクトリをカレントディレクトリに設定。
# ソースコードからのインクルードでは ../include/XXXX.h としてインクルードするのでここは
# このままでいい
target_include_directories(KNJGfx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# ユーザーモジュールライブラリとしてリンクする
target_link_libraries(usermod INTERFACE KNJGfx)

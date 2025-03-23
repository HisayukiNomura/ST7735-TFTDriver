# C/C++から呼びだすサンプルをビルドするときに使われる cmake ファイル。
# Python拡張として使用される時にはこちらは使用しない。


cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(SPIDisplay C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


#サブディレクトリを追加
add_subdirectory(lib-st7735)

# Add executable. Default name is the project name, version 0.1

add_executable(SPIDisplay SPIDisplay.cpp lib-st7735/src/hw.cpp lib-st7735/src/ST7735_TFT.cpp lib-st7735/src/st7735_initcmd.cpp lib-st7735/src/KanjiHelper.cpp lib-st7735/src/GFX.cpp lib-st7735/src/GFXModule.c)

pico_set_program_name(SPIDisplay "SPIDisplay")
pico_set_program_version(SPIDisplay "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(SPIDisplay 0)
pico_enable_stdio_usb(SPIDisplay 1)

# Add the standard library to the build
target_link_libraries(SPIDisplay
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(SPIDisplay PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(SPIDisplay 
        hardware_spi
        )

pico_add_extra_outputs(SPIDisplay)


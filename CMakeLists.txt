cmake_minimum_required(VERSION 3.10)

# プロジェクト名とバージョン
project(myeq VERSION 1.0 LANGUAGES C)

# GStreamerのパッケージを探す
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GST_BASE REQUIRED gstreamer-base-1.0)
pkg_check_modules(GST_AUDIO REQUIRED gstreamer-audio-1.0)

# ソースファイルを指定
set(SOURCES
    gstmyeq.c
    gstmyeq.h
)

# 共有ライブラリを作成
add_library(myeq SHARED ${SOURCES})

# コンパイルオプションを設定
target_compile_options(myeq PRIVATE ${GST_CFLAGS_OTHER})

# インクルードディレクトリを設定
target_include_directories(myeq PRIVATE ${GST_INCLUDE_DIRS} ${GST_BASE_INCLUDE_DIRS} ${GST_AUDIO_INCLUDE_DIRS})

# リンクするライブラリを設定
target_link_libraries(myeq PRIVATE ${GST_LIBRARIES} ${GST_BASE_LIBRARIES} ${GST_AUDIO_LIBRARIES})

# インストールターゲットを設定
install(TARGETS myeq
    LIBRARY DESTINATION lib/gstreamer-1.0
)
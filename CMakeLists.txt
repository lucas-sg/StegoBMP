cmake_minimum_required(VERSION 3.13)
project(StegoBMP)
set(CMAKE_CXX_STANDARD 14)
include_directories(include)

file(GLOB stego_SRC "lsbi/rc4.c" "stego/*.c" "*.c")

add_executable(bmpParserTest tests/bmpParser.c bmpParser.c)
add_executable(testLsb tests/testLsb.c bmpParser.c embed.c stego/lsbEmbed.c stego/lsbExtract.c stego/lsbHelper.c lsbi/rc4.c)
add_executable(main ${stego_SRC})
target_include_directories(main PRIVATE include)

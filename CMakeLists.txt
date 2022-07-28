cmake_minimum_required(VERSION 3.22)
project(mmn14)

set(CMAKE_CXX_STANDARD 14)

add_executable(mmn14 first_scan.c first_scan.h tmp-preproccessor/assembler_1.c preprocessor.c preprocessor.h linked_list_utils.c macro.c macro.h linked_list_utils.h assembler.c assembler.h second_scan.h symbol_table.h queue.h output.h compilation.h)

cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SEGALIB $ENV{SATURN_SBL}/segalib)
set(SEGASGL $ENV{SATURN_SGL})

set(SATURN_COMMON $ENV{SATURN_COMMON})
set(SATURN_CMAKE $ENV{SATURN_CMAKE})

set(TARGET_NAME wold4sdl)
set(TARGET_PATH wold4sdl)

project( ${TARGET_NAME} VERSION 1.0
                  DESCRIPTION "Sega Saturn Wolf4SDL"
                  LANGUAGES CXX ASM)

set(as_flags )
set(cxx_flags
        -O2 -g3 -m2 -fpermissive
        -Wformat-diag -Wno-conversion-null
        -fno-lto -Wshadow -Wall -Wswitch -Wpadded
        -fno-common -Wextra -Wno-narrowing -fno-web
        -fno-builtin -funit-at-a-time -Wwrite-strings
        -Wl,--strip-all -Wl,--verbose -mno-fsrra
        -maccumulate-outgoing-args -fomit-frame-pointer
        -fno-rtti -fno-exceptions
         )

set(SOURCES
  src/low.s
  src/saturn.cpp
  src/pcmsys.cpp
  src/signon.cpp
  src/workarea.cpp
  src/id_ca.cpp
  src/id_in.cpp
  src/id_pm.cpp
  src/id_sd.cpp
  src/id_vl.cpp
  src/id_vh.cpp
  src/id_us_1.cpp
  src/wl_agent.cpp
  src/wl_act1.cpp
  src/wl_act2.cpp
  src/wl_draw.cpp
  src/wl_game.cpp
  src/wl_play.cpp
  src/wl_menu.cpp
  src/wl_state.cpp
  src/wl_inter.cpp
  src/wl_text.cpp
  src/wl_main.cpp
  )

set(CD_FILES
      0.bin
    )

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(
    ${TARGET_NAME}.elf PUBLIC
    .
    ${SEGALIB}/include
    ${SEGASGL}/include
    )

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${cxx_flags}>
                                                 $<$<COMPILE_LANGUAGE:ASM>:${as_flags}> )

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T root/SL.LNK"
    "SHELL:-Xlinker -e -Xlinker ___Start -Xlinker -S -nostartfiles"
    #"SHELL:-lc -lgcc -lm"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )

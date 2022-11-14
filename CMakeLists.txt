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
        -Os -m2 -fpermissive
        -Wno-conversion-null
        -Wshadow
        -Wall
        -Wswitch
        -Wpadded
        -Wextra
        -Wno-narrowing
        #-Wwrite-strings
        -Wformat-diag
        -falign-functions=4
        -Wl,--strip-all
        -Wl,--verbose
        -mno-fsrra
        -maccumulate-outgoing-args
        -fomit-frame-pointer
        -funit-at-a-time
        -fno-rtti
        -fno-web
        -fno-lto
        -fno-exceptions
        -fno-common
        -fno-builtin
        -c
        -lm
         )

set(SOURCES
  src/low.s

  #src/fmopl.cpp
  src/id_ca.cpp
  src/id_in.cpp
  src/id_pm.cpp
  src/id_sd.cpp
  src/id_us_1.cpp
  src/id_vh.cpp
  src/id_vl.cpp

  src/pcmsys.cpp
  src/saturn.cpp
  #src/sdl_winmain.cpp
  src/signon.cpp

  src/wl_act1.cpp
  src/wl_act2.cpp
  src/wl_agent.cpp
  #src/wl_atmos.cpp
  #src/wl_cloudsky.cpp
  #src/wl_debug.cpp
  #src/wl_dir3dspr.cpp
  src/wl_draw.cpp
  #src/wl_floorceiling.cpp
  src/wl_game.cpp
  src/wl_inter.cpp
  src/wl_main.cpp
  src/wl_menu.cpp
  #src/wl_parallax.cpp
  src/wl_play.cpp
  #src/wl_shade.cpp
  src/wl_state.cpp
  src/wl_text.cpp
  src/workarea.cpp
  )

set(CD_FILES
      0.bin
    )

add_compile_definitions(MODEL_S _SH)

add_executable( ${TARGET_NAME}.elf ${SOURCES} )

target_include_directories(
    ${TARGET_NAME}.elf PUBLIC
    .
    src
    sdl
    ${SEGALIB}/include
    ${SEGASGL}/include
    )

target_compile_options(${TARGET_NAME}.elf PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${cxx_flags}>
                                                 $<$<COMPILE_LANGUAGE:ASM>:${as_flags}> )

target_link_options(${TARGET_NAME}.elf PUBLIC
    "SHELL:-Xlinker -Map -Xlinker ${TARGET_NAME}.map"
    "SHELL:-Xlinker --strip-debug"
    "SHELL:-Xlinker -fno-lto"
    "SHELL:-Xlinker -z -Xlinker muldefs"
    "SHELL:-m2 -nostartfiles"
    "SHELL:-T ${PROJECT_SOURCE_DIR}/root/sl.lnk"
    "SHELL:-Xlinker -e -Xlinker ___Start -Xlinker -S -nostartfiles"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_gfs.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_dma.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_int.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_sys.a )

add_custom_target(run_${TARGET_NAME} ALL DEPENDS ${TARGET_NAME}.elf ${TARGET_NAME}.bin)

add_custom_command(OUTPUT ${TARGET_NAME}.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_OBJCOPY}
                   ARGS -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin )

file(GLOB_RECURSE EXISTING_FILES
      LIST_DIRECTORIES true
      ${PROJECT_SOURCE_DIR}/CD/*)

if (EXISTING_FILES)
    file (REMOVE_RECURSE ${EXISTING_FILES})
endif()

file(GLOB SL_BIN
      ${PROJECT_SOURCE_DIR}/root/sl.bin)

if (SL_BIN)
    file (REMOVE ${SL_BIN})
endif()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin
                 DESTINATION ${PROJECT_SOURCE_DIR}/root
                 RENAME sl.bin )

install(FILES ${PROJECT_SOURCE_DIR}/root/IP.BIN
             DESTINATION ${PROJECT_SOURCE_DIR}/CD )

install(FILES ${PROJECT_SOURCE_DIR}/FILES.txt
            DESTINATION ${PROJECT_SOURCE_DIR}/CD )

install(FILES ${PROJECT_SOURCE_DIR}/EXCLUDE.txt
            DESTINATION ${PROJECT_SOURCE_DIR}/CD )

install(FILES ${PROJECT_SOURCE_DIR}/TRCKwolf.CUE
            DESTINATION ${PROJECT_SOURCE_DIR}/CD )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/root
            DESTINATION ${PROJECT_SOURCE_DIR}/CD
            PATTERN "sndw/*.pcm" EXCLUDE
            PATTERN "sndw/*.wav" EXCLUDE
            PATTERN "wav/*.wav" EXCLUDE
            )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/root/wav/
            DESTINATION ${PROJECT_SOURCE_DIR}/CD
            FILES_MATCHING PATTERN "*.wav"
            )

set(MAKE_CMD $ENV{SATURN_CD}/mkisofs -v -sysid "\"SEGA SEGASATURN\"" -volid "WOLF3D"
                    -volset "WOLF3D" -publisher "VBT"
                    -preparer "VBT" -appid "CD0001"
                    -abstract "ABS.TXT" -copyright "CPY.TXT"
                    -biblio "BIB.TXT" -generic-boot IP.BIN
                    -full-iso9660-filenames -o ${PROJECT_SOURCE_DIR}/CD/${TARGET_NAME}.iso
                    -exclude-list EXCLUDE.txt
                    -graft-points
                    -path-list FILES.txt)

install(CODE "execute_process(COMMAND
                               ${MAKE_CMD}
                               WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/CD)")

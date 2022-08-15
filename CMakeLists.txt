cmake_minimum_required(VERSION 3.0)

SET(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(SEGALIB $ENV{SATURN_SBL}/segalib)
set(SEGASGL $ENV{SATURN_SGL})
set(JOSGL $ENV{SATURN_JOENGINE}/common/SGL_302j/LIB_ELF)
set(JOSGL_INC $ENV{SATURN_JOENGINE}/common/SGL_302j/INC)

set(SATURN_COMMON $ENV{SATURN_COMMON})
set(SATURN_CMAKE $ENV{SATURN_CMAKE})

set(TARGET_NAME wold4sdl)
set(TARGET_PATH wold4sdl)

project( ${TARGET_NAME} VERSION 1.0
                  DESCRIPTION "Sega Saturn Wolf4SDL"
                  LANGUAGES CXX ASM)

set(as_flags )
set(cxx_flags
        -O2 -m2 -fpermissive
        -Wno-conversion-null
        -Wshadow -Wall -Wswitch -Wpadded
        -Wextra -Wno-narrowing -Wwrite-strings
        -fno-web -fno-lto
        -falign-functions=4
        -fno-builtin -funit-at-a-time
        -Wl,--strip-all -Wl,--verbose -mno-fsrra
        -maccumulate-outgoing-args -fomit-frame-pointer
        -fno-rtti -fno-exceptions
        -fno-common
        -c
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
    src
    sdl
    ${SEGALIB}/include
    ${JOSGL_INC}
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
#    "SHELL:-lc -lgcc"
)

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${JOSGL}/LIBSGL.A )
#target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGASGL}/lib/libsgl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_scl.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_mth.a )

target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_gfs.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_csh.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_spr.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_cdc.a )
target_link_libraries(${TARGET_NAME}.elf PUBLIC ${SEGALIB}/lib/sega_per.a )
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

install(DIRECTORY ${PROJECT_SOURCE_DIR}/root
            DESTINATION ${PROJECT_SOURCE_DIR}/CD
            PATTERN "sndw/*.pcm" EXCLUDE
            PATTERN "sndw/*.wav" EXCLUDE
            )

#list(TRANSFORM CD_FILES PREPEND "${PROJECT_SOURCE_DIR}/CD/" )
#list(TRANSFORM CD_FILES APPEND "\\n" )

#set (filename ${PROJECT_SOURCE_DIR}/CD/FILES.txt)
#install(CODE "file(WRITE ${filename} ${CD_FILES})")

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
                               WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/CD) ")

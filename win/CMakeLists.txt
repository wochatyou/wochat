# the build for Windows X64
project(wochat-win64 CXX)

set(WOCHATAPP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "WoChat application directory")

add_executable(${PROJECT_NAME} WIN32 main.cpp wochat.cpp mqtt.cpp wochat.rc)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/wtl ${CMAKE_SOURCE_DIR} ${MOSQUITTO_SOURCE_DIR} {LIBDUI_SOURCE_DIR} {LIBBITCOIN_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} libdui libbitcoin libmosquitto_static)

set_property(TARGET ${PROJECT_NAME} PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/secp256k1_x64-windows-static/debug/lib/secp256k1_precomputed.lib)
target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/secp256k1_x64-windows-static/debug/lib/secp256k1.lib)
target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/secp256k1_x64-windows-static/lib/secp256k1_precomputed.lib)
target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/secp256k1_x64-windows-static/lib/secp256k1.lib)

target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/crc32c_x64-windows-static/debug/lib/crc32c.lib)
target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/crc32c_x64-windows-static/lib/crc32c.lib)

target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/modp-base64_x64-windows-static/debug/lib/libmodpbase64.lib)
target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/modp-base64_x64-windows-static/lib/libmodpbase64.lib)

target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/mbedtls_x64-windows-static/debug/lib/mbedcrypto.lib)
target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/mbedtls_x64-windows-static/lib/mbedcrypto.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/harfbuzz_x64-windows-static/debug/lib/harfbuzz.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/harfbuzz_x64-windows-static/lib/harfbuzz.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/freetype_x64-windows-static/debug/lib/freetyped.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/freetype_x64-windows-static/lib/freetype.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/cairo_x64-windows-static/debug/lib/cairo.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/cairo_x64-windows-static/lib/cairo.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/fontconfig_x64-windows-static/debug/lib/fontconfig.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/fontconfig_x64-windows-static/lib/fontconfig.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/pixman_x64-windows-static/debug/lib/pixman-1.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/pixman_x64-windows-static/lib/pixman-1.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/expat_x64-windows-static/debug/lib/libexpatdMT.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/expat_x64-windows-static/lib/libexpatMT.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/libpng_x64-windows-static/debug/lib/libpng16d.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/libpng_x64-windows-static/lib/libpng16.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/zlib_x64-windows-static/debug/lib/zlibd.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/zlib_x64-windows-static/lib/zlib.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/bzip2_x64-windows-static/debug/lib/bz2d.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/bzip2_x64-windows-static/lib/bz2.lib)

# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/brotli_x64-windows-static/debug/lib/brotlidec-static.lib)
# target_link_libraries(${PROJECT_NAME} debug c:/vcpkg/packages/brotli_x64-windows-static/debug/lib/brotlicommon-static.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/brotli_x64-windows-static/lib/brotlidec-static.lib)
# target_link_libraries(${PROJECT_NAME} optimized c:/vcpkg/packages/brotli_x64-windows-static/lib/brotlicommon-static.lib)

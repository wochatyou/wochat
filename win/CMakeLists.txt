# the build for Windows X64
project(wochat-win64 CXX)

set(WOCHATAPP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "WoChat application directory")

add_executable(${PROJECT_NAME} WIN32 main.cpp wochat.cpp mqtt.cpp wochat.rc)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/wtl ${CMAKE_SOURCE_DIR} ${MOSQUITTO_SOURCE_DIR} {LIBDUI_SOURCE_DIR} {LIBBITCOIN_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} libdui libbitcoin libmosquitto_static secp256k1 crc32c base64)

set_property(TARGET ${PROJECT_NAME} PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

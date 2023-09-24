project(libbitcoin)

file(GLOB LIBBITCOIN_SRC 
	"bitcoin.cpp"
	"src/crypto/*.cpp"
	"src/common/*.cpp"
	"src/support/*.cpp"
	"src/util/*.cpp"
	"src/univalue/lib/*.cpp"
	"src/wallet/*.cpp"
	"src/*.cpp"
	)

add_library(${PROJECT_NAME} ${LIBBITCOIN_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})


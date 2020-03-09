include_directories(${PROJECT_SOURCE_DIR}/cpp-httplib-master)
set(SERVER_SRC main.cc MsgPraser.cc)
LINK_LIBRARIES(
"/usr/lib64/mysql/libmysqlservices.a"
"/usr/lib64/mysql/libmysqlclient.a"
pthread dl
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(CHttpServer ${SERVER_SRC})

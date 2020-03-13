include_directories(${PROJECT_SOURCE_DIR}/cpp-httplib-master ${PROJECT_SOURCE_DIR}/server)
set(SERVER_SRC main.cc MsgPraser.cc myDB.cc FuncInterface/login+register.cc)
LINK_LIBRARIES(
"/usr/lib64/mysql/libmysqlservices.a"
"/usr/lib64/mysql/libmysqlclient.a"
pthread dl
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(CHttpServer ${SERVER_SRC})

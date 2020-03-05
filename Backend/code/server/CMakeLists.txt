include_directories(${PROJECT_SOURCE_DIR}/cpp-httplib)
set(SERVER_SRC main.cc)
LINK_LIBRARIES(pthread)
add_executable(CHttpServer ${SERVER_SRC})

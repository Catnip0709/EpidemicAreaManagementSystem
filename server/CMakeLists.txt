include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/cpp-httplib-master/out ${PROJECT_SOURCE_DIR}/server)
set(SERVER_SRC 
    main.cc 
    myDB.cc 
    helpFunc.cpp
    httplib.cc 
    FuncInterface/login+register.cc 
    FuncInterface/announcement.cc
    FuncInterface/familyManagement.cc
    FuncInterface/physicalCondition.cc
    FuncInterface/equipment.cc
    FuncInterface/Quarantine.cc)
LINK_LIBRARIES(
"/usr/lib64/mysql/libmysqlservices.a"
"/usr/lib64/mysql/libmysqlclient.a"
pthread dl
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(CHttpServer ${SERVER_SRC})

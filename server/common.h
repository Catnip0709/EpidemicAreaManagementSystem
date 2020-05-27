#ifndef COMMON_H
#define COMMON_H
// 业务值
#define CREATE     10 // 插入
#define RETRIEVE   11 // 读取
#define UPDATE     12 // 更新
#define DELETE     13 // 删除

// 返回值
#define SUCCESS    0
#define ERROR     -1
#define MYSQL_ERR -2

#define BEEN_REGISTER       1000 // 该id已被注册
#define HAVENT_REGISTER     1001 // 该id尚未注册
#define PWD_ERR             1002 // 登录密码错误
#define DATA_ERR            1003 // 数据库数据有误
#define NOT_INCHARGE        1004 // 当前管理员对该栋楼无管理权限
#define ILLEGAL_REQUEST     1005 // 非法请求
#define NOT_ADMINISTRATOR   1006 // 该用户不是管理员
#define DATA_NULL           1007 // 数据缺失

#include<iostream>
#include<string>
#include<vector>
#include<map>

#endif
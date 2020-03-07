数据库名：EpidemicDatabase
# 一、基本信息表
## 1、User表（疫区人员和疫区管理员）
+ 18位身份证号码 userID （主键）<br>
+ 姓名 userName <br>
+ 登陆密码 password <br>
+ 该用户是否是疫区管理员 isAdministrator <br>
```
CREATE TABLE `User` (
  `userID` char(18) NOT NULL, 
  `userName` varchar(20) NOT NULL,
  `password` varchar(255) NOT NULL,
  `isAdministrator` boolean NOT NULL,
  PRIMARY KEY (`userID`) 
);
```
# 二、小区和户相关表
## 1、Neighbor表（住宅小区）
+ 小区号 neighborID （主键）
+ 该小区管理员 administratorID
```
CREATE TABLE `Neighbor` (
  `neighborID` int NOT NULL, 
  `administratorID` char(18) NOT NULL,
  PRIMARY KEY (`neighborID`) 
);
```
## 2、Family表（户）
+ 小区号 neighborID （主键1）
+ 户号 familyID （主键2）
+ 该户负责人 leaderID
+ 是否为空户
+ 该住户所有人ID userID （多人居住在一户用下划线分割，如1111_2222_3333）
```
CREATE TABLE `Family` (
  `neighborID` int NOT NULL, 
  `familyID` int NOT NULL, 
  `leaderID` char(18) NOT NULL,
  `isNull` boolean NOT NULL,
  `userID` varchar(255)
  PRIMARY KEY (`neighborID`, `familyID`) 
);
```

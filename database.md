数据库名：EpidemicDatabase
# 一、基本信息表
## 1、User表（疫区人员）
+ 18位身份证号码 userID （主键）  
+ 姓名 userName  
+ 登陆密码 password  
+ 手机号 phone    
+ 所住栋号 buildingID  
+ 所住户号 familyID  
```
CREATE TABLE `User` (
  `userID` char(18) NOT NULL, 
  `userName` varchar(100) NOT NULL,
  `password` varchar(255) NOT NULL,
  `phone` char(11) NOT NULL,
  `buildingID` int NOT NULL, 
  `familyID` int NOT NULL, 
  PRIMARY KEY (`userID`) 
) CHARSET=utf8;
```

## 2、Admin表（疫区管理员）
+ 18位身份证号码 userID （主键）  
+ 姓名 userName  
+ 登陆密码 password  
+ 手机号 phone    
+ 管理的栋号 buildingID  
```
CREATE TABLE `Admin` (
  `userID` char(18) NOT NULL, 
  `userName` varchar(100) NOT NULL,
  `password` varchar(255) NOT NULL,
  `phone` char(11) NOT NULL,
  `buildingID` varchar(255) NOT NULL, 
  PRIMARY KEY (`userID`) 
) CHARSET=utf8;
```

# 二、小区和户相关表
## 1、Building表（栋）
+ 栋号 buildingID （主键）
+ 该栋楼管理员 administratorID  
栋和管理员是多对多的关系，一个管理员可以管多栋楼，一栋楼可以被多个管理员管
```
CREATE TABLE `Building` (
  `buildingID` int NOT NULL, 
  `administratorID` char(18) NOT NULL
);
```

## 2、Family表（户）
+ 栋号 buildingID （主键1）
+ 户号 familyID （主键2）
+ 该户负责人 leaderID
+ 是否为空户
+ 户的类型（非隔离户0，隔离户1，病患户2）
+ 该住户所有人身份证号 userID （多人居住在一户用下划线分割，如1111_2222_3333）

户的类型：需要每天遍历所有住户是否为隔离人员或病患以实时更新。
```
CREATE TABLE `Family` (
  `buildingID` int NOT NULL, 
  `familyID` int NOT NULL, 
  `leaderID` char(18) NOT NULL,
  `isNull` boolean NOT NULL,
  `type` int NOT NULL,
  `userID` varchar(255),
  PRIMARY KEY (`buildingID`, `familyID`) 
);
```

# 三、基本功能表
## 1、PhysicalCondition 每日身体情况上报表
+ 上报人ID userID (主键1)
+ 上报日期 date （主键2）
+ 当日体温 todayTemperature
+ 当日接触的湖北籍人员身份证号 HuBeiContact  无接触则空，多人接触以下划线分割  
```
CREATE TABLE `PhysicalCondition` (
  `userID` char(18) NOT NULL,
  `date` DATE NOT NULL, 
  `todayTemperature` double NOT NULL,
  `HuBeiContact` varchar(255),
  PRIMARY KEY (`userID`, `date`) 
);
```

## 2、EquipmentStorage 防护用具存量表
+ 防护用具名称 equipment （主键）
+ 剩余存量 storage
```
CREATE TABLE `EquipmentStorage` (
  `equipment` varchar(255) NOT NULL,
  `storage` int NOT NULL, 
  PRIMARY KEY (`equipment`) 
)CHARSET=utf8;
```

## 3、关系网络管理
？

## 4、IsolationManagement  隔离管理
+ 隔离人员身份证 userID （主键1）
+ 当前日期 date （主键2）
+ 是否是病患 isPatient
+ 已隔离天数 days

病患人员的隔离天数置为-1，需要一直隔离直到治愈后（isPatient转为false）再隔离14天才可解除隔离。普通隔离人员（疑似人员、返工人员登）隔离14天后解除隔离。
```
CREATE TABLE `IsolationManagement` (
  `userID` char(18) NOT NULL,
  `date` DATE NOT NULL,
  `isPatient` boolean NOT NULL,
  `days` int NOT NULL, 
  PRIMARY KEY (`userID`, `date`) 
);
```

# 三、公告栏
## 公告表
+ 公告id id （主键）
+ 公告标题 title
+ 公告内容 content
+ 公告级别 state
+ 公告发布日期 date
```
CREATE TABLE `Announcement` (
  `id`      int(11)      NOT NULL, 
  `title`    char(20)     NOT NULL,
  `content`     TEXT NOT NULL ,
  `state`   char(50)      NOT NULL,
  `date` char(10)     NOT NULL,
  PRIMARY KEY (`id`) 
)CHARSET=utf8;

INSERT INTO `announcement` VALUES (1, "重要通知!", "本社区从疫情开始进行封闭管理。每次出入需要进行体温检测，出入需凭借学生证或教师证，无证不允许进入。\n", "重要", "2019-11-1");
```
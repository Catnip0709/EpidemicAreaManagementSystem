数据库名：EpidemicDatabase
# 一、基本信息表
## 1、User表（疫区人员）
+ 18位身份证号码 userID （主键）  
+ 姓名 userName  
+ 登陆密码 password  
+ 手机号 phone    
+ 所住栋号 buildingID  
+ 所住户号 familyID  
+ 当前状态 state
```
CREATE TABLE `User` (
  `userID` char(18) NOT NULL, 
  `userName` varchar(100) NOT NULL,
  `password` varchar(255) NOT NULL,
  `phone` char(11) NOT NULL,
  `buildingID` int NOT NULL, 
  `familyID` int NOT NULL, 
  `state`  int NOT NULL,
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
+ 当日是否与湖北籍人员接触 HuBeiContact  无接触=0，有接触=1  
```
CREATE TABLE `PhysicalCondition` (
  `userID` char(18) NOT NULL,
  `date` char(10) NOT NULL, 
  `todayTemperature` double NOT NULL,
  `HuBeiContact` char(10) NOT NULL,
  PRIMARY KEY (`userID`, `date`) 
);
INSERT INTO `PhysicalCondition` VALUE ("452223199707090000", "2020-03-23", "36.5", "0");
```

## 2、EquipmentStorage 防疫物资存量表
+ 物资id equipmentID（主键）  
+ 物资名称 equipmentName   
+ 剩余存量 storage  
```
CREATE TABLE `EquipmentStorage` (
  `equipmentID` int NOT NULL AUTO_INCREMENT,
  `equipmentName` varchar(255) NOT NULL,
  `storage` int NOT NULL, 
  PRIMARY KEY (`equipmentID`) 
)AUTO_INCREMENT=1 CHARSET=utf8;

INSERT INTO EquipmentStorage (equipmentName,storage) VALUES ("防护手套",100);
```

## 3、ApplyEquipment 物资申请记录表
+ 申请订单ID applyID
+ 申请人身份证 userID
+ 申请人居住栋号 buildingID
+ 申请的物资名称 equipmentName
+ 申请数量 amount  
+ 申请日期 date  
+ 订单当前状态 state （0：已驳回；1：已同意；2：未处理）
```
CREATE TABLE `ApplyEquipment` (
  `applyID` int NOT NULL AUTO_INCREMENT,
  `userID` char(18) NOT NULL,
  `buildingID` INT NOT NULL,
  `equipmentName` char(100) NOT NULL,
  `amount` int NOT NULL, 
  `date` varchar(10) NOT NULL,
  `state` INT NOT NULL,
  PRIMARY KEY (`applyID`) 
)AUTO_INCREMENT=10000 CHARSET=utf8;

INSERT INTO ApplyEquipment (userID,buildingID,equipmentName,amount,date) VALUES ("452223199707090000",2,"手套",3,"2020-05-28",2);
```

## 4、HandleApplication 管理员处理申请记录表  
+ 申请订单ID  applyID 
+ 处理结果    result  
+ 处理人      adminID  
+ 管理员回执  reply  
+ 处理日期    date  
```
CREATE TABLE `HandleApplication` (
  `applyID` int NOT NULL AUTO_INCREMENT,
  `result` INT NOT NULL,
  `adminID` char(18) NOT NULL,
  `reply` TEXT, 
  `date` varchar(10) NOT NULL,
  PRIMARY KEY (`applyID`) 
)CHARSET=utf8;
```

## 5、关系网络管理
？

## 6、IsolationManagement  隔离管理
## 每日特殊情况用户记录  SpecialStateDailyRecord
+ 日期 date （主键）
+ 当日新增的隔离人数 segregation
+ 当日新增的疑似人数 suspected
+ 当日新增的确诊人数 diagnosis
+ 当日新增的死亡人数 die
```
CREATE TABLE `SpecialStateDailyRecord` (
  `date`              char(10)   NOT NULL, 
  `segregation`       INT        NOT NULL,
  `suspected`         INT        NOT NULL,
  `diagnosis`         INT        NOT NULL,
  `die`               INT        NOT NULL,
  `increSegregation`  INT        NOT NULL,
  `increSpected`      INT        NOT NULL,
  `increSuspected`    INT        NOT NULL,
  `increDie`          INT        NOT NULL,
  PRIMARY KEY (`date`) 
)CHARSET=utf8;

INSERT INTO SpecialStateDailyRecord VALUES 
("2020-5-20", 1,1,1,1,0,0,0,0),
("2020-5-21", 2,2,2,2,1,1,1,1),
("2020-5-22", 3,3,3,3,2,2,2,2),
("2020-5-23", 4,4,4,4,5,5,5,5),
("2020-5-24", 5,5,5,5,6,6,6,6),
("2020-5-25", 6,6,6,6,7,7,7,7),
("2020-5-26", 7,7,7,7,8,8,8,8),
("2020-5-27", 8,8,8,8,9,9,9,9); 
```
## 在隔离态的用户的隔离时间表
+ 用户身份证（主键）
+ 开始日期 YYYY-MM-DD
+ 结束日期 YYYY-MM-DD
```
CREATE TABLE `Quarantine` (
  `userID` char(18) NOT NULL, 
  `beginDate` Date,
  `endDate` Date,
  PRIMARY KEY (`userID`) 
) CHARSET=utf8;
```

# 三、公告栏
## 公告表 Announcement
+ 公告id id （主键）  
+ 公告标题 title  
+ 公告内容 content  
+ 公告级别 state  
+ 公告发布日期 date  
```
CREATE TABLE `Announcement` (
  `id`      int(11)      NOT NULL AUTO_INCREMENT, 
  `title`    char(20)     NOT NULL,
  `content`     TEXT NOT NULL ,
  `state`   char(50)      NOT NULL,
  `date` char(10)     NOT NULL,
  PRIMARY KEY (`id`) 
)AUTO_INCREMENT=1 CHARSET=utf8;

INSERT INTO `Announcement` (title, content, state, date) VALUES ("重要通知!", "本社区从疫情开始进行封闭管理。每次出入需要进行体温检测，出入需凭借学生证或教师证，无证不允许进入。\n", "重要", "2019-11-1");
```
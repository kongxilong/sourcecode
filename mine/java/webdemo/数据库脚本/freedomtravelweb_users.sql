CREATE DATABASE  IF NOT EXISTS `freedomtravelweb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `freedomtravelweb`;
-- MySQL dump 10.13  Distrib 5.6.13, for Win32 (x86)
--
-- Host: localhost    Database: freedomtravelweb
-- ------------------------------------------------------
-- Server version	5.6.16

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `userid` int(11) NOT NULL,
  `username` varchar(45) DEFAULT NULL,
  `password` varchar(45) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `membershipstyle` varchar(20) DEFAULT NULL,
  `realname` varchar(45) DEFAULT NULL,
  `sex` varchar(10) DEFAULT NULL,
  `postion` varchar(100) DEFAULT NULL,
  `companyname` varchar(200) DEFAULT NULL,
  `localcity` varchar(20) DEFAULT NULL,
  `mainbussiness` varchar(200) DEFAULT NULL,
  `fixedphonenum` varchar(20) DEFAULT NULL,
  `address` varchar(200) DEFAULT NULL,
  `fax` varchar(20) DEFAULT NULL,
  `cellphone` varchar(20) DEFAULT NULL,
  `iconDir` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (2,'wq','123456','wuwuwuwuw@qq.com','普通会员','吴倩','女','市场部经理','环球遨游有限责任公司','委屈委屈','委屈委屈','8308864','东南大学软件学院学霸实验室','','12345678901','imageUpload/12700000000120140326191827789059.jpg'),(5,'kxl','123456','446263615@qq.com','普通会员','孔喜','男','码农','逍遥游','苏州','无','8308864','东南大学软件学院','','15621291026','imageUpload/01000604512720140327163254497811.jpg'),(6,'stravel','123456','stravel@163.com','普通会员','苏珊','女','市场部经理','苏州顺易旅游公司','江苏','国内旅游','88888888','苏州临泉路330路11栋1100石',NULL,'13325830192',NULL),(7,'zhuqianjing','123456','stravel@163.com','VIP','朱朱','女','总经理','东大','shanghai','旅游','88888888','苏州',NULL,'1111','imageUpload/12700000000120140326105719294279.jpg'),(9,'yuhaohao','111111','yuhaohao@163.com','普通会员','于浩','女','导游','豪大大环球旅游公司','shanghai','国内国际旅游','26537531','上海豪大大厦1111室',NULL,'13268543853',NULL),(11,'hqiuqiu','111111','hqiuqiu@163.com','普通会员','华秋秋','女','副总经理','中国国际旅行社','江苏','国内国际旅游','56474632','南京紫峰大厦',NULL,'13268543853','imageUpload/01000604503320140327142216199035.gif'),(12,'zhaohao','111111','zhaohao@163.com','普通会员','赵浩浩','女','副总经理','中国顺利旅行社','江苏','国内国际旅游','43645321','南京廊坊阿迪大厦',NULL,'13268543853',NULL),(13,'qimeng','123456','1137411250@qq.com','普通会员','齐萌','女','','日月草旅行社','shanghai','旅游','','苏州',NULL,'18860926080',NULL);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-03-28 10:58:48

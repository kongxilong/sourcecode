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
-- Table structure for table `deal`
--

DROP TABLE IF EXISTS `deal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `deal` (
  `dealId` int(11) NOT NULL,
  `purchaserName` varchar(45) DEFAULT NULL,
  `sellerName` varchar(45) DEFAULT NULL,
  `routeId` int(11) DEFAULT NULL,
  `dealNum` int(11) DEFAULT NULL,
  `totalMktPrice` double DEFAULT NULL,
  `totalDsctPrice` double DEFAULT NULL,
  `message` varchar(255) DEFAULT NULL,
  `dealState` int(11) DEFAULT NULL,
  `tradeTime` varchar(45) DEFAULT NULL,
  `dealOver` int(11) DEFAULT NULL,
  `travelStartDate` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`dealId`),
  KEY `routeId_idx` (`routeId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `deal`
--

LOCK TABLES `deal` WRITE;
/*!40000 ALTER TABLE `deal` DISABLE KEYS */;
INSERT INTO `deal` VALUES (34,'kxl',NULL,5,0,0,0,NULL,0,NULL,0,'2014-3-16'),(35,'kxl','kxl',10,1,3700,0,NULL,0,'00:02:38',0,'2014-3-16'),(36,'kxl',NULL,11,0,0,0,NULL,0,NULL,0,'2014-3-16'),(43,'kxl',NULL,9,0,0,0,NULL,0,NULL,0,'2014-3-16'),(44,'kxl',NULL,10,0,0,0,NULL,0,NULL,0,'2014-3-16'),(45,'kxl',NULL,9,0,0,0,NULL,0,NULL,0,'2014-3-16'),(47,'kxl',NULL,6,0,0,0,NULL,0,NULL,0,'2014-3-16'),(48,'kxl',NULL,8,0,0,0,NULL,0,NULL,0,'2014-3-16'),(52,'kxl',NULL,6,0,0,0,NULL,0,NULL,0,'2014-3-16'),(81,'qimeng','qimeng',28,5,2500,0,NULL,1,'2014-03-26 15:28:22.282',1,'2014-3-2'),(83,'qimeng','qimeng',24,1,300,0,NULL,1,'2014-03-26 16:35:20.370',1,'2014-3-27'),(93,'wq','kxl',20,2,0,0,NULL,0,NULL,1,'2014-3-13'),(94,'zhuqianjing','wq',43,3,1500,0,NULL,1,'2014-03-27 14:59:30.250',1,'2014-3-29'),(95,'kxl','zhuqianjing',31,1,3140,0,NULL,1,'2014-03-27 15:07:25.585',1,'2014-3-13'),(96,'kxl','qimeng',33,1,2000,0,NULL,1,'2014-03-27 15:10:11.958',0,'2014-3-13'),(97,'zhuqianjing','wq',43,2,1000,0,NULL,1,'2014-03-27 15:12:03.389',1,'2014-3-29'),(98,'zhuqianjing','wq',43,2,1000,0,NULL,1,'2014-03-27 15:28:21.763',1,'2014-3-28'),(100,'zhuqianjing','wq',43,1,500,0,NULL,1,'2014-03-27 15:30:12.398',0,'2014-3-30'),(101,'kxl','zhuqianjing',31,10,31400,0,NULL,1,'2014-03-27 16:30:54.800',1,'2014-3-11'),(102,'wq','qimeng',33,1,0,0,NULL,0,NULL,0,'2014-4-26'),(103,'zhuqianjing','kxl',45,1,1200,0,NULL,1,'2014-03-28 09:42:09.027',0,'2014-3-28'),(104,'kxl','zhuqianjing',31,1,0,0,NULL,0,NULL,0,'2014-3-29'),(105,'kxl','wq',18,1,0,0,NULL,0,NULL,0,'2014-3-13'),(106,'kxl','zhuqianjing',32,1,0,0,NULL,0,NULL,0,'2014-3-27');
/*!40000 ALTER TABLE `deal` ENABLE KEYS */;
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

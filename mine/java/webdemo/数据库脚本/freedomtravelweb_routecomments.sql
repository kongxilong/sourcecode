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
-- Table structure for table `routecomments`
--

DROP TABLE IF EXISTS `routecomments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `routecomments` (
  `commentId` int(11) NOT NULL,
  `comment` varchar(768) DEFAULT NULL,
  `buyer` varchar(45) DEFAULT NULL,
  `rId` int(11) DEFAULT NULL,
  `commentTime` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`commentId`),
  KEY `routeId_idx` (`rId`),
  CONSTRAINT `rId` FOREIGN KEY (`rId`) REFERENCES `route` (`routeId`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `routecomments`
--

LOCK TABLES `routecomments` WRITE;
/*!40000 ALTER TABLE `routecomments` DISABLE KEYS */;
INSERT INTO `routecomments` VALUES (2,'很好的一次旅行很值得','w*******',20,'2014-03-23 23:32:01.892'),(4,'很好很强大','wq',20,'2014-03-25 09:20:03.383'),(5,'你都没有去过，真的超好玩哟','z*******',21,'2014-03-26 10:53:49.076'),(6,'很不错，就这么定了','zhuqianjing',21,'2014-03-26 10:55:17.211'),(7,'好好玩的一次旅游啊，很满意！','wq',23,'2014-03-26 11:37:05.277'),(8,'线路很好，风景很好','zhuqianjing',29,'2014-03-26 22:23:33.629'),(9,'风景如画，玩得很开心','z*******',43,'2014-03-26 22:23:58.328'),(10,'真的很好，很不错哦','z*******',43,'2014-03-26 22:41:06.783'),(11,'很好','z*******',29,'2014-03-26 23:22:10.318'),(12,'很好，非常好的！','z*******',26,'2014-03-26 23:35:22.736'),(13,'好好玩的一次旅游啊！','kxl',43,'2014-03-26 23:42:47.253'),(14,'不错，不错，真的不错。','z*******',43,'2014-03-26 23:50:18.591'),(15,'asderty','k*******',31,'2014-03-27 13:05:10.580'),(16,'非常好玩','k*******',31,'2014-03-27 15:48:53.927'),(17,'safdghjsdfgh','k*******',31,'2014-03-27 16:32:00.953'),(18,'ASDFGQWEDRFT','kxl',31,'2014-03-27 16:32:20.037'),(19,'很好很不错哦！','zhuqianjing',31,'2014-03-28 09:42:45.237'),(20,'不错哈，O(∩_∩)O哈哈~','z*******',43,'2014-03-28 09:43:05.556');
/*!40000 ALTER TABLE `routecomments` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-03-28 10:58:47

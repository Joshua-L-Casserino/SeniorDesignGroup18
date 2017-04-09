-- MySQL dump 10.13  Distrib 5.7.17, for Win64 (x86_64)
--
-- Host: us-cdbr-azure-east-c.cloudapp.net    Database: homehydroponics
-- ------------------------------------------------------
-- Server version	5.5.45-log

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
-- Table structure for table `equipmentid`
--

DROP TABLE IF EXISTS `equipmentid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `equipmentid` (
  `userName` varchar(16) COLLATE utf8_unicode_ci DEFAULT NULL,
  `equipmentID` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `currentTDS` int(11) NOT NULL,
  `currentPH` decimal(3,1) NOT NULL,
  `currentLUX` int(11) NOT NULL,
  `currentWaterLevel` int(11) NOT NULL,
  `settingTdsHigh` int(11) DEFAULT NULL,
  `settingTdsLow` int(11) DEFAULT NULL,
  `settingPhHigh` decimal(3,1) DEFAULT NULL,
  `settingPhLow` decimal(3,1) DEFAULT NULL,
  `counterTDS` int(11) DEFAULT '300',
  `counterPHup` int(11) DEFAULT '300',
  `counterPHdown` int(11) DEFAULT '300',
  `counterFlowering` int(11) DEFAULT '300',
  `lightOnTime` time DEFAULT NULL,
  `lightOffTime` time DEFAULT NULL,
  `led` tinyint(1) NOT NULL DEFAULT '1',
  `flowering` tinyint(1) NOT NULL DEFAULT '0',
  `currentTimestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `nickname` varchar(16) COLLATE utf8_unicode_ci DEFAULT NULL,
  `datePlanted` text COLLATE utf8_unicode_ci,
  `plants` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`equipmentID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `equipmentid`
--

LOCK TABLES `equipmentid` WRITE;
/*!40000 ALTER TABLE `equipmentid` DISABLE KEYS */;
INSERT INTO `equipmentid` VALUES ('Alex','0001',757,6.2,0,95,NULL,NULL,NULL,NULL,300,300,300,300,NULL,NULL,1,0,'2017-04-03 18:59:29',NULL,NULL,NULL),('Beau','0002',757,6.2,0,95,NULL,NULL,NULL,NULL,300,300,300,300,NULL,NULL,1,0,'2017-04-01 23:19:21',NULL,NULL,NULL),('Rich','0003',757,6.2,0,95,NULL,NULL,NULL,NULL,300,300,300,300,NULL,NULL,1,0,'2017-04-01 23:20:24',NULL,NULL,NULL),('Josh','0004',757,6.2,0,95,NULL,NULL,NULL,NULL,300,300,300,300,NULL,NULL,1,0,'2017-04-08 16:48:59',NULL,NULL,NULL),('Josh','1234',500,6.5,15000,95,800,600,7.5,5.0,300,300,300,300,'00:00:00','16:00:00',1,0,'2017-04-09 01:08:01','Test','Test','Test');
/*!40000 ALTER TABLE `equipmentid` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-04-08 21:14:40

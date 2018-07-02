-- MySQL dump 10.14  Distrib 5.5.52-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: aliszt1
-- ------------------------------------------------------
-- Server version	5.5.52-MariaDB

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
-- Table structure for table `check_vote`
--

DROP TABLE IF EXISTS `check_vote`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `check_vote` (
  `username` varchar(15) NOT NULL,
  `user_candidate` varchar(15) NOT NULL,
  `voted_for` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`username`,`user_candidate`),
  KEY `user_candidate_constraint` (`user_candidate`),
  CONSTRAINT `username_constraint` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `user_candidate_constraint` FOREIGN KEY (`user_candidate`) REFERENCES `vote` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `check_vote`
--

LOCK TABLES `check_vote` WRITE;
/*!40000 ALTER TABLE `check_vote` DISABLE KEYS */;
INSERT INTO `check_vote` VALUES ('acepav','acepav',1),('acepav','durst',1),('acepav','Immor',1),('BC','acepav',1),('BC','durst',1),('BC','Immor',1),('durst','acepav',1),('durst','durst',1),('durst','Immor',1),('HMP','acepav',1),('HMP','durst',1),('HMP','Immor',1),('Immor','acepav',1),('Immor','durst',1),('Immor','Immor',1),('IvanM','acepav',1),('IvanM','durst',1),('Sum','acepav',1),('Sum','durst',1),('synt','acepav',1),('synt','durst',1),('TLtt','acepav',1),('Wit','acepav',1);
/*!40000 ALTER TABLE `check_vote` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `username` varchar(15) NOT NULL DEFAULT '',
  `name` varchar(15) DEFAULT NULL,
  `password` varchar(15) DEFAULT NULL,
  `email` varchar(25) DEFAULT NULL,
  PRIMARY KEY (`username`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Contains all users in the database';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES ('acepav','Aaron','abc','aaliszt@gmail.com'),('admin','admin',' ',NULL),('BC','Barry','unlock','bcad@kent.edu'),('durst','Adam','123','afing@hotmail.com'),('HMP','Hassan','Hello','ttv@yahoo.com'),('Immor','Nick','456','nsolt@gmail.com'),('IvanM','Issac','Simple','timet@gmail.com'),('Sum','Chance','AWP','mount@yahoo.com'),('synt','Eric','def','esman@yahoo.com'),('TLtt','Tim','World','altim@gmail.com'),('Wit','Betty','open','wix@yahoo.com');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`aliszt1`@`localhost`*/ /*!50003 TRIGGER `add_username_to_usertype` AFTER INSERT ON `user` FOR EACH ROW INSERT INTO user_type VALUES (NEW.username, 'member') */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `user_type`
--

DROP TABLE IF EXISTS `user_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_type` (
  `username` varchar(15) NOT NULL DEFAULT '',
  `usertype` varchar(15) NOT NULL DEFAULT 'member',
  PRIMARY KEY (`username`,`usertype`),
  KEY `user_type_constraint` (`usertype`),
  CONSTRAINT `user_type_constraint` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_type`
--

LOCK TABLES `user_type` WRITE;
/*!40000 ALTER TABLE `user_type` DISABLE KEYS */;
INSERT INTO `user_type` VALUES ('acepav','candidate'),('admin','administrator'),('BC','member'),('durst','candidate'),('HMP','member'),('Immor','candidate'),('IvanM','member'),('Sum','member'),('synt','member'),('TLtt','member'),('Wit','member');
/*!40000 ALTER TABLE `user_type` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_unicode_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`aliszt1`@`localhost`*/ /*!50003 TRIGGER `update_vote_usertype` BEFORE UPDATE ON `user_type` FOR EACH ROW BEGIN
	UPDATE vote
	SET usertype = NEW.usertype
	WHERE vote.username = NEW.username;
    DELETE FROM vote WHERE usertype = 'member';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `vote`
--

DROP TABLE IF EXISTS `vote`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vote` (
  `username` varchar(15) NOT NULL,
  `num_votes` int(4) NOT NULL DEFAULT '0',
  `usertype` varchar(15) NOT NULL DEFAULT 'candidate',
  PRIMARY KEY (`username`,`num_votes`),
  CONSTRAINT `vote_constraint` FOREIGN KEY (`username`) REFERENCES `user` (`username`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vote`
--

LOCK TABLES `vote` WRITE;
/*!40000 ALTER TABLE `vote` DISABLE KEYS */;
INSERT INTO `vote` VALUES ('acepav',10,'candidate'),('durst',8,'candidate'),('Immor',5,'candidate');
/*!40000 ALTER TABLE `vote` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-03-18 23:42:02

-- This script is for initialization of the VISHNU MYSQL database
-- Script name          : mysql_create.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 07/06/11
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : release of FMS and IMS modules (VISHNU v1.2.0)

-- Revision nb          : 1.3
-- Revision date        : 02/02/12
-- Revision author      : Kevin Coulomb <kevin.coulomb@sysfera.com>
-- Revision comment     : Add auth System

-- Revision nb          : 1.4
-- Revision date        : 12/07/12
-- Revision author      : Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
-- Revision comment     : Adds support for advanced IO and merging with the WebBoard database


USE vishnu;

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
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `numaccountid` bigint(20) NOT NULL AUTO_INCREMENT,
  `aclogin` varchar(255) DEFAULT NULL,
  `home` varchar(255) DEFAULT NULL,
  `machine_nummachineid` bigint(20) NOT NULL,
  `sshpathkey` varchar(255) DEFAULT NULL,
  `users_numuserid` bigint(20) NOT NULL,
  PRIMARY KEY (`numaccountid`),
  KEY `FKB9D38A2D1CFEDEFC` (`machine_nummachineid`),
  KEY `FKB9D38A2DA63719F2` (`users_numuserid`),
  CONSTRAINT `FKB9D38A2D1CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FKB9D38A2DA63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1045 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_class`
--

DROP TABLE IF EXISTS `acl_class`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_class` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `class` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_entry`
--

DROP TABLE IF EXISTS `acl_entry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_entry` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ace_order` int(11) NOT NULL,
  `acl_object_identity` bigint(20) NOT NULL,
  `audit_failure` bit(1) NOT NULL,
  `audit_success` bit(1) NOT NULL,
  `granting` bit(1) NOT NULL,
  `mask` int(11) NOT NULL,
  `sid` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `acl_object_identity` (`acl_object_identity`,`ace_order`),
  KEY `FK5302D47D8FDB88D5` (`sid`),
  KEY `FK5302D47DB0D9DC4D` (`acl_object_identity`),
  CONSTRAINT `FK5302D47D8FDB88D5` FOREIGN KEY (`sid`) REFERENCES `acl_sid` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK5302D47DB0D9DC4D` FOREIGN KEY (`acl_object_identity`) REFERENCES `acl_object_identity` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_object_identity`
--

DROP TABLE IF EXISTS `acl_object_identity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_object_identity` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `object_id_class` bigint(20) NOT NULL,
  `entries_inheriting` bit(1) NOT NULL,
  `object_id_identity` bigint(20) NOT NULL,
  `owner_sid` bigint(20) DEFAULT NULL,
  `parent_object` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `object_id_class` (`object_id_class`,`object_id_identity`),
  KEY `FK2A2BB009A50290B8` (`parent_object`),
  KEY `FK2A2BB00970422CC5` (`object_id_class`),
  KEY `FK2A2BB00990EC1949` (`owner_sid`),
  CONSTRAINT `FK2A2BB00970422CC5` FOREIGN KEY (`object_id_class`) REFERENCES `acl_class` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2A2BB00990EC1949` FOREIGN KEY (`owner_sid`) REFERENCES `acl_sid` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2A2BB009A50290B8` FOREIGN KEY (`parent_object`) REFERENCES `acl_object_identity` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_sid`
--

DROP TABLE IF EXISTS `acl_sid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_sid` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `principal` bit(1) NOT NULL,
  `sid` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `sid` (`sid`,`principal`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `application`
--

DROP TABLE IF EXISTS `application`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `deleted` bit(1) NOT NULL,
  `description` longtext,
  `identifier` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `class` varchar(255) NOT NULL,
  `author_id` bigint(20) DEFAULT NULL,
  `project_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `identifier` (`identifier`),
  KEY `FK5CA40550BD4995A3` (`project_id`),
  KEY `FK5CA40550E1F3BA8C` (`author_id`),
  CONSTRAINT `FK5CA40550BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK5CA40550E1F3BA8C` FOREIGN KEY (`author_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `application_parameter`
--

DROP TABLE IF EXISTS `application_parameter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application_parameter` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `application_id` bigint(20) NOT NULL,
  `default_value` varchar(255) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `editable` bit(1) NOT NULL,
  `format` varchar(255) NOT NULL,
  `hidden` bit(1) NOT NULL,
  `label` varchar(255) NOT NULL,
  `max_len` bigint(20) NOT NULL,
  `min_len` bigint(20) NOT NULL,
  `name` varchar(255) NOT NULL,
  `optionnal` bit(1) NOT NULL,
  `pos` bigint(20) NOT NULL,
  `possible_values` varchar(255) DEFAULT NULL,
  `regex` varchar(255) DEFAULT NULL,
  `type` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FKFA594F7A3462EA58` (`application_id`),
  CONSTRAINT `FKFA594F7A3462EA58` FOREIGN KEY (`application_id`) REFERENCES `application_version` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `application_version`
--

DROP TABLE IF EXISTS `application_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application_version` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `application_id` bigint(20) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `pre_script` varchar(255) DEFAULT NULL,
  `pre_script_path` varchar(255) DEFAULT NULL,
  `script` varchar(255) NOT NULL,
  `script_path` varchar(255) NOT NULL,
  `validation_script` varchar(255) DEFAULT NULL,
  `validation_script_path` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FKC5B678E99FCAFF4` (`application_id`),
  CONSTRAINT `FKC5B678E99FCAFF4` FOREIGN KEY (`application_id`) REFERENCES `application` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `authaccount`
--

DROP TABLE IF EXISTS `authaccount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `authaccount` (
  `authaccountid` bigint(20) NOT NULL AUTO_INCREMENT,
  `aclogin` varchar(255) DEFAULT NULL,
  `authsystem_authsystemid` bigint(20) NOT NULL,
  `users_numuserid` bigint(20) NOT NULL,
  PRIMARY KEY (`authaccountid`),
  KEY `FK2C887F85A63719F2` (`users_numuserid`),
  KEY `FK2C887F85FC5A9563` (`authsystem_authsystemid`),
  CONSTRAINT `FK2C887F85A63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK2C887F85FC5A9563` FOREIGN KEY (`authsystem_authsystemid`) REFERENCES `authsystem` (`numauthsystemid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `authsystem`
--

DROP TABLE IF EXISTS `authsystem`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `authsystem` (
  `numauthsystemid` bigint(20) NOT NULL AUTO_INCREMENT,
  `authlogin` varchar(255) DEFAULT NULL,
  `authpassword` varchar(255) DEFAULT NULL,
  `authsystemid` varchar(255) DEFAULT NULL,
  `authtype` int(11) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  `uri` varchar(255) DEFAULT NULL,
  `userpwdencryption` int(11) DEFAULT NULL,
  `vishnu_vishnuid` bigint(20) NOT NULL,
  PRIMARY KEY (`numauthsystemid`),
  KEY `FKADC771D7C2584CA8` (`vishnu_vishnuid`),
  CONSTRAINT `FKADC771D7C2584CA8` FOREIGN KEY (`vishnu_vishnuid`) REFERENCES `vishnu` (`vishnuid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clmachine`
--

DROP TABLE IF EXISTS `clmachine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clmachine` (
  `numclmachineid` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  `sshkey` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`numclmachineid`)
) ENGINE=InnoDB AUTO_INCREMENT=282 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `command`
--

DROP TABLE IF EXISTS `command`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `command` (
  `numcommandid` bigint(20) NOT NULL AUTO_INCREMENT,
  `ctype` int(11) DEFAULT NULL,
  `description` TEXT,
  `endtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `starttime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `status` int(11) DEFAULT NULL,
  `vishnuobjectid` varchar(255) DEFAULT NULL,
  `vsession_numsessionid` bigint(20) NOT NULL,
  PRIMARY KEY (`numcommandid`),
  KEY `FK38A5DF4BF58538BC` (`vsession_numsessionid`),
  CONSTRAINT `FK38A5DF4BF58538BC` FOREIGN KEY (`vsession_numsessionid`) REFERENCES `vsession` (`numsessionid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2450 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `description`
--

DROP TABLE IF EXISTS `description`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `description` (
  `numdescriptionid` bigint(20) NOT NULL AUTO_INCREMENT,
  `description` varchar(255) DEFAULT NULL,
  `lang` varchar(255) DEFAULT NULL,
  `machine_nummachineid` bigint(20) NOT NULL,
  PRIMARY KEY (`numdescriptionid`),
  KEY `FK993583FC1CFEDEFC` (`machine_nummachineid`),
  CONSTRAINT `FK993583FC1CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=465 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `filetransfer`
--

DROP TABLE IF EXISTS `filetransfer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `filetransfer` (
  `numfiletransferid` bigint(20) NOT NULL AUTO_INCREMENT,
  `clientmachineid` varchar(255) DEFAULT NULL,
  `destinationfilepath` varchar(255) DEFAULT NULL,
  `destinationmachineid` varchar(255) DEFAULT NULL,
  `errormsg` TEXT,
  `filesize` int(11) DEFAULT NULL,
  `processid` int(11) DEFAULT NULL,
  `sourcefilepath` varchar(255) DEFAULT NULL,
  `sourcemachineid` varchar(255) DEFAULT NULL,
  `starttime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `status` int(11) DEFAULT NULL,
  `transferid` varchar(255) DEFAULT NULL,
  `trcommand` int(11) DEFAULT NULL,
  `userid` varchar(255) DEFAULT NULL,
  `vsession_numsessionid` bigint(20) NOT NULL,
  PRIMARY KEY (`numfiletransferid`),
  KEY `FKFCE97167F58538BC` (`vsession_numsessionid`),
  CONSTRAINT `FKFCE97167F58538BC` FOREIGN KEY (`vsession_numsessionid`) REFERENCES `vsession` (`numsessionid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `global_project_role`
--

DROP TABLE IF EXISTS `global_project_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `global_project_role` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  `is_built_in` bit(1) NOT NULL,
  `is_default` bit(1) NOT NULL,
  `label` varchar(255) NOT NULL,
  `member_specific` bit(1) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `global_project_role_permissions`
--

DROP TABLE IF EXISTS `global_project_role_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `global_project_role_permissions` (
  `global_project_role_id` bigint(20) DEFAULT NULL,
  `permissions_string` varchar(255) DEFAULT NULL,
  KEY `FK74E3BBD8D89FC97` (`global_project_role_id`),
  CONSTRAINT `FK74E3BBD8D89FC97` FOREIGN KEY (`global_project_role_id`) REFERENCES `global_project_role` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `job`
--

DROP TABLE IF EXISTS `job`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `job` (
  `numjobid` bigint(20) NOT NULL AUTO_INCREMENT,
  `batchjobid` varchar(255) DEFAULT NULL,
  `batchtype` int(11) DEFAULT NULL,
  `enddate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `errorpath` varchar(255) DEFAULT NULL,
  `groupname` varchar(255) DEFAULT NULL,
  `job_owner_id` bigint(20) DEFAULT NULL,
  `jobdescription` varchar(255) DEFAULT NULL,
  `jobid` varchar(255) DEFAULT NULL,
  `jobname` varchar(255) DEFAULT NULL,
  `jobpath` varchar(255) DEFAULT NULL,
  `jobprio` int(11) DEFAULT NULL,
  `jobqueue` varchar(255) DEFAULT NULL,
  `jobworkingdir` varchar(255) DEFAULT NULL,
  `machine_id` bigint(20) DEFAULT NULL,
  `memlimit` int(11) DEFAULT NULL,
  `nbcpus` int(11) DEFAULT NULL,
  `nbnodes` int(11) DEFAULT NULL,
  `nbnodesandcpupernode` varchar(255) DEFAULT NULL,
  `outputdir` varchar(255) DEFAULT NULL,
  `outputpath` varchar(255) DEFAULT NULL,
  `owner` varchar(255) DEFAULT NULL,
  `scriptcontent` TEXT,
  `status` int(11) DEFAULT NULL,
  `submitdate` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `submitmachineid` varchar(255) DEFAULT NULL,
  `submitmachinename` varchar(255) DEFAULT NULL,
  `vsession_numsessionid` bigint(20) NOT NULL,
  `wallclocklimit` int(11) DEFAULT NULL,
  `workId` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`numjobid`),
  KEY `FK19BBDF381DC90` (`workId`),
  KEY `FK19BBDF58538BC` (`vsession_numsessionid`),
  KEY `FK19BBD9207FB3B` (`machine_id`),
  KEY `FK19BBD355BF2A6` (`job_owner_id`),
  CONSTRAINT `FK19BBD355BF2A6` FOREIGN KEY (`job_owner_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK19BBD9207FB3B` FOREIGN KEY (`machine_id`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FK19BBDF381DC90` FOREIGN KEY (`workId`) REFERENCES `work` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK19BBDF58538BC` FOREIGN KEY (`vsession_numsessionid`) REFERENCES `vsession` (`numsessionid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=222 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ldapauthsystem`
--

DROP TABLE IF EXISTS `ldapauthsystem`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ldapauthsystem` (
  `ldapauthsystid` bigint(20) NOT NULL AUTO_INCREMENT,
  `authsystem_authsystemid` bigint(20) NOT NULL,
  `ldapbase` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ldapauthsystid`),
  KEY `FK30E4E8BEFC5A9563` (`authsystem_authsystemid`),
  CONSTRAINT `FK30E4E8BEFC5A9563` FOREIGN KEY (`authsystem_authsystemid`) REFERENCES `authsystem` (`numauthsystemid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `machine`
--

DROP TABLE IF EXISTS `machine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `machine` (
  `nummachineid` bigint(20) NOT NULL AUTO_INCREMENT,
  `diskspace` int(11) DEFAULT NULL,
  `machineid` varchar(255) DEFAULT NULL,
  `memory` int(11) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `network` int(11) DEFAULT NULL,
  `site` varchar(255) DEFAULT NULL,
  `sshpublickey` TEXT,
  `status` int(11) DEFAULT NULL,
  `vishnu_vishnuid` bigint(20) NOT NULL,
  PRIMARY KEY (`nummachineid`),
  KEY `FK31314447C2584CA8` (`vishnu_vishnuid`),
  CONSTRAINT `FK31314447C2584CA8` FOREIGN KEY (`vishnu_vishnuid`) REFERENCES `vishnu` (`vishnuid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=467 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `notification`
--

DROP TABLE IF EXISTS `notification`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `notification` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `clazz` varchar(255) NOT NULL,
  `content` varchar(255) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `link_to` varchar(255) NOT NULL,
  `notifier_id` bigint(20) DEFAULT NULL,
  `object_id` int(11) NOT NULL,
  `project_id` bigint(20) NOT NULL,
  `type` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK237A88EB7276891` (`notifier_id`),
  KEY `FK237A88EBBD4995A3` (`project_id`),
  CONSTRAINT `FK237A88EB7276891` FOREIGN KEY (`notifier_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK237A88EBBD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `optionu`
--

DROP TABLE IF EXISTS `optionu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `optionu` (
  `numoptionid` bigint(20) NOT NULL AUTO_INCREMENT,
  `defaultvalue` int(11) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `optionid` int(11) DEFAULT NULL,
  PRIMARY KEY (`numoptionid`)
) ENGINE=InnoDB AUTO_INCREMENT=501 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `optionvalue`
--

DROP TABLE IF EXISTS `optionvalue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `optionvalue` (
  `numoptionvalueid` bigint(20) NOT NULL AUTO_INCREMENT,
  `optionu_numoptionid` bigint(20) NOT NULL,
  `users_numuserid` bigint(20) NOT NULL,
  `value` int(11) DEFAULT NULL,
  PRIMARY KEY (`numoptionvalueid`),
  KEY `FKEBDE0A1CBE23B1A5` (`optionu_numoptionid`),
  KEY `FKEBDE0A1CA63719F2` (`users_numuserid`),
  CONSTRAINT `FKEBDE0A1CA63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FKEBDE0A1CBE23B1A5` FOREIGN KEY (`optionu_numoptionid`) REFERENCES `optionu` (`numoptionid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `parameter_value`
--

DROP TABLE IF EXISTS `parameter_value`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parameter_value` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `parent_id` bigint(20) NOT NULL,
  `type_id` bigint(20) NOT NULL,
  `value` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK2E32855B3CCF497F` (`type_id`),
  KEY `FK2E32855B9D863974` (`parent_id`),
  CONSTRAINT `FK2E32855B3CCF497F` FOREIGN KEY (`type_id`) REFERENCES `application_parameter` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2E32855B9D863974` FOREIGN KEY (`parent_id`) REFERENCES `work` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permission`
--

DROP TABLE IF EXISTS `permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permission` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `label` varchar(255) NOT NULL,
  `member_required` bit(1) NOT NULL,
  `module_id` bigint(20) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` bigint(20) NOT NULL,
  `project_required` bit(1) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `FKE125C5CF6E594880` (`module_id`),
  CONSTRAINT `FKE125C5CF6E594880` FOREIGN KEY (`module_id`) REFERENCES `permission_module` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permission_module`
--

DROP TABLE IF EXISTS `permission_module`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permission_module` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `process`
--

DROP TABLE IF EXISTS `process`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `process` (
  `numprocess` bigint(20) NOT NULL AUTO_INCREMENT,
  `dietname` varchar(255) DEFAULT NULL,
  `launchscript` TEXT,
  `machineid` varchar(255) DEFAULT NULL,
  `pstatus` int(11) DEFAULT NULL,
  `uptime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `vishnuname` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`numprocess`)
) ENGINE=InnoDB AUTO_INCREMENT=825 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project`
--

DROP TABLE IF EXISTS `project`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `closed` bit(1) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted` bit(1) NOT NULL,
  `description` varchar(255) NOT NULL,
  `identifier` varchar(255) NOT NULL,
  `is_public` bit(1) NOT NULL,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name` varchar(30) NOT NULL,
  `parent_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `identifier` (`identifier`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_application`
--

DROP TABLE IF EXISTS `project_application`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_application` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `application_id` bigint(20) NOT NULL,
  `project_id` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FKFAD3D12A9FCAFF4` (`application_id`),
  KEY `FKFAD3D12ABD4995A3` (`project_id`),
  CONSTRAINT `FKFAD3D12A9FCAFF4` FOREIGN KEY (`application_id`) REFERENCES `application` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FKFAD3D12ABD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_machine`
--

DROP TABLE IF EXISTS `project_machine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_machine` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `machine_id` bigint(20) NOT NULL,
  `max_allowed_time` bigint(20) NOT NULL,
  `project_id` bigint(20) NOT NULL,
  `spent_time` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FKA2850D21BD4995A3` (`project_id`),
  KEY `FKA2850D219207FB3B` (`machine_id`),
  CONSTRAINT `FKA2850D219207FB3B` FOREIGN KEY (`machine_id`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FKA2850D21BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_member`
--

DROP TABLE IF EXISTS `project_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_member` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `mail_notifications` bit(1) NOT NULL,
  `member_id` bigint(20) NOT NULL,
  `project_id` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK2EC53E80DA00831D` (`member_id`),
  KEY `FK2EC53E80BD4995A3` (`project_id`),
  CONSTRAINT `FK2EC53E80BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2EC53E80DA00831D` FOREIGN KEY (`member_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_member_project_role`
--

DROP TABLE IF EXISTS `project_member_project_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_member_project_role` (
  `project_member_roles_id` bigint(20) DEFAULT NULL,
  `project_role_id` bigint(20) DEFAULT NULL,
  KEY `FK19B3B13BC94BA396` (`project_role_id`),
  KEY `FK19B3B13B9EDD7798` (`project_member_roles_id`),
  CONSTRAINT `FK19B3B13B9EDD7798` FOREIGN KEY (`project_member_roles_id`) REFERENCES `project_member` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK19B3B13BC94BA396` FOREIGN KEY (`project_role_id`) REFERENCES `project_role` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_role`
--

DROP TABLE IF EXISTS `project_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_role` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  `is_built_in` bit(1) NOT NULL,
  `is_default` bit(1) NOT NULL,
  `label` varchar(255) NOT NULL,
  `member_specific` bit(1) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` bigint(20) NOT NULL,
  `class` varchar(255) NOT NULL,
  `project_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK37FFF5DCBD4995A3` (`project_id`),
  CONSTRAINT `FK37FFF5DCBD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_role_permissions`
--

DROP TABLE IF EXISTS `project_role_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_role_permissions` (
  `project_role_id` bigint(20) DEFAULT NULL,
  `permissions_string` varchar(255) DEFAULT NULL,
  KEY `FKADD15EA1C94BA396` (`project_role_id`),
  CONSTRAINT `FKADD15EA1C94BA396` FOREIGN KEY (`project_role_id`) REFERENCES `project_role` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `role`
--

DROP TABLE IF EXISTS `role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `role` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `authority` varchar(255) NOT NULL,
  `description` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `authority` (`authority`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `state`
--

DROP TABLE IF EXISTS `state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `state` (
  `numstateid` bigint(20) NOT NULL AUTO_INCREMENT,
  `cpuload` int(11) DEFAULT NULL,
  `diskspace` int(11) DEFAULT NULL,
  `machine_nummachineid` bigint(20) NOT NULL,
  `memory` int(11) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`numstateid`),
  KEY `FK68AC4911CFEDEFC` (`machine_nummachineid`),
  CONSTRAINT `FK68AC4911CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_report`
--

DROP TABLE IF EXISTS `test_report`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_report` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `version` bigint(20) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `threshold`
--

DROP TABLE IF EXISTS `threshold`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `threshold` (
  `thresholdid` bigint(20) NOT NULL AUTO_INCREMENT,
  `machine_nummachineid` bigint(20) NOT NULL,
  `typet` int(11) DEFAULT NULL,
  `users_numuserid` bigint(20) NOT NULL,
  `value` int(11) DEFAULT NULL,
  PRIMARY KEY (`thresholdid`),
  KEY `FKA3E1E46B1CFEDEFC` (`machine_nummachineid`),
  KEY `FKA3E1E46BA63719F2` (`users_numuserid`),
  CONSTRAINT `FKA3E1E46B1CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FKA3E1E46BA63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_role`
--

DROP TABLE IF EXISTS `user_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_role` (
  `role_id` bigint(20) NOT NULL,
  `user_id` bigint(20) NOT NULL,
  PRIMARY KEY (`role_id`,`user_id`),
  KEY `FK143BF46ADC10046C` (`role_id`),
  KEY `FK143BF46A813AC84C` (`user_id`),
  CONSTRAINT `FK143BF46A813AC84C` FOREIGN KEY (`user_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK143BF46ADC10046C` FOREIGN KEY (`role_id`) REFERENCES `role` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `numuserid` bigint(20) NOT NULL AUTO_INCREMENT,
  `account_expired` bit(1) DEFAULT NULL,
  `account_locked` bit(1) DEFAULT NULL,
  `confirm_code` varchar(255) DEFAULT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `email` varchar(255) DEFAULT NULL,
  `enabled` bit(1) DEFAULT NULL,
  `firstname` varchar(255) DEFAULT NULL,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `lastname` varchar(255) DEFAULT NULL,
  `pwd` varchar(255) NOT NULL,
  `password_expired` bit(1) DEFAULT NULL,
  `passwordstate` int(11) DEFAULT NULL,
  `privilege` int(11) DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  `userid` varchar(255) NOT NULL,
  `vishnu_vishnuid` bigint(20) NOT NULL,
  PRIMARY KEY (`numuserid`),
  KEY `FK6A68E08C2584CA8` (`vishnu_vishnuid`),
  CONSTRAINT `FK6A68E08C2584CA8` FOREIGN KEY (`vishnu_vishnuid`) REFERENCES `vishnu` (`vishnuid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=887 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vishnu`
--

DROP TABLE IF EXISTS `vishnu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vishnu` (
  `vishnuid` bigint(20) NOT NULL AUTO_INCREMENT,
  `formatidauth` varchar(255) DEFAULT NULL,
  `formatidfiletransfer` varchar(255) DEFAULT NULL,
  `formatidjob` varchar(255) DEFAULT NULL,
  `formatidmachine` varchar(255) DEFAULT NULL,
  `formatiduser` varchar(255) DEFAULT NULL,
  `formatidwork` varchar(255) DEFAULT NULL,
  `updatefreq` int(11) DEFAULT NULL,
  PRIMARY KEY (`vishnuid`)
) ENGINE=InnoDB AUTO_INCREMENT=378 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vsession`
--

DROP TABLE IF EXISTS `vsession`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vsession` (
  `numsessionid` bigint(20) NOT NULL AUTO_INCREMENT,
  `authid` varchar(255) DEFAULT NULL,
  `clmachine_numclmachineid` bigint(20) NOT NULL,
  `closepolicy` int(11) DEFAULT NULL,
  `closure` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `creation` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `lastconnect` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `sessionkey` varchar(255) DEFAULT NULL,
  `state` int(11) DEFAULT NULL,
  `timeout` int(11) DEFAULT NULL,
  `users_numuserid` bigint(20) NOT NULL,
  `vsessionid` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`numsessionid`),
  KEY `FK581B3160C401BD40` (`clmachine_numclmachineid`),
  KEY `FK581B3160A63719F2` (`users_numuserid`),
  CONSTRAINT `FK581B3160A63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK581B3160C401BD40` FOREIGN KEY (`clmachine_numclmachineid`) REFERENCES `clmachine` (`numclmachineid`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1129 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `work`
--

DROP TABLE IF EXISTS `work`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `work` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `application_id` bigint(20) DEFAULT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `date_ended` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `date_started` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `description` varchar(255) DEFAULT NULL,
  `done_ratio` bigint(20) NOT NULL,
  `due_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `estimated_hours` double NOT NULL,
  `identifier` varchar(255) NOT NULL,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `machine_id` bigint(20) DEFAULT NULL,
  `nbcpus` int(11) NOT NULL,
  `owner_id` bigint(20) NOT NULL,
  `priority` int(11) NOT NULL,
  `project_id` bigint(20) DEFAULT NULL,
  `start_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `status` int(11) NOT NULL,
  `subject` varchar(255) NOT NULL,
  `submit_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id`),
  KEY `FK37C7113462EA58` (`application_id`),
  KEY `FK37C711BD4995A3` (`project_id`),
  KEY `FK37C7119207FB3B` (`machine_id`),
  KEY `FK37C711ED217864` (`owner_id`),
  CONSTRAINT `FK37C7113462EA58` FOREIGN KEY (`application_id`) REFERENCES `application_version` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK37C7119207FB3B` FOREIGN KEY (`machine_id`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FK37C711BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK37C711ED217864` FOREIGN KEY (`owner_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- Role Creation;

CREATE USER vishnu_user@'%' IDENTIFIED BY 'vishnu_user';
CREATE USER vishnu_db_admin@localhost IDENTIFIED BY 'vishnu_db_admin';

GRANT ALL ON vishnu TO vishnu_user;
GRANT ALL ON vishnu TO vishnu_db_admin;

GRANT SELECT ON vishnu TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_db_admin";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_db_admin";


-- CREATE ROLE vishnu_user;

-- Grant on right on table

GRANT SELECT, INSERT, UPDATE, DELETE ON vishnu TO vishnu_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON users TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON machine TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON clmachine TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionu TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON state TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON description TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON vsession TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON account TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON optionvalue TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON threshold TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON command TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON filetransfer TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON job TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON process TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authaccount TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON authsystem TO "vishnu_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON ldapauthsystem TO "vishnu_user";

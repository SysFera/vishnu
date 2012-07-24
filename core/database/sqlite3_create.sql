-- This script is for initialization of the VISHNU SQLITE3 database
-- Script name          : sqlite3_create.sql
-- Script owner         : SysFera SA

-- REVISIONS
-- Revision nb          : 1.0
-- Revision date        : 24/07/12
-- Revision author      : Jean-Baptiste Ghoul <jean-baptiste.ghoul@sysfera.com>
-- Revision comment     : release of FMS and IMS modules (VISHNU v1.2.0)



--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `numaccountid` integer primary key autoincrement,
  `aclogin` varchar(255) DEFAULT NULL,
  `home` varchar(255) DEFAULT NULL,
  `machine_nummachineid` integer NOT NULL,
  `sshpathkey` varchar(255) DEFAULT NULL,
  `users_numuserid` integer NOT NULL,
  CONSTRAINT `FKB9D38A2D1CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FKB9D38A2DA63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_class`
--

DROP TABLE IF EXISTS `acl_class`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_class` (
  `id` integer primary key autoincrement,
  `class` varchar(255) NOT NULL
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_entry`
--

DROP TABLE IF EXISTS `acl_entry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_entry` (
  `id` integer primary key autoincrement,
  `ace_order` int(11) NOT NULL,
  `acl_object_identity` integer NOT NULL,
  `audit_failure` bit(1) NOT NULL,
  `audit_success` bit(1) NOT NULL,
  `granting` bit(1) NOT NULL,
  `mask` int(11) NOT NULL,
  `sid` integer NOT NULL,
  UNIQUE (`acl_object_identity`,`ace_order`),
  CONSTRAINT `FK5302D47D8FDB88D5` FOREIGN KEY (`sid`) REFERENCES `acl_sid` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK5302D47DB0D9DC4D` FOREIGN KEY (`acl_object_identity`) REFERENCES `acl_object_identity` (`id`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_object_identity`
--

DROP TABLE IF EXISTS `acl_object_identity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_object_identity` (
  `id` integer primary key autoincrement,
  `object_id_class` integer NOT NULL,
  `entries_inheriting` bit(1) NOT NULL,
  `object_id_identity` integer NOT NULL,
  `owner_sid` bigint(20) DEFAULT NULL,
  `parent_object` bigint(20) DEFAULT NULL,
  UNIQUE (`object_id_class`,`object_id_identity`),
  CONSTRAINT `FK2A2BB00970422CC5` FOREIGN KEY (`object_id_class`) REFERENCES `acl_class` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2A2BB00990EC1949` FOREIGN KEY (`owner_sid`) REFERENCES `acl_sid` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2A2BB009A50290B8` FOREIGN KEY (`parent_object`) REFERENCES `acl_object_identity` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `acl_sid`
--

DROP TABLE IF EXISTS `acl_sid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `acl_sid` (
  `id` integer primary key autoincrement,
  `principal` bit(1) NOT NULL,
  `sid` varchar(255) NOT NULL,
  UNIQUE (`sid`,`principal`)
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `application`
--

DROP TABLE IF EXISTS `application`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `deleted` bit(1) NOT NULL,
  `description` longtext,
  `identifier` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `class` varchar(255) NOT NULL,
  `author_id` bigint(20) DEFAULT NULL,
  `project_id` bigint(20) DEFAULT NULL,
  UNIQUE (`identifier`),
  CONSTRAINT `FK5CA40550BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK5CA40550E1F3BA8C` FOREIGN KEY (`author_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `application_parameter`
--

DROP TABLE IF EXISTS `application_parameter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application_parameter` (
  `version` integer NOT NULL,
  `application_id` integer NOT NULL,
  `default_value` varchar(255) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `editable` bit(1) NOT NULL,
  `format` varchar(255) NOT NULL,
  `hidden` bit(1) NOT NULL,
  `label` varchar(255) NOT NULL,
  `max_len` integer NOT NULL,
  `min_len` integer NOT NULL,
  `name` varchar(255) NOT NULL,
  `optionnal` bit(1) NOT NULL,
  `pos` integer NOT NULL,
  `possible_values` varchar(255) DEFAULT NULL,
  `regex` varchar(255) DEFAULT NULL,
  `type` varchar(255) NOT NULL,
  CONSTRAINT `FKFA594F7A3462EA58` FOREIGN KEY (`application_id`) REFERENCES `application_version` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `application_version`
--

DROP TABLE IF EXISTS `application_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `application_version` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `application_id` integer NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `pre_script` varchar(255) DEFAULT NULL,
  `pre_script_path` varchar(255) DEFAULT NULL,
  `script` varchar(255) NOT NULL,
  `script_path` varchar(255) NOT NULL,
  `validation_script` varchar(255) DEFAULT NULL,
  `validation_script_path` varchar(255) DEFAULT NULL,
  CONSTRAINT `FKC5B678E99FCAFF4` FOREIGN KEY (`application_id`) REFERENCES `application` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `authaccount`
--

DROP TABLE IF EXISTS `authaccount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `authaccount` (
  `authaccountid` integer primary key autoincrement,
  `aclogin` varchar(255) DEFAULT NULL,
  `authsystem_authsystemid` integer NOT NULL,
  `users_numuserid` integer NOT NULL,
  CONSTRAINT `FK2C887F85A63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK2C887F85FC5A9563` FOREIGN KEY (`authsystem_authsystemid`) REFERENCES `authsystem` (`numauthsystemid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `authsystem`
--

DROP TABLE IF EXISTS `authsystem`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `authsystem` (
  `numauthsystemid` integer primary key autoincrement,
  `authlogin` varchar(255) DEFAULT NULL,
  `authpassword` varchar(255) DEFAULT NULL,
  `authsystemid` varchar(255) DEFAULT NULL,
  `authtype` int(11) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  `uri` varchar(255) DEFAULT NULL,
  `userpwdencryption` int(11) DEFAULT NULL,
  `vishnu_vishnuid` integer NOT NULL,
  CONSTRAINT `FKADC771D7C2584CA8` FOREIGN KEY (`vishnu_vishnuid`) REFERENCES `vishnu` (`vishnuid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clmachine`
--

DROP TABLE IF EXISTS `clmachine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clmachine` (
  `numclmachineid` integer primary key autoincrement,
  `name` varchar(255) DEFAULT NULL,
  `sshkey` varchar(255) DEFAULT NULL
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `command`
--

DROP TABLE IF EXISTS `command`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `command` (
  `numcommandid` integer primary key autoincrement,
  `ctype` int(11) DEFAULT NULL,
  `description` TEXT,
  `endtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `starttime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `status` int(11) DEFAULT NULL,
  `vishnuobjectid` varchar(255) DEFAULT NULL,
  `vsession_numsessionid` integer NOT NULL,
  CONSTRAINT `FK38A5DF4BF58538BC` FOREIGN KEY (`vsession_numsessionid`) REFERENCES `vsession` (`numsessionid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `description`
--

DROP TABLE IF EXISTS `description`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `description` (
  `numdescriptionid` integer primary key autoincrement,
  `description` varchar(255) DEFAULT NULL,
  `lang` varchar(255) DEFAULT NULL,
  `machine_nummachineid` integer NOT NULL,
  CONSTRAINT `FK993583FC1CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `filetransfer`
--

DROP TABLE IF EXISTS `filetransfer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `filetransfer` (
  `numfiletransferid` integer primary key autoincrement,
  `clientmachineid` varchar(255) DEFAULT NULL,
  `destinationfilepath` varchar(255) DEFAULT NULL,
  `destinationmachineid` varchar(255) DEFAULT NULL,
  `errormsg` TEXT,
  `filesize` int(11) DEFAULT NULL,
  `processid` int(11) DEFAULT NULL,
  `sourcefilepath` varchar(255) DEFAULT NULL,
  `sourcemachineid` varchar(255) DEFAULT NULL,
  `starttime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `status` int(11) DEFAULT NULL,
  `transferid` varchar(255) DEFAULT NULL,
  `trcommand` int(11) DEFAULT NULL,
  `userid` varchar(255) DEFAULT NULL,
  `vsession_numsessionid` integer NOT NULL,
  CONSTRAINT `FKFCE97167F58538BC` FOREIGN KEY (`vsession_numsessionid`) REFERENCES `vsession` (`numsessionid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `global_project_role`
--

DROP TABLE IF EXISTS `global_project_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `global_project_role` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  `is_built_in` bit(1) NOT NULL,
  `is_default` bit(1) NOT NULL,
  `label` varchar(255) NOT NULL,
  `member_specific` bit(1) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` integer NOT NULL
)
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
  CONSTRAINT `FK74E3BBD8D89FC97` FOREIGN KEY (`global_project_role_id`) REFERENCES `global_project_role` (`id`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `job`
--

DROP TABLE IF EXISTS `job`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `job` (
  `numjobid` integer primary key autoincrement,
  `batchjobid` varchar(255) DEFAULT NULL,
  `batchtype` int(11) DEFAULT NULL,
  `enddate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
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
  `vsession_numsessionid` integer NOT NULL,
  `wallclocklimit` int(11) DEFAULT NULL,
  `workId` bigint(20) DEFAULT NULL,
  CONSTRAINT `FK19BBD355BF2A6` FOREIGN KEY (`job_owner_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK19BBD9207FB3B` FOREIGN KEY (`machine_id`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FK19BBDF381DC90` FOREIGN KEY (`workId`) REFERENCES `work` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK19BBDF58538BC` FOREIGN KEY (`vsession_numsessionid`) REFERENCES `vsession` (`numsessionid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ldapauthsystem`
--

DROP TABLE IF EXISTS `ldapauthsystem`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ldapauthsystem` (
  `ldapauthsystid` integer primary key autoincrement,
  `authsystem_authsystemid` integer NOT NULL,
  `ldapbase` varchar(255) DEFAULT NULL,
  CONSTRAINT `FK30E4E8BEFC5A9563` FOREIGN KEY (`authsystem_authsystemid`) REFERENCES `authsystem` (`numauthsystemid`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `machine`
--

DROP TABLE IF EXISTS `machine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `machine` (
  `nummachineid` integer primary key autoincrement,
  `diskspace` int(11) DEFAULT NULL,
  `machineid` varchar(255) DEFAULT NULL,
  `memory` int(11) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `network` int(11) DEFAULT NULL,
  `site` varchar(255) DEFAULT NULL,
  `sshpublickey` TEXT,
  `status` int(11) DEFAULT NULL,
  `vishnu_vishnuid` integer NOT NULL,
  CONSTRAINT `FK31314447C2584CA8` FOREIGN KEY (`vishnu_vishnuid`) REFERENCES `vishnu` (`vishnuid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `notification`
--

DROP TABLE IF EXISTS `notification`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `notification` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `clazz` varchar(255) NOT NULL,
  `content` varchar(255) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `link_to` varchar(255) NOT NULL,
  `notifier_id` bigint(20) DEFAULT NULL,
  `object_id` int(11) NOT NULL,
  `project_id` integer NOT NULL,
  `type` varchar(255) NOT NULL,
  CONSTRAINT `FK237A88EB7276891` FOREIGN KEY (`notifier_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK237A88EBBD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `optionu`
--

DROP TABLE IF EXISTS `optionu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `optionu` (
  `numoptionid` integer primary key autoincrement,
  `defaultvalue` int(11) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `optionid` int(11) DEFAULT NULL
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `optionvalue`
--

DROP TABLE IF EXISTS `optionvalue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `optionvalue` (
  `numoptionvalueid` integer primary key autoincrement,
  `optionu_numoptionid` integer NOT NULL,
  `users_numuserid` integer NOT NULL,
  `value` int(11) DEFAULT NULL,
  CONSTRAINT `FKEBDE0A1CA63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FKEBDE0A1CBE23B1A5` FOREIGN KEY (`optionu_numoptionid`) REFERENCES `optionu` (`numoptionid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `parameter_value`
--

DROP TABLE IF EXISTS `parameter_value`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parameter_value` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `parent_id` integer NOT NULL,
  `type_id` integer NOT NULL,
  `value` varchar(255) NOT NULL,
  CONSTRAINT `FK2E32855B3CCF497F` FOREIGN KEY (`type_id`) REFERENCES `application_parameter` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2E32855B9D863974` FOREIGN KEY (`parent_id`) REFERENCES `work` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permission`
--

DROP TABLE IF EXISTS `permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permission` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `label` varchar(255) NOT NULL,
  `member_required` bit(1) NOT NULL,
  `module_id` integer NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` integer NOT NULL,
  `project_required` bit(1) NOT NULL,
  UNIQUE (`name`),
  CONSTRAINT `FKE125C5CF6E594880` FOREIGN KEY (`module_id`) REFERENCES `permission_module` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permission_module`
--

DROP TABLE IF EXISTS `permission_module`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permission_module` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` integer NOT NULL,
  UNIQUE (`name`)
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `process`
--

DROP TABLE IF EXISTS `process`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `process` (
  `numprocess` integer primary key autoincrement,
  `dietname` varchar(255) DEFAULT NULL,
  `launchscript` TEXT,
  `machineid` varchar(255) DEFAULT NULL,
  `pstatus` int(11) DEFAULT NULL,
  `uptime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `vishnuname` varchar(255) DEFAULT NULL
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project`
--

DROP TABLE IF EXISTS `project`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `closed` bit(1) NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `deleted` bit(1) NOT NULL,
  `description` varchar(255) NOT NULL,
  `identifier` varchar(255) NOT NULL,
  `is_public` bit(1) NOT NULL,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name` varchar(30) NOT NULL,
  `parent_id` int(11) NOT NULL,
  UNIQUE (`identifier`),
  UNIQUE (`name`)
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_application`
--

DROP TABLE IF EXISTS `project_application`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_application` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `application_id` integer NOT NULL,
  `project_id` integer NOT NULL,
  CONSTRAINT `FKFAD3D12A9FCAFF4` FOREIGN KEY (`application_id`) REFERENCES `application` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FKFAD3D12ABD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_machine`
--

DROP TABLE IF EXISTS `project_machine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_machine` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `machine_id` integer NOT NULL,
  `max_allowed_time` integer NOT NULL,
  `project_id` integer NOT NULL,
  `spent_time` integer NOT NULL,
  CONSTRAINT `FKA2850D219207FB3B` FOREIGN KEY (`machine_id`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FKA2850D21BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_member`
--

DROP TABLE IF EXISTS `project_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_member` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `mail_notifications` bit(1) NOT NULL,
  `member_id` integer NOT NULL,
  `project_id` integer NOT NULL,
  CONSTRAINT `FK2EC53E80BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK2EC53E80DA00831D` FOREIGN KEY (`member_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) 
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
  CONSTRAINT `FK19B3B13B9EDD7798` FOREIGN KEY (`project_member_roles_id`) REFERENCES `project_member` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK19B3B13BC94BA396` FOREIGN KEY (`project_role_id`) REFERENCES `project_role` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `project_role`
--

DROP TABLE IF EXISTS `project_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project_role` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  `is_built_in` bit(1) NOT NULL,
  `is_default` bit(1) NOT NULL,
  `label` varchar(255) NOT NULL,
  `member_specific` bit(1) NOT NULL,
  `name` varchar(255) NOT NULL,
  `pos` integer NOT NULL,
  `class` varchar(255) NOT NULL,
  `project_id` bigint(20) DEFAULT NULL,
  CONSTRAINT `FK37FFF5DCBD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) 
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
  CONSTRAINT `FKADD15EA1C94BA396` FOREIGN KEY (`project_role_id`) REFERENCES `project_role` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `role`
--

DROP TABLE IF EXISTS `role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `role` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `authority` varchar(255) NOT NULL,
  `description` varchar(255) NOT NULL,
  UNIQUE (`authority`)
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `state`
--

DROP TABLE IF EXISTS `state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `state` (
  `numstateid` integer primary key autoincrement,
  `cpuload` int(11) DEFAULT NULL,
  `diskspace` int(11) DEFAULT NULL,
  `machine_nummachineid` integer NOT NULL,
  `memory` int(11) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT `FK68AC4911CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_report`
--

DROP TABLE IF EXISTS `test_report`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_report` (
  `id` integer primary key autoincrement,
  `version` integer NOT NULL,
  `name` varchar(255) NOT NULL
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `threshold`
--

DROP TABLE IF EXISTS `threshold`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `threshold` (
  `thresholdid` integer primary key autoincrement,
  `machine_nummachineid` integer NOT NULL,
  `typet` int(11) DEFAULT NULL,
  `users_numuserid` integer NOT NULL,
  `value` int(11) DEFAULT NULL,
  CONSTRAINT `FKA3E1E46B1CFEDEFC` FOREIGN KEY (`machine_nummachineid`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FKA3E1E46BA63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
)
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_role`
--

DROP TABLE IF EXISTS `user_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_role` (
  `role_id` integer NOT NULL,
  `user_id` integer NOT NULL,
  PRIMARY KEY (`role_id`,`user_id`),
  CONSTRAINT `FK143BF46A813AC84C` FOREIGN KEY (`user_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK143BF46ADC10046C` FOREIGN KEY (`role_id`) REFERENCES `role` (`id`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `numuserid` integer primary key autoincrement,
  `account_expired` bit(1) DEFAULT NULL,
  `account_locked` bit(1) DEFAULT NULL,
  `confirm_code` varchar(255) DEFAULT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
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
  `vishnu_vishnuid` integer NOT NULL,
  CONSTRAINT `FK6A68E08C2584CA8` FOREIGN KEY (`vishnu_vishnuid`) REFERENCES `vishnu` (`vishnuid`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vishnu`
--

DROP TABLE IF EXISTS `vishnu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vishnu` (
  `vishnuid` integer primary key autoincrement,
  `formatidauth` varchar(255) DEFAULT NULL,
  `formatidfiletransfer` varchar(255) DEFAULT NULL,
  `formatidjob` varchar(255) DEFAULT NULL,
  `formatidmachine` varchar(255) DEFAULT NULL,
  `formatiduser` varchar(255) DEFAULT NULL,
  `formatidwork` varchar(255) DEFAULT NULL,
  `updatefreq` int(11) DEFAULT NULL
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vsession`
--

DROP TABLE IF EXISTS `vsession`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vsession` (
  `numsessionid` integer primary key autoincrement,
  `authid` varchar(255) DEFAULT NULL,
  `clmachine_numclmachineid` integer NOT NULL,
  `closepolicy` int(11) DEFAULT NULL,
  `closure` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `creation` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `lastconnect` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `sessionkey` varchar(255) DEFAULT NULL,
  `state` int(11) DEFAULT NULL,
  `timeout` int(11) DEFAULT NULL,
  `users_numuserid` integer NOT NULL,
  `vsessionid` varchar(255) DEFAULT NULL,
  CONSTRAINT `FK581B3160A63719F2` FOREIGN KEY (`users_numuserid`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE,
  CONSTRAINT `FK581B3160C401BD40` FOREIGN KEY (`clmachine_numclmachineid`) REFERENCES `clmachine` (`numclmachineid`) ON DELETE CASCADE
) 
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `work`
--

DROP TABLE IF EXISTS `work`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `work` (
  `id` integer primary key autoincrement,
  `application_id` bigint(20) DEFAULT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `date_ended` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `date_started` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `description` varchar(255) DEFAULT NULL,
  `done_ratio` integer NOT NULL,
  `due_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `estimated_hours` double NOT NULL,
  `identifier` varchar(255) NOT NULL,
  `last_updated` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `machine_id` bigint(20) DEFAULT NULL,
  `nbcpus` int(11) NOT NULL,
  `owner_id` integer NOT NULL,
  `priority` int(11) NOT NULL,
  `project_id` bigint(20) DEFAULT NULL,
  `start_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `status` int(11) NOT NULL,
  `subject` varchar(255) NOT NULL,
  `submit_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  CONSTRAINT `FK37C7113462EA58` FOREIGN KEY (`application_id`) REFERENCES `application_version` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK37C7119207FB3B` FOREIGN KEY (`machine_id`) REFERENCES `machine` (`nummachineid`) ON DELETE CASCADE,
  CONSTRAINT `FK37C711BD4995A3` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK37C711ED217864` FOREIGN KEY (`owner_id`) REFERENCES `users` (`numuserid`) ON DELETE CASCADE
) 
;


DROP TABLE IF EXISTS 'keygen';
CREATE TABLE `keygen` (
  `id` integer primary key autoincrement
) 
;

-- Role Creation;
-- There is no Role or Privilege support in SQLITE3


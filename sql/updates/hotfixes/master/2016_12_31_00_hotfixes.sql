--
-- Table structure for table `spell_misc_difficulty`
--

DROP TABLE IF EXISTS `spell_misc_difficulty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spell_misc_difficulty` (
  `SpellID` int(10) unsigned NOT NULL DEFAULT '0',
  `DifficultyID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spell_misc_difficulty`
--

LOCK TABLES `spell_misc_difficulty` WRITE;
/*!40000 ALTER TABLE `spell_misc_difficulty` DISABLE KEYS */;
/*!40000 ALTER TABLE `spell_misc_difficulty` ENABLE KEYS */;
UNLOCK TABLES;

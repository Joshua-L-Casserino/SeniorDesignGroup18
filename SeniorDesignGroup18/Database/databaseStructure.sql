SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE `equipmenthistory` (
  `equipmentID` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `TDS` int(11) NOT NULL,
  `PH` decimal(3,1) NOT NULL,
  `LUX` int(11) NOT NULL,
  `WaterLevel` int(11) NOT NULL,
  `timestamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`equipmentID`,`timestamp`),
  KEY `equipmentID` (`equipmentID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE `equipmentid` (
  `userName` varchar(16) COLLATE utf8_unicode_ci,
  `equipmentID` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `currentTDS` int(11) NOT NULL,
  `currentPH` decimal(3,1) NOT NULL,
  `currentLUX` int(11) NOT NULL,
  `currentWaterLevel` int(11) NOT NULL,
  `settingTdsHigh` int(11),
  `settingTdsLow` int(11),
  `settingPhHigh` decimal(3,1),
  `settingPhLow` decimal(3,1),
  `counterTDS` int(11) DEFAULT 300,
  `counterPHup` int(11) DEFAULT 300,
  `counterPHdown` int(11) DEFAULT 300,
  `counterFlowering` int(11) DEFAULT 300,
  `lightOnTime` time,
  `lightOffTime` time,
  `led` tinyint(1) NOT NULL DEFAULT 1,
  `flowering` tinyint(1) NOT NULL DEFAULT 0,
  `currentTimestamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `nickname` varchar(16) COLLATE utf8_unicode_ci DEFAULT NULL,
  `datePlanted` text COLLATE utf8_unicode_ci,
  `plants` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`equipmentID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE `presets` (
  `plantType` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `settingTdsHigh` int(11) NOT NULL,
  `settingTdsLow` int(11) NOT NULL,
  `settingPhHigh` decimal(3,1) NOT NULL,
  `settingPhLow` decimal(3,1) NOT NULL,
  `lightOnTime` time NOT NULL,
  `lightOffTime` time NOT NULL,
  `flowering` tinyint(1) NOT NULL,
  PRIMARY KEY (`plantType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE `users` (
  `userName` varchar(16) COLLATE utf8_unicode_ci NOT NULL,
  `userPassword` varchar(16) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`userName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

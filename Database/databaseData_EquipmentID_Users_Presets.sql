-- phpMyAdmin SQL Dump
-- version 4.6.6
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Feb 02, 2017 at 05:18 AM
-- Server version: 10.1.20-MariaDB
-- PHP Version: 7.0.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `id587274_homehydroponicsystem`
--

--
-- Dumping data for table `equipmentid`
--

INSERT INTO `equipmentid` (`userName`, `equipmentID`, `currentTDS`, `currentPH`, `currentLUX`, `currentWaterLevel`, `settingTdsHigh`, `settingTdsLow`, `settingPhHigh`, `settingPhLow`, `counterTDS`, `counterPH`, `counterFlowering`, `lightOnTime`, `lightOffTime`, `led`, `flowering`, `currentTimestamp`, `nickname`, `datePlanted`, `plants`) VALUES
('Beau Inman', '0001', 845, 6.82, 28309, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 04:49:28', NULL, NULL, NULL),
('Beau Inman', '0002', 829, 7.50, 31876, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 04:54:19', NULL, NULL, NULL),
('Beau Inman', '0003', 832, 7.29, 28846, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 04:54:52', NULL, NULL, NULL),
('Alex Costello', '0004', 748, 7.24, 29442, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:08:26', NULL, NULL, NULL),
('Alex Costello', '0005', 870, 7.09, 29845, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:08:26', NULL, NULL, NULL),
('Alex Costello', '0006', 766, 7.29, 30872, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 04:57:29', NULL, NULL, NULL),
('Richard Charmbur', '0007', 867, 7.87, 30903, 4.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:09:22', NULL, NULL, NULL),
('Richard Charmbur', '0008', 802, 7.48, 29401, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:10:39', NULL, NULL, NULL),
('Richard Charmbur', '0009', 777, 7.53, 33363, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:11:13', NULL, NULL, NULL),
('Joshua Casserino', '0010', 823, 6.28, 30985, 3.00, 900, 700, 7.50, 6.50, 300, 300, 300, '04:00:00', '20:00:00', 1, 0, '2017-01-31 05:28:53', 'Living Room', '01/20/2017', 'Random Stuff'),
('Joshua Casserino', '0011', 761, 7.17, 30864, 3.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:14:39', NULL, NULL, NULL),
('Joshua Casserino', '0012', 861, 6.74, 28746, 2.00, 0, 0, 0.00, 0.00, 0, 0, 0, '00:00:00', '00:00:00', 0, 0, '2017-01-31 05:15:31', NULL, NULL, NULL),
('Test Account', '0013', 809, 7.06, 29870, 3.10, 900, 700, 7.50, 6.50, 300, 300, 300, '04:00:00', '20:00:00', 1, 0, '2017-02-02 05:03:00', 'Test Data', 'N/A', 'None');

--
-- Dumping data for table `presets`
--

INSERT INTO `presets` (`plantType`, `settingTdsHigh`, `settingTdsLow`, `settingPhHigh`, `settingPhLow`, `lightOnTime`, `lightOffTime`, `flowering`) VALUES
('Basil', 1120, 700, 6.500, 5.500, '04:00:00', '22:00:00', 0),
('Lettuce', 840, 560, 6.500, 5.500, '04:00:00', '22:00:00', 0);

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`userName`, `userPassword`) VALUES
('alex', 'alex'),
('Alex Costello', 'password'),
('arczi', 'assword'),
('Beau Inman', 'password'),
('Joshua Casserino', 'password'),
('Richard Charmbur', 'password'),
('Test Account', 'password');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

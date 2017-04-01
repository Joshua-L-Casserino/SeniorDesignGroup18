INSERT INTO `equipmentID` (`equipmentID`, `currentTDS`, `currentPH`, `currentLUX`, `currentWaterLevel`, `currentTimestamp`) VALUES
('0001', 757, 6.2, 0, 95, '2017-03-01 00:00:00'),
('0002', 757, 6.2, 0, 95, '2017-03-01 00:00:00'),
('0003', 757, 6.2, 0, 95, '2017-03-01 00:00:00'),
('0004', 757, 6.2, 0, 95, '2017-03-01 00:00:00');

INSERT INTO `presets` (`plantType`, `settingTdsHigh`, `settingTdsLow`, `settingPhHigh`, `settingPhLow`, `lightOnTime`, `lightOffTime`, `flowering`) VALUES
('Basil', 1120, 700, 6.500, 5.500, '04:00:00', '22:00:00', 0),
('Lettuce', 840, 560, 6.500, 5.500, '04:00:00', '22:00:00', 0);

INSERT INTO `users` (`userName`, `userPassword`) VALUES
('Alex', 'password'),
('Beau', 'password'),
('Josh', 'password'),
('Rich', 'password');

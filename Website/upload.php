<?php
	$host = "localhost";
	$user = 'id587274_group18';
	$password = 'password';

	#$user = 'root';
	#$password = 'root';
	$db = 'id587274_homehydroponicsystem';
	$link = new mysqli($host, $user, $password, $db) or die("Unable to connect!");

	if(isset($_POST['equipmentID']))
		$equipmentID = $_POST['equipmentID'];
	if(isset($_POST['PH']))
		$PH= $_POST['PH'];
	if(isset($_POST['TDS']))
		$TDS= $_POST['TDS'];
	if(isset($_POST['LUX']))
		$LUX= $_POST['LUX'];
	if(isset($_POST['WATERLEVEL']))
		$WATERLEVEL= $_POST['WATERLEVEL'];


	$sql = "INSERT INTO `equipmenthistory` (`equipmentID`, `TDS`, `PH`, `LUX`, `WaterLevel`) VALUES ('$equipmentID', '$TDS', '$PH',  '$LUX', '$WATERLEVEL')";
	$result = mysqli_query($link, $sql);
		if ($result)
			echo "ok";
		else
			echo "no";
		
	$sql = "UPDATE `equipmentid` SET `currentTDS`='$TDS',`currentPH`='$PH',`currentLUX`='$LUX',`currentWaterLevel`='$WATERLEVEL' WHERE `equipmentID` = '$equipmentID'";
	$result = mysqli_query($link, $sql);
		if ($result)
			echo "ok";
		else
			echo "no";
	
	$sql = "
				SELECT 1 
				FROM `equipmentid`
				WHERE `equipmentID` = '$equipmentID'
				AND CURTIME() BETWEEN `lightOnTime` AND `lightOffTime`
			";
	$result = mysqli_query($link, $sql);
	
	if($result){
		$led_On = mysqli_num_rows($result);
		if($led_On){
			$sql = "
					UPDATE `equipmentid` SET `led` = 1 WHERE `equipmentID` = '$equipmentID'
					";
			$result = mysqli_query($link, $sql);
		}
		else{
			$sql = "
					UPDATE `equipmentid` SET `led` = 0 WHERE `equipmentID` = '$equipmentID'
					";
			$result = mysqli_query($link, $sql);
		}
	}
	
		mysqli_close($link);
?>
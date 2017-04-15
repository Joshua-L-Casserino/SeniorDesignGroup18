<?php
	require_once('config.php');
	$link = new mysqli(DBHOST, DBUSER, DBPASS, DBNAME) or die("Unable to connect!");

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
	if(isset($_POST['counterTDS']))
		$counterTDS= $_POST['counterTDS'];
	if(isset($_POST['counterPHup']))
		$counterPHup= $_POST['counterPHup'];
	if(isset($_POST['counterPHdown']))
		$counterPHdown= $_POST['counterPHdown'];
	if(isset($_POST['counterFlowering']))
		$counterFlowering= $_POST['counterFlowering'];


	$sql = "INSERT INTO `equipmenthistory` (`equipmentID`, `TDS`, `PH`, `LUX`, `WaterLevel`) VALUES ('$equipmentID', '$TDS', '$PH',  '$LUX', '$WATERLEVEL')";
	$result = mysqli_query($link, $sql);
	
	if($result){
		$sql = "UPDATE `equipmentid` SET `currentTDS`='$TDS',`currentPH`='$PH',`currentLUX`='$LUX',`currentWaterLevel`='$WATERLEVEL', `counterTDS` = '$counterTDS', `counterPHup` = '$counterPHup', `counterPHdown` = '$counterPHdown', `counterFlowering` = '$counterFlowering' WHERE `equipmentID` = '$equipmentID'";
		$result = mysqli_query($link, $sql);
		
		if($result){
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
					UPDATE `equipmentid` SET `led` = 1 WHERE `equipmentID` = '$equipmentID'";
					$result = mysqli_query($link, $sql);
				}
					else{
						$sql = "
						UPDATE `equipmentid` SET `led` = 0 WHERE `equipmentID` = '$equipmentID'
						";
						$result = mysqli_query($link, $sql);
					}
	
				$sqlQuery = "SELECT * FROM `equipmentid` WHERE `equipmentID` = '$equipmentID'";
				$result = mysqli_query($link, $sqlQuery);

				if($result){
					$row = mysqli_fetch_array($result);
					if($row['userName'] != NULL){
						
						echo "$".$row['settingTdsHigh'].",";
						echo "".$row['settingTdsLow'].",";
						echo "".$row['settingPhHigh'].",";
						echo "".$row['settingPhLow'].",";
						echo "".$row['counterTDS'].",";
						echo "".$row['counterPHup'].",";
						echo "".$row['counterPHdown'].",";
						echo "".$row['counterFlowering'].",";
						echo "".$row['led'].",";
						echo "".$row['flowering'].",$";
					}
				}
			}
		}
	}

	mysqli_close($link);
	
?>
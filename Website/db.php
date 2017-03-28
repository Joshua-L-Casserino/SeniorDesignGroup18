<?php
	session_start();
	if(!empty($_GET['equipmentID']))
		$equipmentID = $_GET['equipmentID'];
	
	$user = 'id587274_group18';
	$password = 'password';
	$db = 'id587274_homehydroponicsystem';
	$link = new mysqli('localhost', $user, $password, $db) or die("Unable to connect!");
?>

<html>
<title>MCU profile Page</title>

	<?php
		$sqlQuery = "SELECT * FROM `equipmentid` WHERE `equipmentID` = '$equipmentID'";
		$result = mysqli_query($link, $sqlQuery);
		
		if($result){
			$row = mysqli_num_rows($result);
			if($row['userName'] != NULL){
				echo $row['settingTdsHigh']."/r";
				echo $row['settingTdsLow']."/r";
				echo $row['settingPhHigh']."/r";
				echo $row['settingPhLow']."/r";
				echo $row['counterTDS']."/r";
				echo $row['counterPH']."/r";
				echo $row['counterFlowering']."/r";
				echo $row['led']."/r";
				echo $row['flowering']."/r";
			}
		}
	?>

</html>
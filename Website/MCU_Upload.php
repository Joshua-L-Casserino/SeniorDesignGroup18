<?php
	session_start();
	if(!empty($_POST['equipmentID']))
		$equipmentID = $_POST['equipmentID'];
	
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
			$row = mysqli_fetch_array($result);
			if($row['userName'] != NULL){
				echo "".$row['settingTdsHigh']."|";
				echo "".$row['settingTdsLow']."|";
				echo "".$row['settingPhHigh']."|";
				echo "".$row['settingPhLow']."|";
				echo "".$row['counterTDS']."|";
				echo "".$row['counterPHUp']."|";
				echo "".$row['counterPHDown']."|";
				echo "".$row['counterFlowering']."|";
				echo "".$row['led']."|";
				echo "".$row['flowering']."|"."/r";
			}
		}
	?>

</html>
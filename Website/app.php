<?php


	//$user = 'id587274_group18';
	//$password = 'password';
$host = "localhost";
$user = 'id587274_group18';
$password = 'password';
$db = 'id587274_homehydroponicsystem';
$link = new mysqli($host, $user, $password, $db) or die("Unable to connect!");

	//switch for different php actions, set by post value opCode
	//1 = check if valid user login
	//2 = query to add new user
	//3 = query for user home
	//4 =query for adding equipmentID
	//5 = query for eID history
	//6 = set nickname query
	//7 = delete eID query
	//default = print error
	
	if(empty($_POST['opCode'])){
		echo "Error, make sure you specify what you want the page to do.";
	}else{
		switch ($_POST['opCode']){
			case "1":{
			userLogin($link);
			return;
			}
			case "2":{
			inputNewUser($link);
			return;
			}
			case"3":{
			userHome($host, $user, $password, $db);
			return;
			}
			case"4":{
			addEquipmentID($link);
			return;
			}
			case"5":{
			getEquipmentHistory($host, $user, $password, $db);
			return;
			}
			case"6":{
			setNickname($host, $user, $password, $db);
			return;
			}
			case"7":{
			deleteEID($host, $user, $password, $db);
			return;
			}
			
			default:{
				echo "Error, make sure you specify what you want the page to do.";
				return;
			}
		}
	}	
	
function userLogin($link){
	$inputUser = $_POST['login-id'];
	$inputPassword = $_POST['login-password'];
	

	$sqlQuery = "SELECT* FROM `users` WHERE `userName` = '$inputUser'";

	$result = mysqli_query($link, $sqlQuery);
	$UID = mysqli_fetch_array($result);
	mysqli_close($link);

	if ($UID['userPassword'] === $inputPassword){
		echo "Valid";
	}else{
		echo "Login Failed! Please try again.";
	}
}
	
	
function inputNewUser($link){
	$inputUser = $_POST['newUser-name'];
	$inputPassword = $_POST['newUser-password'];
	$sqlQuery = "INSERT INTO `users` (`userName`, `userPassword`) VALUES ('$inputUser', '$inputPassword')";
	$result = mysqli_query($link, $sqlQuery);
	
	
	if ($result)
		echo "Welcome, let's get started!";
	else
		echo "That user name is already taken. Please try another.";
	
	mysqli_close($link);
}

function userHome($host, $user, $password, $db){
	if(isset($_POST['userName'])){
	 $userName = $_POST['userName'];
	}	
	else
	 $userName = "Alex Costello";
 
	$mysqli = new mysqli($host, $user, $password, $db);
    $myArray = array();
    if ($result = $mysqli->query("SELECT * FROM `equipmentid` WHERE `userName` = '$userName'")) {
        $tempArray = array();
        while($row = $result->fetch_object()) {
                $tempArray = $row;
                array_push($myArray, $tempArray);
            }
        echo json_encode($myArray);
    }

    $result->close();
    $mysqli->close();
	
	
}
function addEquipmentID($link){
	
	$userName = $_POST['userName'];
	
	if(!empty($_POST['equipmentID'])){
		$equipmentID = $_POST['equipmentID'];
		$sqlQuery = "UPDATE `equipmentid` SET `userName`= '$userName' WHERE `equipmentID` = '$equipmentID'";
		$sqlQuery2 = "SELECT * FROM `equipmentid` WHERE `equipmentID` = '$equipmentID'";
		$result = mysqli_query($link, $sqlQuery);
		$result2 = mysqli_query($link, $sqlQuery2);
		#echo $result;
		#echo $result2->num_rows;
		if($result && $result2->num_rows==1){
			mysqli_close($link);
			echo "Success";
		}else{
			echo "The equipmentID was not added please check the input and try again.";
			mysqli_close($link);
		}
	}
	
	
}

function getEquipmentHistory($host, $user, $password, $db){
	//$userName = $_POST['userLoggedIn'];
	$equipmentID = $_POST['equipmentID'];
	
	$mysqli = new mysqli($host, $user, $password, $db);
	$myArray = array();
	if ($result = $mysqli->query("SELECT * FROM `equipmenthistory` WHERE `equipmentID` = '$equipmentID' ORDER BY `timestamp` DESC ")) {
        $tempArray = array();
        while($row = $result->fetch_object()) {
                $tempArray = $row;
                array_push($myArray, $tempArray);
            }
        echo json_encode($myArray);
    }
	

}  
function setNickname($host, $user, $password, $db){
	
}
function deleteEID($host, $user, $password, $db){
	$EID = $_POST['equipmentID'];


	$sqlQuery =  "UPDATE `equipmentid` SET `userName`= '' WHERE `equipmentID` = '$EID'";
	$result = mysqli_query($link, $sqlQuery);

		if($result){
			mysqli_close($link);
			
			echo "Success";
		}else{
			echo $EID;
			echo "The equipmentID was not removed please check the input and try again.";
			mysqli_close($link);
			
		}



}

?>

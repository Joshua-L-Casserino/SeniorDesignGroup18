<?php
	require_once('config.php');
	$link = new mysqli(DBHOST, DBUSER, DBPASS, DBNAME) or die("Unable to connect!");
	
	session_start();
	if(!$_SESSION['current'])
		header('Location: noPermissions.php');
    $userName = $_SESSION['userLoggedIn'];

?>
<html>

	<head>
		<meta http-equiv="content-type" content="text/html; charset=windows-1252">
		
		<script type="text/javascript" src="content/jquery/jquery-3.0.0.min.js"></script>
		<link href="content/bootstrap/css/bootstrap.min.css" rel="stylesheet">
		<script src="https://use.fontawesome.com/808114f81f.js"></script>

		<script src="content/list.min.js"></script>
		<link rel="stylesheet" type="text/css" href="content/list.css">
		<script src="http://listjs.com/no-cdn/list.js"></script>
		<script src="content/scripts/events.js"></script>
		
	</head>
	<body>
		<?php
			include_once("header.php");
		?>

		<div class="container-fluid">
			<div class="row">
				<div class="col-md-8 col-md-offset-2">
					<div class="col-md-6">	
						<h2>Current Equipment Readings</h2>
					</div>
					<div class="col-md-6">	
						<div class="btn-group" role="group" aria-label="..." style="float:right">
							<a href="userProfile.php" class="btn btn-default">User Profile</a>
						</div>
					</div>
				</div>
				<div class="col-md-8 col-md-offset-2">
					<?php
						$sqlQuery = "SELECT * FROM `equipmentid` WHERE `userName` = '$userName'";
						$result = mysqli_query($link, $sqlQuery);
						if(mysqli_fetch_array(mysqli_query($link, $sqlQuery)) > 0){
							echo "<table class='table table-striped'>";
							echo "	<tbody class='public-events-list'>";
							echo "		<thead>";
							echo "			<th>Equipment ID</th>";
							echo "			<th>Nickname</th>";
							echo "			<th>Time</th>";
							echo "			<th>pH</th>";
							echo "			<th>TDS</th>";
							echo "			<th>Lux</th>";
							echo "			<th>History</th>";
							echo "		</thead>";
							while($row = mysqli_fetch_array($result)){
								echo "<form method='GET' />";
								echo "	<tr>";
								echo "		<td class='".$row['equipmentID']."'>".$row['equipmentID']."</td>";
								echo "		<td class='".$row['nickname']."'>".$row['nickname']."</td>";
								echo "		<td class='".$row['currentTimestamp']."'>".$row['currentTimestamp']."</td>";
								echo "		<td class='".$row['currentPH']."'>".$row['currentPH']."</td>";
								echo "		<td class='".$row['currentTDS']."'>".$row['currentTDS']."</td>";
								echo "		<td class='".$row['currentLUX']."'>".$row['currentLUX']."</td>";
								echo "		<td class='event-details'>";
								echo "		<a href='equipmentHistory.php?equipmentID=".$row['equipmentID']."' class='btn btn-default'>Info</a>";
								echo "		</td>";
								echo "	</tr>";
								echo "</form>";
							}
							echo "	</tbody>";
							echo "</table>";
						}else{
							echo "	<div class='container-fluid'>
										<div class='row'>
											<div class='col-md-6 col-md-offset-3'>
												<div class='panel panel-danger'>
													<div class='panel-body'>
														There are no systems currently attached to this profile. To attach a system 
														to this profile please go to your user profile and upload an equipment ID to 
														recieve the system measurements.<br><br>
														
													</div>
												</div>
											</div>
										</div>
									</div>";
						}
						mysqli_close($link);
					?>
				</div>
			</div>
		</div>
		<div>
			<?php
				include_once "footer.php";
			?>
		</div>
	</body>
	
</html>
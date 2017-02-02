<?php
	session_start();
	$userID = $_SESSION['userLoggedIn'];
	if(!empty($_GET['equipmentID']))
		$equipmentID = $_GET['equipmentID'];
	$user = 'id587274_group18';
	$password = 'password';
	$db = 'id587274_homehydroponicsystem';
	$link = new mysqli('localhost', $user, $password, $db) or die("Unable to connect!");
?>

<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=windows-1252">
		<script type="text/javascript" src="content/jquery/jquery-3.0.0.min.js"></script>
		<link href="content/bootstrap/css/bootstrap.min.css" rel="stylesheet">
		<script src="https://use.fontawesome.com/808114f81f.js"></script>
		
				
		<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
		<script type="text/javascript">
		
		google.charts.load('current', {packages: ['corechart', 'line']});
		google.charts.setOnLoadCallback(drawLogScales);

		function drawLogScales() {
		  var pH = new google.visualization.DataTable();
		  var tds = new google.visualization.DataTable();
		  var lux = new google.visualization.DataTable();
		  var waterLevel = new google.visualization.DataTable();
		  pH.addColumn('string', 'X');
		  pH.addColumn('number', 'pH');
		  tds.addColumn('string', 'X');
		  tds.addColumn('number', 'TDS');
		  lux.addColumn('string', 'X');
		  lux.addColumn('number', 'LUX');
		  waterLevel.addColumn('string', 'X');
		  waterLevel.addColumn('number', 'Water Level');

		<?php
				$sqlQuery = "SELECT * FROM `equipmenthistory` WHERE `equipmentID` = '$equipmentID' ORDER BY `timestamp` DESC ";
				$result02 = mysqli_query($link, $sqlQuery);
				$currentDate = "";
				$currentDay = "";
				$title = "";
				$avgPH = 0;
				$avgTDS = 0;
				$avgLUX = 0;
				$avgWL = 0;
				$counter = 0;
				$check = 0;
				if(!empty($_POST['graphOptions']))
				{
					$numRows = 0;
					if($_POST['graphOptions'] === 'Last 10 Readings')
						$limit = 10;
					else if($_POST['graphOptions'] === 'Last 100 Readings')
						$limit = 100;
					else if($_POST['graphOptions'] === 'Last 1000 Readings')
						$limit = 1000;
					else if($_POST['graphOptions'] === 'This Days Readings')
						$limit = mysqli_num_rows($result02);
					else if($_POST['graphOptions'] === 'This Months Readings')
						$limit = mysqli_num_rows($result02);
					else if($_POST['graphOptions'] === 'This Years Readings')
						$limit = mysqli_num_rows($result02);
					else if($_POST['graphOptions'] === 'Total Readings')
						$limit = mysqli_num_rows($result02);
					
					while(($row = mysqli_fetch_array($result02)) && ($numRows < $limit))
					{
						$string = "";
						$temp = explode(' ',$row['timestamp'],2);
						$date = explode('-',$temp[0],3);
						$year = $date[0];
						$month = $date[1];
						$day = $date[2];
						$time = $temp[1];
						$tempTime = explode(':', $time, 3);
						
						if($_POST['graphOptions'] === 'Last 10 Readings')
						{
							$title = "Last 10 measurements";
							if($currentDate != $temp[0])
							{
								$currentDate = $temp[0];
								$string .= $temp[0]." ".$tempTime[0].":".$tempTime[1];
							}
							else
								$string .= $tempTime[0].":".$tempTime[1];
							
							echo "pH.addRows([['".$string."', ".$row['PH']."]]);";
							echo "tds.addRows([['".$string."', ".$row['TDS']."]]);";
							echo "lux.addRows([['".$string."', ".$row['LUX']."]]);";
							echo "waterLevel.addRows([['".$string."', ".$row['WaterLevel']."]]);";
						}
						else if($_POST['graphOptions'] === 'Last 100 Readings')
						{
							$title = "Last 100 measurements";
							if($currentDate != $temp[0])
							{
								$currentDate = $temp[0];
								$string .= $temp[0]." ".$tempTime[0].":".$tempTime[1];
							}
							else
								$string .= $tempTime[0].":".$tempTime[1];
							
							echo "pH.addRows([['".$string."', ".$row['PH']."]]);";
							echo "tds.addRows([['".$string."', ".$row['TDS']."]]);";
							echo "lux.addRows([['".$string."', ".$row['LUX']."]]);";
							echo "waterLevel.addRows([['".$string."', ".$row['WaterLevel']."]]);";
						}
						else if($_POST['graphOptions'] === 'Last 1000 Readings')
						{
							$title = "Last 1000 measurements";
							if($currentDate != $temp[0])
							{
								if($counter > 1)
								{
									$string .= $currentDate;
									$pH = $avgPH/$counter;
									$TDS = $avgTDS/$counter;
									$LUX = $avgLUX/$counter;
									$WL = $avgWL/$counter;
									echo "pH.addRows([['".$string."', ".$pH."]]);";
									echo "tds.addRows([['".$string."', ".$TDS."]]);";
									echo "lux.addRows([['".$string."', ".$LUX."]]);";
									echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
									$counter = 1;
									$currentDate = $temp[0];
									$avgPH = $row['PH'];
									$avgTDS = $row['TDS'];
									$avgLUX = $row['LUX'];
									$avgWL  = $row['WaterLevel'];
								}
								$counter = 1;
								$currentDate = $temp[0];
								$avgPH = $row['PH'];
								$avgTDS = $row['TDS'];
								$avgLUX = $row['LUX'];
								$avgWL  = $row['WaterLevel'];
							}
								else
								{
									$avgPH += $row['PH'];
									$avgTDS += $row['TDS'];
									$avgLUX += $row['LUX'];
									$avgWL  += $row['WaterLevel'];
									$counter++;
								}
						}
						else if($_POST['graphOptions'] === 'This Days Readings')
						{
							$title = $temp[0]." measurements";
							if(($currentDate != $temp[0]) && ($counter > 0))
								break;
							else
								$string .= $tempTime[0].":".$tempTime[1];
							
							$currentDate = "".$temp[0];
							echo "pH.addRows([['".$string."', ".$row['PH']."]]);";
							echo "tds.addRows([['".$string."', ".$row['TDS']."]]);";
							echo "lux.addRows([['".$string."', ".$row['LUX']."]]);";
							echo "waterLevel.addRows([['".$string."', ".$row['WaterLevel']."]]);";
							$counter++;
						}
						else if($_POST['graphOptions'] === 'This Months Readings')
						{
							if(($currentDate != $month) && ($counter > 0))
							{
								$string .= $currentDay;
								$pH = $avgPH/$counter;
								$TDS = $avgTDS/$counter;
								$LUX = $avgLUX/$counter;
								$WL = $avgWL/$counter;
								echo "pH.addRows([['".$string."', ".$pH."]]);";
								echo "tds.addRows([['".$string."', ".$TDS."]]);";
								echo "lux.addRows([['".$string."', ".$LUX."]]);";
								echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
								break;
							}
							
							$title = $year."-".$month." measurements";
							
							if($currentDay != $day)
							{
								if($counter > 1)
								{
									$string .= $currentDay;
									$pH = $avgPH/$counter;
									$TDS = $avgTDS/$counter;
									$LUX = $avgLUX/$counter;
									$WL = $avgWL/$counter;
									echo "pH.addRows([['".$string."', ".$pH."]]);";
									echo "tds.addRows([['".$string."', ".$TDS."]]);";
									echo "lux.addRows([['".$string."', ".$LUX."]]);";
									echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
									$counter = 1;
									$currentDay = "".$day;
									$avgPH = $row['PH'];
									$avgTDS = $row['TDS'];
									$avgLUX = $row['LUX'];
									$avgWL  = $row['WaterLevel'];
								}
								$counter = 1;
								$currentDay = "".$day;
								$avgPH = $row['PH'];
								$avgTDS = $row['TDS'];
								$avgLUX = $row['LUX'];
								$avgWL  = $row['WaterLevel'];
							}
							else
							{
								$avgPH += $row['PH'];
								$avgTDS += $row['TDS'];
								$avgLUX += $row['LUX'];
								$avgWL  += $row['WaterLevel'];
								$counter++;
							}
							$currentDate = "".$month;
						}
						else if($_POST['graphOptions'] === 'This Years Readings')
						{
							if(($currentDate != $year) && ($counter > 0))
							{
								$string .= $currentDay;
								$pH = $avgPH/$counter;
								$TDS = $avgTDS/$counter;
								$LUX = $avgLUX/$counter;
								$WL = $avgWL/$counter;
								echo "pH.addRows([['".$string."', ".$pH."]]);";
								echo "tds.addRows([['".$string."', ".$TDS."]]);";
								echo "lux.addRows([['".$string."', ".$LUX."]]);";
								echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
								break;
							}
							
							$title = "Total measurements for the ".$year;
							
							if($currentDay != $month)
							{
								if($counter > 1)
								{
									$string .= $currentDay;
									$pH = $avgPH/$counter;
									$TDS = $avgTDS/$counter;
									$LUX = $avgLUX/$counter;
									$WL = $avgWL/$counter;
									echo "pH.addRows([['".$string."', ".$pH."]]);";
									echo "tds.addRows([['".$string."', ".$TDS."]]);";
									echo "lux.addRows([['".$string."', ".$LUX."]]);";
									echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
									$counter = 1;
									$currentDay = "".$month;
									$avgPH = $row['PH'];
									$avgTDS = $row['TDS'];
									$avgLUX = $row['LUX'];
									$avgWL  = $row['WaterLevel'];
								}
								$counter = 1;
								$currentDay = "".$month;
								$avgPH = $row['PH'];
								$avgTDS = $row['TDS'];
								$avgLUX = $row['LUX'];
								$avgWL  = $row['WaterLevel'];
							}
							else
							{
								$avgPH += $row['PH'];
								$avgTDS += $row['TDS'];
								$avgLUX += $row['LUX'];
								$avgWL  += $row['WaterLevel'];
								$counter++;
							}
							$currentDate = "".$year;
						}
						else if($_POST['graphOptions'] === 'Total Readings')
						{
							if(($currentDate != $year) && ($counter > 0))
							{
								$string .= $currentDate."-".$currentDay;
								$pH = $avgPH/$counter;
								$TDS = $avgTDS/$counter;
								$LUX = $avgLUX/$counter;
								$WL = $avgWL/$counter;
 								echo "pH.addRows([['".$string."', ".$pH."]]);";
								echo "tds.addRows([['".$string."', ".$TDS."]]);";
								echo "lux.addRows([['".$string."', ".$LUX."]]);";
								echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
								$counter = 1;
							}							
							else if(($currentDay != $month))
							{
								if($counter > 1)
								{
									$string .= $currentDay;
									$pH = $avgPH/$counter;
									$TDS = $avgTDS/$counter;
									$LUX = $avgLUX/$counter;
									$WL = $avgWL/$counter;
									echo "pH.addRows([['".$string."', ".$pH."]]);";
									echo "tds.addRows([['".$string."', ".$TDS."]]);";
									echo "lux.addRows([['".$string."', ".$LUX."]]);";
									echo "waterLevel.addRows([['".$string."', ".$WL."]]);\n";
									$counter = 1;
									$currentDay = "".$month;
									$avgPH = $row['PH'];
									$avgTDS = $row['TDS'];
									$avgLUX = $row['LUX'];
									$avgWL  = $row['WaterLevel'];
								}
								$counter = 1;
								$currentDay = "".$month;
								$avgPH = $row['PH'];
								$avgTDS = $row['TDS'];
								$avgLUX = $row['LUX'];
								$avgWL  = $row['WaterLevel'];
							}
							else
							{
								$avgPH += $row['PH'];
								$avgTDS += $row['TDS'];
								$avgLUX += $row['LUX'];
								$avgWL  += $row['WaterLevel'];
								$counter++;
							}
							$currentDate = "".$year;
							$title = "Total measurements for Equipment ID:".$equipmentID;
							$check = true;
						}
						$numRows++;
						$check++;
					}
				}
			echo "
					  var pHoptions = {
						hAxis: {
						  title: 'Time',
						  logScale: false,
						  direction: '-1'
						},
						vAxis: {
						  title: 'Scale',
						  logScale: false
						},
						
						colors: ['#2D7A00'],
						title: 'pH: ".$title."',
						curveType: 'function',
						legend: { position: 'bottom' }
					  };
					  
					  var TDSoptions = {
						hAxis: {
						  title: 'Time',
						  logScale: true,
						  direction: '-1'
						},
						vAxis: {
						  title: 'Scale',
						  logScale: false
						},
						
						colors: ['#2D7A00'],
						title: 'TDS: ".$title."',
						curveType: 'function',
						legend: { position: 'bottom' }
					  };
					  
					  var LUXoptions = {
						hAxis: {
						  title: 'Time',
						  logScale: true,
						  direction: '-1'
						},
						vAxis: {
						  title: 'Scale',
						  logScale: false
						},
						
						colors: ['#2D7A00'],
						title: 'LUX: ".$title."',
						curveType: 'function',
						legend: { position: 'bottom' }
					  };
					  
					  var WLoptions = {
						hAxis: {
						  title: 'Time',
						  logScale: true,
						  direction: '-1'
						},
						vAxis: {
						  title: 'Scale',
						  logScale: false
						},
						
						colors: ['#2D7A00'],
						title: 'WaterLevel: ".$title."',
						curveType: 'function',
						legend: { position: 'bottom' }
					  };
					  
					  var PHchart = new google.visualization.LineChart(document.getElementById('PHchart_div'));
					  PHchart.draw(pH, pHoptions);
					  var TDSchart = new google.visualization.LineChart(document.getElementById('TDSchart_div'));
					  TDSchart.draw(tds, TDSoptions);
					  var LUXchart = new google.visualization.LineChart(document.getElementById('LUXchart_div'));
					  LUXchart.draw(lux, LUXoptions);
					  var WaterLevelchart = new google.visualization.LineChart(document.getElementById('WaterLevelchart_div'));
					  WaterLevelchart.draw(waterLevel, WLoptions);
				}
			";
		?>
		</script>
	</head>
	<body>
		<?php
			include_once "header.php";
		?>
		<div class="container-fluid">
			<div class="row">
				<div class="col-md-8 col-md-offset-2">
				
					<div class="panel panel-primary">
					
						<div class="panel-heading" style="background-color: #2D7A00;">
							Equipment History
						</div>
						
						<div class="panel-body">
							<?php
								$userName = $_SESSION['userLoggedIn'];
								$sqlQuery = "SELECT * FROM `equipmentid` WHERE `equipmentID` = '$equipmentID' AND `userName` = '$userName' ";
								$result = mysqli_query($link, $sqlQuery);
								$row = mysqli_fetch_array($result);
								$sqlQuery = "SELECT * FROM `equipmenthistory` WHERE `equipmentID` = '$equipmentID' ORDER BY `timestamp` DESC ";
								$result02 = mysqli_query($link, $sqlQuery);
								
								echo "	<div>
											Equipment ID: ".$row['equipmentID']."
										</div>";
								echo "	<div>
											Equipment Nickname: ".$row['nickname']."
										</div>";
								echo "	<div>
											Plants: ".$row['plants']."
										</div>";
								echo "	<div>
											Date Planted: ".$row['datePlanted']."
										</div>";
								if($row['counterTDS'] <= 10){
									echo "	<div>
												<p class='bg-danger'><strong>Remaining TDS Solution: ".$row['counterTDS']."</strong></p>
											</div>";
								}else{
									echo "	<div>
												Remaining TDS Solution: ".$row['counterTDS']."
											</div>";
								}
								if($row['counterPH'] <= 10){
									echo "	<div>
												<p class='bg-danger'><strong>Remaining pH Solution: ".$row['counterPH']."</strong></p>
											</div>";
								}else{
									echo "	<div>
												Remaining pH Solution: ".$row['counterPH']."
											</div>";
								}
								if($row['counterFlowering'] <= 10){
									echo "	<div>
												<p class='bg-danger'><strong>Remaining Flowering Solution: ".$row['counterFlowering']."</strong></p>
											</div>";
								}else{
									echo "	<div>
												Remaining Flowering Solution: ".$row['counterFlowering']."
											</div>";
								}
								if($row['flowering']){
									echo "	<div>
												Flowering mode is currently on
											</div>";
								}else{
									echo "	<div>
												Flowering mode is currently off
											</div>";
								}
								if($row['led']){
									echo "	<div>
												LED lighting is currently on
											</div>
											<div>
												Lights are set to turn on at ".$row['lightOnTime']."
											</div>
											<div>
												Lights are set to turn off at ".$row['lightOffTime']."
											</div>";
								}else{
									echo "	<div>
												LED lighting is currently off
											</div>";
								}

								echo "	<br>";
								echo "	<form method='POST' />";
								echo "		<div class='dropdown'>";
								echo"			<select class='btn btn-default' name='graphOptions'>";
								echo "				<option size = 100 ></option>";
								echo "				<option value = 'Last 10 Readings'>Last 10 Readings</option>";
								echo "				<option value = 'Last 100 Readings'>Last 100 Readings</option>";
								echo "				<option value = 'Last 1000 Readings'>Last 1000 Readings</option>";
								echo "				<option value = 'This Days Readings'>This Days Readings</option>";
								echo "				<option value = 'This Months Readings'>This Months Readings</option>";
								echo "				<option value = 'This Years Readings'>This Years Readings</option>";
								echo "				<option value = 'Total Readings'>Total Readings</option>";
								echo "			</select>";
								echo "			<button class='btn btn-default' type='submit' name='displayGraphs' value = 'Display'>Display Graph</button>";
								echo "		</div>";
								echo "	</form>";
								
								if(!empty($_POST['graphOptions']))
								{								
									echo "<div id='PHchart_div' style='width: 900px; height: 500px'></div>";
									echo "<div id='TDSchart_div' style='width: 900px; height: 500px'></div>";
									echo "<div id='LUXchart_div' style='width: 900px; height: 500px'></div>";
									echo "<div id='WaterLevelchart_div' style='width: 900px; height: 500px'></div>";
								}
								
								mysqli_close($link);
							?>	
						</div>
					</div>
					<div class="row">
						<center>
							<a href="userHome.php" class="btn btn-primary">Back to Home Screen</a>
						</center>
					</div>
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
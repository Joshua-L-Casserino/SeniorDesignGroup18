<?php
	session_start();
	if(!$_SESSION['current'])
		header('Location: noPermissions.php');
	$userName = $_SESSION['userLoggedIn'];
	$user = 'id587274_group18';
	$password = 'password';
	$db = 'id587274_homehydroponicsystem';
	$link = new mysqli('localhost', $user, $password, $db) or die("Unable to connect!");
?>

<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=windows-1252">
		<link href="content/bootstrap/css/bootstrap.min.css" rel="stylesheet">
		<script src="https://use.fontawesome.com/808114f81f.js"></script>
		<script src="content/list.min.js"></script>
		<link rel="stylesheet" type="text/css" href="content/list.css">
		
		<style type="text/css">
            dummydeclaration { padding-left: 4em; } /* Firefox ignores first declaration for some reason */
            tab1 { padding-left: 4em; }
            tab2 { padding-left: 8em; }
        </style>
	</head>
	<body>	
		<?php
			include_once("header.php");
		?>
		<div class="row">
			<div class="col-md-6 col-md-offset-3">
				<div class="panel panel-primary">
					<div class="panel-heading" style="background-color: #2D7A00;">
						Add Equipment ID
					</div>
					<form method="POST" />
						<div class="panel-body">						
							<div class="form-group">
								<label for="newEvent-name">Equipment ID</label>
								<input size="32" maxlength="4" type="text" class="form-control" name="equipmentID" placeholder="Please input the Equipment ID">
							</div>
							<center><button class='btn btn-default' type='submit' name='addEquiptmentID'>Add Equipment ID</button></center>
						</div>
					</form>
					<?php
						if(!empty($_POST['equipmentID'])){
							$equipmentID = $_POST['equipmentID'];
							$sqlQuery = "UPDATE `equipmentid` SET `userName`= '$userName' WHERE `equipmentID` = '$equipmentID'";
							$result = mysqli_query($link, $sqlQuery);
							if($result){
								mysqli_close($link);
								header('Location:equipmentAdded.php');
							}else{
								echo "The equipment was not added please check the input and try again.";
								mysqli_close($link);
							}
						}
					?>
				</div>	
			</div>
			
		</div>
		<div class="row">
			<div class="col-md-6 col-md-offset-3">
				<div class="panel panel-primary">
					<div class="panel-heading" style="background-color: #2D7A00;">
						Equipment Profile Settings
					</div>
					<form method="POST" >
						<div class="panel-body">						
							<div class="form-group">
								<label for="equipmentSettings">Equipment Settings</label>
								<?php
									$sqlQuery = "SELECT `equipmentID` FROM `equipmentid` WHERE `userName` = '$userName'";
									$result = mysqli_query($link, $sqlQuery);
									
									echo "	<div class='dropdown'>";
									echo"		<select class='btn btn-default' name='EID'>";
									echo "			<option size =100 ></option>";
									while($row = mysqli_fetch_array($result)){
										echo "		<option value='".$row['equipmentID']."'>".$row['equipmentID']."</option>";
									}
									echo "		</select>";
									echo"	</div>";
									
									if(!empty($_POST['EID'])){
										$selected = $_POST['EID'];
										$_SESSION['EID'] = $selected;
										$sqlQuery = "SELECT * FROM `equipmentid` WHERE `equipmentID` = '$selected'";
										$result = mysqli_query($link, $sqlQuery);
										$row = mysqli_fetch_array($result);
										
										echo "	<br>
												<div>
													Equipment ID: ".$row['equipmentID']."
												</div>";
										echo "	<div>
													Equipment Nickname: ".$row['nickname']."
													<tab1>
														<input size='16' maxlength='16' type='text' name='nickname' placeholder='Update Nickname'/>
													</tab1>
												</div>";
										echo "	<div>
													Plants: ".$row['plants']."
													<tab1>
														<input size='35' maxlength='31' type='text' name='plants' placeholder='Update Plants'/>
													</tab>
												</div>";
										echo "	<div>
													Date Planted: ".$row['datePlanted']."&nbsp;
													<tab1>
														<input size='35' maxlength='31' type='text' name='datePlanted' placeholder='Update Date Planted'/>
													</tab>												</div>";
										if($row['counterTDS'] <= 10){
											echo "	<div>
														<p class='bg-danger'><strong>Remaining TDS Solution: ".$row['counterTDS']."
															<tab1>
																<input type='checkbox' name='resetTDS' value='true'> Reset Soultion Amount
															</tab1>
														</strong></p>
													</div>";
										}else{
											echo "	<div>
														Remaining TDS Solution: ".$row['counterTDS']."
														<tab1>
															<input type='checkbox' name='resetTDS' value='true'> Reset Soultion Amount
														</tab1>
													</div>";
										}
										echo "	<div>
													TDS high end Setting: ".$row['settingTdsHigh']."
													<tab1>
														<input min = '0' max = '2100' type='number' name='settingTdsHigh'/>
													</tab1>
												</div>
												<div>
													TDS low end Setting: ".$row['settingTdsLow']."&nbsp;
													<tab1>
														<input min = '0' max = '2100' type='number' name='settingTdsLow'/>
													</tab1>
												</div>";
										if($row['counterPH'] <= 10){
											echo "	<div>
														<p class='bg-danger'><strong>Remaining pH Solution: ".$row['counterPH']."
															<tab1>
																<input type='checkbox' name='resetPH' value='true'> Reset Soultion Amount
															</tab1>
														</strong></p>
													</div>";
										}else{
											echo "	<div>
														Remaining pH Solution: ".$row['counterPH']."
														<tab1>
															<input type='checkbox' name='resetPH' value='true'> Reset Soultion Amount
														</tab1>
													</div>";
										}
										echo "	<div>
													pH high end Setting: ".$row['settingPhHigh']."
													<tab1>
														<input min = '0' max = '14' step='any' type='number' name='settingPhHigh'/>
													</tab1>
												</div>
												<div>
													pH low end Setting: ".$row['settingPhLow']."&nbsp;
													
													<tab1>
														<input min = '0' max = '14' step='any' type='number' name='settingPhLow'/>
													</tab1>
												</div>";
										if($row['counterFlowering'] <= 10){
											echo "	<div>
														<p class='bg-danger'><strong>Remaining Flowering Solution: ".$row['counterFlowering']."
															<tab1>
																<input type='checkbox' name='resetFlowering' value='true'> Reset Soultion Amount
															</tab1>
														</strong></p>
													</div>";
										}else{
											echo "	<div>
														Remaining Flowering Solution: ".$row['counterFlowering']."
														<tab1>
															<input type='checkbox' name='resetFlowering' value='true'> Reset Soultion Amount
														</tab1>
													</div>";
										}
										if($row['flowering']){
											echo "	<div>
														Flowering mode is currently on
														<tab1><input type='radio' name='flowering' value='turnLightOn'>Turn on</tab1>
														<tab1><input type='radio' name='flowering' value='turnLightOff'>Turn off</tab1>
													</div>";
										}else{
											echo "	<div>
														Flowering mode is currently off
														<tab1><input type='radio' name='flowering' value='turnFloweringOn'>Turn on</tab1>
														<tab1><input type='radio' name='flowering' value='turnFloweringOff'>Turn off</tab1>
													</div>";
										}
										if($row['led']){
											echo "	<div>
														LED lighting is currently on
														&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
														<tab1><input type='radio' name='light' value='turnLightOn'>Turn on</tab1>
														<tab1><input type='radio' name='light' value='turnLightOff'>Turn off</tab1>
													</div>
													<div>
														Lights are set to turn on at ".$row['lightOnTime']."&nbsp;
														<tab1>
															<input min = '0' max = '23' type='number' name='lightOnTimehours' placeholder='Hours'/>  : 
															<input min = '0' max = '59' type='number' name='lightOnTimeminutes' placeholder='Minutes'/> : 
															<input min = '0' max = '59' type='number' name='lightOnTimesec' placeholder='Sec'/>
														</tab1>
													</div>
													<div>
														Lights are set to turn off at ".$row['lightOffTime']."&nbsp;
														<tab1>
															<input min = '0' max = '23' type='number' name='lightOffTimehours' placeholder='Hours'/>  : 
															<input min = '0' max = '59' type='number' name='lightOffTimeminutes' placeholder='Minutes'/> : 
															<input min = '0' max = '59' type='number' name='lightOffTimesec' placeholder='Sec'/>
														</tab1>
													</div>";
										}else{
											echo "	<div>
														LED lighting is currently off
														&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
														<tab1><input type='radio' name='light' value='turnLightOn'>Turn on</tab1>
														<tab1><input type='radio' name='light' value='turnLightOff'>Turn off</tab1>
													</div>
													<div>
														<tab1>
															Lights are set to turn on at ".$row['lightOnTime']."&nbsp;
															<tab1>
																<input min = '0' max = '23' type='number' name='lightOnTimehours' placeholder='Hours'/>  : 
																<input min = '0' max = '59' type='number' name='lightOnTimeminutes' placeholder='Minutes'/> : 
																<input min = '0' max = '59' type='number' name='lightOnTimesec' placeholder='Sec'/>
															</tab1>
														</tab>
													</div>
													<div>
														<tab1>
															Lights are set to turn off at ".$row['lightOffTime']."&nbsp;
															<tab1>
																<input min = '0' max = '23' type='number' name='lightOffTimehours' placeholder='Hours'/>  : 
																<input min = '0' max = '59' type='number' name='lightOffTimeminutes' placeholder='Minutes'/> : 
																<input min = '0' max = '59' type='number' name='lightOffTimesec' placeholder='Sec'/>
															</tab1>
														</tab1>
													</div>
													<br>";
										}
										echo "
												<center><button class='btn btn-default' type='submit' name='UpdateSetting' value = 'Update'>Update Equipment Settings</button></center>
												</div>
												<center><button class='btn btn-default' type='submit' name='displaySettings' value = 'Display'>Display Equipment Settings</button></center>
										";
									}else{
										echo "</div>
										<center><button class='btn btn-default' type='submit' name='displaySettings' value = 'Display'>Display Equipment Settings</button></center>";
									}
									if(!empty($_POST['UpdateSetting'])){
											$selected = $_SESSION['EID'];
											$beginning = "UPDATE `equipmentid` SET ";
											$comma = 0;
											
											if(!empty($_POST['settingTdsHigh'])){
												$temp = $_POST['settingTdsHigh'];
												$beginning .= "`settingTdsHigh`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['settingTdsLow'])){
												$temp = $_POST['settingTdsLow'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`settingTdsLow`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['settingPhHigh'])){
												$temp = $_POST['settingPhHigh'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`settingPhHigh`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['settingPhLow'])){
												$temp = $_POST['settingPhLow'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`settingPhLow`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['resetTDS'])){
												$temp = 300;
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`counterTDS`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['resetPH'])){
												$temp = 300;
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`counterPH`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['resetFlowering'])){
												$temp = 300;
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`counterFlowering`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['lightOnTimehours']) && !empty($_POST['lightOnTimeminutes']) && !empty($_POST['lightOnTimesec'])){
												$tempHours = $_POST['lightOnTimehours'];
												$tempMinutes = $_POST['lightOnTimeminutes'];
												$tempSeconds = $_POST['lightOnTimesec'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`lightOnTime`= '".$tempHours.':'.$tempMinutes.':'.$tempSeconds."'";
												$comma = 1;
											}
											if(!empty($_POST['lightOffTimehours']) && !empty($_POST['lightOffTimeminutes']) && !empty($_POST['lightOffTimesec'])){
												$tempHours = $_POST['lightOffTimehours'];
												$tempMinutes = $_POST['lightOffTimeminutes'];
												$tempSeconds = $_POST['lightOffTimesec'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`lightOffTime`= '".$tempHours.':'.$tempMinutes.':'.$tempSeconds."'";
												$comma = 1;
											}
											if(!empty($_POST['light'])){
												if($_POST['light'] === "turnLightOn"){
													if($comma === 1){
														$beginning .= ", ";
														$comma = 0;
													}
													$beginning .= "`led`= '1'";
													$comma = 1;
												}else{
													if($comma === 1){
														$beginning .= ", ";
														$comma = 0;
													}
													$beginning .= "`led`= '0'";
													$comma = 1;
											}}
											if(!empty($_POST['flowering'])){
												if($_POST['flowering'] === "turnFloweringOn"){
													if($comma === 1){
														$beginning .= ", ";
														$comma = 0;
													}
													$beginning .= "`flowering`= '1'";
													$comma = 1;
												}else{
													if($comma === 1){
														$beginning .= ", ";
														$comma = 0;
													}
													$beginning .= "`flowering`= '0'";
													$comma = 1;
											}}
											if(!empty($_POST['nickname'])){
												$temp = $_POST['nickname'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`nickname`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['datePlanted'])){
												$temp = $_POST['datePlanted'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`datePlanted`= '".$temp."'";
												$comma = 1;
											}
											if(!empty($_POST['plants'])){
												$temp = $_POST['plants'];
												if($comma === 1){
													$beginning .= ", ";
													$comma = 0;
												}
												$beginning .= "`plants`= '".$temp."'";
											}
											$string = "";
											$string .= $beginning;
											$string .= " WHERE `userName`= '$userName' AND `equipmentID`='$selected'";
											
											$result = mysqli_query($link, $string);
											mysqli_close($link);
										}
								?>
						</div>
					</form>
				</div>		
			</div>	
		</div>
		<div class="row">
			<div class="col-md-6 col-md-offset-3">
				<div class="panel panel-primary">
					<div class="panel-heading" style="background-color: #2D7A00;">
						Remove Equipment ID
					</div>
					<form method="POST" />
						<div class="panel-body">						
							<div class="form-group">
								<center><label for="newEvent-name">Remove Equipment ID</label></center>
								<center>
									<?php
										$sqlQuery = "SELECT `equipmentID` FROM `equipmentid` WHERE `userName` = '$userName'";
										$result2 = mysqli_query($link, $sqlQuery);
										
										echo "	<div class='dropdown'>";
										echo"		<select class='btn btn-default' name='removeEID'>";
										echo "			<option size =100 ></option>";
										while($row = mysqli_fetch_array($result2)){
											echo "		<option value='".$row['equipmentID']."'>".$row['equipmentID']."</option>";
										}
										echo "		</select>";
										echo"	</div>";
										
										if(!empty($_POST['removeEID'])){
											$EID = $_POST['removeEID'];
											$sqlQuery = "UPDATE `equipmentid` SET `userName`= '' WHERE `equipmentID` = '$EID'";
											$result2 = mysqli_query($link, $sqlQuery);
											if($result2){
												mysqli_close($link);
												header('Location:equipmentRemoved.php');
											}
											else{
												echo "The equipment was not removed please check your input and try again.";
												mysqli_close($link);
											}
										}
									?>
								</center>
							</div>
							<center><button class='btn btn-default' type='submit' name='removeEquiptmentID'>Remove Equipment ID</button></center>
						</div>
					</form>
				</div>	
			</div>
		</div>
		
		<center>
			<div class="btn-group" role="group" aria-label="..." style="float:center">
				<a href="userHome.php" class="btn btn-default">User Home Page</a>
				</div>
		</center>
		<?php
			include_once "footer.php";
		?>
	</body>
</html>
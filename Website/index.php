<?php
	require_once('config.php');
	$link = new mysqli(DBHOST, DBUSER, DBPASS, DBNAME) or die("Unable to connect!");
	session_start();
?>

<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=windows-1252">
		
		<script type="text/javascript" src="content/jquery/jquery-3.0.0.min.js"></script>
		<link href="content/bootstrap/css/bootstrap.min.css" rel="stylesheet">
		<script src="https://use.fontawesome.com/808114f81f.js"></script>
		
		<script src="content/scripts/login.js"></script>
	</head>
	<body>	
		<div id="header"></div>
		<?php
			$_SESSION['current'] = false;
			include_once ('header.php');
		?>
		</div>
		<div class="container-fluid">
			<div class="row">
				<div class="col-md-4 col-md-offset-2">
					<div class="panel panel-primary">	
						<div class="panel-heading">
							Existing Users
						</div>
						<div class="panel-body">
							<form method="POST" />
								<div class="form-group">
									<label for="login-id">User Name</label>
									<input size="32" maxlength="16" type="text" class="form-control" name="login-id" placeholder="Please enter your user name">
								</div>
								<div class="form-group">
									<label for="login-password">Password</label>
									<input size="32" maxlength="16" type="password" class="form-control" name="login-password" placeholder="Please enter your password">
								</div>
								
								<div class="col-sm-6">
									<button class="btn btn-primary" type="submit" name="login-button">Login</button>
								</div>								
								
							</form>
							<?php
								if(!empty($_POST['login-id']) && !empty($_POST['login-password'])){
									$inputUser = $_POST['login-id'];
									$inputPassword = $_POST['login-password'];
									
									$sqlQuery = "SELECT* FROM `users` WHERE `userName` = '$inputUser'";
									
									$result = mysqli_query($link, $sqlQuery);
									$UID = mysqli_fetch_array($result);
									mysqli_close($link);
									
									if ($UID['userPassword'] === $inputPassword){
										$_SESSION['userLoggedIn'] = $inputUser;
										$_SESSION['current'] = true;
										header('Location: userHome.php');
									}
									else{
										echo "Login Failed! Please try again.";
									}
								}
								else if(!empty($_POST['login-id'])){
									echo "Please enter your password!";
								}
								else if(!empty($_POST['login-password'])){
									echo "Please enter your User ID!";
								}
								else{}
							?>

						</div>	
						
					</div>
				</div>
				
				<div class="col-md-4">
					
					<div class="panel panel-info">
						<div class="panel-heading">
							New Users
						</div>
						
						<div class="panel-body">
							<form method="POST" />
								<div class="form-group">
									<label for="newUser-name">User Name</label>
									<input size="32" maxlength="16" type="text" class="form-control" name="newUser-name" placeholder="Please pick a user name">
								</div>									
								
								<div class="form-group">
									<label for="newUser-password">Password</label>
									<input size="32" maxlength="16" type="password" class="form-control" name="newUser-password" placeholder="Please pick a password">
								</div>
								
								<button class="btn btn-default" type="submit" name="register-button">Register</button>
							</form>
							<?php
								if(!empty($_POST['newUser-name']) && !empty($_POST['newUser-password'])){
									$inputUser = $_POST['newUser-name'];
									$inputPassword = $_POST['newUser-password'];
									$sqlQuery = "INSERT INTO `users` (`userName`, `userPassword`) VALUES ('$inputUser', '$inputPassword')";
									$result = mysqli_query($link, $sqlQuery);
									mysqli_close($link);
									if ($result)
										header('Location: accountRegistered.php');
									else
										echo "That user name is already taken. Please try another.";
							}
							else if(!empty($_POST['newUser-name']) && empty($_POST['newUser-password']))
								echo "Please enter your password name.";
							else if(empty($_POST['newUser-name']) && !empty($_POST['newUser-password']))
								echo "Please enter your user name.";
							?>
						</div>	
					</div>					
				</div>
			</div>
		</div>
		<?php
			include_once('footer.php');
		?>
	</body>
	
</html>
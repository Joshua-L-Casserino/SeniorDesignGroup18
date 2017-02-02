<?php
	session_start();
	if(!$_SESSION['current'])
		header('Location: noPermissions.php');
	$userName = $_SESSION['userLoggedIn'];
	$user = ' id587274_group18@2a02:4780:bad:c0de::13';
	$password = '';
	$db = 'id587274_homehydroponicsystem';
	$link = new mysqli('localhost', $user, $password, $db) or die("Unable to connect!");
?>

<html>

	<head>
		<meta http-equiv="content-type" content="text/html; charset=windows-1252">
		<script type="text/javascript" src="content/jquery/jquery-3.0.0.min.js"></script>
		<link href="content/bootstrap/css/bootstrap.min.css" rel="stylesheet">
		<script src="https://use.fontawesome.com/808114f81f.js"></script>
	</head>
	
	<body>
		<?php
			include_once('header.php');
		?>
		<div class="container-fluid">
			<div class="row">
				<div class="col-md-6 col-md-offset-3">
					<div class="panel panel-danger">
						<div class="panel-heading">
							Equipment ID Added!
						</div>
						<div class="panel-body">
							Your account has been updated with the equipment ID!<br><br>
							<a href='userProfile.php' class='btn btn-primary'>Return to User Profile page.</a>
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
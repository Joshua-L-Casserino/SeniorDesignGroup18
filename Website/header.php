<div class="container-fluid">
	<div class="row" style="background-color: #2D7A00; height:80px; border-bottom: thick solid #2D7A00; 
				border-right: thick solid #2D7A00; 
				margin-bottom: 15px;">
		<div class="col-md-12" style=" height: 100%;">
			<div id="header" class="row" >
				<div class="col-md-6" style="color: white;">
					<h1><i class="" aria-hidden="true"></i> Home Hydroponics System  </h1>
				</div>
				<?php
					if($_SESSION['current']){
						$userName = $_SESSION['userLoggedIn'];
						echo "	<div class='col-md-6'  id='current-user-listing'>
									<div style='float: right; color:white; margin-top:10px; margin-right:10px;'>
										<div style='float: right; class='col-md-6'  id='current-user-listing'>
											";
						echo "Username: ".$userName;
						echo "				
											<form action= 'logOut.php' /> 
												<div style='float: right; color:black;'>
													<button class='btn btn-primary' type='submit' name='logout-button'>
														Log Out
													</button>
												</div>
											</form>
										</div>
									</div>
								</div>
							";
					}
				?>
			</div>
		</div>
	</div>
</div>
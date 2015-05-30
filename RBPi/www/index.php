<?php 
	if(isset($_GET['trigger']) && $_GET['trigger'] == 1) {
		error_reporting(E_ALL);
		exec('gpio write 7 0');
		usleep(1000000);
		exec('gpio write 7 1');
	}
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Smart Garage</title>
		<link rel="stylesheet" href="css/original.css" type="text/css">
		<meta name="apple-mobile-web-app-capable" content="yes">	
		<script type="text/javascript" src="js/jquery-1.10.2.min.js"></script>    
		<script type="text/javascript" src="js/script.js"></script>

	</head>
	<body>
		<h1 id="W" >Welcome</h1>
		<h1 id="W" >Home</h1>
		
		<div class="loadingDiv">
			<img id="loadingImg" style="display:none" src = "loading.gif">
			<p id="loadingP" style="display:none">Loading...</p>
		</div>
		<div class='awrap'>
			<a href='?trigger=1'>Push To Open</a>
		</div>
	</body>
</html>

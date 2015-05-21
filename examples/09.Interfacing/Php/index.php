<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" 
   "http://www.w3.org/TR/html4/strict.dtd">
 
<html>
	<head>
		<meta http-equiv="refresh" content="1">
		<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	</head>
 
	<body>
		<p>The Temperature in my Office is :</p>
		<?php
			if ($handle = fopen("/dev/ttyACM0", "rb"))	// we just read data
			{
				$data = fread($handle, 8);		// we get 8 bytes from our device
				print "<center><h1>";
				print $data;
				print "</h1></center>";
				fclose($handle);
			}
		?>
	</body>
</html>


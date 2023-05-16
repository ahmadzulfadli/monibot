<?php
require 'koneksi/konek.php';
$app = new Monibot;
$data = $app->garfik_data();
?>

<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>monibot</title>
	<link rel="shortcut icon" href="assets/image/logo1.svg">
	<!-- halfmoon css -->
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/halfmoon/css/halfmoon.min.css">
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<link rel="stylesheet" href="assets/css/style.css">

	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<!-- halfmoon js -->
	<script src="https://cdn.jsdelivr.net/npm/halfmoon/js/halfmoon.min.js"></script>
</head>

<body class="with-custom-webkit-scrollbars with-custom-css-scrollbars">
	<!-- page wrapper with sidebar -->
	<div id="page-wrapper" class="page-wrapper with-navbar with-sidebar with-transitions" data-sidebar-type="overlayed-sm-and-down">
		<!-- Navbar (immediate child of the page wrapper) -->
		<nav class="navbar">
			<!-- Navbar content (with toggle sidebar button) -->

			<button class="btn btn-action " onclick="halfmoon.toggleSidebar()" type="button" style="background-color: #5e81ac;"></button>

			<!-- Navbar brand -->
			<a href="#" class="navbar-brand ">
				MONIBOT
			</a>
			<!-- Navbar text -->
			<span class="navbar-text text-monospace">Teknik Elektro USR</span>
		</nav>
		<div class="sidebar-overlay" onclick="halfmoon.toggleSidebar()"></div>
		<!-- sidebar -->
		<div class="sidebar">
			<div class="sidebar-menu">
				<center>
					<img src="assets/image/logo.svg" width="100px" height="100px" style="border-radius: 40%; margin-top: 10px;">
					<h4>MONIBOT</h4>
				</center>
				<!-- Dashboard table and graph -->
				<ul class="nav nav-pills nav-stacked">
					<li><a href="index.php">Home</a></li>
					<li><a href="tabel.php">Tabel</a></li>
					<li class="active"><a href="#">Grafik</a></li>
				</ul><br>
			</div>
		</div>
		<!-- end of sidebar -->
		<!-- content wrapper -->
		<div class="content-wrapper">

			<!-- content -->
			<div class="content">
				<div class="col-lg-12">
					<div class="card">
						<div id="chart"></div>
					</div>
				</div>
			</div>
			<!-- end of content -->
		</div>
		<!-- end of content wrapper -->
	</div>
	</div>
	<!-- end of page wrapper with sidebar -->
	<script src="https://cdn.jsdelivr.net/npm/apexcharts"></script>
	<script>
		var options = {
			chart: {
				type: 'area',
				height: 500,
				toolbar: {
					show: true,
					tools: {
						download: true,
						selection: true,
						zoom: true,
						zoomin: true,
						zoomout: true,
						pan: true
					}
				}
			},
			colors: ['#ff0000', '#00ff00', '#0000ff'],
			series: [{
				name: 'temp',
				data: []
			}, {
				name: 'humd',
				data: []
			}, {
				name: 'ppmch2',
				data: []
			}, {
				name: 'ppmco2',
				data: []
			}],
			xaxis: {
				categories: []
			},
			yaxis: {
				title: {
					text: 'Nilai Sensor'
				}
			},
			legend: {
				position: 'top',
				horizontalAlign: 'left'
			},
			dataLabels: {
				enabled: false
			},
			markers: {
				size: 1,
				hover: {
					size: 2
				}
			},
			tooltip: {
				shared: true
			},
			animation: {
				easing: 'easeinout',
				speed: 3000
			},
			title: {
				text: 'Grafik Sensor Monibot',
				align: 'center',
				margin: 20,
				style: {
					fontSize: '24px',
					fontWeight: 'bold',
					fontFamily: 'Helvetica, Arial, sans-serif',
					color: '#333'
				}
			}
		};

		var categories = [];
		var Sensor_1 = [];
		var Sensor_2 = [];
		var Sensor_3 = [];
		var Sensor_4 = [];

		<?php foreach ($data as $row) : ?>
			categories.push("<?php echo $row['timestamp']; ?>");
			Sensor_1.push(parseFloat("<?php echo $row['data_temp']; ?>"));
			Sensor_2.push(parseFloat("<?php echo $row['data_humd']; ?>"));
			Sensor_3.push(parseFloat("<?php echo $row['data_ppmch2']; ?>"));
			Sensor_4.push(parseFloat("<?php echo $row['data_ppmco2']; ?>"));
		<?php endforeach; ?>

		options.series[0].data = Sensor_1;
		options.series[1].data = Sensor_2;
		options.series[2].data = Sensor_3;
		options.series[3].data = Sensor_4;
		options.xaxis.categories = categories;

		var chart = new ApexCharts(document.querySelector("#chart"), options);
		chart.render();
	</script>

</body>

</html>
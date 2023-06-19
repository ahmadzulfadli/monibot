<?php
require 'koneksi/konek.php';

class Content
{
	private $app;

	function __construct()
	{
		$this->app = new Monibot;
	}

	public function render_table()
	{
		$search = isset($_GET['search']) ? $_GET['search'] : '';
		$data = $this->app->table_data($search);



		if ($data) {
			$num_results = $this->app->count_data();
			$last_data = $data[0];

			echo '<div class="table-responsive">
				<table class="table table-striped">
					<thead>
						<tr>
							<th>No</th>
							<th>Waktu</th>
							<th>Temperature</th>
							<th>Humidity</th>
							<th>PPMCH4</th>
							<th>PPMCO</th>
						</tr>
					</thead>
					<tbody>';

			$i = 1;
			foreach ($data as $row) {

				echo '<tr>
					<td>' . $i . '</td>
					<td>' . $row['timestamp'] . '</td>
					<td>' . $row['data_temp'] . '</td>
					<td>' . $row['data_humd'] . '</td>
					<td>' . $row['data_ppmch4'] . '</td>
					<td>' . $row['data_ppmco'] . '</td>
				</tr>';

				$i++;
			}

			echo '</tbody></table></div>';
		} else {
			echo '<p>Tidak ada data yang ditemukan.</p>';
		}
	}

	public function render_search_form()
	{
		$search = isset($_GET['search']) ? $_GET['search'] : '';

		echo '<div class="form-group mr-2">
				<label for="search">Cari Data:</label>
				<input type="text" class="form-control ml-2 mr-2" id="search" name="search" value="' . $search . '">
			</div>
			<button type="submit" class="btn btn-primary">Cari</button>';
	}
}

$content = new Content;
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
					<li class="active"><a href="#">Tabel</a></li>
					<li><a href="grafik.php">Grafik</a></li>
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
						<div class="card-body">
							<center>
								<h5 class="card-title">Data Sensor</h5>
							</center>
							<form class="form-inline float-right" method="get" action="tabel.php">
								<?php $content->render_search_form() ?>
							</form>
							<table class="table table-striped">
								<?php $content->render_table(); ?>
							</table>
							<a href="tabel_detail.php"><button type="button" class="btn btn-primary">Lihat Semua</button></a>
						</div>
					</div>
				</div>
			</div>
			<!-- end of content -->
		</div>
		<!-- end of content wrapper -->
	</div>
	<script src="https://cdn.jsdelivr.net/npm/halfmoon/js/halfmoon.min.js"></script>
</body>

</html>
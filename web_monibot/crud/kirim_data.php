<?php

//include library utama
include_once '../koneksi/konek.php';

// Buat Instance baru
$app = new Monibot();

// Baca query dari alamat
$app->query_string = $_SERVER['QUERY_STRING'];

// Cek apakah ada query bernama mode?
if ($app->is_url_query('mode')) {

    // Bagi menjadi beberapa operasi
    switch ($app->get_url_query_value('mode')) {

        default:
            $app->read_json();

        case 'save':
            if ($app->is_url_query('temp') && $app->is_url_query('humd') && $app->is_url_query('ppmch4') && $app->is_url_query('ppmco')) {
                $temp = $app->get_url_query_value('temp');
                $humd = $app->get_url_query_value('humd');
                $ppmch4 = $app->get_url_query_value('ppmch4');
                $ppmco = $app->get_url_query_value('ppmco');
                $app->create_data($temp, $humd, $ppmch4, $ppmco);
            } else {
                $error = [
                    'temp' => 'required',
                    'humd' => 'required',
                    'ppmch4' => 'required',
                    'ppmco' => 'required',
                ];
                echo $app->error_handler($error);
            }
            break;
        case 'delete':
            if ($app->is_url_query('id')) {
                $id = $app->get_url_query_value('id');
                $app->delete_data($id);
            } else {
                $error = [
                    'id' => 'required',
                ];
                echo $app->error_handler($error);
            }
            break;
    }
} else {
    $app->read_json();
}

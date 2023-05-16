<?php
class Monibot
{

    private $connection;
    public $query_string;

    function __construct()
    {
        $this->cors();
        $this->connection = $this->open_connection();
    }


    /**
     * CRUD Section
     * This section for CRUD Operation
     * Create - Read - Update - Delete
     */
    function create_data($temp, $humd, $ppmch2, $ppmco2)
    {
        $sql_query = "INSERT INTO data_monibot (data_temp, data_humd, data_ppmch2, data_ppmco2) VALUES ('" . $temp . "','" . $humd . "','" . $ppmch2 . "','" . $ppmco2 . "')";
        return $this->execute_query($sql_query);
    }

    function read_json()
    {
        $sql_query = "SELECT * FROM data_monibot ORDER BY timestamp DESC";
        echo $this->execute_query($sql_query, [], true);
    }

    function read_data()
    {
        $sql_query = "SELECT * FROM data_monibot ORDER BY timestamp DESC";
        $result = $this->connection->query($sql_query);

        if ($result->num_rows > 0) {
            $data = array();
            while ($row = $result->fetch_assoc()) {
                $data[] = $row;
            }

            return $data;
        } else {
            return false;
        }
    }

    function table_data($search)
    {
        $sql_query = "SELECT * FROM data_monibot";
        if (empty($search)) {
            $sql_query .= " ORDER BY timestamp DESC LIMIT 10";
        } else {
            $sql_query .= " WHERE temp LIKE '%$search%' OR humd LIKE '%$search%' OR ppmch2 LIKE '%$search%'OR ppmco2 LIKE '%$search%'";
        }

        $result = $this->connection->query($sql_query);
        if ($result->num_rows > 0) {
            $data = array();
            while ($row = $result->fetch_assoc()) {
                $data[] = $row;
            }

            return $data;
        } else {
            return false;
        }
    }

    function table_data_detail($search)
    {
        $sql_query = "SELECT * FROM data_monibot";
        if (empty($search)) {
            $sql_query .= " ORDER BY timestamp DESC";
        } else {
            $sql_query .= " WHERE temp LIKE '%$search%' OR humd LIKE '%$search%' OR ppmch2 LIKE '%$search%'OR ppmco2 LIKE '%$search%'";
        }

        $result = $this->connection->query($sql_query);
        if ($result->num_rows > 0) {
            $data = array();
            while ($row = $result->fetch_assoc()) {
                $data[] = $row;
            }

            return $data;
        } else {
            return false;
        }
    }

    function get_arah($arah_id)
    {
        $query = "SELECT nama FROM arah WHERE id = $arah_id";
        $result = mysqli_query($this->connection, $query);
        $row = mysqli_fetch_assoc($result);
        return $row['nama'];
    }

    function get_status($status_id)
    {
        $query = "SELECT nama FROM status WHERE id = $status_id";
        $result = mysqli_query($this->connection, $query);
        $row = mysqli_fetch_assoc($result);
        return $row['nama'];
    }

    function garfik_data()
    {
        $sql_query = "SELECT * FROM data_monibot";
        $sql_query .= " ORDER BY timestamp DESC LIMIT 15";
        $result = $this->connection->query($sql_query);

        if ($result->num_rows > 0) {
            $data = array();
            while ($row = $result->fetch_assoc()) {
                $data[] = $row;
            }

            return array_reverse($data);
        } else {
            return false;
        }
    }

    function count_data()
    {
        $sql_query = "SELECT * FROM data_monibot ORDER BY timestamp DESC";
        $result = $this->connection->query($sql_query);
        $amount_data = $result->num_rows;
        return $amount_data;
    }




    /* function update_data($id, $update_data = [])
    {
        $sql_query = "UPDATE data_monibot SET " . $update_data['column'] . " = " . $update_data['value'] . " WHERE id = " . $id . "";
        $this->execute_query($sql_query);
    } */

    function delete_data($id)
    {
        $sql_query = "DELETE FROM data_monibot WHERE id = " . $id . "";
        echo $this->execute_query($sql_query);
    }

    // Connection Section
    // This section for RDBMS Operation Only


    function error_handler($params = [])
    {
        $data = [];
        foreach ($params as $param => $rules) {
            $data[$param] = $rules;
        }
        $data['status'] = false;
        $data['message'] = 'error on operation';
        return json_encode($data);
    }

    function is_url_query($string_value)
    {
        $query = array();
        parse_str($this->query_string, $query);
        if (array_key_exists($string_value, $query)) {
            return true;
        }
        return false;
    }

    function get_url_query_value($string_value)
    {
        $query = array();
        parse_str($this->query_string, $query);
        return $query[$string_value];
    }

    /**
     * Connection Section
     * This section for RDBMS Operation Only
     * 
     */

    private function open_connection()
    {
        $servername = "localhost";
        $username = "root";
        $password = "Kucinghitam123";
        $dbname = "monibot";
        $conn = new mysqli($servername, $username, $password, $dbname) or die("Failed connect: %s\n" . $conn->error);
        return $conn;
    }

    private function close_connection()
    {
        $this->connection->close();
    }

    private function execute_query($sql, $data = [], $is_read = null)
    {
        $executed = $this->connection->query($sql);
        if ($executed == TRUE) {
            $data['status'] = true;
            $data['message'] = 'data operation success';

            if (!is_null($is_read) && $is_read) {
                $data['data'] = [];
                if ($executed->num_rows != 0) {
                    while ($row = $executed->fetch_assoc()) {
                        $data['data'][] = $row;
                    }
                }
            }

            return json_encode($data);
        }

        $data['status'] = false;
        $data['message'] = 'data operation failed';
        return json_encode($data);
    }

    private function cors()
    {
        // Allow from any origin
        if (isset($_SERVER['HTTP_ORIGIN'])) {
            // Decide if the origin in $_SERVER['HTTP_ORIGIN'] is one
            // you want to allow, and if so:
            header("Access-Control-Allow-Origin: {$_SERVER['HTTP_ORIGIN']}");
            header('Access-Control-Allow-Credentials: true');
            header('Access-Control-Max-Age: 86400');    // cache for 1 day
        }

        // Access-Control headers are received during OPTIONS requests
        if ($_SERVER['REQUEST_METHOD'] == 'OPTIONS') {

            if (isset($_SERVER['HTTP_ACCESS_CONTROL_REQUEST_METHOD']))
                // may also be using PUT, PATCH, HEAD etc
                header("Access-Control-Allow-Methods: GET, POST, OPTIONS");

            if (isset($_SERVER['HTTP_ACCESS_CONTROL_REQUEST_HEADERS']))
                header("Access-Control-Allow-Headers: {$_SERVER['HTTP_ACCESS_CONTROL_REQUEST_HEADERS']}");

            exit(0);
        }
    }
}

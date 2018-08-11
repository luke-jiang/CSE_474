<?php
    // CSE 474
    // Final Project
    
    // This is the PHP file for front-end display
    // Send new coordinates to the database and retrieve averaged coordinates
    // from the database.
    
    
    $xcoord = $_GET["xcoord"];
    $ycoord = $_GET["ycoord"];
    
    // Set up connection to mysql database.
    $servername = getenv('IP');
    $username = getenv('C9_USER');
    $password = "";
    $database = "theDatabase";
    $dbport = 3306;

    // Create connection.
    $db = new mysqli($servername, $username, $password, $database, $dbport);

    // Check connection.
    if ($db->connect_error) {
        die("Connection failed: " . $db->connect_error);
    } 
    //echo "Connected successfully (".$db->host_info.")";
    
    // Query from database.
    $db->query("INSERT INTO coords (x, y) VALUES ('$xcoord', '$ycoord');");
    $x_query = $db->query("SELECT AVG(x) FROM coords;");
    $x_result = $x_query->fetch_assoc();
    $y_query = $db->query("SELECT AVG(y) FROM coords;");
    $y_result = $y_query->fetch_assoc();
    
    // Store queried coordinates and parse into JSON.
    header('Content-Type: application/json');
    $avg = array();
    $avg["xcoord"] = $x_result["AVG(x)"];
    $avg["ycoord"] = $y_result["AVG(y)"];
    $output = json_encode($avg);
    print($output);

?>
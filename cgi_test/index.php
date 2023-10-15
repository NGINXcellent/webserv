<?php

# this is a commentary 

header("Content-Type: text/html");

$name = getenv('USER_NAME');
$server = getenv('WEBSERV');

echo "Hello ${name} to the amazing ${server}";
?>

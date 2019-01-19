<?php
header("content-type:text/html;charset:utf-8");
echo "<br>[PHP] <br>";
var_dump($_SERVER['REQUEST_METHOD']);
//var_dump($_SERVER['REQUEST_URI']);
//echo " <br>";

if (($stream = fopen('php://input', "r")) !== FALSE){
    var_dump(stream_get_contents($stream));
	echo " <br>";
}

 $name = $_GET["filename"];
 $path = "";
 if(strripos($name, "\\")){
	 $path = substr($name, 0, strripos($name, "\\"));
 }
 else if(strripos($name, "/")){
	 $path = substr($name, 0, strripos($name, "/"));
 }
 $filename = $name;
 echo "Path: ".$path." <br>";
 echo "File: ".$filename." <br>";

 if(file_exists($filename)){
	 if(unlink($filename)){
		 echo "Delete (".$filename.") success. <br>";
	 }
	 else{
		 echo "Delete (".$filename.") failed. <br>";
	 }
 }
 else{
	 echo "Delete (".$filename.") failed - file not exist. <br>";
 }
?>
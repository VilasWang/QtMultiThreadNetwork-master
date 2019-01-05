<?php
 header("content-type:text/html;charset:utf-8");
 echo "[PHP] <br>";
 var_dump($_SERVER['REQUEST_METHOD']);
 echo " <br>";

 /*$str = isset($GLOBALS['HTTP_RAW_POST_DATA']) ? $GLOBALS['HTTP_RAW_POST_DATA'] : file_get_contents("php://input");*/
 $str = file_get_contents("php://input");
 $targetname = $_GET["filename"];
 $targetpath = "";
 if(strripos($targetname, "\\")){
	 $targetpath = substr($targetname, 0, strripos($targetname, "\\"));
 }
 else if(strripos($targetname, "/")){
	 $targetpath = substr($targetname, 0, strripos($targetname, "/"));
 }
 //判断该用户文件夹是否已经有这个文件夹  
 if(strlen($targetpath) > 0 && !file_exists($targetpath)) {  
	mkdir($targetpath); 
 }
 $filename = $targetname;
 if(file_exists($filename)){
	 $temp_name=time().rand(1,1000).substr($targetname, strrpos($targetname, "."));
	 $filename=$targetpath."/".$temp_name;
	 echo "Warning: The file already exists, will try to save as ".$temp_name." <br>";
 }
 echo "Path: ".$targetpath." <br>";
 echo "File: ".$filename." <br>";
 $fp = fopen($filename,'w+');
 fwrite($fp, $str, strlen($str));
 fclose($fp);
 echo "Result: success <br>";
?>
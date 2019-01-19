<?php
 header("content-type:text/html;charset:utf-8");
 echo "<br>[PHP] <br>";
 //var_dump($_SERVER['REQUEST_METHOD']);
 //echo " <br>";

 /*$str = isset($GLOBALS['HTTP_RAW_POST_DATA']) ? $GLOBALS['HTTP_RAW_POST_DATA'] : file_get_contents("php://input");*/
 $str = file_get_contents("php://input");
 $rootPath = "../";
 $targetfile = $_GET["filename"];
 $targetpath = $rootPath;
 if(strripos($targetfile, "\\")){
	 $targetpath = $rootPath.substr($targetfile, 0, strripos($targetfile, "\\"));
 }
 else if(strripos($targetfile, "/")){
	 $targetpath = $rootPath.substr($targetfile, 0, strripos($targetfile, "/"));
 }
 //判断该用户文件夹是否已经有这个文件夹  
 if(strlen($targetpath) > 0 && !file_exists($targetpath)){  
	mkdir($targetpath); 
 }
 $filename = $rootPath.$targetfile;
 if(file_exists($filename)){
	 $temp_name=time().rand(1,1000).substr($targetfile, strrpos($targetfile, "."));
	 $filename=$targetpath."/".$temp_name;
	 echo "Warning: The file already exists, will try to save as ".$temp_name." <br>";
 }
 $fp = fopen($filename,'w+');
 $fname = $targetfile;
 if(strripos($targetfile, "\\")){
	 $fname = substr($targetfile, strrpos($targetfile, "\\")+1);
 }
 else if(strripos($targetfile, "/")){
	 $fname = substr($targetfile, strrpos($targetfile, "/")+1);
 }
 if($fp!=FALSE && fwrite($fp, $str, strlen($str))){
	fclose($fp);
	echo $fname." upload success. <br>";
 }
 else{
	echo $fname." upload failed. <br>";
 }
?>
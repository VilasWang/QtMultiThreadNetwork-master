<?php 
header("Content-type:text/html;charset=utf-8"); 
$filename=$_GET["filename"];
$file_name=iconv("utf-8","gb2312",$filename); 
$file_sub_path=$_SERVER['DOCUMENT_ROOT']."/resource/"; 
$file_path=$file_sub_path.$file_name; 
echo $file_path;
if(!file_exists($file_path)){ 
	echo "404 not found!"; 
	return; 
} 
$fp=fopen($file_path,"r"); 
$file_size=filesize($file_path);
Header("Content-type: application/octet-stream"); 
Header("Accept-Ranges: bytes"); 
Header("Accept-Length:".$file_size); 
Header("Content-Disposition: attachment; filename=".$file_name); 
$buffer=1024; 
$file_count=0; 
while(!feof($fp) && $file_count<$file_size){ 
	$file_con=fread($fp,$buffer); 
	$file_count+=$buffer; 
	echo $file_con; 
} 
fclose($fp); 
?>

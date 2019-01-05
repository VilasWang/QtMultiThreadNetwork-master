<?php  
header("content-type:text/html;charset:utf-8");
echo "[PHP] <br>";
var_dump($_SERVER['REQUEST_METHOD']);
echo " <br>";

$fileInfo = $_FILES["sendfile"];
echo "Upload file: ".$fileInfo["name"]." <br>";
echo "Type: ".$fileInfo["type"]." <br>";
echo "Size: ".($fileInfo["size"] / 1024)." kB <br>";
echo "Temp file: ".$fileInfo["tmp_name"]." <br>";
$targetname = $_POST["filename"];
echo "Target file: ".$targetname." <br>";
$targetpath = $_POST["path"];
echo "Target path: ".$targetpath." <br>";

if ($fileInfo["error"] > 0){
    echo "Error：".$fileInfo["error"]." <br>";
}
else{
	$maxSize=512*1024*1024;
	if($fileInfo["size"] <= $maxSize){
		//判断是否上传成功（是否使用post方式上传）  
		if(is_uploaded_file($fileInfo["tmp_name"])) {
			$uploaded_file=$fileInfo["tmp_name"];

			//判断该用户文件夹是否已经有这个文件夹  
			if(strlen($targetpath) > 0 && !file_exists($targetpath)) {  
				mkdir($targetpath); 
			}
			if(strlen($targetname) <= 0){
				$targetname = $fileInfo["name"];
			}
			$move_to_file=$targetpath."/".$targetname;
			if(strlen($targetpath) <= 0){
				$move_to_file=$targetname;
			}
			if (file_exists($move_to_file)){
				$temp_name=time().rand(1,1000).substr($targetname, strrpos($targetname, "."));
				echo "Warning: The file already exists, will try to save as ".$temp_name." <br>";

				$move_to_file=$targetpath."/".$temp_name;
				if(strlen($targetpath) <= 0){
					$move_to_file=$temp_name;
				}
			}
			if (!file_exists($move_to_file)){
				if(move_uploaded_file($uploaded_file, iconv("utf-8","gb2312",$move_to_file))) {  
					echo "Result: success <br>";
				} else {  
					echo "Result: failed <br>";  
				}
			}
			else{
				echo "Result: failed - ".$targetname." already exists <br>";
			}
		} else {  
			echo "Result: failed <br>";  
		}
	} else {  
		echo "Result: failed - File cannot be larger than 512MB <br>";  
	}
}
?>

<?php
$dir = '../uploads/';
$fp = opendir($dir);
$result = array();
$json = array();

while($filename = readdir($fp)){
	if($filename != '.' && $filename != '..' && $filename != '.gitignore'){
		if(is_file($dir . $filename)){
			$file_id = str_replace('.png', '', $filename);
			$result[$file_id] = filectime($dir . $filename);
		}
	}
	arsort($result); // 更新日時が新しい順
}

$count = 0;
foreach($result as $key => $value){
	if($count >= 15){
		break;
	}
	$json[] = $key;
	$count++;
}

echo json_encode($json);

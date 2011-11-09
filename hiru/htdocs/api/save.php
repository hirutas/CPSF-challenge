<?php
$url = $_POST['url'];
$img = str_replace('data:image/png;base64,', '', $url);
$filepath = '../uploads/';
$filename = md5(microtime() + 'cpsf') . '.png';


$fp = fopen($filepath . $filename , 'w');
fwrite($fp, base64_decode($img));
fclose($fp);

echo $img;
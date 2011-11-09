<?php
$prefix = 'uploads/';
if(array_key_exists('id', $_GET)){
	$path = $_GET['id'];
	// ディレクトリトラバーサル攻撃の阻止
	$path_clean = str_replace(array('.', '/'), '', $path);
	$url = $prefix . $path_clean . '.png';
	// ファイルの存在チェック
	if(!is_file($url)){
		$url = null;
	}
}else{
	$url = null;
}
?>
<!DOCTYPE HTML>
<html lang="ja">
<head>
	<meta charset="UTF-8">
	<title>お絵かきCanvas</title>
	<link type="text/css" rel="stylesheet" href="css/common.css" />
	<link type="text/css" rel="stylesheet" href="css/show.css" />
</head>
<body>
	<header>
		<h1><a href="/oekaki">お絵かきCanvas</a></h1>
	</header>
	<div id="main">
	<?php if($url): ?>
	<img src="<?php echo $url ?>" alt="" width="500" height="500" />
	<?php else: ?>
	<p>Sorry, Not found.</p>
	<?php endif; ?>
	</div>
	<div id="social">
	<?php if($url): ?>
	<a href="https://twitter.com/share" class="twitter-share-button" data-count="horizontal" data-via="hiru_ecn" data-lang="ja">ツイート</a><script type="text/javascript" src="//platform.twitter.com/widgets.js"></script>
	<?php endif; ?>
	</div>
  <footer>
    <a href="http://www.ht.sfc.keio.ac.jp/cpsf/" target="_blank">CPSF</a> - Cyber-Physical Space Family in Keio Univ. Hide Tokuda Lab.<br />
    developed by <a href="http://twitter.com/#!/hiru_ecn" target="_blank">@hiru_ecn</a>
  </footer>
</body>
</html>

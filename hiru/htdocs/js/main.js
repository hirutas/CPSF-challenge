// オブジェクトを1つだけグローバルな名前空間に追加
new function(){
	// private variables
	// var hoge = "huga";
	myCanvasObj = {
		// public variables
		mouseX: 0,
		mouseY: 0,
		jqBody: null,
		jqCanvas: null,
		domCanvas: null,
		jqWorks: null,
		ctx: null,
		isMouseDown: false,
		works: [],
		// public functions
		initialize: function(jqb, jqc, jqw){
			// set canvas objects
			this.jqBody = jqb;
			this.jqCanvas = jqc;
			this.domCanvas = this.jqCanvas.get(0);
			this.ctx = this.domCanvas.getContext('2d');
			this.jqWorks = jqw;
			// set mouse events
			this.jqBody.mousedown(function(e){
				myCanvasObj.mouseX = e.pageX - this.offsetLeft; // コールバック関数内ではthisは使えない
				myCanvasObj.mouseY = e.pageY - this.offsetTop;
				myCanvasObj.ctx.beginPath();
				myCanvasObj.ctx.moveTo(this.mouseX, this.mouseY);
				myCanvasObj.isMouseDown = true;
				console.log('mousedown');
			});
			this.jqBody.mouseup(function(e){
				myCanvasObj.isMouseDown = false;
				console.log('mouseup');
			});
			this.jqCanvas.mouseout(function(e){
				console.log('mouseout');
			});
			this.jqCanvas.mouseover(function(e){
				myCanvasObj.ctx.beginPath();
				myCanvasObj.ctx.moveTo(this.mouseX, this.mouseY);
				console.log('mouseover');
			});
			this.jqCanvas.mousemove(function(e){
				myCanvasObj.mouseX = e.pageX - this.offsetLeft;
				myCanvasObj.mouseY = e.pageY - this.offsetTop;
				myCanvasObj.drawLine();
			});
			myCanvasObj.refleshWorks();
			console.log('initialized');
		},
		drawLine: function(){
			if(this.isMouseDown){
				this.ctx.lineTo(this.mouseX, this.mouseY);
				this.ctx.stroke();
				console.log(this.mouseX + ', ' + this.mouseY);
			}
		},
		changeColor: function(hex){
			this.ctx.strokeStyle = '#' + hex;
		},
		changeWidth: function(width){
			this.ctx.lineWidth = width;
		},
		saveImage: function(){
			var img = new Image();
			var type = 'image/png';
			img.src = this.domCanvas.toDataURL(type);
			img.onload = function(){
				$.ajax({
					type: 'post',
					url: 'api/save.php',
					data: {'url': img.src},
					success: function(data){
						alert('保存しました｡');
						myCanvasObj.refleshWorks();
					},
					error: function(data){
						alert('error: ' + data);
					}
				});
			}
		},
		refleshWorks: function(){
			$.ajax({
				type: 'get',
				url: 'api/getlist.php',
				dataType: 'json',
				success: function(data){
					this.works = data;
					console.log(this.works);
					myCanvasObj.jqWorks.empty();
					$.each(this.works, function(){
						console.log(this);
						var work = $('<div>').addClass('work').append(
							$('<a>').attr({'href': 'show.php?id=' + this}).append(
								$('<img>').attr({'src': 'uploads/' + this + '.png', 'width': 60, 'height': 60})
							)
						);
						myCanvasObj.jqWorks.append(work);
					});
				}
			});
		}
	};
} // コンストラクタを即時実行する

// onLoad
$(function(){
	myCanvasObj.initialize($('body'), $('canvas#main'), $('div.works'));
	$('#colorPicker').ColorPicker({
		flat: true,
		color: '#000000',
		onChange: function(hsb, hex, rgb) {
			console.log(hex);
			myCanvasObj.changeColor(hex);
		}
	});
	
	var resetSelectWidth = function(){
		$('#width1').css('border', 'solid 1px #bbb');
		$('#width3').css('border', 'solid 1px #bbb');
		$('#width5').css('border', 'solid 1px #bbb');
	};
	$('#width1').click(function(){
		myCanvasObj.changeWidth(1);
		resetSelectWidth();
		$(this).css('border', 'solid 1px #ff0000');
	});
	$('#width3').click(function(){
		myCanvasObj.changeWidth(3);
		resetSelectWidth();
		$(this).css('border', 'solid 1px #ff0000');
	});
	$('#width5').click(function(){
		myCanvasObj.changeWidth(5);
		resetSelectWidth();
		$(this).css('border', 'solid 1px #ff0000');
	});
	$('#save').click(function(){
		myCanvasObj.saveImage();
	});
});


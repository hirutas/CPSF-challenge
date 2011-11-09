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
		ctx: null,
		isMouseDown: false,
		// public functions
		initialize: function(jqb, jqc){
			// set canvas objects
			this.jqBody = jqb;
			this.jqCanvas = jqc;
			this.domCanvas = this.jqCanvas.get(0);
			this.ctx = this.domCanvas.getContext('2d');
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
						alert('save successed');
					},
					error: function(data){
						alert('error: ' + data);
					}
				});
			}
		}
	};
} // コンストラクタを即時実行する

// onLoad
$(function(){
	myCanvasObj.initialize($('body'), $('canvas#main'));
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


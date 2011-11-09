// オブジェクトを1つだけグローバルな名前空間に追加
new function(){
	myCanvasObj = {
		// public variables
		mouseX: 0,
		mouseY: 0,
		jqCanvas: null,
		domCanvas: null,
		ctx: null,
		isMouseDown: false,
		// public functions
		initialize: function(jqc){
			// set canvas objects
			this.jqCanvas = jqc;
			this.domCanvas = this.jqCanvas.get(0);
			this.ctx = this.domCanvas.getContext('2d');
			// set mouse events
			this.jqCanvas.mousedown(function(e){
				myCanvasObj.mouseX = e.pageX - this.offsetLeft; // コールバック関数内ではthisは使えない
				myCanvasObj.mouseY = e.pageY - this.offsetTop;
				myCanvasObj.isMouseDown = true;
				myCanvasObj.ctx.beginPath();
				myCanvasObj.ctx.moveTo(this.mouseX, this.mouseY);
				console.log('mousedown');
			});
			this.jqCanvas.mouseup(function(e){
				myCanvasObj.mouseX = e.pageX - this.offsetLeft;
				myCanvasObj.mouseY = e.pageY - this.offsetTop;
				myCanvasObj.isMouseDown = false;
				console.log('mouseup');
			});
			this.jqCanvas.mouseout(function(e){
				myCanvasObj.mouseX = e.pageX - this.offsetLeft;
				myCanvasObj.mouseY = e.pageY - this.offsetTop;
				myCanvasObj.isMouseDown = false;
				console.log('mouseout');
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
		}
	};
} // コンストラクタを即時実行する

// onLoad
$(function(){
	myCanvasObj.initialize($('canvas#main'));
	
});


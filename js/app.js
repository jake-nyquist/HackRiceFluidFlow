var wrapper = document.getElementById("signature-pad"),
    clearButton = wrapper.querySelector("[data-action=clear]"),
    saveButton = wrapper.querySelector("[data-action=save]"),
    canvas = wrapper.querySelector("canvas"),
    signaturePad;

var interval;
var cancel = false;

/** Setup Flags to control mouse input */
var drawMode = true;
var autoDetect = false;
var touch = false;

var isDrawing = function() {
  if (autoDetect) return !touch;
  else return drawMode;
}

// Adjust canvas coordinate space taking into account pixel ratio,
// to make it look crisp on mobile devices.
// This also causes canvas to be cleared.
function resizeCanvas() {
    // When zoomed out to less than 100%, for some very strange reason,
    // some browsers report devicePixelRatio as less than 1
    // and only part of the canvas is cleared then.
    var ratio =  Math.max(window.devicePixelRatio || 1, 1);
    canvas.width = canvas.offsetWidth * ratio;
    canvas.height = canvas.offsetHeight * ratio;
    canvas.getContext("2d").scale(ratio, ratio);
}

var RenderInterface = function(width, height) {
  this.time = 0;
  this.width = width;
  this.height = height;

  this.resize = Module.cwrap('resize', null, ['number', 'number']);
  this.setbd = Module.cwrap('setbd', null, ['number', 'number']);
  this.addhit = Module.cwrap('addhit', null, ['number', 'number']);
  this.step = Module.cwrap('step', 'number', []);
  this.getMin = Module.cwrap('getMin', 'number', []);
  this.getMax = Module.cwrap('getMax', 'number', []);

  this.resize(this.height, this.width);
}
var frames = 0;
var t1 = new Date;
var t2 = new Date;
var createRender = function() {
	var renderInterface = new RenderInterface(canvas.width, canvas.height);
	signaturePad = new SignaturePad(canvas, renderInterface);
	var refreshLoop = function() {


		signaturePad.refresh();
    frames += 1;
    if (frames == 10) {
      t2 = new Date;
      time = (t2-t1)/10.0;
      document.getElementById("performance").innerText = (time).toString();
      document.getElementById("fps").innerText = (1/(time/1000.0)).toString();
      t1 = new Date;
      frames = 0;
    }
		if (!cancel)
			window.requestAnimationFrame(refreshLoop);
		else
			cancel = false;
	}
		//console.log('rl')
    // t2 = new Date;
    // console.log(t2 -t1);
    // t1 = t2;
	window.requestAnimationFrame(refreshLoop);
}

var clear = function() {
	cancel = true;
  resizeCanvas();
  createRender();
}

RenderInterface.prototype.getNextFrame = function() {
  //console.log('I am generating the next frame');
  var ptr = this.step();
  return {"ptr": ptr, min: this.getMin(), max: this.getMax()};
}

RenderInterface.prototype.addBoundaryPoint = function(x,y) {
  console.log("Add Boundary Point: "+ x.toString() + ", " + y.toString());
  this.setbd(x,y);
}

RenderInterface.prototype.tapSurface = function(x,y) {
  console.log("Surface tapped: "+ x.toString() + ", " + y.toString());
  this.addhit(x,y);

}

window.onresize = clear;
resizeCanvas();

window.clear = clear;
window.draw = function() {drawMode = true};
window.beat = function() {drawMode = false};

// $('#draw').click(function() {drawMode = true;});
// $('#beat').click(function() {drawMode = false;});
// $('#clear').click(clear);

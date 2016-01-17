var wrapper = document.getElementById("signature-pad"),
    clearButton = wrapper.querySelector("[data-action=clear]"),
    saveButton = wrapper.querySelector("[data-action=save]"),
    canvas = wrapper.querySelector("canvas"),
    signaturePad;

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

  this.resize = Module.cwrap('resize', null, ['number']);
  this.setbd = Module.cwrap('setbd', null, ['number', 'number']);
  this.addhit = Module.cwrap('addhit', null, ['number', 'number']);
  this.step = Module.cwrap('step', 'number', []);

  this.resize(this.width);
}

RenderInterface.prototype.getNextFrame = function() {
  console.log('I am generating the next frame');
  var ptr = this.step();
}

RenderInterface.prototype.addBoundaryPoint = function(x,y) {
  console.log("Add Boundary Point: "+ x.toString() + ", " + y.toString());
  this.setbd(x,y);
}

RenderInterface.prototype.tapSurface = function(x,y) {
  console.log("Surface tapped: "+ x.toString() + ", " + y.toString());
  this.addhit(x,y);

}
window.onresize = resizeCanvas;
resizeCanvas();

function createRender()
{
	var renderInterface = new RenderInterface(canvas.width, canvas.height);
	signaturePad = new SignaturePad(canvas, renderInterface);

	var refreshLoop = function() {
		signaturePad.refresh();
		//console.log('rl')
		setTimeout(refreshLoop, 100)
	}


	refreshLoop()
}

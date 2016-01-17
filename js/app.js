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
}

RenderInterface.prototype.getNextFrame = function() {
  console.log('I am generating the next frame');
}

RenderInterface.prototype.addBoundaryPoint = function(x,y) {
  console.log("Add Boundary Point: "+ x.toString() + ", " + y.toString());
}

RenderInterface.prototype.tapSurface = function(x,y) {
  console.log("Surface tapped: "+ x.toString() + ", " + y.toString());

}
window.onresize = resizeCanvas;
resizeCanvas();

var renderInterface = new RenderInterface(canvas.width, canvas.height);

signaturePad = new SignaturePad(canvas, renderInterface);

// clearButton.addEventListener("click", function (event) {
//     signaturePad.clear();
// });
//
// saveButton.addEventListener("click", function (event) {
//     if (signaturePad.isEmpty()) {
//         alert("Please provide signature first.");
//     } else {
//         window.open(signaturePad.toDataURL());
//     }
// });

var refreshLoop = function() {
  signaturePad.refresh();
  //console.log('rl')
  setTimeout(refreshLoop, 100)
}


refreshLoop()

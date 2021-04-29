// Global variables
var canvas, engine, scene, camera, score = 0;
var TOAD_MODEL;
// An array to store each ending of the lane
var ENDINGS = [];
var ENEMIES = [];

/**
* Load the scene when the canvas is fully loaded
*/
document.addEventListener("DOMContentLoaded", function () {
    if (BABYLON.Engine.isSupported()) {
        initScene();
        initGame();
    }
}, false);

/**
 * Creates a new BABYLON Engine and initialize the scene
 */
function initScene() {
    // Get canvas
    canvas = document.getElementById("renderCanvas");
    // Create babylon engine
    engine = new BABYLON.Engine(canvas, true);

    // Create scene
    scene = new BABYLON.Scene(engine);

    // Create the camera
    camera = new BABYLON.FreeCamera("camera", new BABYLON.Vector3(0,4,-10), scene);
    camera.setTarget(new BABYLON.Vector3(0,0,10));
    camera.attachControl(canvas);

    // Create light
    var light = new BABYLON.PointLight("light", new BABYLON.Vector3(0,5,-5), scene);

    engine.runRenderLoop(function () {
      scene.render();
    });
}
/**
 * Initialize the game
 */
function initGame() {

    BABYLON.Mesh.CreateSphere("sphere", 10, 1, scene);
}

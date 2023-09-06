# wasm-video-recorder
A Qt WebAssembly camera viewer / recorder.

This can be built for desktop but the point is to test out pure Qt camera abilities in a WebAssembly target.
As of Qt 6.5.2, this doesn't get a non-null QCameraFormat, even though the permissions mechanism seems to work. It also can't access the camera device descriptions.

The wasm demo resides here: https://metadevo.github.io/wasm-video-recorder/

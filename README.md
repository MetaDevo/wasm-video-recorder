# wasm-video-recorder
A Qt WebAssembly camera viewer / recorder.

This can be built for desktop but the point is to test out pure Qt camera abilities in a WebAssembly target.
As of Qt 6.5.2, this doesn't get anything in the QCameraFormat list from QCameraDevice objects, even though the permissions mechanism seems to work.

The wasm demo resides here: https://metadevo.github.io/wasm-video-recorder/

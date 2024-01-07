# engine

## What It Does

The engine is a C++ library that handles windowing, mouse, keyboard input, events, audio, graphics and provides
functions and classes that aid in the making of video games. It can be thought of as a platform on top of which games
can be built on.

To make a game with this engine, you create an executable that links to the library and includes its header files.

## Features

This engine, at its current state, is very limited and it doesn't provide a lot of features. It can't even compare to
top engines like `Godot`, `Unity` or `Unreal Engine`. But even though it doesn't have a way to dynamically develop
scenes (through some sort of GUI, by altering them in real time), it is enough to be considered a small and
feature-lacking **core** of a game engine. That is, additional features can be easily added to it, or GUI implemented
on top of it.

### Present features

- Window creation with various configurable properties
- Mouse and keyboard input through events
- Big event system which also supports user-made events
- Loading and playing WAV files on multiple channels
- Loading OBJ mesh files
- Object rendering (very flexible VAO and VBO system)
- Material system
- Shader program creation from (possibly multiple) source files
- One directional light and up to four point lights (Phong lighting model)
- Shadow rendering
- Debug lines rendering
- Crisp 2D text rendering with SDFs (TrueType fonts)
- Skybox rendering
- Anti-aliasing (MSAA)
- Textures
- Cameras
- Scene system
- Error handling through exceptions

### Missing features

There is no point in writing a list of missing features, because there are arguably an infinite amount of them. Anyone
can come and say that it's lacking feature X. But it is not difficult at all to support more file formats, more features
and more things. I only wrote the features I needed (or wanted) for the game.

At the very end I wished that I implemented support for the `Dear ImGui` library (for debug GUI), even though I didn't
feel the need for it while developing the engine.

## Minimal Example

```c++
#include <engine/engine.hpp>

struct MyScene : public bb::Scene {
    MyScene()
        : bb::Scene("my_scene") {}

    void on_enter() override {

    }

    void on_exit() override {

    }

    void on_update() override {

    }
};

int main() {
    bb::ApplicationProperties properties;

    try {
        bb::Application application {properties};
        application.add_scene<MyScene>();
        application.run("my_scene");
    } catch (bb::RuntimeError error) {
        bb::log_message("An error occurred: %d\n", error);
        return 1;
    }

    return 0;
}
```

## Third Party Libraries

- Assimp - mesh loading
- EnTT - event dispatcher
- glad - OpenGL loading
- GLM - math
- resmanager - resource management
- SDL2 - window and input handling
- SDL_image - image loading
- SDL_mixer - audio handling
- stb_include - include preprocessing for shaders
- stb_truetype - font loading and baking
- utfcpp - UTF8 support for text rendering

## Graphics

The engine uses modern `OpenGL` (version 4.3) to achieve its rendering of 2D and 3D graphics. This means that
programmable shaders are possible and actually needed to render anything.

I implemented the `Phong lighting model`, the most basic one. The Phong model uses forward rendering, which means that
lighting is rendered when objects are rendered. This is opposed to the common deferred rendering, where lighting
is applied later on and only where really visible. I made the engine support one directional light and (up to) four
point lights. If someone wants more lights, they might want to implement deferred rendering or forward+ rendering and
then modify the engine slightly instead of allowing more lights in the forward rendering scheme.

I also implemented `shadow mapping` with PCF (Percentage Closer Filtering), though the built in PCF offered by OpenGL
might be more desirable. I used a shadow map size of 2048, but a higher one of like 4096 yields much higher quality
shadows, but at a bigger performance cost.

Shadow map
![shadow map](/gallery/renderdoc/shadow_map.png)

`Text rendering` is supported, with more than ASCII characters (but not Asian hieroglyphs or backwards writing). I make
use of a technique called signed distance fields to render high quality characters at any scale.

Font map
![font map](/gallery/renderdoc/font_map.png)

The `material system` is, of course, tightly coupled to `shaders`. It offers a nice way to define how objects should be
rendered and a way to automatically send uniform variables to the shader program at the right time.

Vertex input of platform
![vertex input of platform](/gallery/renderdoc/vertex_input.png)
Vertex shader of platform
![vertex shader of platform](/gallery/renderdoc/vertex_shader.png)
Fragment shader of platform
![fragment shader of platform](/gallery/renderdoc/fragment_shader.png)

A lot of math is involved in implementing these graphics. Namely, vectors and matrices are used everywhere to represent
positions in space, colors and transformations. I didn't implement these mathematical concepts and algorithms myself
from scratch and instead I used a very popular math library called `GLM` that was designed just for these.

`Cameras` can be easily manipulated. I created the concept of a camera in 3D space as a thing composed of a `projection`
and a `view` matrix, just data. A camera controller is a separate entity that can use the camera and do certain
additional things. 2D cameras are usually just an orthographic projection matrix.

I made the rendering API `immediate mode`, because that is easier to use, in my opinion. Immediate mode means that every
frame the whole scene must be presented to be rendered. This makes the API flexible and more dynamic. This is opposed
to retained mode rendering, where the scene is presented once at the start and then rendered every frame. Any change
to the scene then must be made through state changing functions. Retained mode rendering is usually more efficient when
implemented correctly.

I needed to use `RenderDoc` a few times to better see what was happening with the graphics pipeline.

The entire rendering of a single frame looks like this:

Shadow map
![shadow map](/gallery/renderdoc/1.png)
Platform rendered
![platform rendered](/gallery/renderdoc/2.png)
Paddle rendered
![paddle rendered](/gallery/renderdoc/3.png)
Ball rendered
![ball rendered](/gallery/renderdoc/4.png)
First brick rendered
![first brick rendered](/gallery/renderdoc/5.png)
More bricks rendered
![more bricks rendered](/gallery/renderdoc/6.png)
Even more bricks rendered
![even more bricks rendered](/gallery/renderdoc/7.png)
Font map
![font map](/gallery/renderdoc/8.png)
Final image
![final image](/gallery/renderdoc/9.png)

## Resource Management

To manage resources of any kind, I used dynamic memory and smart pointers. `RAII` makes this very easy. Sometimes
resources are needed in multiple places and a reference-counting system solves this problem really nicely. When a
resource is no longer used by anyone (has no owners), it is automatically freed. But it is important to understand when
to use unique and weak pointers instead of shared pointers.

I also used a helper library named `resmanager` to more easily handle resources through in-code **string** identifiers
that are actually evaluated at compile time to generate unique **integer** identifiers for the resources. This makes
the management both easier and more performant.

# brick-breaker

## Gameplay

You control a paddle and use a ball to destroy bricks. Get a score as high as you can.

Catch the orbs to make more points or finish the level faster!

- Green orb - speed up
- Blue orb - speed down
- Yellow orb - extra life
- Black orb - die
- Red orb - fire ball
- Grayish orb - normal ball
- Pink orb - extra ball

### Controls in game

- Left, right - move the paddle
- Space - launch the balls
- WASD, QE - move the camera
- R - restart the game
- Escape - exit to menu
- Middle click - enable/disable mouse input
- Mouse - move the paddle
- Left click - launch the balls

### Controls in main menu

- Up, down - select level
- L - reload levels

The levels are loaded dynamically from the disk. Each level is described by a `JSON` file. If a level file is
corrupted or invalid, the game simply prints a message and doesn't create any brick.

This way levels are so easy to create, that even my little sister, who is just thirteen years old could make one. She
made the one named *Goofy*.

## Math

Of course, math was needed to implement many, many things. I used the features offered by the engine together with many
functions from `GLM` to implement translating, rotating and scaling objects with various velocities, handling collision
detection and resolution, ricocheting the balls off the paddle at various angles and rotating the ball correctly as it's
moving.

### Translation, rotation and scale

The entire transformation of an object in 3D space can be represented by a `transformation matrix`, which is a 4x4
matrix. The order of operations matters and they must be applied in this order:

```c++
// C++ example

glm::vec3 position {}, rotation {}, scale {};

glm::mat4 matrix {1.0f};
matrix = glm::translate(matrix, position);
matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
matrix = glm::scale(matrix, scale);
```

### Collision detection and resolution

I implemented collision detection between a sphere and an axis aligned bounding box, that is what I needed. For
resolution I needed to know on which side did the sphere collide with the box. Knowing the center positions of the
ball and the box, I could cross a vector between them and test if that vector's segment intersects one of the box's X
or Z sides. It must intersect one and this way I find the collision side.

### Ricocheting the ball

When the ball collides with the paddle, its ricochet angle is determined by the position of the impact along the
paddle. Specifically, hitting the left side of the paddle should direct the ball more to the left, maintaining the
same velocity. Same thing applies to the right side. Hitting the center, the ball should ricochet straight forward.
I used some basic linear algebra, trigonometry and polar coordinates to solve this problem.

### Rotating the ball

Knowing the ball's radius and its velocity I could calculate the angle at which it should rotate as it's moving. Basic
math.

Multiplying the velocity values of the objects with a special value called `delta time`, which is the difference
between the previous frame and the current frame time, turns the units from *pixels per frame* to *meters per second*.
In the 3D space, a length of 1 is one meter.

## Assets

All the assets were created with `Blender` and `GIMP` by me except for the *wood-bare* and *skybox* textures, all the
sounds, the *teapot* model and the *code-new-roman.regular* font. But almost all of them needed to be processed after
downloading.

## Gallery

![main menu](/gallery/game/1.png)
![in debug](/gallery/game/2.png)
![in debug](/gallery/game/3.png)
![in debug](/gallery/game/4.png)
![in release](/gallery/game/5.png)
![in release](/gallery/game/6.png)
![in release](/gallery/game/7.png)
![in release](/gallery/game/8.png)

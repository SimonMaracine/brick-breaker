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

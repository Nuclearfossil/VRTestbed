# Starfighter VR Test project
## Overview
I've wanted to build a Starfighter sim for a while now. I've also wanted to mess around with VR, so this is
the project I've decided to spend some time with.

A lot of the code is not production ready, as it's intent is to try a lot of different things (control schemes,
HUD options, etc).  I also expect there to be a mix of C++ and Blueprint code, depending on need.

## Version
Currently works with Unreal 4.16.3

## Classes
### Pawns
#### StarfighterPawn
The main Pawn for visualizing your starfighter around. It's essentially a container for the visualization components
as well as the camera and mover components. That class does not actually perform the movements, but gives the appropriate
commands to the mover.

#### StarfighterMovementComponent
This component defines how to move our starfighter. Different starfighters should derive from this to define their own
custom movement behaviours.

## TODO
+ Lasers
  + [X] Add in the mesh for laser emitters
  + [X] Laser effects
  + [ ] Lasers collide with targets
+ Startup Menu
  + [X] New Startup level
  + [X] Menu for selecting VR mode
  + [X] Proceed to the test level
+ Pause Menu
  + [X] Return to main menu
  + [X] Return to game
+ HUD
  + 2D
    + [X] Show velocity
    + [X] Show Targeting reticle
  + 3D
    + [ ] Show velocity
    + [ ] Show Targeting reticle
  + Positioning of camera
    + [X] Head placement exposed via Blueprints
+ Turrets
  + [ ] Models
  + [ ] Simple AI for 
  + [ ] Turrets to fire back
+ Better movement control
  + [X] Gradient speeds (0%, 25%, 50%, 75%, 100%) 
  + [ ] Joystick Control 

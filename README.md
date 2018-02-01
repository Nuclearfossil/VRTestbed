# Starfighter VR Test project
## Overview
I've wanted to build a Starfighter sim for a while now. I've also wanted to mess around with VR, so this is
the project I've decided to spend some time with.

A lot of the code is not production ready, as it's intent is to try a lot of different things (control schemes,
HUD options, etc).  I also expect there to be a mix of C++ and Blueprint code, depending on need.

## Notes:

The best branch to work from would be `dev.4.18.3`. It has been 'tested' and confirmed to work. `Master` is still a work in progress (I know, it should always be stable - but I pulled the repo from Perforce's Helix server before it went down and I never had a proper chance to get everything situated correctly).

There are two maps to work from: `Startup` and `Parkinglot`. `Startup` is the main menu level and `Parkinglot` is nothing more than a test for VR and starfighter control.

Movement is controlled currently by the mouse and keyboard.
Use `W` and `A` to accelerate and deccelerate. `0` to come to a full stop.

The are assets aren't mine. They're from a gentleman who posted his models for a Star Wars Battlefront title he was working on a while ago. You can get the full details in the `raw.zip` file.  TODO: Add the appropriate links to his work here.

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
  + [ ] SFX
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
  + [ ] Simple AI for tracking
  + [ ] Turrets to fire back
    + [ ] Different laser effects for turrets.
+ Asteroids
  + [ ] Proper collision
  + [ ] Handle collision in VR in a manner that doesn't cause the player to vomit.
+ Better movement control
  + [X] Gradient speeds (0%, 25%, 50%, 75%, 100%) 
  + [ ] Joystick Control 
+ Shields
  + [ ] Shield effects
  + [ ] Shield management

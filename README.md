# 3D Map Shooter

A first-person 3D shooter built with C++, OpenGL and GLUT. You explore a walled map with buildings and a spinning windmill, and eliminate every enemy to win. All textures are generated procedurally in code, so the project needs no external image files.

## Features

- First-person camera with mouse look and WASD movement
- Jumping and gravity
- Wall and building collision
- Ray-based shooting with a cooldown
- 5 enemies to eliminate, with a live counter on screen
- Victory screen that returns to the menu after 5 seconds
- Main menu with keyboard and mouse selection
- Admin mode: fly freely around the map without enemies
- Day and night mode with a sun, a moon and different skies
- Animated windmill that can be paused
- Procedural textures: brick walls, tiled floor, glass and panel buildings, metal tower, painted blades, sky, sun and moon

## Controls

### Menu

| Input | Action |
| --- | --- |
| Up / Down arrow | Change selection |
| Enter or Space | Confirm |
| Left mouse click | Click a menu item |

### In game

| Input | Action |
| --- | --- |
| W / A / S / D | Move |
| Mouse | Look around |
| Left mouse button | Shoot |
| Space | Jump |
| N | Toggle day / night |
| Z | Pause / resume windmill |
| Esc | Quit |

### Admin mode

| Input | Action |
| --- | --- |
| W / A / S / D | Move |
| H | Fly up |
| L | Fly down |
| Q | Return to the menu |
| N / Z | Same as in game |

## Building

Install MinGW-w64 and freeglut, then:

```bash
g++ main.cpp -o shooter.exe -lfreeglut -lopengl32 -lglu32
shooter.exe
```

The source is `main.cpp`. It uses `windows.h` for keyboard state, timing and mouse centering.

## Project Structure

| File | Description |
| --- | --- |
| `main.cpp` | The whole game: map, collision, enemies, shooting, menu and procedural textures |

## How It Works

- **Map:** walls and buildings are textured cubes drawn at a scale of 2x. A matching list of collision boxes keeps the player from walking through them.
- **Collision:** X and Z movement are tested separately, so the player slides along walls. Each box has a top height, which lets the player jump onto lower buildings.
- **Shooting:** a ray is cast from the camera along the view direction and tested against a sphere around each enemy. The closest hit is eliminated.
- **Textures:** each texture is generated pixel by pixel at startup with a seeded pseudo-random function, then uploaded with mipmaps.
- **Day / night:** switches the sky texture, the light colours and the sun or moon sprite.

## Requirements

- A C++ compiler
- OpenGL, GLU and GLUT (freeglut)

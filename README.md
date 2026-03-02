# Grid Pathfinding with Jgraph (Lab 1)

## Overview

This project visualizes shortest‑path grid navigation using Jgraph. I wrote this C++ program (`GridGame`) which computes a shortest path on a 10×10 grid with obstacles using breadth‑first search (BFS) and generates a sequence of Jgraph files that show a green player walking along a blue path toward a red target. 
Each frame is a separate Jgraph file (`step_00.jgr`, `step_01.jgr` and more) that can be converted to PNG or PDF, giving an animation‑like sequence. (PS: I converted in PNG)

## Files

1. `GridGame.cpp`: C++ source that:
  - Builds a 10×10 grid.
  - Places a fixed vertical wall of black obstacle cells.
  - Runs BFS from start `(0,0)` to target `(9,9)` to find a shortest path avoiding obstacles.
  - Reconstructs the path into a vector of cells.
  - Emits one Jgraph description per animation step (`step_XX.jgr`), showing:
    - Gray grid lines.
    - Black obstacle wall.
    - Blue polyline for the shortest path so far.
    - Green box for the player.
    - Red box for the target.
2. `Makefile`: Compiles `GridGame` and generates example JPG/PNG frames using `jgraph` and `convert`/`ps2pdf`. 
3. `step_XX.jgr`: Jgraph files for individual frames (generated). (eg. step_01, step_02...)
4. `step_XX.png`: Rendered images of selected frames (generated). (eg. step_01, step_02...)

## Compilation

Assumes a Unix‑like environment with:
- `jgraph`
- Tools: `convert` (ImageMagick) and/or `ps2pdf` 

To compile:
```bash
make
```

This builds an executable:
```bash
./GridGame
```

Running it produces a sequence of `step_XX.jgr` files in the current directory.

## Generating Images
Here, I use the Makefile target to generate 18 output images:

```bash
make images
```

A typical `images` target:

- Runs `./GridGame` to produce `step_00.jgr` … `step_18.jgr`.
- Uses `jgraph` and `convert` (or `ps2pdf`) to create images such as:
  - `step_00.png` : Start state, player at top‑left, only first blue path point.
  - `step_03.png` : Player has moved a few cells; path segment grows.
  - `step_09.png` : Player turns the corner around the obstacle wall.
  - `step_15.png` : Player close to the target.
  - `step_18.png` : Final frame with the path drawn almost completely across the grid.

You can also convert a specific frame manually:

```bash
jgraph step_00.jgr > step_00.ps
convert -density 300 step_00.ps step_00.png
```

## How It Works

### Grid and obstacles
1. The grid is 10×10.
2. Each cell is either:
  - Empty (gray grid intersection),
  - Obstacle (black square),
  - Start/target/player position.
3. `init_fixed_obstacles()` clears the grid and then places a simple vertical wall in column 5 between rows 2 and 7, guaranteeing that a path exists but forces a detour. 

### Shortest‑path computation (BFS)

1. BFS starts at the player’s start cell `(0,0)`.  
2. It explores the grid in four directions (up, down, left, right) while skipping obstacles and out‑of‑bounds cells.  
3. It records:
  - `dist[r][c]` : distance from start.
  - `parent[r][c]` : predecessor cell used to reconstruct the path.  
4. When the target `(9,9)` is reached, the code walks backward via `parent` to reconstruct the full path, then reverses it to obtain an ordered path from start to target. 

### Jgraph output

For each animation step `step`:
1. `path_index = step`.
2. `player_pos = shortest_path[path_index]`.

`print_frame(step)` emits Jgraph commands to:

- Draw grid lines (light gray).
- Draw the black obstacle wall.
- Draw the blue path consisting of all cells from `shortest_path[0]` up to `shortest_path[path_index]`.
- Draw the target cell as a red box.
- Draw the player cell as a green box. 

Thus, as `step` increases, the blue polyline grows and the green player moves along it, while the grid and obstacles remain fixed.

## Each jgraph 

After `make images`, open the generated frames:

1. `step_00.png` : Player at the start; blue path begins at the top‑left corner.
2. `step_06.png` : Player moving downward along the left edge.
3. `step_12.png` : Player has turned the corner and is moving along the bottom row.
4. `step_18.png` : Final frame near the target in the lower‑right corner.

These images together form a frame‑by‑frame visualization of BFS shortest‑path navigation on the grid. 

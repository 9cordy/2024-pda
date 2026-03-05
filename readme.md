# 2024 Physical Design Automation (PDA)

This repository contains four algorithmic projects implemented for the "Physical Design Automation" course in 2024. The implementations cover critical stages of the VLSI Physical Design flow, including data structure management, floorplanning, placement legalization, and global routing.

---

## Tech Stack
* **Language**: C / C++
* **Algorithms**: Simulated Annealing (SA), A* Search Algorithm, Contour-based Placement
* **Concepts**: B*-Tree, Corner Stitching, Half-Perimeter Wire Length (HPWL), Grid-based Routing (G-Cell)

---

## Projects Overview

### 1. [Corner Stitching](./corner_stitching)
* **Description**: An implementation of the Corner Stitching data structure to manage planar shapes and empty spaces in layout environments.
* **Implementation Details**:
  * Maintains the "maximal horizontal stripe" property for space tiles.
  * Executes coordinate point-finding via four-pointer traversal (`rt`, `tr`, `bl`, `lb`).
  * Handles block insertion by performing vertical and horizontal tile splitting, followed by adjacent space tile merging.

### 2. [Fixed-Outline Floorplanning](./floorplanning)
* **Description**: A floorplanner that places rectangular hard macros within a specified boundary to optimize total chip area and wirelength (HPWL).
* **Implementation Details**:
  * Utilizes a B*-Tree representation to map topological relations between macros.
  * Determines macro coordinates using a Horizontal Contour Line.
  * Applies Simulated Annealing (SA) with three perturbation moves: Rotate, Move, and Swap.

### 3. [Multi-Row Height Legalization](./legalizer)
* **Description**: A standard cell legalizer designed to eliminate overlaps and align cells to legal sites, with specific handling for multi-row height cells.
* **Implementation Details**:
  * Constructs an initial placement grid to map available intervals across placement rows.
  * Validates insertion sites based on cell width and multiple-row constraints.
  * Incorporates a row compaction procedure to shift existing cells and resolve site unavailabilities.

### 4. [Die-to-Die Global Router](./global_router)
* **Description**: A grid-based global router specialized for Die-to-Die (D2D) connections across two chips, managing routing capacities and directional layer constraints.
* **Implementation Details**:
  * Discretizes the routing region into a 2D G-Cell grid and sorts nets based on Manhattan distance to manage congestion priority.
  * Implements the A* Search algorithm using a custom cost function ($f(n) = g(n) + h(n)$) to balance wirelength, via counts, and congestion penalties.
  * Enforces layer-specific routing directions (Metal 1 for horizontal, Metal 2 for vertical) and dynamically inserts vias during transitions.

---

## Compilation and Execution
Each project is independently maintained with its own `Makefile`. 

To run a specific project:
1. Navigate to the corresponding sub-directory.
2. Run `make` to compile the executable.
3. Follow the specific execution commands outlined in the local `readme.md` of each project.
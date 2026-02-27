# Die-to-Die (D2D) Global Router

## Introduction
This project implements a **Global Router** designed for **Die-to-Die (D2D)** connections, typically used in advanced packaging (e.g., CoWoS, MCM). The router connects pairs of bumps between two different chips (Chip 1 and Chip 2) across a Redistribution Layer (RDL) grid.

## Features
* **A* Search Algorithm**: Utilizes a cost-driven A* search to find the optimal routing path for each net.
* **Multi-Layer Routing**: Supports routing on two metal layers (Metal 1 for horizontal and Metal 2 for vertical routing).
* **Capacity Management**: Dynamically tracks the routing capacity of each Grid Cell (G-Cell) to avoid congestion and overflow.
* **Cost Optimization**: Minimizes a weighted cost function including wirelength, via counts, and congestion-based metal costs.

## Technical Details
### Routing Model
* **Grid-Based**: The routing area is divided into a 2D grid of G-Cells.
* **Layer Constraints**: 
    * **Metal 1 (M1)**: Preferred for horizontal segments.
    * **Metal 2 (M2)**: Preferred for vertical segments.
* **Via Insertion**: Automatically inserts a via whenever the routing direction switches between horizontal and vertical.

### Algorithm: A* Search
The router explores the grid using the following cost function:
$$f(n) = g(n) + h(n)$$
* $g(n)$: The actual cost from the source bump to the current cell (includes wirelength and via costs).
* $h(n)$: The heuristic Manhattan distance to the target bump.

## Compilation and Execution
```bash
# To compile the program and generate the "D2DGRter" binary
make

# To execute the router
./D2DGRter <input.gmp> <input.gcl> <input.cst> <output.lg>
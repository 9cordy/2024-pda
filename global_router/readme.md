# Die-to-Die Global Router

## Introduction
This project implements a **Global Router** specialized for **Die-to-Die** connections. It efficiently connects bumps between two dies across a 2D grid while considering routing capacities and layer-specific costs.

## Algorithm Flow
* **Grid Construction**: Discretizes the routing region into a 2D G-Cell grid and initializes horizontal and vertical capacities for multiple layers.
* **Net Sorting**: Orders the nets based on their Manhattan distance to prioritize shorter connections and manage congestion effectively.
* **Path Finding (A* Search)**: 
    * Utilizes the **A* Search algorithm** to find the optimal path with the lowest cost.
    * Incorporates a cost function that balances wirelength, via counts, and congestion penalties.
* **Backtracing**: Reconstructs the routing path from the target bump to the source once the A* search reaches the destination.
* **Capacity Update**: Dynamically deducts the used routing resources from the G-Cell capacities to prevent overflow during subsequent net routing.
* **Layer Assignment**: Assigns routing segments to Metal 1 (horizontal) or Metal 2 (vertical) and inserts vias at direction transition points.

## Compilation and Execution
```bash
# To compile
make

# To execute
./D2DGRter <input.gmp> <input.gcl> <input.cst> <output.lg>

# To remove object files
make clean
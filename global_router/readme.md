# Die-to-Die Global Routing

## Introduction
This project implements a **Global Router** designed for **Die-to-Die** connections in advanced packaging. The tool connects pairs of bumps between two dies (Chip 1 and Chip 2). The routing ensures that all nets are connected while minimizing a complex cost function involving wirelength, via counts, and grid congestion.

## Algorithm Flow
* **Grid and Capacity Initialization**: Discretizes the routing area into G-Cells and initializes horizontal/vertical capacities for two metal layers.
* **Net Sorting**: Orders nets by their Manhattan distance to manage routing priority and potential congestion.
* **A* Search Pathfinding**: 
    * Explores the grid using an **A* Search algorithm** to find the minimum-cost path for each net.
    * **Cost Function**: $f(n) = g(n) + h(n)$.
    * $g(n)$ incorporates wire costs (weighted by $\alpha, \beta$), congestion penalties ($\gamma, \delta$), and Via costs.
* **Layer and Direction Constraints**:
    * **Metal 1 (M1)**: Reserved for horizontal routing segments.
    * **Metal 2 (M2)**: Reserved for vertical routing segments.
    * Automatically inserts a **Via** when switching between M1 and M2.
* **Dynamic Capacity Update**: After routing each net, the available capacity of the traversed G-Cells is updated to prevent overflow in subsequent nets.
* **Backtracing**: Reconstructs the final routing path from the target bump to the source once the target is reached.

## Compilation and Execution
```bash
# To compile
make

# To execute
./D2DGRter <input.gmp> <input.gcl> <input.cst> <output.lg>

# To remove object files
make clean
# Fixed-Outline Floorplanning

## Introduction
This project implements a fixed-outline floorplanner designed to place rectangular hard macros within a specified boundary without overlaps. The tool optimizes a weighted cost function of total chip area and wirelength (HPWL) using a **B*-Tree** representation and **Simulated Annealing (SA)** optimization.

## Algorithm Flow
* **B*-Tree Representation**: Maps the topological relations between macros into a binary tree structure, where each node corresponds to a macro.
* **Contour-based Placement**: Traverses the B*-Tree to determine macro coordinates ($x, y$) using a **Horizontal Contour Line**.
* **Cost Evaluation**: 
    * Calculates the total bounding box **Area** of the placement.
    * Computes the **Total Wirelength** using the **Half-Perimeter Wire Length (HPWL)** model based on net connections.
* **Simulated Annealing Optimization**: Iteratively perturbs the B*-Tree through three types of moves to find the optimal solution:
    1. **Rotate**: Changes the orientation of a macro.
    2. **Move**: Relocates a node to a different position in the tree.
    3. **Swap**: Exchanges the positions of two nodes in the tree.
* **Best State Recovery**: Tracks the solution with the minimum cost throughout the annealing process and restores it as the final output.

## Compilation and Execution
```bash
# To compile
make

# To execute
./floorplanning <alpha_value> <input.block> <input.net> <output>

# To remove object files
make clean
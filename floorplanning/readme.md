# Fixed-Outline Floorplanning

## Introduction
This project implements a fixed-outline floorplanner designed to place a set of rectangular hard macros within a specified chip boundary without any overlaps. The primary goal is to optimize a weighted cost function of the total chip area and the total wirelength (HPWL).

## Methodology
The floorplanner utilizes a **B*-Tree** representation to manage macro placements and **Simulated Annealing** for optimization.

### Data Structures
* **B*-Tree Representation**: Used to represent the topological relations between macros. Each node in the tree represents a macro, and the tree structure determines the relative $x$ and $y$ coordinates.
* **Horizontal Contour Line**: A linked-list based contour structure is implemented to efficiently calculate the $y$-coordinates of macros during B*-Tree traversal, ensuring $O(n)$ placement complexity.

### Optimization: Simulated Annealing (SA)
To explore the solution space, the following operations are performed during the annealing process:
1. **Macro Rotation**: Swapping the width and height of a macro.
2. **Macro Move**: Moving a node to a different location within the B*-Tree.
3. **Macro Swap**: Swapping the positions of two nodes in the tree.

### Cost Function
The cost is calculated as:
$$Cost = \alpha \cdot \text{Area} + (1 - \alpha) \cdot \text{Wirelength (HPWL)}$$
* **Area**: The area of the final bounding box of the floorplan.
* **Wirelength**: Calculated using the **Half-Perimeter Wire Length (HPWL)** model based on the center points of the macros.

## Compilation and Execution
```bash
# To compile the program and generate the "floorplanning" binary
make

# To execute the floorplanner
./floorplanning <alpha_value> <input.block> <input.net> <output>
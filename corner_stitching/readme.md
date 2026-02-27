# Corner Stitching - Physical Design Automation Lab 1

## Introduction
This project implements the **Corner Stitching** data structure, a powerful technique used in VLSI layout tools for managing planar shapes and empty spaces. The implementation organizes space tiles as **maximal horizontal stripes**, ensuring no space tile has other space tiles immediately to its right or left.

## Features
* **Point Finding**: Efficiently locates the tile containing a specific (X, Y) coordinate by traversing tile pointers (rt, tr, bl, lb).
* **Block Creating**: Supports dynamic insertion of rectangular blocks into the layout while maintaining the maximal horizontal stripe property.
* **Neighbor Traversal**: Automatically identifies and counts adjacent solid blocks and blank tiles for any given tile.
* **Tile Merging**: Includes logic to merge vertically adjacent blank tiles to keep the data structure optimized.

## Data Structure Details
The implementation uses a `class Tile` with the following pointers:
* `rt` (right-top), `tr` (top-right)
* `bl` (bottom-left), `lb` (left-bottom)
* `type`: Distinguishes between **Blank (0)** and **Solid (1)** tiles.

## Compilation and Execution
```bash
# To compile and generate the "corner_stitching" binary
make

# To execute the program with input and output files
./corner_stitching <input_file_name> <output_file_name>

# To clean up object files and the binary
make clean
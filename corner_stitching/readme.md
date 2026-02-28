# Corner Stitching

## Introduction
This project implements the **Corner Stitching** data structure, a powerful technique for managing planar shapes and empty spaces in VLSI layout tools. The implementation ensures the **maximal horizontal stripe** property, where no space tile is adjacent to another space tile on its left or right sides.

## Algorithm Flow
* **Tile Initialization**: Creates an initial layout consisting of a single bounding box represented as a blank space tile.
* **Point Finding**: Efficiently traverses the tile structure using four pointers (`rt`, `tr`, `bl`, `lb`) to locate the tile containing a specific coordinate.
* **Block Insertion**: 
    * Identifies all tiles overlapping with the new solid block area.
    * Performs vertical and horizontal splitting of existing tiles to accommodate the new block.
* **Space Tile Merging**: After insertion, vertically adjacent blank tiles are merged to maintain the maximal horizontal stripe property.
* **Neighbor Traversal**: Dynamically identifies all solid and blank neighbors for a given tile by following the corner pointers.

## Compilation and Execution
```bash
# To compile
make

# To execute
./corner_stitching <input_file_name> <output_file_name>

# To remove object files
make clean
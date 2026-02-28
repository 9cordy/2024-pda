# Multi-Row Height Standard Cell Legalization

## Introduction
This project implements a **Legalizer** for standard cell placement, specifically designed to handle **multi-row height** cells. The tool eliminates overlaps by moving cells from their global placement positions to the nearest legal sites within the row structure while minimizing total displacement.

## Algorithm Flow
* **Initial Site Mapping**: Generates an initial placement grid and identifies available intervals across all placement rows.
* **Cell Removal**: Temporarily removes banking cells or cells requiring legalization from the layout to re-evaluate the available space.
* **Optimal Site Search**: 
    * Searches for legal insertion sites in the target row and adjacent rows within a defined step range.
    * Validates that the selected site can accommodate the cell's width and multiple-row height constraints.
* **Direct Cell Insertion**: Places the cell into the identified optimal position and updates the occupancy status of the affected sites.
* **Row Compaction**: If no legal site is found, a compaction procedure is triggered to shift existing cells and create contiguous free space for the new cell.

## Compilation and Execution
```bash
# To compile
make

# To execute
./Legalizer <input.lg> <input.opt> <output.lg>

# To remove object files
make clean
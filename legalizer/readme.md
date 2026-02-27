# Multi-Row Height Standard Cell Legalization - Physical Design Automation Lab 3

## Introduction
This project implements a **Legalizer** for standard cell design, specifically handling **multi-row height** cells. The goal is to move cells from their initial (possibly overlapping) global placement positions to the nearest legal sites within the placement rows, ensuring no overlaps and satisfying row-based constraints.

## Methodology: Insert Cell Directly
Based on the experimental results, this project focuses on the **Direct Insertion** approach for robust legalization.

### Legalization Flow:
1. **Initial Sorting**: Cells are sorted based on their initial $x$-coordinates to process them in a structured order.
2. **Cell Removal**: Overlapping or illegal cells are temporarily removed from the layout to be re-inserted.
3. **Optimal Position Search**: 
    * For each cell, the algorithm identifies the best insertion row based on the $y$-distance from its target position.
    * It searches for available sites in the current row and neighboring rows using a specific step size (e.g., 200 sites) to balance runtime and quality.
4. **Site Validation**: Ensures that the selected site has enough contiguous free space to accommodate the cell's width and that it aligns with the site grid.
5. **Compaction (Macro-level)**: If no legal site is found, a **Compaction** procedure is triggered. It sorts cells in a row by their $x$-coordinates and shifts them to reduce dead space, creating new vacancies for insertion.

## Features
* **Multi-row Support**: Correctly handles cells that span across multiple placement rows.
* **Dead Space Optimization**: Uses compaction logic to handle high-density regions (e.g., in `testcase1_ALLO_5000`).
* **Overlap Elimination**: Guaranteed overlap-free placement after the legalization process.

## Compilation and Execution
```bash
# To compile the program and generate the "Legalizer" binary
make

# To execute the legalizer
./Legalizer <input.lg> <input.opt> <output.lg>
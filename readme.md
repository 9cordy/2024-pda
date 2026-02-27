# 2024 Physical Design Automation (PDA) Core Algorithms

This repository contains four major projects developed during the "Physical Design Automation (PDA)" course in 2024. These projects cover the critical stages of the VLSI Physical Design flow, implemented entirely in C++ with a strong emphasis on performance optimization, advanced data structures, and algorithmic efficiency.

---

## Tech Stack & Core Skills
* **Programming Languages**: C++ (C++11 standard, heavy use of STL containers)
* **Core Technologies**: Advanced Data Structures (B*-tree, Corner Stitching), Heuristic Algorithms, Memory Management
* **Domain Knowledge**: VLSI Physical Design Flow, Floorplanning, Legalization, Global Routing

---

## Projects Overview

### 1. [Corner Stitching (Layout Data Structure)](./corner_stitching)
* **Description**: Implemented the classic **Corner Stitching** data structure to dynamically manage 2D geometric layout information and empty space representations.
* **Technical Highlights**: 
  * Maintained space tiles as "maximal horizontal stripes" to ensure optimal layout querying.
  * Engineered to handle massive block creation and point-finding operations while strictly passing a highly constrained 1-minute execution time limit.

### 2. [Floorplanning (B*-tree)](./floorplanning)
* **Description**: Developed a floorplanner to determine the locations of hard macros while optimizing the total chip area and wirelength.
* **Technical Highlights**: 
  * Utilized the **B*-tree (B-star tree)** data structure to represent topological relations between modules.
  * Implemented simulated annealing (or heuristic search) to explore the solution space, preventing module overlapping and minimizing dead space.

### 3. [Legalizer (Standard Cell Legalization)](./legalizer)
* **Description**: Implemented a Legalizer to resolve overlapping standard cells generated after global placement, ensuring all cells are aligned to standard placement rows without violating design rules.
* **Technical Highlights**: 
  * Handled complex constraints including fixed cells (macros) and multi-row cells.
  * Optimized the total displacement (Manhattan distance) of standard cells from their original global placement positions to maintain timing/wirelength integrity.

### 4. [Die-to-Die Global Router (D2DGRter)](./global_router)
* **Description**: Developed a grid-based global routing algorithm for modern Die-to-Die (D2D) packaging architectures.
* **Technical Highlights**: 
  * Parsed complex netlists and grid graphs to construct routing paths.
  * Balanced routing congestion and minimized total wirelength across multi-die layout boundaries.

---

## How to Build and Run
Each project directory contains a standalone `Makefile` and specific testcases. 

To compile and execute a project:
1. Navigate to the desired project directory (e.g., `cd Legalizer`).
2. Run `make` to compile the source code into an executable binary (using `-O3` optimization).
3. Execute the binary with the required input/output arguments as specified in the local project instructions.
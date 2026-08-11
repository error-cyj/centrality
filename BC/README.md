# TD-BC: Tree Decomposition-Based Betweenness Centrality Framework

`TD-BC` is a high-performance C++ framework designed for computing exact Betweenness Centrality (BC) on undirected weighted graphs. By combining **Tree Decomposition (TD)** with **Shortcut Graph** techniques, `TD-BC` significantly accelerates traditional Brandes algorithm computations, particularly on complex networks and large-scale spatial road networks (such as `NY.gr`).

---

## Table of Contents

- [Features](#features)
- [Input Graph Format](#input-graph-format)
- [Output File Format](#output-file-format)
- [Build & Compilation](#build--compilation)
- [Usage Guide](#usage-guide)
- [Project Structure](#project-structure)

---

## Features

1. **Tree Decomposition Optimization**: Hierarchically partitions the graph structure to reduce redundant global graph traversals during shortest-path calculations.
2. **Shortcut Graph Maintenance**: Dynamically constructs and updates shortcut edges, accelerating shortest-path count ($\sigma$) propagation and dependency accumulation ($\delta$).
3. **Undirected Weighted Graph Support**: Provides exact Betweenness Centrality computation natively for graphs with positive integer edge weights.
4. **Performance & Execution Logging**: Tracks detailed node exploration counts (`scan_cnt`), edge scan counts (`scan_cnt_edge`), and execution runtime breakdown.

---

## Input Graph Format

The framework reads edge-list formatted text files representing weighted undirected graphs.

### Format Specification

1. **First Line**: Contains two space-separated positive integers $n$ and $m$:
   - `n`: Total number of vertices (nodes) in the graph.
   - `m`: Total number of edges in the graph.
2. **Next $m$ Lines**: Each line contains three space-separated integers `u v w`:
   - `u`: Source vertex ID (**1-based indexing**, range $1 \sim n$).
   - `v`: Target vertex ID (**1-based indexing**, range $1 \sim n$).
   - `w`: Edge weight/distance (positive integer).

> **Note**: Vertices are automatically converted to 0-based indexing ($0 \sim n-1$) internally during file parsing.

### Example Input (`NY.gr`)

When running on benchmark road networks like New York (`NY.gr`):

```text
264346 733846
1 2 120
2 3 45
3 4 89
...
```

*Explanation:*
- The graph `NY.gr` consists of 264,346 vertices and 733,846 edges.
- Each subsequent line defines an edge between two 1-indexed vertices along with its integer weight/distance.

---

## Output File Format

When executing `TD_BC_test`, two output files are generated:

1. **BC Result File (`<output_bc_file>`)**:
   - Contains $n$ lines. Line $i$ (0-indexed) contains the computed Betweenness Centrality value for vertex $i$.
   - Formatted as fixed-point floating-point values (6 decimal places).

2. **Performance Log File (`<output_log_file>`)**:
   - Stores node exploration statistics, edge traversal counts, tree decomposition construction metadata, and execution timing.

---

## Build & Compilation

The project is managed with a `Makefile` and requires a C++ compiler supporting C++11 or higher (e.g., GNU `g++`).

### Compilation Command

Run the following command in the project root directory:

```bash
make all
```

This compiles the source code and generates the executable binary `TD_BC_test`.

### Cleaning Build Artifacts

```bash
make clean
```

---

## Usage Guide

Execute `TD_BC_test` by passing **3 command-line arguments**:

```bash
./TD_BC_test <input_graph_file> <output_bc_file> <output_log_file>
```

### Argument Description

| Position | Parameter | Description |
| :--- | :--- | :--- |
| `argv[1]` | `<input_graph_file>` | Path to the input graph dataset (e.g., `NY.gr` or `data/NY.gr`) |
| `argv[2]` | `<output_bc_file>` | Path to save the output Betweenness Centrality scores (e.g., `NY.bc`) |
| `argv[3]` | `<output_log_file>` | Path to save the performance and traversal log (e.g., `NY.log`) |

### Execution Example

```bash
./TD_BC_test NY.gr NY.bc NY.log
```

**Console Output Preview:**

```text
start reading graph
after reading graph
start brandes
inited
start make tree
update tree distance
start TD_SPT
...
scan_cnt 8452109 scan_cnt_edge 23412089
NY.gr  12.345678
```

---

## Project Structure

| File / Module | Description |
| :--- | :--- |
| `TD_BC_test.cpp` / `TD.cpp` | Main entry point, Tree Decomposition algorithm logic, and Brandes BC computation routines. |
| `graph.h` / `graph.cpp` | Core graph representation, adjacency storage, and edge iteration interfaces. |
| `int_undirected_graph.h` / `.cpp` | `Int_Undirected_Graph` class implementation using `std::map` adjacency structure. |
| `SelEle.h` / `SelEle.cpp` | Priority queue element definitions and vertex degree ordering for Tree Decomposition. |
| `input.h` | Helper structures (`rational_number`, `Shortcut`, `Shortcut_Graph`, and `Node`). |
| `Makefile` | Project compilation rules and directives. |

# Tree Decomposition-Based Graph Computation (TD_ECC)

An optimized C++ implementation of Tree Decomposition-based graph processing algorithms for computing vertex eccentricity and shortest path trees on large-scale road networks (e.g., DIMACS datasets like `NY.gr`).

## Overview

This repository provides an efficient tree-decomposition framework (`TD_ECC`) tailored for large undirected graphs. Key algorithmic components include:
- **Graph Contraction & Reduction:** Contraction hierarchy / degree-based vertex ordering to construct tree decomposition (`make_tree`, `reduce`).
- **Shortcut Graph Construction:** Efficient shortcut creation and merging (`Shortcut_Graph`) for exact distance preservation.
- **Tree-Decomposition Shortest Path Trees (TD-SPT):** Accelerated shortest-path searches utilizing tree hierarchy and upward searches (`upward_search`, `TD_DFS`, `TD_SPT_DFS`).
- **Eccentricity / Max-Distance Computation:** Fast computation of maximum distances (`max_dis`) across graph vertices.

---

## Directory Structure

| File | Description |
| :--- | :--- |
| `TD_ECC.cpp` | Core tree decomposition algorithms, TD-SPT search logic, and program main entry point. |
| `int_undirected_graph.h` / `.cpp` | Integer undirected graph data structure supporting weighted edges and dynamic adjacency updates. |
| `graph.h` / `.cpp` | Base `Graph` class definition, priority types, and high-resolution timer (`get_time()`). |
| `SelEle.h` / `.cpp` | Dynamic element selection/ordering data structure for vertex contraction. |
| `Makefile` | Build configuration file for compiling the binary executable. |

---

## Input & Output Formats

### Input Graph Format (`argv[1]`)
The program reads custom or formatted DIMACS edge-list graph files (e.g., `NY.gr`).

**Header Line:**
```text
<num_vertices> <num_edges>
```

**Edge Lines (`m` lines):**
```text
<u> <v> <weight>
```
*Note:* The parser assumes **1-based node indexing** in the input file and automatically converts node IDs to 0-based indexing internally.

#### Example (`NY.gr` sample):
```text
4 4
1 2 5
2 3 10
3 4 3
1 4 20
```

---

### Output Result File (`argv[2]`)
Stores the computed maximum distance (eccentricity) for each vertex `0` to `n - 1`, written line-by-line:
```text
15
10
13
18
```

---

### Time & Performance Log (`argv[3]`)
Contains per-node scan counters followed by timing statistics appended at the end of the file:
```text
<scan_log_0>
<scan_log_1>
...
total <total_runtime_in_seconds>
td_dfs_time <td_dfs_runtime_in_seconds>
up_search <upward_search_runtime_in_seconds>
```

---

## Compilation & Building

### Prerequisites
- C++ compiler with C++11 or later support (e.g., `g++` 4.8+)
- Make build tool
- Linux/POSIX environment (uses `<sys/time.h>`)

### Build Executable
Run `make` to compile the `TD_ECC` executable with `-O3` optimization:
```bash
make
```

To clean intermediate object files and binaries:
```bash
make clean
```

---

## Running the Program

Run `TD_ECC` by providing the input graph file, output result file, and log file as command-line arguments:

```bash
./TD_ECC <input_graph> <output_result> <time_log>
```

### Example Usage with `NY.gr`
```bash
./TD_ECC NY.gr output.txt time_log.txt
```

#### Terminal Output Example:
```text
start reading graph
after reading graph
start brandes
start TD_SPT
...
scan_cnt 14502093
scan_cnt_edge 58201948
NY.gr  1.23456
```

---

## License

This project is open-source. Feel free to modify and adapt it for academic research or graph processing experiments.

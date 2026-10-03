# Weighted Network Configurator — C++ / Qt

A desktop academic prototype for **enumerating, ranking and visualizing feasible weighted network configurations** under per-player degree constraints.

The project originated as part of a master's-level academic project and has been repackaged for a public technical portfolio. The original source snapshot is preserved alongside a small portability cleanup.

## What the program does

Given a set of players, the program accepts:

- the required number of links for each player;
- admissible pairwise links;
- a numeric weight for every admissible link.

It then recursively enumerates feasible undirected configurations, computes the aggregate link weight, sorts the resulting configurations and visualizes them in a Qt desktop interface.

```mermaid
flowchart LR
    A[Players + required degree] --> B[Admissible weighted links]
    B --> C[Recursive configuration search]
    C --> D[Feasibility checks]
    D --> E[Rank by aggregate weight]
    E --> F[Qt graph visualization]
```

## Why this repository is in my portfolio

It demonstrates a different part of my technical background than my Python research repositories:

- C++17;
- Qt Widgets desktop development;
- graph-like data structures;
- recursive / combinatorial search;
- constraint checking;
- ranking of feasible configurations;
- GUI visualization;
- migration of a legacy Visual Studio project to a portable CMake layout.

## Repository structure

```text
.
├── CMakeLists.txt
├── src/
│   ├── core/                  # algorithmic core, no Qt dependency
│   └── gui/                   # Qt Widgets interface
├── tests/
│   └── core_smoke.cpp
├── examples/                  # small legacy input/output examples
├── docs/
│   ├── ALGORITHM.md
│   ├── LIMITATIONS.md
│   └── PROJECT_HISTORY.md
└── legacy/
    ├── original-source/       # unchanged extracted source snapshot
    ├── visual-studio/         # original VS/Qt project metadata
    └── experimental-console/  # historical console experiment
```

## Build

### Core + tests only

Requires a C++17 compiler and CMake 3.21+.

```bash
cmake -S . -B build -DBUILD_QT_GUI=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

### Qt GUI

Install Qt 6 with the Widgets module, then:

```bash
cmake -S . -B build -DBUILD_QT_GUI=ON
cmake --build build
```

Run the generated `network_configurator` executable.

> The public package targets Qt 6. The original Visual Studio project metadata
> is preserved under `legacy/visual-studio/` for provenance.

## Input model

The GUI uses a symmetric matrix:

- column `0` stores the required number of links for the player;
- the remaining cells store pairwise link weights;
- `0` means that the link is not enabled;
- the matrix is mirrored automatically for manual input.

A small 4-player weighted example is preloaded in the GUI.

## Search logic

The core algorithm builds configurations recursively. Before an edge is accepted,
the program verifies that neither endpoint has reached its required degree and that
the edge is not duplicated. Completed configurations are ranked by the sum of
selected edge weights.

More detail: [docs/ALGORITHM.md](docs/ALGORITHM.md).

## Original source and cleanup

The original archive included Visual Studio/Qt build artefacts and IDE caches.
They are excluded from the public repository.

The unchanged extracted source is still available under
[`legacy/original-source`](legacy/original-source). The public `src/` tree applies
small portability and correctness cleanups without changing the basic model.

See [docs/PROJECT_HISTORY.md](docs/PROJECT_HISTORY.md).

## Limitations

This is an academic prototype. The recursive enumeration is not intended for
large graph instances, and the historical file-input branch was not completed.

See [docs/LIMITATIONS.md](docs/LIMITATIONS.md).

## Author

**Maxim Zavrazhnov**  
PhD student, Peter the Great St. Petersburg Polytechnic University  
Background: applied mathematics, systems analysis, data analysis and cooperative game theory.

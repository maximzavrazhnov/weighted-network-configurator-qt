# Project provenance and cleanup

This repository was prepared from the original master's-project archive.

The original archive contained a normal Visual Studio development tree together
with large generated artefacts such as `.vs/`, IntelliSense databases, `ipch`,
`obj`, PDB files and Qt/Visual Studio build outputs. Those generated artefacts
are intentionally excluded from the public repository.

## Preserved material

- original C++ classes for players, configurations and the recursive search;
- original Qt Widgets GUI source;
- original `.ui` and `.qrc` files;
- the main Visual Studio project files;
- small example input/output text files;
- an old experimental console project under `legacy/experimental-console/`.

## Cleanup performed for the public version

The public `src/` tree is a small modernization layer:

- standard copy/move semantics instead of non-const custom assignment;
- const-correct comparisons and accessors;
- defensive checks around malformed asymmetric links;
- portable CMake build;
- optional Qt 6 GUI build;
- a core smoke test independent of Qt;
- generated binaries/caches removed.

The unchanged extracted source remains available under `legacy/original-source/`
for comparison.

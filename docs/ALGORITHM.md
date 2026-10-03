# Algorithm overview

The program models an undirected weighted network of **players**.

Each player has:

- a required number of links (`numOfLinks`);
- a list of admissible counterparties;
- a weight associated with each admissible link.

The recursive search in `System::step` explores feasible combinations of links. A link is added only when:

1. both endpoints still have free degree capacity;
2. the link is not already present;
3. the counterpart exists in the player set;
4. the symmetric link is present in the input model.

For each completed or terminal configuration, the program stores:

- the adjacency structure;
- the aggregate weight of selected links.

Configurations are sorted by total weight and can be browsed in the Qt GUI.

## Why the search is combinatorial

For `n` players, the number of possible undirected edges can reach `n(n-1)/2`.
Degree constraints and admissible-link lists prune the search space, but the
problem remains combinatorial in the general case.

## Original vs cleaned version

The repository contains two layers:

- `legacy/original-source/` — source extracted unchanged from the original
  Visual Studio / Qt project;
- `src/` — a portability cleanup that keeps the same algorithmic idea while
  fixing C++ copy/const correctness, defensive bounds checks and build structure.

No claim is made that the cleanup changes the underlying mathematical model.

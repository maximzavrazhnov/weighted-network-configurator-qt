# Scope and limitations

This is an academic desktop prototype, not a production network-optimization
library.

Important limitations:

- exhaustive/recursive enumeration can grow quickly with the number of players;
- the GUI expects a symmetric weighted adjacency input;
- the file-input branch visible in the historical GUI was unfinished;
- the objective is aggregate link weight under degree/admissibility constraints;
  it does not model uncertainty, dynamic updates or large-scale graph optimization;
- no claim is made that a higher aggregate weight is a causal measure of team
  performance.

The repository is published primarily to demonstrate C++/Qt development,
graph-like data structures, combinatorial search and visualization.

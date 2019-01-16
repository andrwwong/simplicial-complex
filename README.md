# Simplicial Complex

Simplicial Complex is a data mining method that utilizes a top-down approach similar to FP-growth. It was invented by Dr. Ty Lin. The algorithm creates association rules by representing numbers in n-dimensional planes as simplexes. Each row of points the data set is visualized as an abstract vertex, and every point in a given row is represented as a closed simplex. The union of all such closed simplexes form a simplicial complex, which is a combinatorial representation of polyhedron. Run with VisualStudio 2013.

Below is a list of basic terminology to better understand the program:
- Base: A closed simplex which has a support that exceeds or meets the minimum support threshold.
- Closed Simplex: The set of all points which make up an n-dimensinal shape. All simplexes in the program are closed.
- Cone: A closed simplex.
- Minimum Support Threshold: The minimum number of times a closed simplex must appear in the data set for the simplex to be     considered useful.
- Support: The number of times a simplex appears.

Files:
- SimplicialComplex.cpp: source file
- Data.txt: Example data file, with the first number in every row representing the number of points in the given row
- Results.txt: Example results using Data.txt. The brackets represent a base; the number outside of the bracket is the base's   support.

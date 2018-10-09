#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <time.h>

/*
	Author: Andrew Wong

	This program creates association rules by representing numbers in n-dimensional planes as
	n-dimensional shapes called simplexes. A top down approach similar to FP-growth is implemented.

	Each row of points in the data set is visualized as an abstract vertex, with every point
	represented as a closed simplex. (Note: All simplexes in this program are to be assumed to
	be closed.) The union of all such closed simplexes form a simplicial complex, which is a
	combinatorial representation of a  polyhedron.

	The aforementioned representation of points transforms association rules into a geometric
	traversal problem.

	Below is a list of basic terminology to better understand the program:
	- Base: A closed simplex which has a support that exceeds or meets the minimum support threshold.
	- Closed Simplex: The set of all points which make up an n-dimensinal shape. All simplexes in
							the program are closed, whether or not it is explicity stated.
	- Cone: A closed simplex.
	- Minimum Support Threshold: The minimum number of times the closed simplex must exist in the
											data set for the simplex to be considered useful.
	- Support: The number of times a simplex appears.


*/
class SimplicialComplex {
	public:
		SimplicialComplex();
		void run();
	private:

		std::map<int, std::vector<int>> points;

		std::vector<std::pair<std::vector<int>, int>> frequencyOfPoints, simplicialComplexes;
		std::vector<std::vector<int>> cones;
		std::vector<int> sortedPoints, cone;
		int minSupp, support, count, positionInOne;
		double runtimeReading, runtimeAlgorithm, runtimeWriting;
		long clockStart;
		bool isDone;

		void readFile(std::string filename);
		void writeToFile(std::string filename);
		void firstStep();
		void createCone();
		void countFreq();
};

SimplicialComplex::SimplicialComplex() {
	count = 2;
	minSupp = 1;
	isDone = false;
}

/*
	Reads in a data file in the form
	x # # # # # # #
	where x represents the number of points the row and # represents
	a single point

	filename: the name of the file
*/
void SimplicialComplex::readFile(std::string filename) {
	std::stringstream reader;
	std::string input = "";
	std::ifstream file;
	int point;

	file.open(filename);

	// The file is read in line by line.
	for (int numRow = 0; getline(file, input); numRow++) {
		reader = std::stringstream(input);
		reader >> point;

		while (reader >> point)
			points[point].push_back(numRow);
	}

	file.close();

	runtimeReading = (clock() - clockStart) / 1000.0;
}

// The data set is sorted from smallest to largest. A zero-dimensional simplex is constructed.
void SimplicialComplex::firstStep() {
	// The dataset is pruned. The zero-dimensional simplexes with a support that exceeds or meets
	// the minimum support threshold are kept, while the rest is disregarded.
	for (const auto &point : points) {
		if (point.second.size() > minSupp)
			frequencyOfPoints.push_back(std::make_pair(std::vector<int> { point.first },
												 point.second.size()));
	}

	// Sorts the dataset from smallest to largest.
	struct {
		bool operator()(std::pair<std::vector<int>, int> a,
							 std::pair<std::vector<int>, int> b) const {
			return a.second > b.second;
		}
	} customCompare;

	sort(frequencyOfPoints.begin(), frequencyOfPoints.end(), customCompare);

	for (size_t i = 0; i < frequencyOfPoints.size(); i++)
		sortedPoints.push_back(frequencyOfPoints[i].first[0]);

	// A zero-dimensional simplex is constructed.
	cone = { sortedPoints[0] };
	cones.push_back(points[cone[0]]);
	positionInOne = 0;
}

// The current simplex's support is found.
void SimplicialComplex::countFreq() {
	support = 0;
	std::vector<int> simplex, intersectingPoints;
	simplex = { cones[0] };

	// Counts the number of times a group of points appears in the other planes.
	// ie. The support of the simplex is counted.
	for (size_t i = 1; i < cones.size() && simplex.size() > 0; i++) {
		set_intersection(cones[i].begin(), cones[i].end(), simplex.begin(), simplex.end(),
							  back_inserter(intersectingPoints));
		simplex = intersectingPoints;
		intersectingPoints.clear();
	}

	support = base.size();
}

/*
	If the support of the simplex exceeds or meets the minimum support threshold,
	the simplex can be called a simplical complex. In essence, the cone becomes a base.

	If not all points have been tested, a new simplex is constructed from the points
	in the data set. Either an extra point is added to the simplex or a zero-dimensional simplex
	is constructed in a plane.
*/
void SimplicialComplex::createCone() {
	// If the support of the simplex is greater than or equal to the minimum support threshold,
	// the (now) simplicial complex is added to the list of results.
	if (support >= minSupp)
		simplicialComplexes.push_back(make_pair(cone, support));

	// If all the points have been tested, the program terminates.
	if (cone[0] == sortedPoints[sortedPoints.size() - 1])
		isDone = true;
	// If all possible combinations with the base have not yet been tested, a new cone
	// is created by adding a point - and therefore dimension - to the simplex.
	else if (support > minSupp && cone[cone.size() - 1] != sortedPoints[sortedPoints.size() - 1]) {
		positionInOne++;
		cone.push_back(sortedPoints[positionInOne]);
		cones.push_back(points.at(cone[cone.size() - 1]));
	}
	// If the simplex's support is less than or equal to the minimum support threshold,
	// a new zero-dimensional simplex is constructed in the same plane.
	else if (support <= minSupp && cone.size() > 1 &&
				cone[1] != sortedPoints[sortedPoints.size() - 1]) {
		int currentPoint = cone[0];
		int nextPoint = (std::find(sortedPoints.begin(), sortedPoints.end(), cone[0]) -
							  sortedPoints.begin()) + count;

		count++;
		positionInOne = nextPoint;

		// A new simplex is constructed.
		cone.clear();
		cone = { currentPoint, sortedPoints[nextPoint] };

		cones.clear();
		cones.push_back(points.at(cone[0]));
		cones.push_back(points.at(cone[1]));
	}
	// A new zero-dimensional simplex is constructed in a new plane.
	else {
		int point = (std::find(sortedPoints.begin(), sortedPoints.end(), cone[0]) -
						 sortedPoints.begin()) + 1;

		count = 2;
		positionInOne = point;

		// A new simplex is constructed.
		cone.clear();
		cone.push_back(sortedPoints[point]);

		cones.clear();
		cones.push_back(points.at(cone[0]));
	}
}

/*
	Writes found association rules to the specified file.

	filename: the file to write to
*/
void SimplicialComplex::writeToFile(std::string filename) {
	std::ofstream file;
	file.open(filename);

	for (size_t i = 0; i < simplicialComplexes.size(); i++) {
		file << "[";

		for (size_t j = 0; j < (int)(simplicialComplexes[i].first.size()) - 1; j++)
			file << simplicialComplexes[i].first[j] << " ";

		file << simplicialComplexes[i].first[simplicialComplexes[i].first.size() - 1] <<
				  "] " << simplicialComplexes[i].second << "\n";
	}

	file.close();
}

/*
	Forms association rules by mining the data. The results are written to the specified
	file, and the time taken to run the test printed to the console.
*/
void SimplicialComplex::run() {
	clockStart = clock();
	readFile("Data.txt");
	firstStep();

	while (!isDone) {
		countFreq();
		createCone();
	}

	runtimeAlgorithm = (clock() - clockStart) / 1000.0;

	clockStart = clock();
	writeToFile("Results.txt");
	runtimeWriting = (clock() - clockStart) / 1000.0;

	std::cout << "Time to read file: " << runtimeReading << " seconds\n";
	std::cout << "Time to run algorithm: " << runtimeAlgorithm << " seconds\n";
	std::cout << "Time to write to file: " << runtimeWriting << " seconds\n";
	std::cout << "Simplicial Complex has successfully run. \n";
}


int main() {
	SimplicialComplex simplicialComplex;
	simplicialComplex.run();
}

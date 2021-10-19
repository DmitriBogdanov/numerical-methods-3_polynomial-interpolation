#pragma once

#include <vector>
#include <cmath>



using uint = unsigned int;
using Grid = std::vector<double>;

constexpr double PI = 3.14159265358979323846;

// a - left bound
// b - right bound
// N - number of points in the grid
Grid make_grid(double a, double b, uint N) {
	Grid grid(N);

	const auto step = (b - a) / (N - 1);

	for (uint i = 0; i < N; ++i) grid[i] = a + i * step;

	return grid;
}

Grid make_chebyshev_grid(double a, double b, uint N) {
	Grid grid(N);

	
	const auto middle = (a + b) / 2.;
	const auto halfDifference = (b - a) / 2.;
	const auto coef = PI / 2. / (N + 1.);

	for (uint i = 0; i < N; ++i) grid[i] = middle + halfDifference * std::cos(coef * (2. * i + 1.));

	return grid;
}
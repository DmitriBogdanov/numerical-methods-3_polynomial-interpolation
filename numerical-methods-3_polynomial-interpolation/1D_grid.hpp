#pragma once

#include <vector>
#include <cmath>



using uint = unsigned int;
using Array = std::vector<double>;
using Function = double(double);

constexpr double PI = 3.14159265358979323846;

// a - left bound
// b - right bound
// N - number of points in the grid
inline Array make_grid(double a, double b, uint N) {
	Array grid(N + 1);

	const auto step = (b - a) / N;

	for (uint i = 0; i < N + 1; ++i) grid[i] = a + i * step;

	return grid;
}

inline Array make_chebyshev_grid(double a, double b, uint N) {
	Array grid(N);

	
	const auto middle = (a + b) / 2.;
	const auto halfDifference = (b - a) / 2.;
	const auto coef = PI / 2. / (N + 1.);

	for (uint i = 0; i < N; ++i) grid[i] = middle + halfDifference * std::cos(coef * (2. * i + 1.));

	return grid;
}

inline Array evaluate_at_grid(Function *f, const Array &grid) {
	Array res(grid.size());

	for (size_t i = 0; i < grid.size(); ++i)
		res[i] = f(grid[i]);

	return res;
}
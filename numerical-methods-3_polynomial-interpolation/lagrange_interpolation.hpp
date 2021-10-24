#pragma once

#include "1D_grid.hpp"



// Returns L(xp)
inline double lagrange_interpolate(Function *f, const Array &x, const Array &y, double xp) {
	const auto N = x.size();

	double res = 0.;

	for (size_t i = 0; i < N; ++i) {
		double c = 1.;

		for (size_t j = 0; j < N; ++j) {
			if (i != j) c *= (xp - x[j]) / (x[i] - x[j]);
		}

		c *= y[i];

		res = res + c;
	}

	return res;
}

// Applies 'lagrange_interpolate()' at given grid
inline Array lagrange_interpolate_at_grid(Function *f, const Array &x, const Array &y, const Array &grid) {
	Array interpolation(grid.size());

	for (size_t i = 0; i < grid.size(); ++i)
		interpolation[i] = lagrange_interpolate(f, x, y, grid[i]);

	return interpolation;
}
#pragma once

#include "1D_grid.hpp"
#include "tmatrix.hpp"



inline Array spline_interpolate_at_grid(Function *f, const Array &x, const Array &y, const Array &grid) {
	const size_t N = x.size() - 1;

	// Fill h[i]
	Array h(N);
	for (size_t i = 0; i < N; ++i) h[i] = x[i + 1] - x[i];

	// Fill g[i]
	Array g(N);
	for (size_t i = 0; i < N; ++i) g[i] = (y[i + 1] - y[i]) / h[i];

	// Construct tridiagonal system
	DMatrix A_(N + 1, 3), b_(N + 1, 1);

	A_[0][1] = 1.;
	A_[0][2] = 0.;
	for (size_t i = 1; i < N; ++i) {
		A_[i][0] = h[i - 1];
		A_[i][1] = 2. * (h[i - 1] + h[i]);
		A_[i][2] = h[i];
		b_(i) = 3. * (g[i] - g[i - 1]);
	}
	A_[N][0] = 0.;
	A_[N][1] = 1.;

	// Find c[i]
	Array c(N + 1);

	tridiagonal_solve_in_place(A_, b_);
	for (size_t i = 0; i < N; ++i) c[i] = b_(i);

	// Find b[i]
	Array b(N);

	for (size_t i = 0; i < N; ++i) b[i] = g[i] - h[i] * (c[i + 1] + 2. * c[i]) / 3.;

	// Find d[i]
	Array d(N);

	for (size_t i = 0; i < N; ++i) d[i] = (c[i + 1] - c[i]) / h[i] / 3.;

	// Find a[i]
	Array a(N);

	for (size_t i = 0; i < N; ++i) a[i] = y[i];

	// Finally, interpolate at given grid
	Array res(grid.size());

	for (size_t k = 0, i = 1; k < grid.size(); ++k) {
		// Adjust current interval [ x[i-1] ; x[i] ] untill it contains grid[k]
		while (x[i] < grid[k]) ++i;
			

		// Interpolate
		const auto DX = grid[k] - x[i - 1];

		res[k] =
			a[i - 1] +
			b[i - 1] * DX +
			c[i - 1] * DX * DX +
			d[i - 1] * DX * DX * DX;
	}

	return res;
}
#include <exception>

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

#include <cmath>



#include "static_timer.hpp"
#include "lagrange_interpolation.hpp"
#include "spline_interpolation.hpp"



// Range of interpolation
constexpr double A = -1.;
constexpr double B = 1.;

// Precalculate consts for functions
const double R1_CONST = 3. + std::pow(2., 1. / 3.);
const double R2_CONST_1 = 5. * std::sqrt(2.);
const double R2_CONST_2 = 4.;
const double R2_CONST_3 = std::sqrt(6.);
const double R2_CONST_4 = std::sqrt(3.);
	// cant make these constexpr since most math function are not constexpr
	// sadge

// Interpolated functions
double R1(double x) {
	return std::cos(std::pow(x, 5) - x + R1_CONST);
}

double R2(double x) {
	return std::atan((std::pow(x, 3) - R2_CONST_1 * x - R2_CONST_2) / (R2_CONST_3 * x + R2_CONST_4));
}

double f_default(double x) {
	return R1(x) + R2(x) - 1.;
}

double f_runge(double x) {
	return 1. / (25. + x * x);
}

double f_const(double x) {
	return 1.;
}

double f_linear(double x) {
	return x;
}

double f_square(double x) {
	return x * x;
}



void save_grid_with_values_as(const Array &arrayX, const Array &arrayY, const std::string &filepath) {
	std::ofstream outFile(filepath);

	constexpr std::streamsize ALIGNMENT = 20;
	const auto N = arrayX.size();

	for (size_t i = 0; i < N; ++i)
		outFile
		<< std::setw(ALIGNMENT) << arrayX[i] 
		<< std::setw(ALIGNMENT) << arrayY[i]
		<< '\n';
}

int main(int* argc, char** argv) {

	try {
		// Select function to interpolate
		Function *f = &f_default;

		std::cout << ">>> Making grids...\n";

		// Select grid
		const auto x = make_grid(A, B, 16);
		const auto y = evaluate_at_grid(f, x);

		// Select interpolation grid
		const auto intpGrid = make_grid(A, B, 256);

		std::cout << ">>> Computing...\n";

		// Interpolate
		const auto intpValues = spline_interpolate_at_grid(f, x, y, intpGrid);

		// Evaluate true values at the same grid
		const auto trueValues = evaluate_at_grid(f, intpGrid);

		// Save the result
		save_grid_with_values_as(x, y, "[output]/grid.txt");
		save_grid_with_values_as(intpGrid, intpValues, "[output]/intp.txt");
		save_grid_with_values_as(intpGrid, trueValues, "[output]/true.txt");

	}
	// If caught any errors, show error message
	catch (const std::runtime_error& err) {
		std::cerr << "RUNTIME EXCEPTION -> " << err.what() << std::endl;
	}
	catch (...) {
		std::cerr << "CAUGHT UNKNOWN EXCEPTION" << std::endl;
	}

	return 0;
}
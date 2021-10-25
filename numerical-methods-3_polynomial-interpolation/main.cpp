#include <exception>
#include <tuple>

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

constexpr auto CONFIG_PATH = "config.txt";

// Parse config and return input/output filepaths
// @return 1 => input filepath
// @return 2 => output filepath
// @return 3 => epsilon
// @return 4 => max iterations

std::tuple<std::string, int, size_t, int, size_t, int> parse_config() {
	std::ifstream inConfig(CONFIG_PATH);

	if (!inConfig.is_open()) throw std::runtime_error("Could not open config file");

	std::string outputFilepath;
	int gridType;
	size_t gridSize;
	int intpGridType;
	size_t intpGridSize;
	int intpType;

	std::string dummy; // used to skip comments

	inConfig
		>> dummy >> outputFilepath
		>> dummy >> gridType
		>> dummy >> gridSize
		>> dummy >> intpGridType
		>> dummy >> intpGridSize
		>> dummy >> intpType;

	return { outputFilepath, gridType, gridSize, intpGridType, intpGridSize, intpType };
}

std::string grid_type_string(int val) {
	switch (val) {
	case 0: return "Evenly_spaced_grid";
	case 1: return "Chebyshev_grid";
	default: throw std::runtime_error("Unknown grid type");
	}
}

std::string interpolation_type_string(int val) {
	switch (val) {
	case 0: return "Lagrange_polynomial_interpolation";
	case 1: return "Spline_interpolation";
	default: throw std::runtime_error("Unknown grid type");
	}
}

Array build_grid(int gridType, size_t gridSize) {
	std::cout << "\n##### Grid -> " << grid_type_string(gridType) << "\n##### N    -> " << gridSize 
		<< "\n>>> Building the grid...\n";

	switch (gridType) {
	case 0: return make_grid(A, B, gridSize);
	case 1: return make_chebyshev_grid(A, B, gridSize);
	default: throw std::runtime_error("Unknown grid type");
	}
}

Array interpolate(Function* f, const Array &x, const Array &y, const Array &intpGrid, int intpType) {
	std::cout << "\n##### Interpolation -> " << interpolation_type_string(intpType)
		<< "\n>>> Interpolating...\n\n";

	switch (intpType) {
	case 0: return lagrange_interpolate_at_grid(f, x, y, intpGrid);
	case 1: return spline_interpolate_at_grid(f, x, y, intpGrid);
	default: throw std::runtime_error("Unknown grid type");
	}
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

	std::cout << ">>> Grid & values saved at <" + filepath + ">\n";
}

void save_used_options(int gridType, size_t gridSize, int intpGridType, size_t intpGridSize, int intpType, const std::string &filepath) {
	std::ofstream outFile(filepath);

	outFile
		<< grid_type_string(gridType) << '\n'
		<< gridSize << '\n'
		<< grid_type_string(intpGridType) << '\n'
		<< intpGridSize << '\n'
		<< interpolation_type_string(intpType);

	std::cout << ">>> Used options saved at <" + filepath + ">\n";
}

int main(int* argc, char** argv) {

	try {
		const auto [outputFilepath, gridType, gridSize, intpGridType, intpGridSize, intpType] = parse_config();

		// Select function to interpolate
		Function *f = &f_default;

		// Select grid
		const auto x = build_grid(gridType, gridSize);
		const auto y = evaluate_at_grid(f, x);

		// Select interpolation grid
		const auto intpGrid = build_grid(intpGridType, intpGridSize);

		// Interpolate
		const auto intpValues = interpolate(f, x, y, intpGrid, intpType);

		// Evaluate true values at the same grid
		const auto trueValues = evaluate_at_grid(f, intpGrid);

		// Save the result
		save_grid_with_values_as(x, y, outputFilepath + "/grid.txt");
		save_grid_with_values_as(intpGrid, intpValues, outputFilepath + "/intp.txt");
		save_grid_with_values_as(intpGrid, trueValues, outputFilepath + "/true.txt");
		save_used_options(gridType, gridSize, intpGridType, intpGridSize, intpType, outputFilepath + "/options.txt");
	}
	// If caught any errors, show error message
	catch (const std::runtime_error& err) {
		std::cerr << "\nRUNTIME EXCEPTION -> " << err.what() << std::endl;
	}
	catch (...) {
		std::cerr << "\nCAUGHT UNKNOWN EXCEPTION" << std::endl;
	}

	return 0;
}
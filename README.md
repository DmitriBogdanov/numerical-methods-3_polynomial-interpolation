# Numerical methods 3 / Polynomial Interpolation

Contains implementations of following interpolation methods:

* Lagrange polynomial interpolation
* Cubic spline interpolation

Note that present implementations are intended for academic purposes, as such they are not meant to be used in any sort of high-performance production code.

## Compilation

* Recommended compiler: MSVC v142
* Requires C++17 support

## Usage

Place config file of the following format into the same folder as executable:

* Line 1: OUTPUT_FOLDER [value without whitespaces]
* Line 2: GRID_TYPE [0 for even grid / 1 for chebyshev grid]
* Line 3: GRID_SIZE [value]
* Line 4: GRID_TYPE [0 for even grid / 1 for chebyshev grid]
* Line 5: GRID_SIZE [value]
* Line 6: INTERPOLATION_TYPE [0 for lagrange / 1 for spline]

Refer to 'config.txt.' as an example. Upon execution no furter inputs are required.

## Version history

* 00.04
    * Fixed reversed ordering of chebyshev grid that caused issues during spline interpolation

* 00.03
    * Implemented spline interpolation
    * Implemented linear solve for tridiagonal matrices

* 00.02
    * Implemented lagrange interpolation
    * Added generalized concept of functions and a set of functons to be interpolated
    * Added methods for saving resulting grids as files

* 00.01
    * Implemented generation of 1D chebyshev grids

## License

This project is licensed under the MIT License - see the LICENSE.md file for details
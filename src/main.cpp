#include <iostream>
#include <string>

#include "matrix.h"

using namespace std;

int main() {
	Matrix<int, 3, 3> m;
	m.set(1, 1, 4);

	cout << m << endl;

	Matrix<int, 3, 3> m2;
	m2.set(1, 1, 3);

	cout << m2 << endl;

	MatrixSum<Matrix<int, 3, 3>, Matrix<int, 3, 3>> s(m, m2);

	cout << s.get(1, 1) << endl;

	cout << m + m2 << endl;

	Matrix<int, 3, 2> m3;

	cout << m3 << std::endl;

}

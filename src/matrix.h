#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>

template<typename Type>
concept canBeInMatrix = requires(Type obj, std::ostream stream) {
	obj += obj;
	{obj + obj} -> std::convertible_to<Type>;
	{obj * obj} -> std::convertible_to<Type>;
};

template<typename Type>
concept isMatrix = requires(Type obj, int num1, int num2) {
	obj.get(num1, num2);
};

template<typename Type>
concept isSizedMatrix = requires(Type obj) {
	requires isMatrix<Type>;
	{obj.getSizeI()} -> std::same_as<int>;
	{obj.getSizeJ()} -> std::same_as<int>;
};

template<typename Type>
concept isPrintableMatrix = requires(Type obj, std::ostream stream, int num1, int num2) {
	requires isSizedMatrix<Type>;
	stream << obj.get(num1, num2);
};

template<typename MatrixType, typename Type>
concept isTypedMatrix = requires(MatrixType obj, int num1, int num2) {
	requires isMatrix<MatrixType>;
	requires canBeInMatrix<Type>;
	{obj.get(num1, num2)} -> std::same_as<Type>;
};

template<typename MatrixType, typename Type>
concept isModifiableMatrix = requires(MatrixType obj, int num1, int num2, Type val) {
	requires isTypedMatrix<MatrixType, Type>;
	obj.set(num1, num2, val);
};

template<typename Type, int sizeI, int sizeJ> requires canBeInMatrix<Type>
class Matrix {
	public:
		Matrix() : size(sizeI * sizeJ) {
			this->value = new Type[this->size];

			for(auto i(0) ; i<this->size ; i++) {
				this->value[i] = Type();
			}
		}

		Matrix(isTypedMatrix<Type> auto const& other) : size(sizeI * sizeJ) {
			this->value = new Type[this->size];
			std::cout << "Copy constructing" << std::endl;

			for(auto i(0) ; i<sizeI ; i++) {
				for(auto j(0) ; j<sizeJ ; j++) {
					std::cout << i << "/" << j << std::endl;
					this->value[i * sizeJ + j] = other.get(i, j);
				}
			}
		}

		~Matrix() {
			delete[] this->value;
			this->value = nullptr;
		}

		// Getters / Setters
		
		Type get(int i, int j) const {
			return this->value[i * sizeJ + j];
		}

		void set(int i, int j, Type val) {
			this->value[i * sizeJ + j] = val;
		}

		constexpr int getSizeI() const {
			return sizeI;
		}

		constexpr int getSizeJ() const {
			return sizeJ;
		}

		const Type* getValue() const {
			return this->value;
		}

		// Operators



	private:
		Type* value;

		const int size;

};

template<isMatrix Type>
class MatrixSum {
	public:
		MatrixSum(Type const& a, Type const& b) {
			this->matrixA = &a;
			this->matrixB = &b;

			std::cout << "MatrixSum constructed" << std::endl;
		}

		// Getters / Setters
		
		auto get(int i, int j) const {
			return this->matrixA->get(i, j) + this->matrixB->get(i, j);
		}


	protected:
		const Type* matrixA;
		const Type* matrixB;

};

template<isSizedMatrix Type>
class SizedMatrixSum : public MatrixSum<Type> {
	public:
		SizedMatrixSum(Type const& a, Type const& b) : MatrixSum<Type>::MatrixSum(a, b) {
			
		}

		int getSizeI() const {
			return this->matrixA.getSizeI();
		}

		int getSizeJ() const {
			return this->matrixA.getSizeJ();
		}
};

template<isPrintableMatrix Type>
std::ostream& operator << (std::ostream& stream, Type const& a) {
	for(auto i(0) ; i<a.getSizeI() ; i++) {
		for(auto j(0) ; j<a.getSizeJ() ; j++) {
			stream << a.get(i, j) << " ";
		}
		stream << std::endl;
	}

	return stream;
}

template<isMatrix Type>
MatrixSum<Type> sum(Type const& a, Type const& b) {
	return MatrixSum<Type>(a, b);
}

template<isSizedMatrix Type>
SizedMatrixSum<Type> sum(Type const& a, Type const& b) {
	return SizedMatrixSum<Type>(a, b);
}

#endif

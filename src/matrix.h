#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>

template<typename MatrixType>
concept isMatrix = requires(MatrixType obj, int num1, int num2) {
	obj.get(num1, num2);
	{obj.getSizeI()} -> std::same_as<int>;
	{obj.getSizeJ()} -> std::same_as<int>;
};

template<typename MatrixType>
concept isPrintableMatrix = requires(MatrixType obj, std::ostream stream, int num1, int num2) {
	requires isMatrix<MatrixType>;
	stream << obj.get(num1, num2);
};

template<typename MatrixType, typename Type>
concept isTypedMatrix = requires(MatrixType obj, int num1, int num2) {
	requires isMatrix<MatrixType>;
	{obj.get(num1, num2)} -> std::same_as<Type>;
};

template<typename Type, int sizeI, int sizeJ>
concept isSizedMatrix = requires(Type obj) {
	requires isMatrix<Type>;
	requires (obj.getSizeI() == sizeI);
	requires (obj.getSizeJ() == sizeJ);
};

template<typename Type>
concept isSquareMatrix = requires(Type obj) {
	requires isMatrix<Type>;
	requires (obj.getSizeI() == obj.getSizeJ());
};

template<typename Type1, typename Type2>
concept isAddableMatrix = requires(Type1 obj1, Type2 obj2, int num1, int num2) {
	requires isMatrix<Type1>;
	requires isMatrix<Type2>;
	obj1.get(num1, num2) + obj2.get(num1, num2);
	requires (obj1.getSizeI() == obj2.getSizeI());
	requires (obj1.getSizeJ() == obj2.getSizeJ());
};

template<typename Type1, typename Type2>
concept isMultipliableMatrix = requires(Type1 obj1, Type2 obj2, int num1, int num2) {
	requires isMatrix<Type1>;
	requires isMatrix<Type2>;
	obj1.get(num1, num2) + obj2.get(num1, num2);
	obj1.get(num1, num2) * obj1.get(num1, num2);
	requires (obj1.getSizeJ() == obj2.getSizeI());
};

template<typename Type, typename MatrixType>
concept isPutableInMatrix = requires(MatrixType matrix, Type val, int num1, int num2) {
	requires isTypedMatrix<MatrixType, Type>;
	matrix.set(num1, num2, val);
};

template<typename Type, int sizeI, int sizeJ>
class Matrix {
	public:
		Matrix() : size(sizeI * sizeJ) {
			this->value = new Type[this->size];

			for(auto i(0) ; i<this->size ; i++) {
				this->value[i] = Type();
			}
		}

		template<typename MatrixType> requires isSizedMatrix<MatrixType, sizeI, sizeJ> && isTypedMatrix<MatrixType, Type>
		Matrix(MatrixType const& other) : size(sizeI * sizeJ) {
			std::cout << "Copy constructing" << std::endl;
			this->value = new Type[this->size];

			for(auto i(0) ; i<sizeI ; i++) {
				for(auto j(0) ; j<sizeJ ; j++) {
					std::cout << i << "/" << j << std::endl;
					this->value[i * sizeJ + j] = other.get(i, j);
				}
			}
		}

		template<typename MatrixType> requires isSizedMatrix<MatrixType, sizeI, sizeJ> && isTypedMatrix<MatrixType, Type>
		Matrix(MatrixType&& other) : size(sizeI * sizeJ) {
			std::cout << "Move constructing" << std::endl;
			this->value = other.stealValue();
		}

		~Matrix() {
			delete[] this->value;
			this->value = nullptr;
		}

		Type* stealValue() {
			Type* toReturn(this->value);
			this->value = nullptr;
			return toReturn;
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

template<isMatrix Type1, isMatrix Type2> requires isAddableMatrix<Type1, Type2>
class MatrixSum {
	public:
		MatrixSum(Type1 const& a, Type2 const& b) {
			this->matrixA = &a;
			this->matrixB = &b;

			std::cout << "MatrixSum constructed" << std::endl;
		}

		// Getters / Setters
		
		auto get(int i, int j) const {
			return this->matrixA->get(i, j) + this->matrixB->get(i, j);
		}

		constexpr int getSizeI() const {
			return this->matrixA->getSizeI();
		}

		constexpr int getSizeJ() const {
			return this->matrixA->getSizeJ();
		}


	protected:
		const Type1* matrixA;
		const Type2* matrixB;

};

/* template<isMatrix Type1, isMatrix Type2> requires isMultipliableMatrix<Type1, Type2> */
/* class MatrixProduct { */
/* 	public: */
/* 		MatrixProduct(Type1 const& a, Type2 const& b) { */
/* 			this->matrixA = &a; */
/* 			this->matrixB = &b; */
/* 		} */

/* 		auto get(int i, int j) const { */
/* 			isPutableInMatrix<Type1> auto toReturn; */
/* 			for(auto k(0) ; k<this->matrixA->getSizeJ() ; k++) { */
/* 				toReturn += this->matrixA->get(i, k) * this->matrixB->get(k, j); */
/* 			} */
/* 			return toReturn; */
/* 		} */

/* 		constexpr int getSizeI() const { */
/* 			return this->matrixA->getSizeI(); */
/* 		} */

/* 		constexpr int getSizeJ() const { */
/* 			return this->matrixB->getSizeJ(); */
/* 		} */

/* 	private: */
/* 		const Type1* matrixA; */
/* 		const Type2* matrixB; */

/* }; */

template<isPrintableMatrix Type>
std::ostream& operator << (std::ostream& stream, Type const& a) {
	/* std::cout << "lvalue func" << std::endl; */
	for(auto i(0) ; i<a.getSizeI() ; i++) {
		for(auto j(0) ; j<a.getSizeJ() ; j++) {
			stream << a.get(i, j) << " ";
		}
		stream << std::endl;
	}

	return stream;
}

template<isPrintableMatrix Type>
std::ostream& operator << (std::ostream& stream, Type const&& a) {
	/* std::cout << "rvalue func" << std::endl; */
	for(auto i(0) ; i<a.getSizeI() ; i++) {
		for(auto j(0) ; j<a.getSizeJ() ; j++) {
			stream << a.get(i, j) << " ";
		}
		stream << std::endl;
	}

	return stream;
}

template<isMatrix Type1, isMatrix Type2> requires isAddableMatrix<Type1, Type2>
MatrixSum<Type1, Type2> operator + (Type1 const& a, Type2 const& b) {
	return MatrixSum(a, b);
}

template<isSquareMatrix MatrixType, isPutableInMatrix<MatrixType> Type>
Type trace(MatrixType const& matrix) {
	Type toReturn;
	for(auto i(0) ; i<matrix.getSizeI(); i++) {
		toReturn += matrix.get(i, i);
	}
	return toReturn;
}


#endif

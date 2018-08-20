
#include "matrix.h"

template<typename T>
Matrix<T>::Matrix(int a, int b){
	_m = a; 
	_n = b;                                         //保护数据赋值
	_matrix = new T[_m * _n];                    //动态分配内存
}

template<typename T>
Matrix<T>::~Matrix(){
	if (nullptr != _matrix) {
		delete [] _matrix;
		_matrix = nullptr;
	}
}

template<typename T>
void Matrix<T>::set_matrix(T* rmatr) {                   //设置矩阵
	int size = _m * _n;
	for (int i = 0; i < size; i++){
		*(_matrix + i) = rmatr[i];                          //矩阵成员赋初值
	}
}

template<typename T>
Matrix<T>* Matrix<T>::get_rev(){
	Matrix<T>* pR = new Matrix<T>(_n, _m);
	for (int j = 0; j < _n; j++){
		for (int i = 0; i < _m; i++)
			*(pR->_matrix + i + _m * j) = *(_matrix + i * _n + j);
	}
	return pR;
}

template<typename T>
Matrix<T>* Matrix<T>::get_mul(Matrix<T> *b){
	if (b->_m != _n) {
		return nullptr;
	}

	Matrix<T> *pR = new Matrix<T>(_m, b->_n);
	for (int i = 0; i < _m; i++){
		for (int j = 0; j < b->_n; j++){
			double temp = 0.0;
			for (int k = 0; k < _n; k++) {
				temp += *(_matrix + i * _n + k) * *(b->_matrix + k * b->_n + j);
			}
			*(pR->_matrix + i * b->_n + j) = temp;
		}
	}

	return pR;
}

template<typename T>
int Matrix<T>::get_m() const{
	return _m;
}

template<typename T>
int Matrix<T>::get_n() const{
	return _n;
}

template<typename T>
T* Matrix<T>::get_matrix() const{
	return _matrix;
}

template<typename T>
T Matrix<T>::get_data(int i, int j) const{
	if (i < _m && j < _n && i >= 0 && j >= 0) {
		return (*(_matrix + i * _n + j));
	}
	
	return 0.0;
}

template<typename T>
void Matrix<T>::set_data(int i, int j, T t){
	if (i < _m && j < _n && i >= 0 && j >= 0){
		*(_matrix + i * _n + j) = t;
	}
}
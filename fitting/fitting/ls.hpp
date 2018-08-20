#include "ls.h"
#include "matrix.hpp"
#include "linequ.hpp"
#include <math.h>

template<typename T>
LS<T>::LS()
{
	//_points_num = 0;
	//_polynomial_num = 0;
	//_polynomial_solution = nullptr;
}

template<typename T>
LS<T>::~LS(){
	if (nullptr != _polynomial_solution) {
		delete[] _polynomial_solution;
		_polynomial_solution = nullptr;
	}
}

template<typename T>
void LS<T>::set_polynomial_num(int t){
	_polynomial_num = t + 1;
	if (nullptr != _polynomial_solution) {
		delete[] _polynomial_solution;
	}
	_polynomial_solution = new T[_polynomial_num];
}

template<typename T>
void LS<T>::add_point(T x, T y){
	_vertexs[_points_num][0] = x;
	_vertexs[_points_num++][1] = y;
}

template<typename T>
bool LS<T>::solve(){
	if (_points_num <= 0 || _polynomial_num <= 0) {
		return false;
	}
	Matrix<T> *A = new Matrix<T>(_points_num, _polynomial_num);
	for (int j = 0; j < _points_num; j++) {
		A->set_data(j, 0, 1.0);
	}
		
	for (int i = 1; i < _polynomial_num; i++){
		for (int j = 0; j < _points_num; j++){
			A->set_data(j, i, A->get_data(j, i - 1) * _vertexs[j][0]);
		}
	}
	Matrix<T> *B = A->get_rev();
	Matrix<T> *b = new Matrix<T>(_points_num, 1);
	for (int i = 0; i < _points_num; i++) {
		b->set_data(i, 0, _vertexs[i][1]);
	}
	Matrix<T>*C = B->get_mul(A);
	Matrix<T> *d = B->get_mul(b);

	Linequ<T> *pL = new Linequ<T>(_polynomial_num);
	pL->set_linequ(C->get_matrix(), d->get_matrix());
	pL->solve();
	T* t = pL->get_solution();
	for (int i = 0; i < _polynomial_num; i++) {
		_polynomial_solution[i] = t[i];
	}
		
	return true;
}

template<typename T>
T* LS<T>::get_solution() const{
	return _polynomial_solution;
}

template<typename T>
T LS<T>::calc_y(T x){
	T y = 0.0, temp = 1.0;
	for (int i = 0; i < n; i++)
	{
		y += _polynomial_solution[i] * temp;
		temp *= x;
	}
	return y;
}

template<typename T>
void LS<T>::restart(){
	if (nullptr = _polynomial_solution) {
		delete[] _polynomial_solution;
	}
	_solution = nullptr;
	_points_num = 0;
	_polynomial_num = 0;
}



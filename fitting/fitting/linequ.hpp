
#include "linequ.h"
#include <math.h>
template<typename T>
Linequ<T>::Linequ(int dims) {
	_index = dims;
	_sums = new T[dims];
	_matrix_a = new T[dims * dims];
	_solu = new T[dims];
}

template<typename T>
Linequ<T>::~Linequ() {
	delete[] _sums;
	delete[] _matrix_a;
	delete[] _solu;
}

template<typename T>
void Linequ<T>::set_matrix(T* rmatr) {
	for (int i = 0; i<_index*_index; i++) {
		*(_matrix_a + i) = rmatr[i];
	}
}

template<typename T>
void Linequ<T>::set_linequ(T* a, T* b) {
	set_matrix(a);
	for (int i = 0; i < _index; i++) {
		_sums[i] = b[i];
	}
}

template<typename T>
int Linequ<T>::solve() {
	int k, is, p, q;
	T d, t;
	int* js = new int[_index];
	int l = 1;
	for (int k = 0; k <= _index - 2; k++) {                             //消去过程
		d = 0.0f;
		for (int i = k; i <= _index - 1; i++)
			for (int j = k; j <= _index - 1; j++) {
				t = fabs(_matrix_a[i*_index + j]);
				if (t>d)
				{
					d = t; js[k] = j; is = i;
				}
			}
		if (d + 1.0 == 1.0) l = 0;
		else {
			if (js[k] != k)
				for (int i = 0; i <= _index - 1; i++) {
					p = i * _index + k; q = i * _index + js[k];
					t = _matrix_a[p];
					_matrix_a[p] = _matrix_a[q];
					_matrix_a[q] = t;
				}
			if (is != k) {
				for (int j = k; j <= _index - 1; j++) {
					p = k * _index + j; q = is * _index + j;
					t = _matrix_a[p];
					_matrix_a[p] = _matrix_a[q];
					_matrix_a[q] = t;
				}
				t = _sums[k];
				_sums[k] = _sums[is];
				_sums[is] = t;
			}
		}
		if (l == 0) {
			delete[] js;
			return 0;
		}
		d = _matrix_a[k*_index + k];
		for (int j = k + 1; j <= _index - 1; j++) {
			p = k * _index + j;
			_matrix_a[p] = _matrix_a[p] / d;
		}
		_sums[k] = _sums[k] / d;
		for (int i = k + 1; i <= _index - 1; i++) {
			for (int j = k + 1; j <= _index - 1; j++) {
				p = i * _index + j;
				_matrix_a[p] = _matrix_a[p] - _matrix_a[i*_index + k] * _matrix_a[k*_index + j];
			}
			_sums[i] = _sums[i] - _matrix_a[i*_index + k] * _sums[k];
		}
	}
	d = _matrix_a[(_index - 1)*_index + _index - 1];
	if (fabs(d) + 1.0 == 1.0) {
		delete[] js;
		return 0;
	}

	_solu[_index - 1] = _sums[_index - 1] / d;                               //回代过程
	for (int i = _index - 2; i >= 0; i--) {
		t = 0.0;
		for (int j = i + 1; j <= _index - 1; j++)
			t = t + _matrix_a[i*_index + j] * _solu[j];
		_solu[i] = _sums[i] - t;
	}
	js[_index - 1] = _index - 1;
	for (int k = _index - 1; k >= 0; k--)
		if (js[k] != k) {
			t = _solu[k]; _solu[k] = _solu[js[k]]; _solu[js[k]] = t;
		}
	delete[] js;
	return(1);
}

template<typename T>
T* Linequ<T>::get_solution() const {
	return _solu;
}

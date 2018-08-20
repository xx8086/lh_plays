#pragma once

template<typename T>
class Matrix{
public:
	Matrix(int a = 2, int b = 2);
	~Matrix();
public:
	int get_m() const;
	int get_n() const;
	Matrix<T>* get_rev();
	Matrix<T>* get_mul(Matrix<T>*);
	void set_matrix(T* rmatr);
	void set_data(int i, int  j, T t);
	T get_data(int i, int  j) const;
	T* get_matrix() const;
private:
	T * _matrix;
	int _m = 0;
	int _n = 0;
};


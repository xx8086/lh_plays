#pragma once

template<typename T>
class Linequ{
public: 
	Linequ(int dims = 2); 
	~Linequ();
	void set_linequ(T* a, T* b);  //方称赋值
	void set_matrix(T* rmatr);
	int solve();                          //全选主元高斯消去法求解方程
	T* get_solution() const;

private:
	T* _sums;                         //方程右端项
	T* _matrix_a;
	T* _solu;                         //方程的解
	int _index;
};



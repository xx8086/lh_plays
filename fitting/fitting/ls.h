#pragma once

template<typename T>
class LS{
#define MAX_POINTS_NUM (2000)
public:
	LS();
	~LS();
public:
	void restart();
	bool solve();
	
	void set_polynomial_num(int num);//num次多项式拟合
	void add_point(T x, T y);
	T* get_solution() const;//获取多项式系数
	T calc_y(T x);//根据x坐标计算y坐标
private:
	int _points_num = 0;//点的个数
	int _polynomial_num = 0;//多项式次数
	T _vertexs[MAX_POINTS_NUM][2];//最多可以拟合2000个点
	T* _polynomial_solution = nullptr;
};

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
	
	void set_polynomial_num(int num);//num�ζ���ʽ���
	void add_point(T x, T y);
	T* get_solution() const;//��ȡ����ʽϵ��
	T calc_y(T x);//����x�������y����
private:
	int _points_num = 0;//��ĸ���
	int _polynomial_num = 0;//����ʽ����
	T _vertexs[MAX_POINTS_NUM][2];//���������2000����
	T* _polynomial_solution = nullptr;
};

#pragma once

template<typename T>
class Linequ{
public: 
	Linequ(int dims = 2); 
	~Linequ();
	void set_linequ(T* a, T* b);  //���Ƹ�ֵ
	void set_matrix(T* rmatr);
	int solve();                          //ȫѡ��Ԫ��˹��ȥ����ⷽ��
	T* get_solution() const;

private:
	T* _sums;                         //�����Ҷ���
	T* _matrix_a;
	T* _solu;                         //���̵Ľ�
	int _index;
};



#pragma once

#include <vector>
#include <cmath>
#include "ls.hpp"

class PolynomialFitting{
public:
	PolynomialFitting();
	virtual ~PolynomialFitting();
public :
	void run();

private:
	template<typename T>
	bool load_datas(const char* src_datas, std::vector<T>& datas);
	template<typename T>
	bool save_datas(const char* src_datas, std::vector<T>& datas);
	template<typename T>
	std::vector<T> fit_curve(std::vector<T> arr_1, std::vector<T> arr_2, int n);
	template<typename T>
	std::vector<T> get_fitting(std::vector<T> co_arr, std::vector<T> point_arr);

};


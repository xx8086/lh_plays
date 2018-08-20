#include "polynomial_fitting.h"
#include <fstream>
#include <iostream>

PolynomialFitting::PolynomialFitting(){
}


PolynomialFitting::~PolynomialFitting(){
}

template<typename T>
bool PolynomialFitting::load_datas(const char* src_datas, std::vector<T>& datas) {
	if (nullptr == src_datas) {
		return false;
	}

	std::string filepath(src_datas);
	std::ifstream infile;
	infile.open(filepath);
	if (!infile) {
		return false;
	}

	T temp;
	while (infile >> temp) {
		datas.emplace_back(temp);
	}

	return true;
}

template<typename T>
bool PolynomialFitting::save_datas(const char* write_path, std::vector<T>& datas) {
	if (nullptr == write_path) {
		return false;
	}
	std::ofstream outfile;
	outfile.open(write_path);
	if (!outfile) {
		return false;
	}

	int fitting_size = datas.size();
	for (int j = 0; j < fitting_size; j++) {
		outfile << datas[j] << std::endl;
	}
	outfile.close();
	return true;
}
void PolynomialFitting::run() {
	std::vector<float> x_datas;
	std::vector<float>  y_datas;
	load_datas<float>("../res/src_datas_x.txt", x_datas);
	load_datas<float>("../res/src_datas_y.txt", y_datas);
	

	std::vector<float> curve_arr = fit_curve<float>(y_datas, x_datas, 4); 
	std::vector<float> fitting = get_fitting<float>(curve_arr, y_datas);
	save_datas<float>("../res/res_x_0821.txt", fitting);

	curve_arr.clear();
	fitting.clear();
	curve_arr = fit_curve<float>(x_datas, y_datas, 4);
	fitting = get_fitting<float>(curve_arr, x_datas);
	save_datas<float>("../res/res_y_0821.txt", fitting);
}

template<typename T>
std::vector<T> PolynomialFitting::fit_curve(std::vector<T> arr_1, std::vector<T> arr_2, int n) {
	LS<T> m_cls;
	std::vector<T> coefficients_set;
	if (arr_1.size() != arr_2.size()) {
		std::cout << " 输入数据有误！" << std::endl;
	}
	if (arr_1.size() == arr_2.size()) {
		for (int i = 0; i < arr_1.size(); i++) {
			m_cls.add_point(arr_1[i], arr_2[i]);
		}
		m_cls.set_polynomial_num(n);
		m_cls.solve();
		T *t_paracof = m_cls.get_solution();
		for (int i = 0; i < n + 1; i++) {
			coefficients_set.push_back(t_paracof[i]); //多项式的系数项，第一项为常数项，最后一项为x^n项
		}
	}
	return coefficients_set;
}

template<typename T>
std::vector<T> PolynomialFitting::get_fitting(std::vector<T> co_arr, std::vector<T> point_arr) {
	std::vector<T> final_point;
	int point_arr_size = point_arr.size();
	int co_arr_size = co_arr.size();
	if (point_arr_size == 0 || co_arr_size == 0) {
		std::cout << "数据点有误！" << std::endl;
	}
	if (point_arr_size > 0 && co_arr_size > 0) {
		for (int i = 0; i < point_arr_size; i++) {
			T temp = 0;
			for (int j = 0; j < co_arr_size; j++) {
				temp += pow(point_arr[i], j)*co_arr[j];
			}
			final_point.push_back(temp);
		}
	}
	return final_point;
}
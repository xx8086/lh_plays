//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "stb_image_write.h"
#include "stb_image.h"

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *heap_scene() {
	int n = 500;
	int rrond = 4;
	hitable **list = new hitable*[n + 1];
	//list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 0;
	int s = 0;
	float allt = (2*rrond + 1)*(2 * rrond + 1);
	for (int a = -rrond; a < rrond; a++) {
		for (int b = -rrond; b < rrond; b++) {
			vec3 center(a*0.5f, b*0.5f, 0.1f);
			float t = float(i) / allt;
			if (0 == s) {
				list[i++] = new sphere(center, 0.2f, new lambertian(vec3(t, 0.0f, 0.0f)));
				s = 1;
			}
			else if (1 == s) {
				list[i++] = new sphere(center, 0.2, new dielectric(1.5f *t));
				s = 2;
				}
			else if(2 == s){
				list[i++] = new sphere(center, 0.2,
					new metal(vec3(0.5*(1 + t), 0.5*(1 + t), 0.5*(1 + t)), 0.5*t));
				s = 0;
			}
		}
	}

	//list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0, new dielectric(1.5));
	//list[i++] = new sphere(vec3(-5.0f, 1.0f, 0.0f), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	//list[i++] = new sphere(vec3(5.0f, 1.0f, 0.0f), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0f));

	return new hitable_list(list, i);
}
hitable *random_scene() {
	int n = 500;
	int rrond = 2;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	std::mt19937 e;	e.seed(std::random_device()());
	std::uniform_real_distribution<double> ur(0.0, 1.0);
	for (int a = -rrond; a < rrond; a++) {
		for (int b = -rrond; b < rrond; b++) {
			float choose_mat = ur(e);
			vec3 center(a + 0.9*ur(e), 0.1, b + 0.9*ur(e));
			if ((center - vec3(4, 0.1, 0)).length() > 0.9) {
				printf("%f, %f, %f\n", center.x(), center.y(), center.z());
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(ur(e)*ur(e), ur(e)*ur(e), ur(e)*ur(e))));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + ur(e)), 0.5*(1 + ur(e)), 0.5*(1 + ur(e))), 0.5*ur(e)));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-5.0f, 1.0f, 0.0f), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(5.0f, 1.0f, 0.0f), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0f));

	return new hitable_list(list, i);
}

int g_ready = 0; // 全局标志位.
//[index_begin, index_end)
void draw_image(hitable *world, unsigned char* datas, camera* cam, int index_begin,  int all_thread_nums, int nx, int ny) {
	int ns = 2;
	std::mt19937 e;	e.seed(std::random_device()());
	std::uniform_real_distribution<float> ur(0.0, 1.0);

	float fnx = 1.0f / float(nx);
	float fny = 1.0f / float(ny);
	int line_nums = ny/all_thread_nums;//每个线程处理的行数
	int line_begin = index_begin * line_nums;
	int line_end = (index_begin + 1) * line_nums - 1;
	int index = index_begin * nx * 3;//起始位置
	//for (int j = line_end; j >= line_begin; j--) {
	for (int j = line_begin; j <= line_end; j++) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + ur(e)) * fnx;
				float v = float(j + ur(e)) * fny;
				ray r = cam->get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			datas[index++] = static_cast<unsigned char>(255.99*col[0]);
			datas[index++] = static_cast<unsigned char>(255.99*col[1]);
			datas[index++] = static_cast<unsigned char>(255.99*col[2]);
		}
	}
	printf("线程 %d 完成， 绘制 %d->%d行\n", std::this_thread::get_id(), line_begin, line_end);
	g_ready += 1 >> index_begin;
}

int main() {
	hitable *list[5];
	float R = cos(M_PI / 4);
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);
	world = heap_scene();

	//vec3 lookfrom(13, 2, 3);
	vec3 lookfrom(0, 0, 8);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	int nx = 800;
	int ny = 600;
	unsigned char* datas = new unsigned char[nx*ny*3];
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
	
	const int threads_num = 4;
	int complete = 0;
	std::thread threads[threads_num];
	for (int i = 0; i < threads_num; i++) {
		complete += 1 >> i;
		threads[i] = std::thread(draw_image, world, datas, &cam, i, threads_num, nx, ny);
	}

	for (auto & th : threads) {
		th.detach();
	}
		
	using namespace std::chrono_literals;
	std::cout << "begin run" << std::endl; // flush is intentional
	auto start = std::chrono::high_resolution_clock::now();
	
	std::chrono::milliseconds dura(5000);
	while (g_ready != complete) {
		std::this_thread::sleep_for(dura);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "runed time: " << elapsed.count()/1000 << " s\n";
	stbi_write_jpg("write.jpg", nx, ny, 3, datas, nx * 3);
	delete[] datas;
	getchar();
}




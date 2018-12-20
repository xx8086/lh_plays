#pragma once

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


class raytracing{
public:
	raytracing();
	~raytracing();
public:
	void make_sphere();
	void run(const int main_thread_loop_polling_time_ms = 2000);

private:
	hitable_list * random_scene();
	hitable_list * heap_scene();

private:
	void release();
	vec3 color(const ray& r, hitable *world, int depth);
	static void draw_image(raytracing* self, hitable *world, unsigned char* datas, camera* cam, int index_begin, int all_thread_nums, int nx, int ny);
private:
	int _ready = 0; // ȫ�ֱ�־λ.
	int _complete = 0;
	int _nx = 1200;
	int _ny = 900;
	unsigned char* _datas = nullptr;
	hitable_list *_world = nullptr;
	camera _cam;
	
};


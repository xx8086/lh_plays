#include "raytracing.h"

raytracing::raytracing(){
	release();
}

raytracing::~raytracing(){
	release();
}

vec3 raytracing::color(const ray& r, hitable *world, int depth) {
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
		vec3 unit_direction = unit_vector(r.direction);
		float t = 0.5*(unit_direction.y + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable_list* raytracing::heap_scene() {
	int n = 500;
	int rrond = 3;
	hitable **list = new hitable*[n + 1];
	int i = 0;
	int s = 0;
	float allt = (2 * rrond + 1)*(2 * rrond + 1);
	for (int a = -rrond; a < rrond; a++) {
		for (int b = -rrond; b < rrond; b++) {
			vec3 center(a*0.5f, b*0.5f + 1.6f, 9.0f);
			float t = float(i) / allt;
			if (0 == s) {
				list[i++] = new sphere(center, 0.2f, new lambertian(vec3(t, 0.0f, 0.0f)));
				s = 1;
			}
			else if (1 == s) {
				list[i++] = new sphere(center, 0.2, new dielectric(1.5f *t));
				s = 2;
			}
			else if (2 == s) {
				list[i++] = new sphere(center, 0.2,
					new metal(vec3(0.5*(1 + t), 0.5*(1 + t), 0.5*(1 + t)), 0.5*t));
				s = 0;
			}
		}
	}

	return new hitable_list(list, i);
}

hitable_list* raytracing::random_scene() {
	int n = 500;
	int rrond = 11;
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

	list[i++] = new sphere(vec3(0.0f, 1.0f, 3.0f), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-3.5f, 1.0f, 3.0f), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(3.5f, 1.0f, 3.0f), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0f));

	return new hitable_list(list, i);
}

void raytracing::release() {
	if (nullptr != _datas) {
		delete[] _datas;
		_datas = nullptr;
	}

	if (nullptr != _world) {
		delete _world;
		_world = nullptr;
	}
}

void raytracing::make_sphere() {
	release();

# if 0
	hitable** list = new hitable*[4];
	float R = cos(M_PI / 4);
	list[0] = new sphere(vec3(0, 2, 12), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -98.5, 9), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 2, 9), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 2, 9), 0.5, new dielectric(1.5));
	_world = new hitable_list(list, 4);
#else
	_world = random_scene();// i7 12线程 179.373s
	//_world = heap_scene();// i7 12线程 50.4867s
#endif
								   //vec3 lookfrom(13, 2, 3);
	vec3 lookfrom(0.0f, 3.0f, 15.0f);
	vec3 lookat(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 9.0f;
	float aperture = 0.1f;
	_datas = new unsigned char[_nx * _ny * 3];
	_cam.set_camera(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), 60.0f, float(_nx) / float(_ny), aperture, dist_to_focus);

}

void raytracing::run(const int main_thread_loop_polling_time_ms) {
	const int thread_num = 4;
	std::thread threads[thread_num];
	for (int i = 0; i < thread_num; i++) {
		_complete += 1 << i;
		threads[i] = std::thread(draw_image, this, _world, _datas, &_cam, i, thread_num, _nx, _ny);
	}

	for (auto & th : threads) {
			th.detach();
	}

	using namespace std::chrono_literals;
	auto start = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds dura(main_thread_loop_polling_time_ms);
	while (_ready != _complete) {
		std::this_thread::sleep_for(dura);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "runed time: " << elapsed.count() / 1000 << " s\n";

	stbi_write_jpg("ray_sphere.jpg", _nx, _ny, 3, _datas, _nx * 3);
}

void raytracing::draw_image(raytracing* self, hitable *world, unsigned char* datas, camera* cam, int index_begin, int all_thread_nums, int nx, int ny) {
	int ns = 3;
	std::mt19937 e;	e.seed(std::random_device()());
	std::uniform_real_distribution<float> ur(0.0, 1.0);

	float fnx = 1.0f / float(nx);
	float fny = 1.0f / float(ny);
	float fns = 1.0f / float(ns);
	int line_nums = ny / all_thread_nums;//每个线程处理的行数
	int line_begin = index_begin * line_nums;
	int line_end = (index_begin + 1) * line_nums - 1;
	int index = line_begin * nx * 3;//起始位置
	printf("线程 %d 开始， 绘制 %d->%d行, index[%d, %d)\n", index_begin, line_begin, line_end, index, (1 + line_end) * nx * 3);
	for (int j = line_begin; j <= line_end; j++) {
		//for (int j = line_end; j >= line_begin; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++) {
				float u = float(i + ur(e)) * fnx;
				float v = float(j + ur(e)) * fny;
				ray r = self->_cam.get_ray(u, v);
				col += self->color(r, world, 0);
			}
			col *= fns;
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			self->_datas[index++] = static_cast<unsigned char>(255.99*col[0]);
			self->_datas[index++] = static_cast<unsigned char>(255.99*col[1]);
			self->_datas[index++] = static_cast<unsigned char>(255.99*col[2]);
		}
	}
	printf("线程 %d 完成， 绘制 %d->%d行\n", index_begin, line_begin, line_end);
	self->_ready += 1 << index_begin;
}


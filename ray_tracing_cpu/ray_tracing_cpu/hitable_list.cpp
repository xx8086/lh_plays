#include "hitable_list.h"



hitable_list::hitable_list(){
	release();
}

hitable_list::~hitable_list(){
	release();
}

void hitable_list::release() {
	if (nullptr != list) {
		for (int i = 0; i < list_size; i++) {
			if (nullptr != list[i]) {
				delete list[i];
				list[i] = nullptr;
			}
		}
		delete [] list;
		list = nullptr;
	}
	list_size = 0;
}

hitable_list::hitable_list(hitable **l, int n) { 
	list = l; 
	list_size = n; 
}

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	float closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}


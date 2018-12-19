#pragma once
#include "hitable.h"

class hitable_list : public hitable{
public:
	hitable_list();
	hitable_list(hitable **l, int n);
	~hitable_list();
public:
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

private:
	void release();
public:
	hitable * * list;
	int list_size;
};


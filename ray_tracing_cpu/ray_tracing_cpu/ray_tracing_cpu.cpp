// ray_tracing_cpu.cpp: 定义控制台应用程序的入口点。
//


#include "raytracing.h"

int main(){
	raytracing rayt;
	rayt.make_sphere();
	rayt.run(5000);

    return 0;
}


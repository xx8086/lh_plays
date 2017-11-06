#pragma once
#include <xmmintrin.h>

class CLhSSE
{
public:
    CLhSSE();
    ~CLhSSE();
public:
    void runadd();
    void runmul();
private:
    __m128 addwidthassembly(__m128 a, __m128 b);
    __m128 addwidthintrinsics(__m128 a, __m128 b);

    __m128 mulvectormatrix(__m128& v,
        __m128& mrow1, __m128& mrow2, __m128& mrow3, __m128& mrow4);
private:
};


#include "lh_simd.h"
#include <stdio.h>

#define SHUFFLE_PARAM(x, y , z, w)  \
((x) | ((y) <<2) | ((z)<<4) | ((w)<<6))

#define _mm_replicate_x_ps(v) \
_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(0, 0, 0, 0))

#define _mm_replicate_y_ps(v) \
_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(1, 1, 1, 1))

#define _mm_replicate_z_ps(v) \
_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(2, 2, 2, 2))

#define _mm_replicate_w_ps(v) \
_mm_shuffle_ps((v), (v), SHUFFLE_PARAM(3, 3, 3, 3))

#define _mm_madd_ps(a, b, c) \
_mm_add_ps(_mm_mul_ps(a, b), c)

__m128 CLhSSE::addwidthassembly(__m128 a, __m128 b){
    __m128 r;
    __asm {
        movaps xmm0, xmmword ptr[a]
        movaps xmm1, xmmword ptr[b]
        addps xmm0, xmm1
        movaps xmmword ptr[r], xmm0
    }
    return r;
}

__m128 CLhSSE::addwidthintrinsics(__m128 a, __m128 b){
    return _mm_add_ps(a, b);
}

void CLhSSE::runadd() {
    __declspec(align(16)) float A[] = { 2.0f, -1.0f, 3.0f, 4.0f };
    __declspec(align(16)) float B[] = { -1.0f, 3.0f, 4.0f, 2.0f };
    __declspec(align(16)) float C[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    __declspec(align(16)) float D[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    __m128 a = _mm_load_ps(A);
    __m128 b = _mm_load_ps(B);
    _mm_store_ps(C, addwidthassembly(a, b));
    _mm_store_ps(D, addwidthintrinsics(a, b));

    float* F[] = { A, B, C, D };
    for (int i = 0;  i < 4; i++){
        for (int j = 0; j < 4;  j++) {
            printf("%g\t", F[i][j]);
        }
        printf("\n");
    }
}

__m128 CLhSSE::mulvectormatrix(__m128& v,
    __m128& mrow1, __m128& mrow2, __m128& mrow3, __m128& mrow4)
{
   __m128 result;
   result = _mm_mul_ps(_mm_replicate_x_ps(v), mrow1);
   result = _mm_madd_ps(_mm_replicate_y_ps(v), mrow2, result);
   result = _mm_madd_ps(_mm_replicate_z_ps(v), mrow3, result);
   result = _mm_madd_ps(_mm_replicate_w_ps(v), mrow4, result);
   return result;
}


void CLhSSE::runmul(){
    __declspec(align(16)) float A[] = { 2.0f, -1.0f, 3.0f, 4.0f };
    __declspec(align(16)) float B[] = { -1.0f, 3.0f, 4.0f, 2.0f };
    __declspec(align(16)) float C[] = { 3.0f, 4.0f, 2.0f, -1.0f };
    __declspec(align(16)) float D[] = { 4.0f, 2.0f, -1.0f, 3.0f };
    __declspec(align(16)) float V[] = { 1.0f, 1.0f, 3.0f, 4.0f };

    __m128 row1 = _mm_load_ps(A);
    __m128 row2 = _mm_load_ps(B);
    __m128 row3 = _mm_load_ps(C);
    __m128 row4 = _mm_load_ps(D);
    __m128 rowv = _mm_load_ps(V);
    __m128 r =  mulvectormatrix(rowv, row1, row2, row3, row4);

    __declspec(align(16)) float R[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    _mm_store_ps(R, r);
    for (int i = 0; i < 4; i++) {
        printf("%g\t", R[i]);
    }
    printf("\n");
}

CLhSSE::CLhSSE()
{
}

CLhSSE::~CLhSSE()
{
}
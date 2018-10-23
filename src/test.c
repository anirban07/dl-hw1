#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include "uwnet.h"
#include "matrix.h"
#include "image.h"
#include "test.h"
#include "args.h"

double what_time_is_it_now()
{
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void test_matrix_speed()
{
    int i;
    int n = 128;
    matrix a = random_matrix(512, 512, 1);
    matrix b = random_matrix(512, 512, 1);
    double start = what_time_is_it_now();
    for(i = 0; i < n; ++i){
        matrix d = matmul(a,b);
        free_matrix(d);
    }
    printf("Matmul elapsed %lf sec\n", what_time_is_it_now() - start);
    start = what_time_is_it_now();
    for(i = 0; i < n; ++i){
        matrix at = transpose_matrix(a);
        free_matrix(at);
    }
    printf("Transpose elapsed %lf sec\n", what_time_is_it_now() - start);
}

void test_im2col_3i_1c_2f_1s() {
    size_t i;
    float im_data[3 * 3 * 1];
    float col_data[2 * 2 * 3 * 3] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        2, 3, 0, 5, 6, 0, 8, 9, 0,
        4, 5, 6, 7, 8, 9, 0, 0, 0,
        5, 6, 0, 8, 9, 0, 0, 0, 0
    };
    for (i = 0; i < 9; i++) {
        im_data[i] = i + 1;
    }
    image im = make_image(3, 3, 1);
    for (i = 0; i < 9; i++) {
        im.data[i] = im_data[i];
    }
    matrix col = im2col(im, 2, 1);

    for (i = 0; i < 2 * 2 * 3 * 3; i++) {
        assert(abs(col.data[i] - col_data[i]) < 0.0001);
    }
    printf("test_im2col_3i_1c_2f_1s passed\n");
}

void test_im2col_3i_2c_2f_1s() {
    int im_h = 3;
    int im_w = 3;
    int num_ch = 2;
    int f_size = 2;
    int stride = 1;
    size_t i;
    float im_data[im_h * im_w * num_ch];
    float col_data[2 * 2 * 2 * 3 * 3] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        2, 3, 0, 5, 6, 0, 8, 9, 0,
        4, 5, 6, 7, 8, 9, 0, 0, 0,
        5, 6, 0, 8, 9, 0, 0, 0, 0,

        11, 12, 13, 14, 15, 16, 17, 18, 19,
        12, 13, 00, 15, 16, 00, 18, 19, 00,
        14, 15, 16, 17, 18, 19, 00, 00, 00,
        15, 16, 00, 18, 19, 00, 00, 00, 00
    };
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        if (i < im_h * im_w) {
            im_data[i] = i + 1;
        } else {
            im_data[i] = i + 2;
        }
    }
    image im = make_image(im_h, im_w, num_ch);
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im.data[i] = im_data[i];
    }
    matrix col = im2col(im, f_size, stride);

    for (i = 0; i < 2 * 2 * 2 * 3 * 3; i++) {
        assert(abs(col.data[i] - col_data[i]) < 0.0001);
    }
    printf("test_im2col_3i_2c_2f_1s passed\n");
}

void test_im2col_3i_1c_2f_2s() {
    int im_h = 3;
    int im_w = 3;
    int num_ch = 1;
    int f_size = 2;
    int stride = 2;

    size_t i;
    float im_data[im_h * im_w * num_ch];
    float col_data[2 * 2 * 2 * 2] = {
        1, 3, 7, 9,
        2, 0, 8, 0,
        4, 6, 0, 0,
        5, 0, 0, 0
    };
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im_data[i] = i + 1;
    }
    image im = make_image(im_h, im_w, num_ch);
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im.data[i] = im_data[i];
    }
    matrix col = im2col(im, f_size, stride);

    for (i = 0; i < 2 * 2 * 2 * 2; i++) {
        assert(abs(col.data[i] - col_data[i]) < 0.0001);
    }
    printf("test_im2col_3i_1c_2f_2s passed\n");
}

void test_im2col_2i_1c_3f_1s() {
    int im_h = 2;
    int im_w = 2;
    int num_ch = 1;
    int f_size = 3;
    int stride = 1;

    size_t i;
    float im_data[im_h * im_w * num_ch];
    float col_data[3 * 3 * 2 * 2] = {
        0, 0, 0, 1,
        0, 0, 1, 2,
        0, 0, 2, 0,
        0, 1, 0, 3,
        1, 2, 3, 4,
        2, 0, 4, 0,
        0, 3, 0, 0,
        3, 4, 0, 0,
        4, 0, 0, 0
    };
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im_data[i] = i + 1;
    }
    image im = make_image(im_h, im_w, num_ch);
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im.data[i] = im_data[i];
    }
    matrix col = im2col(im, f_size, stride);

    for (i = 0; i < 3 * 3 * 2 * 2; i++) {
        assert(abs(col.data[i] - col_data[i]) < 0.0001);
    }
    printf("test_im2col_2i_1c_3f_1s passed\n");
}

void test_large() {
    int n = 32;
    int im_h = 512;
    int im_w = 512;
    int num_ch = 3;
    int f_size = 5;
    int stride = 1;

    size_t i;
    float im_data[im_h * im_w * num_ch];
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im_data[i] = i;
    }
    image im = make_image(im_h, im_w, num_ch);
    for (i = 0; i < im_h * im_w * num_ch; i++) {
        im.data[i] = im_data[i];
    }
    double start = what_time_is_it_now();
    for(i = 0; i < n; ++i){
        matrix col = im2col(im, f_size, stride);
        free_matrix(col);
    }
    printf("im2col elapsed %lf sec\n", what_time_is_it_now() - start);

}

void test_im2col() {
    test_im2col_3i_1c_2f_1s();
    test_im2col_3i_2c_2f_1s();
    test_im2col_3i_1c_2f_2s();
    test_im2col_2i_1c_3f_1s();
    test_large();
}

void run_tests()
{
    // test_matrix_speed();
    test_im2col();
    //printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}


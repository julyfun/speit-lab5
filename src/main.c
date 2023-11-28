#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    LESS = -1,
    EQUAL = 0,
    GREATER = 1,
} Ordering;

// 比较接口
typedef struct {
    Ordering (*partial_cmp)(void* self, void* other);
} PartialOrd;

// 包装 int 类型
typedef struct {
    int value;
} Int;

// Int 类实现 partial_cmp 方法
Ordering int_partial_cmp(void* self, void* other) {
    Int* int_self = (Int*)self;
    Int* int_other = (Int*)other;

    if (int_self->value < int_other->value) {
        return LESS;
    } else if (int_self->value > int_other->value) {
        return GREATER;
    } else {
        return EQUAL;
    }
}

typedef struct {
    double real;
    double imag;
} Complex;

double complex_magnitude(Complex* self) {
    return self->real * self->real + self->imag * self->imag;
}

Ordering complex_partial_cmp(void* self, void* other) {
    Complex* complex_self = (Complex*)self;
    Complex* complex_other = (Complex*)other;

    double self_magnitude = complex_magnitude(complex_self);
    double other_magnitude = complex_magnitude(complex_other);

    if (self_magnitude < other_magnitude) {
        return LESS;
    } else if (self_magnitude > other_magnitude) {
        return GREATER;
    } else {
        return EQUAL;
    }
}

void insertion_sort(void* arr, int len, int elem_size, PartialOrd* partial_ord) {
    char* char_arr = (char*)arr;

    for (int i = 1; i < len; i++) {
        for (int j = i; j > 0; j--) {
            void* a = char_arr + (j - 1) * elem_size;
            void* b = char_arr + j * elem_size;

            // 调用统一的比较接口，无需关心具体类型
            if (partial_ord->partial_cmp(a, b) == GREATER) {
                // 交换 j - 1 和 j 号元素
                char temp[elem_size];
                memcpy(temp, a, elem_size);
                memcpy(a, b, elem_size);
                memcpy(b, temp, elem_size);
            } else {
                break;
            }
        }
    }
}

int main() {
    int lens[] = { 10, 100, 1000, 10000 };
    int num_tests[] = { 100, 100, 100, 3 };
    srand(time(0));
    fprintf(stderr, "Insertion sort performance for Integers:\nlen:  average time(s):\n");
    for (int i = 0; i < sizeof(lens) / sizeof(int); i++) {
        int len = lens[i];
        // 动态申请数组
        Int* arr = (Int*)malloc(sizeof(Int) * len);

        int total_time = 0;
        for (int j = 0; j < num_tests[i]; j++) {
            for (int k = 0; k < len; k++) {
                arr[k] = (Int) { rand() };
            }
            clock_t start = clock();
            // 调用排序函数时传入对应的比较函数
            insertion_sort(
                arr,
                len,
                sizeof(Int),
                &((PartialOrd) { .partial_cmp = int_partial_cmp })
            );
            clock_t end = clock();
            total_time += end - start;
        }

        free(arr);
        printf("%d, %lf\n", len, (double)total_time / CLOCKS_PER_SEC / num_tests[i]);
    }
    fprintf(stderr, "Insertion sort performance for the Complex:\nlen:  average time(s):\n");
    for (int i = 0; i < sizeof(lens) / sizeof(int); i++) {
        int len = lens[i];
        Complex* arr = (Complex*)malloc(sizeof(Complex) * len);

        int total_time = 0;
        for (int j = 0; j < num_tests[i]; j++) {
            for (int k = 0; k < len; k++) {
                arr[k] = (Complex) { rand(), rand() };
            }
            clock_t start = clock();
            insertion_sort(
                arr,
                len,
                sizeof(Complex),
                &((PartialOrd) { .partial_cmp = complex_partial_cmp })
            );
            clock_t end = clock();
            total_time += end - start;
        }

        free(arr);
        printf("%d, %lf\n", len, (double)total_time / CLOCKS_PER_SEC / num_tests[i]);
    }
}

void test() {
    Int int_arr[] = { { 5 }, { 2 }, { 8 }, { 1 }, { 9 } };
    int int_len = sizeof(int_arr) / sizeof(Int);
    PartialOrd int_partial_ord = { int_partial_cmp };

    Complex complex_arr[] = { { 3.0, 4.0 },
                              { 1.0, 2.0 },
                              { 5.0, 6.0 },
                              { 2.0, 3.0 },
                              { 4.0, 5.0 } };
    int complex_len = sizeof(complex_arr) / sizeof(Complex);
    PartialOrd complex_partial_ord = { complex_partial_cmp };

    insertion_sort(int_arr, int_len, sizeof(Int), &int_partial_ord);
    insertion_sort(complex_arr, complex_len, sizeof(Complex), &complex_partial_ord);

    printf("Sorted Int array: ");
    for (int i = 0; i < int_len; i++) {
        printf("%d ", int_arr[i].value);
    }
    printf("\n");

    printf("Sorted Complex array: ");
    for (int i = 0; i < complex_len; i++) {
        printf("(%lf, %lf) ", complex_arr[i].real, complex_arr[i].imag);
    }
    printf("\n");
}

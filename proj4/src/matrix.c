#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col) {
    // Task 1.1 TODO
	int num_cols = mat->cols;
	return mat->data[row * num_cols + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
    // Task 1.1 TODO
	int num_cols = mat->cols;
	mat->data[row * num_cols + col] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    // Task 1.2 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Allocate space for the matrix data, initializing all entries to be 0. Return -2 if allocating memory failed.
    // 4. Set the number of rows and columns in the matrix struct according to the arguments provided.
    // 5. Set the `parent` field to NULL, since this matrix was not created from a slice.
    // 6. Set the `ref_cnt` field to 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.

    // validate dimensions
	if (rows <= 0 || cols <= 0) return -1;
    // allocate memory for new matrix struct
	matrix *new_mat = (matrix *)malloc(sizeof(matrix));
	if (new_mat == NULL) return -2;
    // allocate memory for matrix data
	int num_data = rows * cols;
    double *mat_data = (double *)malloc((size_t)num_data * sizeof(double));
	if (mat_data == NULL) return -2;
    for (int i = 0; i < num_data; i++) mat_data[i] = 0;
    // set new matrix's fields
    new_mat->data = mat_data;
	new_mat->rows = rows;
	new_mat->cols = cols;
	new_mat->parent = NULL;
	new_mat->ref_cnt = 1;

	*mat = new_mat;
	return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
	if (mat == NULL) return;
	if (mat->parent == NULL) {
		mat->ref_cnt -= 1;
		if (mat->ref_cnt == 0) {
			free(mat->data);
			free(mat);
			return;
		}
        return;
	}
	deallocate_matrix(mat->parent);
	free(mat);
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0 || offset <= 0) return -1;
    matrix *new_mat = (matrix *)malloc(sizeof(matrix));
    if (new_mat == NULL) return -2;
    new_mat->data = from->data + offset;
    new_mat->rows = rows;
    new_mat->cols = cols;
    new_mat->parent = from;
    new_mat->ref_cnt = 1;
    from->ref_cnt++;
    *mat = new_mat;
    return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
    // Task 1.5 TODO
    int num_data = mat->rows * mat->cols;

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < num_data / 4 * 4; i += 4) {
            __m256d vec = _mm256_set1_pd(val);
            _mm256_storeu_pd(mat->data + i, vec);
        }
        #pragma omp for
        for (int i = num_data / 4 * 4; i < num_data; i++) {
            mat->data[i] = val;
        }
    }
    
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
    // Task 1.5 TODO
    int num_data = mat->rows * mat->cols;

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < num_data / 4 * 4; i += 4) {
            __m256d vec = _mm256_loadu_pd(mat->data + i);
            __m256d zeros = _mm256_setzero_pd();
            __m256d negVec = _mm256_sub_pd(zeros, vec);
            __m256d absVec = _mm256_max_pd(negVec, vec);
            _mm256_storeu_pd(result->data + i, absVec);
        }

        #pragma omp for
        for (int i = num_data / 4 * 4; i < num_data; i++) {
            double currData = mat->data[i];
            if (currData < 0) {
                currData = 0 - currData;
            }
            result->data[i] = currData;
        }

    }
    
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
    // Task 1.5 TODO
    int num_data = mat->rows * mat->cols;
    __m256d zero = _mm256_setzero_pd();
    
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < num_data / 4 * 4; i += 4) {
            __m256d vec = _mm256_loadu_pd(mat->data + i);
            __m256d neged = _mm256_sub_pd(zero, vec);
            _mm256_storeu_pd(result->data + i, neged);
        }

        #pragma omp for
        for (int i = num_data / 4 * 4; i < num_data; i++) {
            result->data[i] = 0 - mat->data[i];
        }        
    }
    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.5 TODO
    int num_data = mat1->rows * mat1->cols;

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < num_data / 4 * 4; i += 4) {
            __m256d vec1 = _mm256_loadu_pd(mat1->data + i);
            __m256d vec2 = _mm256_loadu_pd(mat2->data + i);
            __m256d sum = _mm256_add_pd(vec1, vec2);
            _mm256_storeu_pd(result->data + i, sum);
        }

        #pragma omp for
        for (int i = num_data / 4 * 4; i < num_data; i++) {
            result->data[i] = mat1->data[i] + mat2->data[i];
        }
    }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.5 TODO
    int num_data = mat1->rows * mat1->cols;

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < num_data / 4 * 4; i += 4) {
            __m256d vec1 = _mm256_loadu_pd(mat1->data + i);
            __m256d vec2 = _mm256_loadu_pd(mat2->data + i);
            __m256d sum = _mm256_sub_pd(vec1, vec2);
            _mm256_storeu_pd(result->data + i, sum);
        }

        #pragma omp for
        for (int i = num_data / 4 * 4; i < num_data; i++) {
            result->data[i] = mat1->data[i] - mat2->data[i];
        }
    }

    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */

double add_m256d(__m256d a) {
    double sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += a[i];
    }
    return sum;
}

/*convert a row-major matrix and store the column-major version into result*/
void transpose(matrix *result, matrix *mat) {
    #pragma omp parallel for
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            double currData = mat->data[i * mat->cols + j];
            result->data[j * mat->rows + i] = currData;
        }
    }
}

int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    int vec_len = mat1->cols;
    int prod_num_row = mat1->rows, prod_num_col = mat2->cols;

    matrix *trans_mat2 = NULL;
    allocate_matrix(&trans_mat2, mat2->cols, mat2->rows);
    transpose(trans_mat2, mat2);

    for (int i = 0; i < prod_num_row; i++) {
        for (int j = 0; j < prod_num_col;j++) {
            double scalar_sum = 0;

            #pragma omp parallel
            {
                __m256d vec_prod = _mm256_setzero_pd();
                double sub_sum = 0;

                #pragma omp for
                for (int k = 0; k < vec_len / 4 * 4; k += 4) {
                    __m256d vec1 = _mm256_loadu_pd(mat1->data + k);
                    __m256d vec2 = _mm256_loadu_pd(trans_mat2->data + k);
                    vec_prod = _mm256_add_pd(_mm256_mul_pd(vec1, vec2), vec_prod);
                }

                #pragma omp for
                for (int k = vec_len / 4 * 4; k < vec_len; k++) {
                    sub_sum += mat1->data[i * mat1->cols + k] * trans_mat2->data[j * mat1->cols + k];
                }

                #pragma omp critical
                {
                    scalar_sum += add_m256d(vec_prod);
                    scalar_sum += sub_sum;
                }

                result->data[i * prod_num_col + j] = scalar_sum;
            }
        }
    }
    deallocate_matrix(trans_mat2);
    return 0;
}


/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    // Task 1.6 TODO
    if (pow == 0) {
        fill_matrix(result, 0);
        for (int i = 0; i < (size_t)(mat->rows * mat->cols); i += mat->cols + 1) {
            result->data[i] = 1;
        }
    } else if (pow % 2 == 0) {
        // set temporary null matrix pointer
        matrix *tmp = NULL;
        allocate_matrix(&tmp, mat->rows, mat->cols);
        //perform recursion
        mul_matrix(tmp, mat, mat);
        pow_matrix(result, tmp, pow / 2);
        // free it
        deallocate_matrix(tmp);
    } else if (pow % 2 == 1) {
        matrix *tmp1 = NULL;
        allocate_matrix(&tmp1, mat->rows, mat->cols);
        matrix *tmp2 = NULL;
        allocate_matrix(&tmp2, mat->rows, mat->cols);

        mul_matrix(tmp1, mat, mat);
        pow_matrix(tmp2, tmp1, (pow - 1) / 2);
        mul_matrix(result, tmp2, mat);

        deallocate_matrix(tmp1);
        deallocate_matrix(tmp2);
    }
    return 0;
}

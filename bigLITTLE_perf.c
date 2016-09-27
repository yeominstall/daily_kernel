/* big.LITTLE architecture perf tool */
/* Linux 3.10 */
/* square matrix multiplication */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define DIM	500		// dimension of matrix
#define N_CORE	8		// number of cpu core
#define T_MAX	128		// number of threads
#define E_MIN	-10		// min. of random element
#define E_MAX	10		// max. of random element

/* define matrix */
typedef struct {
	int m[DIM][DIM];
	int t_id;
} matrix;

void matrix_mult (int (*A)[DIM], int (*B)[DIM], int (*res)[DIM]);	// matrix multiply function

/* main */
int main (void) {

	int i, j;

	// matrix set
	int A[DIM][DIM];
	int B[DIM][DIM];
	int res[DIM][DIM] = {0, };

	srand (time(NULL));
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			A[i][j] = (rand() % ((E_MAX * 2) + 1)) + E_MIN;
			B[i][j] = (rand() % ((E_MAX * 2) + 1)) + E_MIN;
		}
	}
	pthread_t *t_ids = (int*)malloc((T_MAX) * sizeof (pthread_t));
	
	matrix_mult (A, B, res);

	// generate threads
	//pthread_create (*t_ids, *attr, matrix_mult, NULL);
	// core assign
	// if core_number in 0~3

	// if core_number in 4~7
}

/* matrix multiply */
void matrix_mult (int (*A)[DIM], int (*B)[DIM], int (*res)[DIM]){

	int i, j, k;

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (k = 0; k < DIM; k++) {
				res[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

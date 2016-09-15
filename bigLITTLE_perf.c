/* big.LITTLE architecture perf tool */
/* Linux 3.10 */
/* Matrix multiplication */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define DIM 1024	// dimension of matrix
#define N 8		// number of cpu core
#define T 100		// number of threads

/* define matrix struct */
typedef struct {
	int mat[DIM][DIM];
} matrix;


/* matrix multiply function */


/* main */
int main (void){
	// generate threads

	// core assign
	// if core_number in 0~3

	// if core_number in 4~7
}

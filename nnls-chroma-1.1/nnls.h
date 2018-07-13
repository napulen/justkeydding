#ifndef NNLS_H
#define NNLS_H

#ifdef __cplusplus
extern "C" {
#endif

int nnls(float *a, int mda, int m, int n, 
	 float *b, float *x, float *rnorm, 
	 float *w, float *zz, int *index, int *mode);

#ifdef __cplusplus
}
#endif

#endif


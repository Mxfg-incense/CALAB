#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define MATRIX_SIZE 1024

void matmul_naive(double *a, double *b, double **cc)
{
  double *c = (double *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    for (int j = 0; j < MATRIX_SIZE; j++)
    {
      for (int k = 0; k < MATRIX_SIZE; k++)
      {
        c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
      }
    }
  }
  *cc = c;
  free(c);
}

void matmul_optimized_slices(double *a, double *b, double **cc)
{
   double *c = (double *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));
  int id_thread, num_thread;
#pragma omp parallel private(id_thread)
  {
    id_thread = omp_get_thread_num();
    num_thread = omp_get_num_threads();

    for (int i = id_thread; i < MATRIX_SIZE; i += num_thread)
    {
      for (int j = 0; j < MATRIX_SIZE; j++)
      {
        c[i * MATRIX_SIZE + j] = 0.0;
        for (int k = 0; k < MATRIX_SIZE; k++)
        {
          c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
        }
      }
    }
  }
  *cc = c;
  free(c);
}

void matmul_optimized_chunks(double *a, double *b, double **cc)
{
   double *c = (double *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));
  int id_thread, num_thread;
#pragma omp parallel private(id_thread)
  {
    id_thread = omp_get_thread_num();
    num_thread = omp_get_num_threads();

    int chunk_size = MATRIX_SIZE / num_thread;
    int begin = id_thread * chunk_size;
    int end = (id_thread + 1) * chunk_size;
    if (id_thread == num_thread - 1)
    {
      end = MATRIX_SIZE;
    }

    for (int i = begin; i < end; i++)
    {
        for (int k = 0; k < MATRIX_SIZE; k++)
      for (int j = 0; j < MATRIX_SIZE; j++)
      {
        {
          c[(i - begin) * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
        }
      }
    }
    // for (int i = begin; i < end; i++)
    // {
    //   for (int j = 0; j < MATRIX_SIZE; j++)
    //   {
    //     c[i * MATRIX_SIZE + j] = buffer[(i - begin) * MATRIX_SIZE + j];
    //   }
    // }

  }
    *cc = c;
    free(c);
}


void check_correctness(double * a,double *b ) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
        if (fabs(a[i * MATRIX_SIZE + j] - b[i * MATRIX_SIZE + j]) > 0.01)
        {
      printf("Incorrect answer");
      return;
            }
        }
    }
}

int main()
{
  double *a = (double *)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
  double *b = (double *)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
  double *c = (double *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(double));

  if (!a || !b || !c)
  {
    printf("Error: could not allocate memory.\n");
    return 1;
  }

  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    for (int j = 0; j < MATRIX_SIZE; j++)
    {
      a[i * MATRIX_SIZE + j] = (double)rand() / RAND_MAX;
      b[i * MATRIX_SIZE + j] = (double)rand() / RAND_MAX;
    }
  }

  double begin_time = omp_get_wtime();
  matmul_naive(a, b, &c);
  double* ans = c;
  double end_time = omp_get_wtime();
  printf("Naive: %.5f seconds\n", end_time - begin_time);

  begin_time = omp_get_wtime();
  matmul_optimized_slices(a, b, &c);
  end_time = omp_get_wtime();
  printf("Optimized (slices): %.5f seconds\n", end_time - begin_time);
  check_correctness(ans, c);

  begin_time = omp_get_wtime();
  matmul_optimized_chunks(a, b, &c);
  end_time = omp_get_wtime();
  printf("Optimized (chunks): %.5f seconds\n", end_time - begin_time);
  check_correctness(ans, c);


  free(a);
  free(b);
  free(c);
  return 0;
}

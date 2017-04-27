#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#ifdef __GNUC__
#define restrict __restrict__
typedef size_t size_type;
#else
#define restrict __restrict
typedef long size_type;
#endif

static void rescale( double * restrict out_, const int * restrict in_, size_type n, const double intercept, const double slope )
{
#ifdef _MSC_VER
  #pragma loop(hint_parallel(0))
#else
  double * out = __builtin_assume_aligned (out_, 64);
  const int * in = __builtin_assume_aligned (in_, 32);
#endif
	for( size_type i = 0; i < n; ++i )
        out[i] = slope * in[i] + intercept;
}

static void rescale_short( int * restrict out, const short * restrict in, size_type n, const int intercept, const int slope )
{
#ifdef _MSC_VER
  #pragma loop(hint_parallel(0))
#endif
	for( size_type i = 0; i < n; ++i )
        out[i] = slope * in[i] + intercept;
}

static void rescale_int( int * restrict out, const int * restrict in, size_type n, const int intercept, const int slope )
{
#ifdef _MSC_VER
  #pragma loop(hint_parallel(0))
#endif
	for( size_type i = 0; i < n; ++i )
        out[i] = slope * in[i] + intercept;
}
struct rescale { double intercept; double slope; };

int main()
{
  const unsigned int dimz = 700;
  size_t size = 1024 * 1024 * 1;
#if 0
  double_a64 * outvol = (double_a64*)_aligned_malloc( size * sizeof(double),64);
  int_a32 * outvol2 = (int_a32*)_aligned_malloc( size * sizeof(int),32);
  int_a32 * vol = (int_a32*)_aligned_malloc( size * sizeof(int), 32);
#else
#if 0
	double * outvol = aligned_alloc(64, size * sizeof(double));
	int * outvol2 = aligned_alloc(32, size * sizeof(int));
	unsigned short * vol = aligned_alloc(16, size * sizeof(unsigned short));
#else
	double * outvol = memalign(64, size * sizeof(double));
	int * outvol2 = memalign(32, size * sizeof(int));
	int * vol = memalign(32, size * sizeof(int));
	short * svol = memalign(16, size * sizeof(short));
#endif
#endif
  unsigned short val = 0;
  for( size_t i = 0; i < size; ++i )
  {
    vol[i] = val++;
  }
  printf( "%d %d\n", sizeof(double), sizeof( unsigned short ) );

  double intercept = 0.0;
  double slope = 1.0;
  const struct rescale all[] = {
    {0.,1.},
    {0.,1.46398},
    {0.,1.52479},
    {0.,2.01514},
    {0.,2.2674},
    {0.,4.},
    {0,5.66056},
    {-1000.,1.},
    {-1024.,1.},
    {-1024.,1.00024},
  };
  const int n = sizeof all / sizeof *all;

  for( int i = 0; i < n; ++i )
  {
    const struct rescale * r = all + i;
    printf( "%g %g\n", r->intercept, r->slope );
    for( unsigned int z = 0; z < dimz; ++z )
    {
      rescale( outvol, vol, size, r->intercept, r->slope );
			rescale_short( outvol2, svol, size, r->intercept, r->slope );
			rescale_int( outvol2, vol, size, r->intercept, r->slope );
    }
  }

  return 0;
}

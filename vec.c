#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __GNUC__
#define restrict __restrict__
#else
#define restrict __restrict
#endif

static void rescale( double * restrict out, const int * restrict in, long n, const double intercept, const double slope )
{
  #pragma loop(hint_parallel(0))
	for( long i = 0; i < n; ++i )
        out[i] = slope * in[i] + intercept;
}

static void rescale_int( int_a32 * restrict out, const int_a32 * restrict in, unsigned long n, const int intercept, const int slope )
{
  #pragma loop(hint_parallel(0))
	for( unsigned long i = 0; i < n; ++i )
        out[i] = slope * in[i] + intercept;
}
struct rescale { double intercept; double slope; };

int main()
{
  const unsigned int dimz = 700;
  size_t size = 1024 * 1024 * 1;
  double_a64 * outvol = (double_a64*)_aligned_malloc( size * sizeof(double),64);
  int_a32 * outvol2 = (int_a32*)_aligned_malloc( size * sizeof(int),32);
  int_a32 * vol = (int_a32*)_aligned_malloc( size * sizeof(int), 32);
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
			rescale_int( outvol2, vol, size, r->intercept, r->slope );
    }
  }

  return 0;
}

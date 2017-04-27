#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#define restrict __restrict__

//static void rescale( double * restrict out_, unsigned short * restrict in_, size_t n, const double intercept, const double slope )
static void rescale( double * restrict out, unsigned short * restrict in, size_t n, const double intercept, const double slope )
{
#if 0
double *out = __builtin_assume_aligned (out_, 64);
const unsigned short * in = __builtin_assume_aligned (in_, 16);
#endif
	for( size_t i = 0; i < n; ++i )
	{
        out[i] = slope * in[i] + intercept;
	}
}

static void rescale_int( int * restrict out, unsigned short * restrict in, size_t n, const int intercept, const int slope )
{
#if 0
double *out = __builtin_assume_aligned (out_, 64);
const unsigned short * in = __builtin_assume_aligned (in_, 16);
#endif
	for( size_t i = 0; i < n; ++i )
	{
        out[i] = slope * in[i] + intercept;
	}
}

struct rescale { double intercept; double slope; };

int main()
{
        const unsigned int dimz = 700;
	size_t size = 1024 * 1024 * 1;
#define aligned_alloc memalign
	double * outvol = aligned_alloc(64, size * sizeof(double));
	int * outvol2 = aligned_alloc(32, size * sizeof(int));
	unsigned short * vol = aligned_alloc(16, size * sizeof(unsigned short));



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

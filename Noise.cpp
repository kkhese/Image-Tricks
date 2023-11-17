#include "Noise.h"
#include <math.h>

// The size of grid: default 1
#define GRID_SIZE 1
// Seed setup for random value. Changin this results different result
const unsigned int SEED_X1 = 2273472206;
const unsigned int SEED_X2 = 2278162623;
const unsigned int SEED_Y1 = 1440014778;
const unsigned int SEED_Y2 = 1524485263;
const unsigned int SEED_Z1 = 2813546167;
const unsigned int SEED_Z2 = 2305132234;
const unsigned int SEED_Q0 = 3498573726;
const unsigned int SEED_Q1 = 3476519523;
const unsigned int SEED_Q2 = 3905844518;

// function for calculating XY, YZ, XZ perlin noise
float perlinNoise(float x, float y, float z, int choose)
{
	int x0 = (int)(floor(x / GRID_SIZE));
	int y0 = (int)(floor(y / GRID_SIZE));
	int z0 = (int)(floor(z / GRID_SIZE));

	// Limiting randum fraction from -1 to 1
	float x_fac = x / GRID_SIZE - x0;
	float y_fac = y / GRID_SIZE - y0;
	float z_fac = z / GRID_SIZE - z0;

	//float x_fade = ((-2) * (x_fac) * (x_fac) * (x_fac)) + (3 * (x_fac) * (x_fac));
	//float y_fade = ((-2) * (y_fac) * (y_fac) * (y_fac)) + (3 * (y_fac) * (y_fac));
	//float z_fade = ((-2) * (z_fac) * (z_fac) * (z_fac)) + (3 * (z_fac) * (z_fac));
	
	// Using sinusoidal signal for making smoothe variations of direction vector
	float x_fade = (1 - cos(x_fac * 3.14159265f)) * 0.5f;
	float y_fade = (1 - cos(y_fac * 3.14159265f)) * 0.5f;
	float z_fade = (1 - cos(z_fac * 3.14159265f)) * 0.5f;
	// Ceiling of x,y,z
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;
	
	// Initiallize variables for 4 lattice points for 2D, fixing 1D
	float lattice00[2] = { 0.0, };
	float lattice01[2] = { 0.0, };
	float lattice10[2] = { 0.0, };
	float lattice11[2] = { 0.0, };
	// Initiallize direction vector at each lattice
	float direction00[2] = { 0.0, 0.0 };
	float direction01[2] = { 0.0, 0.0 };
	float direction10[2] = { 0.0, 0.0 };
	float direction11[2] = { 0.0, 0.0 };

	// For z direction
	if (choose == 0)
	{
		// applying random vector to each lattice
		lattice00[0] = latticecos(x0, y0, 0);
		lattice00[1] = latticesin(x0, y0, 0);
		lattice01[0] = latticecos(x0, y1, 0);
		lattice01[1] = latticesin(x0, y1, 0);
		lattice10[0] = latticecos(x1, y0, 0);
		lattice10[1] = latticesin(x1, y0, 0);
		lattice11[0] = latticecos(x1, y1, 0);
		lattice11[1] = latticesin(x1, y1, 0);
		// applying random dirction to each lattice
		direction00[0] = (-x_fac);
		direction00[1] = (-y_fac);
		direction01[0] = (-x_fac);
		direction01[1] = (1.0 - y_fac);
		direction10[0] = (1.0 - x_fac);
		direction10[1] = (-y_fac);
		direction11[0] = (1.0 - x_fac);
		direction11[1] = (1.0 - y_fac);
	}
	// For x direction
	if (choose == 1)
	{
		// applying random vector to each lattice
		lattice00[0] = latticecos(0, y0, z0);
		lattice00[1] = latticesin(0, y0, z0);
		lattice01[0] = latticecos(0, y0, z1);
		lattice01[1] = latticesin(0, y0, z1);
		lattice10[0] = latticecos(0, y1, z0);
		lattice10[1] = latticesin(0, y1, z0);
		lattice11[0] = latticecos(0, y1, z1);
		lattice11[1] = latticesin(0, y1, z1);
		// applying random dirction to each lattice
		direction00[0] = (-y_fac);
		direction00[1] = (-z_fac);
		direction01[0] = (-y_fac);
		direction01[1] = (1.0 - z_fac);
		direction10[0] = (1.0 - y_fac);
		direction10[1] = (-z_fac);
		direction11[0] = (1.0 - y_fac);
		direction11[1] = (1.0 - z_fac);
	}
	// For y direction
	else if (choose == 2)
	{
		// applying random vector to each lattice
		lattice00[0] = latticecos(x0, 0, z0);
		lattice00[1] = latticesin(x0, 0, z0);
		lattice01[0] = latticecos(x1, 0, z0);
		lattice01[1] = latticesin(x1, 0, z0);
		lattice10[0] = latticecos(x0, 0, z1);
		lattice10[1] = latticesin(x0, 0, z1);
		lattice11[0] = latticecos(x1, 0, z1);
		lattice11[1] = latticesin(x1, 0, z1);
		// applying random dirction to each lattice
		direction00[0] = (-z_fac);
		direction00[1] = (-x_fac);
		direction01[0] = (-z_fac);
		direction01[1] = (1.0 - x_fac);
		direction10[0] = (1.0 - z_fac);
		direction10[1] = (-x_fac);
		direction11[0] = (1.0 - z_fac);
		direction11[1] = (1.0 - x_fac);
	}
	//dot product at 4 lattice
	float value00 = (float)(lattice00[0]*direction00[0] + lattice00[1] * direction00[1]);
	float value01 = (float)(lattice01[0]*direction01[0] + lattice01[1] * direction01[1]);
	float value10 = (float)(lattice10[0]*direction10[0] + lattice10[1] * direction10[1]);
	float value11 = (float)(lattice11[0]*direction11[0] + lattice11[1] * direction11[1]);

	float value = 0.0;
	// Perlin noise for z direction
	if (choose == 0)
	{
		float value0 = interpolate(value00, value01, y_fade);
		float value1 = interpolate(value10, value11, y_fade);
		value = interpolate(value0, value1, x_fade);
	}
	// Perlin noise for x direction
	else if (choose == 1)
	{
		float value0 = interpolate(value00, value01, z_fade);
		float value1 = interpolate(value10, value11, z_fade);
		value = interpolate(value0, value1, y_fade);
	}
	// Perlin noise for y direction
	else if (choose == 2)
	{
		float value0 = interpolate(value00, value01, x_fade);
		float value1 = interpolate(value10, value11, x_fade);
		value = interpolate(value0, value1, z_fade);
	}
	// returning noise value by choose
	return value;
}
// pseudorandom value generator for 3D
unsigned int pseudorandom(int x, int y, int z)
{
	// polynomial random value for z  
	if (z == 0)
	{
		unsigned int n = (SEED_X1 * x) +
			(SEED_Y1 * y);
		unsigned int quad_term = SEED_Q2 * n * n +
			SEED_Q1 * n +
			SEED_Q0;
		return quad_term +
			(SEED_X2 * x) +
			(SEED_Y2 * y);
	}
	// polynomial random value for x
	else if (x == 0)
	{
		unsigned int n = (SEED_Y1 * y) +
			(SEED_Z1 * z);
		unsigned int quad_term = SEED_Q2 * n * n +
			SEED_Q1 * n +
			SEED_Q0;
		return quad_term +
			(SEED_Y2 * y) +
			(SEED_Z2 * z);
	}
	// polynomial random value for y
	else if (y == 0)
	{
		unsigned int n = (SEED_Z1 * z) +
			(SEED_X1 * x);
		unsigned int quad_term = SEED_Q2 * n * n +
			SEED_Q1 * n +
			SEED_Q0;
		return quad_term +
			(SEED_Z2 * z) +
			(SEED_X2 * x);
	}
}
// Lattice to random values in 3D world to one direction
float latticecos(int x, int y, int z)
{	
	if (z == 0)
	{
		unsigned int value = pseudorandom(x, y, 0);
		float radians = (float)(value);  // very random
		return (cos(radians));
	}
	else if (x == 0)
	{
		unsigned int value = pseudorandom(0, y, z);
		float radians = (float)(value);  // very random
		return (cos(radians));
	}
	else if (y == 0)
	{
		unsigned int value = pseudorandom(x, 0, z);
		float radians = (float)(value);  // very random
		return (cos(radians));
	}
}
// Lattice to random values in 3D world to another direction
float latticesin(int x, int y, int z)
{
	if (z == 0)
	{
		unsigned int value = pseudorandom(x, y, 0);
		float radians = (float)(value);  // very random
		return (sin(radians));
	}
	else if (x == 0)
	{
		unsigned int value = pseudorandom(0, y, z);
		float radians = (float)(value);  // very random
		return (sin(radians));
	}
	else if (y == 0)
	{
		unsigned int value = pseudorandom(x, 0, z);
		float radians = (float)(value);  // very random
		return (sin(radians));
	}
}

// interpolate between two points
float interpolate(float v0, float v1, float fraction)
{
	return (v0 * (1.0f - fraction)) + (v1 * fraction);
}
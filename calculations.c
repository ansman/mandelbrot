#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel mandelcalc(float xs, float ys, float stepx, float stepy, unsigned int width, unsigned int height, const unsigned int iterations, __global float * output) {
	int xt = get_global_id(0);
	int yt = get_global_id(1);
	
	float x0 = xs + (float)xt * stepx;
	float y0 = ys + (float)yt * stepy;
	
	float x, y;
	x = y = 0;
	
	int iteration = 0;
	
	while(iteration < iterations) {
		float xx = x*x;
		float yy = y*y;
		
		if(xx+yy > 4)
			break;
		
		y = 2*x*y + y0;
		x = xx-yy+x0;
		++iteration;
	}
	
	output[xt + yt*width] = (float)iteration/(float)iterations;
}
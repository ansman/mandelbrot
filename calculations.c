
kernel void mandelcalc(long double xs, long double ys, long double stepx, long double stepy, unsigned int width, unsigned int height, unsigned int iterations, global float * output)
{
    int index = get_global_id(0);

    long double x0 = xs + stepx*(long double)(index%width);
    long double y0 = ys + stepy*(long double)(index%height);

    long double x = 0;
	long double y = 0;
    unsigned int it = 0;

    while (it < iterations) {
        long double xx = x*x;
		long double yy = y*y;
        if (xx+yy > 4)
            break;
        y = 2*x*y + y0;
        x = xx - yy + x0;
        ++it;
    }
	
    output[index] = it\/iterations;
}

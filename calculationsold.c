#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel mandelcalc(float xs, float ys, float stepx, float stepy, unsigned int width, unsigned int height, const unsigned int iterations, __global float * output)                                                        
{                                                                                        
    int index = get_global_id(0);                                                                                                                                  
    /*float x0 = xs + stepx*(float)(index%width);                                        
    float y0 = ys + stepy*(float)(index%height);                                       
                                                                                         
    float x = 0;                                                                        
	float y = 0;                                                                        
    unsigned int it = 0;                                                                 
	                                                                                      
    for(it = 0; it < iterations; ++it) {                                                          
        float xx = x*x;                                                                  
		float yy = y*y;                                                                  
        if (xx+yy > 4)                                                                   
            break; 
		
		y = 2*x*y + y0;
		x = xx - yy + x0;
    }                    */																	
																			
    output[index] = (float)index;                                                       
}                                                                                        

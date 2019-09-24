#include "dsk6713_aic23.h"
#include <dsk6713_led.h>
#include <math.h>

#define DSK6713_AIC23_INPUT_LINE 0x0011
#define N 512 
#define PI2 6.28318530718
#define THRESHOLD_VALUE	1.0e3

Uint16 inputsource = DSK6713_AIC23_INPUT_LINE;
Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;

volatile short buffer_count;
volatile short nFlag;
short freq_sample = 8000;
short n,k,index_i, index_j, i;
volatile short input_buffer[N] = {0};
short DTMF697,DTMF770,DTMF852,DTMF941,DTMF1209,DTMF1336,DTMF1477,DTMF1633;

volatile float x[N] = {0};
float Xre[N] = {0}, Xim[N] = {0};
float P[N] = {0};

interrupt void c_int11()
{
	input_buffer[buffer_count] = input_sample();
	output_sample((short)x[buffer_count++]);

	if(buffer_count >= N)
	{
		buffer_count = 0;
		nFlag = 0;
		for(i=0; i<N; i++)
		{
			x[i] = (float)input_buffer[i];
		}
	}
	return;
}

int round(float number)
{
    return (number >= 0) ? (int)(number + 0.5f) : (int)(number - 0.5f);
}

void main(void)
{
	DTMF697 = round(N*697/freq_sample);
	DTMF770 = round(N*770/freq_sample);
	DTMF852 = round(N*852/freq_sample);
	DTMF941 = round(N*941/freq_sample);
	DTMF1209 = round(N*1209/freq_sample);
	DTMF1336 = round(N*1336/freq_sample);
	DTMF1477 = round(N*1477/freq_sample);
	DTMF1633 = round(N*1633/freq_sample);

	nFlag = 1;
	buffer_count = 0;

	DSK6713_LED_init();
	DSK6713_LED_off(0);
	DSK6713_LED_off(1);
  	DSK6713_LED_off(2);
  	DSK6713_LED_off(3);

	comm_intr();

	while(1)
	{
  		while(nFlag);                                                                           
                                                                                             
	    nFlag = 1;                                                                              
		                                                                                        
 		for (k=0 ; k<N ; ++k)                                                                   
 		{                                                                                       
 	        	Xre[k] = 0;   	       	                                                                   
 		        for (n=0 ; n<N ; ++n)
 		        {
 		        	Xre[k] += x[n] * cos(n * k * PI2 / N);
 		        }		     
 		                            	                                                                                           
 	        	Xim[k] = 0;                                                                     
 		        for (n=0 ; n<N ; ++n)
 		        {
 		        Xim[k] -= x[n] * sin(n * k * PI2 / N);     
				}
 		                     	                                                                                            
 	        	P[k] = sqrt(Xre[k]*Xre[k] + Xim[k]*Xim[k]);                                           				
    	}                                                                                   

		if(P[DTMF697] < THRESHOLD_VALUE && P[DTMF770] < THRESHOLD_VALUE && P[DTMF852] < THRESHOLD_VALUE && P[DTMF941] < THRESHOLD_VALUE && P[DTMF1209] < THRESHOLD_VALUE && P[DTMF1336] < THRESHOLD_VALUE && P[DTMF1477] < THRESHOLD_VALUE && P[DTMF1633] < THRESHOLD_VALUE){
			index_i == 4;
			index_j == 2;
		}
		else{
 			if(P[DTMF697] > P[DTMF770] && P[DTMF697] > P[DTMF852] && P[DTMF697] > P[DTMF941])       
 				index_i = 1;                                                                        
                                                                                              
	 		if(P[DTMF770] > P[DTMF697] && P[DTMF770] > P[DTMF852] && P[DTMF770] > P[DTMF941])       
	 			index_i = 2;                                                                        
	                                                                                              
	 		if(P[DTMF852] > P[DTMF697] && P[DTMF852] > P[DTMF770] && P[DTMF697] > P[DTMF941])       
	 			index_i = 3;                                                                        
	                                                                                              
	 		if(P[DTMF941] > P[DTMF697] && P[DTMF941] > P[DTMF770] && P[DTMF941] > P[DTMF852])       
	 			index_i = 4;                                                                        
	                                                                                              
	 		if(P[DTMF1209] > P[DTMF1336] && P[DTMF1209] > P[DTMF1477] && P[DTMF1209] > P[DTMF1633]) 
	 			index_j = 1;                                                                        
	                                                                                              
	 		if(P[DTMF1336] > P[DTMF1209] && P[DTMF1336] > P[DTMF1477] && P[DTMF1336] > P[DTMF1633]) 
	 			index_j = 2;                                                                        
	                                                                                              
	 		if(P[DTMF1477] > P[DTMF1209] && P[DTMF1477] > P[DTMF1336] && P[DTMF1477] > P[DTMF1633]) 
	 			index_j = 3;                                                                        
	                                                                                              
	 		if(P[DTMF1633] > P[DTMF1209] && P[DTMF1633] > P[DTMF1336] && P[DTMF1633] > P[DTMF1477]) 
	 			index_j = 4;                                                                        
	    }
                                                                                              
 	if(index_i == 1 && index_j == 1) // 1                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 1 && index_j == 2) // 2                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 1 && index_j == 3) // 3                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 2 && index_j == 1) // 4                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 2 && index_j == 2) // 5                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 2 && index_j == 3) // 6                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 3 && index_j == 1) // 7                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 3 && index_j == 2) // 8                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 3 && index_j == 3) // 9                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 4 && index_j == 2) // 0                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_off(3);                                                                 
 	}                                                                                           
 	if(index_i == 1 && index_j == 4) // A                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 2 && index_j == 4) // B                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_off(2);                                                                 
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 3 && index_j == 4) // C                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 4 && index_j == 4) // D                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_off(1);                                                                     
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 4 && index_j == 1) // *                                                       
 	{	                                                                                        
 		DSK6713_LED_off(0);                                                                     
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
 	if(index_i == 4 && index_j == 3) // #                                                       
 	{	                                                                                        
 		DSK6713_LED_on(0);                                                                      
 		DSK6713_LED_on(1);                                                                      
   		DSK6713_LED_on(2);                                                                  
   		DSK6713_LED_on(3);                                                                  
 	}                                                                                           
                                                                                              
 	}	                                                                                        
}

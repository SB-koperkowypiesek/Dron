/*
 * filters.c
 *
 *  Created on: 24.09.2021
 *      Author: symon
 */


#include "filters.h"


void FIR_Filter_Init(FIR_Filter *fir) {

//	Allocate memory for arrays
	fir->buffer = (float *)malloc(sizeof(fir->buffer)*fir->length);
	fir->impulse_responce = (float *)malloc(sizeof(fir->impulse_responce)*fir->length);

//	Clear filter buffer
	for (uint8_t i = 0; i < fir->length; i++) {
		fir->buffer[i] = 0;
		fir->impulse_responce[i] = 0;
	}

//	Clear buffer index
fir->buffer_index=0.0f;

//	Clear filter output
fir->output=0.0f;

}

float FIR_Filter_filtering(FIR_Filter *fir, float input) {
//	Add new data to buffer
fir->buffer[fir->buffer_index]=input;

//	Increment buffer_index and loop if necessary
fir->buffer_index++;
if(fir->buffer_index>fir->length){
	fir->buffer_index=0;
}
//	Reset old output value
fir->output=0;

uint8_t sum_index=fir->buffer_index;

//	Decrement sum_index and loop if necessary
for(uint8_t i=0; i<fir->length;i++){
	if(sum_index >0){
		sum_index--;
	}
	else{
		sum_index =fir->length-1;
	}
//	Compute new output (via convolution)
	 fir->output+=fir->impulse_responce[i]*fir->buffer[sum_index];
}
return fir->output;
}


void FIIR_Filter_Init(IIR_Filter *iir) {

//	Allocate memory for arrays
	iir->buffer_input = (float *)malloc(sizeof(iir->buffer_input)*(iir->filter_order));
	iir->buffer_output = (float *)malloc(sizeof(iir->buffer_output)*(iir->filter_order-1));
	iir->forward_coefficients = (float *)malloc(sizeof(iir->forward_coefficients)*(iir->filter_order));
	iir->feedback_coefficients = (float *)malloc(sizeof(iir->feedback_coefficients)*(iir->filter_order-1));


//	Clear filter buffers and coefficients
	//FORWARD PART:
	for (uint8_t i = 0; i < iir->filter_order; i++) {
		iir->buffer_input[i] = 0;
		iir->forward_coefficients[i]=0;
	}
	//FEEDBACK PART:
	for (uint8_t i = 0; i < iir->filter_order-1; i++) {
		iir->buffer_output[i] = 0;
		iir->feedback_coefficients[i]=0;
	}

//	Clear buffer index
iir->buffer_index=0.0f;

//	Clear filter output
iir->output=0.0f;

}

float IIR_Filter_filtering(IIR_Filter *iir, float input) {
//	Add new data to buffer_input
iir->buffer_input[iir->buffer_index]=input;

//	Reset old output value
iir->output=0;

uint8_t sum_index=iir->buffer_index+1;

//	FORWARD PART:
//	Decrement sum_index and loop if necessary
for(uint8_t i=0; i<iir->filter_order;i++){
	if(sum_index >0){
		sum_index--;
	}
	else{
		sum_index =iir->filter_order-1;
	}
//	Compute new output (via convolution)
	 iir->output+=iir->forward_coefficients[i]*iir->buffer_input[sum_index];
}

//	FEEDBACK PART:
sum_index=iir->buffer_index+1;
//	Decrement sum_index and loop if necessary
for(uint8_t i=0; i<iir->filter_order-1;i++){
	if(sum_index >0){
		sum_index--;
	}
	else{
		sum_index =iir->filter_order-2;
	}
iir->output+=iir->feedback_coefficients[i]*iir->buffer_output[sum_index];
}

//	Add new data to buffer_output
iir->buffer_output[iir->buffer_index]=iir->output;

//	Increment buffer_index and loop if necessary
iir->buffer_index++;
if(iir->buffer_index>iir->filter_order){
	iir->buffer_index=0;
}

return iir->output;
}




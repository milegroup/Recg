#include "getLocalPeak.h"
#include "commonFunctions.h"
#include <math.h>
#include <stdlib.h>



int isSignChange( int initSign, double *signal, int pos ){
	return ( (initSign < 0.0 && signal[pos] >= 0.0 ) || ( initSign >= 0.0 && signal[pos] < 0.0 ) );
}

int applyFirstCriteria(double *signal, int signalLength, int initialPos, int searchForward, int thr ){
	int initSign = (signal[initialPos] >= 0.0) ? 1 : -1;
	//int searchPosition = (searchForward) ? initialPos + 1 : initialPos - 1;
	int searchPosition = initialPos;

	// Recorre hasta que baja del umbral o hai un cambio de signo
	while( fabs(signal[searchPosition]) > thr ){
		if( isSignChange(initSign, signal, searchPosition) || (searchPosition == 0) || (searchPosition >= signalLength -1) ){
		//if( isSignChange(initSign, signal, searchPosition) || (searchPosition == 0)  ){
			break;
		}
		searchPosition = (searchForward) ? searchPosition + 1 : searchPosition - 1;		
	}
	return searchPosition;
}

int applySecondCriteria(double *signal, int signalLength, int initialPos, int searchForward, int sw ){
	double *signalAbs = getArrayAbs(signal, signalLength);		
	double *returnedPeak;
	int searchForMinimum; 
	// Buscar onset segundo criterio
	//int searchPosition = (searchForward) ? initialPos +1 : initialPos - 1;
	int searchPosition = initialPos;
	if( searchPosition != 0 && searchPosition != (signalLength - 1 ) ){ // Si ya está en la primera posición o en la última, que no busque		
		returnedPeak = getLocalPeak(signalAbs, signalLength, searchPosition, sw, searchForward, searchForMinimum = 1);		
		searchPosition = returnedPeak[0];
		free(returnedPeak);
	}
	free(signalAbs);
	return searchPosition;
}

int getOnset(double *signal, int signalLength, int peak, int pre, int sw, double thr){
	int searchForward;
	int onsetPos_criteria1 = applyFirstCriteria(signal, signalLength, pre, searchForward = 0, thr );
	int onsetPos_criteria2 = applySecondCriteria(signal, signalLength, pre, searchForward = 0, sw);	
	int toret =  abs( peak - onsetPos_criteria1 ) <= abs( peak - onsetPos_criteria2 )  ? onsetPos_criteria1 : onsetPos_criteria2 ;
	// De las posiciones dadas por los 2 criterios tomar la mas cercana a la onda P
	return toret;
}


int getEnd(double *signal, int signalLength, int peak, int post, int sw, double thr){
	int searchForward;
	int endPos_criteria1 = applyFirstCriteria(signal, signalLength, post, searchForward = 1, thr);
	int endPos_criteria2 = applySecondCriteria(signal, signalLength, post, searchForward = 1, sw);	
	int toret = abs( peak - endPos_criteria1) <= abs(peak - endPos_criteria2) ? endPos_criteria1 : endPos_criteria2 ;
	return toret;
}
#include "getLocalPeak.h"
#include "commonFunctions.h"
#include "ecgDelineationWaveletGetMML.h"
#include <stdlib.h>

int getLeftIndex_new(int qrsPosition, int qrsLength){
	return (qrsPosition - qrsLength < 0) ? 0 : qrsPosition - qrsLength;
}

int getRightIndex_new(int qrsPosition, int qrsLength, int signalLength){
	return (qrsPosition + qrsLength > signalLength) ? signalLength : qrsPosition + qrsLength;
}

double * getNpreNpostPositions_new(double *scale2, int lengthScale2, int *nqrsPositions, int numQrsPositions, int qrsLength){
	double * returnNpreNpostPositions = (double *) malloc(sizeof(double) * 2 * numQrsPositions );
	int returnPos = 0;

	int leftIndex;
	int rightIndex;
	double maxValue;
	int maxPosition;
	double minValue;
	int minPosition;
	int i;
	int searchWindow = 3;
	int *mmlPositions;
	int maxPos;
	int minPos;

	// Recorre todas las posiciones del QRS, las divide entre 2 para obtener su posicion en la escala 2 que tiene la mitad del tamaño de la escala 1
	// y después obtiene el máximo y mínimo que rodean a N QRS
	for(i = 0; i < numQrsPositions; i++){
		leftIndex = getLeftIndex_new(nqrsPositions[i] / 2, qrsLength / 2);
		rightIndex = getRightIndex_new(nqrsPositions[i] / 2, qrsLength / 2, lengthScale2 );

		mmlPositions =  getMML_interval(scale2, lengthScale2, leftIndex, rightIndex, searchWindow);
		maxPos = mmlPositions[0];
		minPos = mmlPositions[1];
		free(mmlPositions);

		// El primer pico, ya sea un máximo o un mínimo, lo guarda como nPre y el segundo como nPost
		returnNpreNpostPositions[returnPos++] = ( minPos < maxPos ) ? minPos : maxPos;
		returnNpreNpostPositions[returnPos++] = ( minPos < maxPos ) ? maxPos : minPos;
	}	
	return returnNpreNpostPositions;
		
}

 
/**
	A partir de un pico busca el pico que forma un maximum modulus line adyacente con el pico de referencia que se le pasa
	en un intervalo de tiempo. Con la opción forward busca hacia delante, sino hacia atrás.
*/
int getAdjacentMMLPeak_new(double *scale2, int lengthScale2, int refPeakPos, int intervalToSearch, int swLocalPeak, int searchForward){
	double refPeakValue = scale2[refPeakPos];
	
	int mmlLeftIndex = (searchForward) ? refPeakPos : (refPeakPos - intervalToSearch);
	int mmlRightIndex = (searchForward) ? (refPeakPos + intervalToSearch) : refPeakPos;
	
	int * localPeaks;
	int adjPeakPos;

	localPeaks = ( refPeakValue > 0 ) ? 
				 getAllLocalMinimums_interval(scale2, lengthScale2, mmlLeftIndex, mmlRightIndex, swLocalPeak) :  
				 getAllLocalMaximums_interval(scale2, lengthScale2, mmlLeftIndex, mmlRightIndex, swLocalPeak);

	adjPeakPos = selectCorrectAdjacentPeakFromArray(scale2, localPeaks, refPeakPos);
	free(localPeaks);

	return adjPeakPos;
}

/*
	Cambiar no caso de que npre_npost solo teña as posicions, e non teña tamén os valores das posicions
*/ 
double * getNFirstNlast_new(double *scale2, int lengthScale2, double *npre_npost, int intervalSize, int numQrsPositions){
	int leftIndex = 0;
	int rightIndex = intervalSize;
	int nprePos;
	int npostPos;
	int npre_npost_cont = 0;
	int swLocalPeak = 3;
	int intervalToSearch = 20; // prueba, 20 muestras
	int searchForward;
	int nFirst;
	int nLast;

	double * returnNFirstNlast = (double *) malloc(sizeof(double) * 2 * numQrsPositions );
	int returnPos = 0;


	
	nprePos = (int) npre_npost[npre_npost_cont++];
	npostPos = (int) npre_npost[npre_npost_cont++];	

	while(rightIndex < (lengthScale2 -1)){
		// Update thresholds
	    // thr.qrs.pre <- 0.06 * max(fabs(wtD2[left.index:right.index]))
	    // thr.qrs.post <- 0.09 * max(fabs(wtD2[left.index:right.index]))		

		// Para cada nPre y nPost busca su nFirst y nLast
		while( nprePos < rightIndex && npostPos < rightIndex && nprePos != 0 && npostPos != 0 ){				
			// place nFirst	
			nFirst = getAdjacentMMLPeak(scale2, lengthScale2, nprePos, intervalToSearch, swLocalPeak, searchForward = 0 );	
			returnNFirstNlast[returnPos++] = nFirst; // position		

			// place nLast
			nLast = getAdjacentMMLPeak(scale2, lengthScale2, npostPos, intervalToSearch, swLocalPeak, searchForward = 1 );	
			returnNFirstNlast[returnPos++] = nLast; // position			
			
					
			// next iteration
			nprePos = (int) npre_npost[npre_npost_cont++];
			npostPos = (int) npre_npost[npre_npost_cont++];
		}		

		// update indexes
		if( rightIndex == lengthScale2 - 2){
			break;
		}
		leftIndex = rightIndex;
		rightIndex += intervalSize;		
		if(rightIndex > (lengthScale2 - 1)){
			rightIndex = lengthScale2 - 2;
		}
	}	
	return returnNFirstNlast;
}

double * getQRS_onset_end_new(double *scale2, int lengthScale2, double *nfirst_nlast, int *nqrsPositions, int numQrsPositions){
	double *returnQRS_onset_end = (double *) malloc(sizeof(double) * 2 * numQrsPositions);
	int returnPos = 0;
	int i = 0;
	int contPos = 0;
	int nFirstPos;
	double nFirstValue;
	int nLastPos; 
	double nLastValue;
	double thr_qrs_onset;
	double thr_qrs_end;	
	int sw = 3;
	int scaleFactor = 2;

	while( i < numQrsPositions ){
		// Calculate thresholds
		nFirstPos = nfirst_nlast[contPos++];
		nFirstValue = scale2[nFirstPos];
		nLastPos = nfirst_nlast[contPos++];
		nLastValue = scale2[nLastPos];
		thr_qrs_onset = ( nFirstValue > 0.0 ) ? 0.05 * fabs(nFirstValue) : 0.07 * fabs(nFirstValue);
		thr_qrs_end = ( nLastValue > 0.0 ) ? 0.125 * fabs(nLastValue) : 0.71 * fabs(nLastValue);

		returnQRS_onset_end[returnPos++] = getOnset(scale2, lengthScale2, nqrsPositions[i], nFirstPos, sw, scaleFactor, thr_qrs_onset );
		returnQRS_onset_end[returnPos++] = getOnset(scale2, lengthScale2, nqrsPositions[i], nLastPos, sw, scaleFactor, thr_qrs_end );

		i++;
	}
	return returnQRS_onset_end;
}




void delineateQRS_new(double *scale1, double *scale2, int *lengthScale1, int *lengthScale2, int *nqrsPositions, int *numQrsPositions, int *qrsLength, 
				  int *intervalSize, int *returnOnsetPositions, int *returnEndPositions, int *returnQpeakPositions, int *returnSpeakPositions,
				  int *returnNprePositions, int *returnNpostPositions, int *returnNFirstPositions, int *returnNLastPositions ){

	double * npre_npost = (double *) getNpreNpostPositions_new(scale2, *lengthScale2, nqrsPositions, *numQrsPositions, *qrsLength);


}
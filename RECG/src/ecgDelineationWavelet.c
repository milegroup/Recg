#include "getLocalPeak.h"
#include "commonFunctions.h"
#include "ecgDelineationWaveletGetMML.h"
#include <stdlib.h>
#include <math.h>


int getLeftIndex(int qrsPosition, int qrsLength){
	return (qrsPosition - qrsLength < 0) ? 0 : qrsPosition - qrsLength;
}

int getRightIndex(int qrsPosition, int qrsLength, int signalLength){
	return (qrsPosition + qrsLength > signalLength) ? signalLength : qrsPosition + qrsLength;
}


// Obtener las posiciones de nPre y nPost a partir de la posicion N del QRS. Busca un mínimo y un máximo 
// adyacentes al punto N
// Devuelve un puntero a un array de double, que tiene 4 posiciones. 0 - posicion de n.pre, 1 - altura de n.pre, 2 - posicion de n.post, 3 - altura de n.post
double * getNpreNpostPositions(double *scale2, int lengthScale2, int *nqrsPositions, int numQrsPositions, int qrsLength){
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
		leftIndex = getLeftIndex(nqrsPositions[i] / 2, qrsLength / 2);
		rightIndex = getRightIndex(nqrsPositions[i] / 2, qrsLength / 2, lengthScale2 );

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
int getAdjacentMMLPeak(double *scale2, int lengthScale2, int refPeakPos, int intervalToSearch, int swLocalPeak, int searchForward){
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
double * getNFirstNlast_prueba(double *scale2, int lengthScale2, double *npre_npost, int intervalSize, int numQrsPositions){
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


double * getNFirstNlast(double *scale2, int lengthScale2, double *npre_npost, int intervalSize, int numQrsPositions){
	int leftIndex = 0;
	int rightIndex = intervalSize;
	int nprePos;
	double npreValue;
	int npostPos;
	double npostValue;
	int npre_npost_cont = 0;
	int sw = 3;
	double * returnedPeak;

	double * returnNFirstNlast = (double *) malloc(sizeof(double) * 2 * numQrsPositions );
	int returnPos = 0;
	int searchForward;
	int searchForMinimum;

	
	nprePos = (int) npre_npost[npre_npost_cont++];
	npreValue = scale2[nprePos];
	npostPos = (int) npre_npost[npre_npost_cont++];
	npostValue = scale2[npostPos];

	while(rightIndex < (lengthScale2 -1)){
		// Update thresholds
	    // thr.qrs.pre <- 0.06 * max(fabs(wtD2[left.index:right.index]))
	    // thr.qrs.post <- 0.09 * max(fabs(wtD2[left.index:right.index]))		

		// Para cada nPre y nPost busca su nFirst y nLast
		while( nprePos < rightIndex && npostPos < rightIndex && nprePos != 0 && npostPos != 0 ){				
			// place nFirst		
			if(npreValue > 0 ){
				returnedPeak = (double *) getLocalPeak(scale2, lengthScale2, nprePos, sw, searchForward = 0, searchForMinimum = 1);
				returnNFirstNlast[returnPos++] = returnedPeak[0]; // position
			}
			else{
				returnedPeak = (double *) getLocalPeak(scale2, lengthScale2, nprePos, sw, searchForward = 0, searchForMinimum = 0);
				returnNFirstNlast[returnPos++] = returnedPeak[0]; // position
			}
			free(returnedPeak);
			// place nLast
			if(npostValue > 0){
				returnedPeak = (double *) getLocalPeak(scale2, lengthScale2, npostPos, sw, searchForward = 1, searchForMinimum = 1);
				returnNFirstNlast[returnPos++] = returnedPeak[0]; // position
			}
			else{
				returnedPeak = (double *) getLocalPeak(scale2, lengthScale2, npostPos, sw, searchForward = 1, searchForMinimum = 0);
				returnNFirstNlast[returnPos++] = returnedPeak[0]; // position
			}
			free(returnedPeak);						
			// next iteration
			nprePos = (int) npre_npost[npre_npost_cont++];
			npreValue = scale2[nprePos];
			npostPos = (int) npre_npost[npre_npost_cont++];
			npostValue = scale2[npostPos];
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

/*
	Esta función recibe como parámetro una posición en la escala 2 en la que hay un pico, y busca en el pico en la escala 1.
	Teniendo en cuenta que puede haber un desfase entre las posiciones de los picos entre ambas escalas, se utiliza un 
	intervalSize como una ventana que indica el desfase que se permite. Si por ejemplo intervalSize = 2, significa que el desfase
	que puede existir es de 2 muestras, y busca el pico desde las 2 muestras anteriores hasta las 2 muestras siguientes en la escala 1.
	También hai que tener en cuenta que las posiciones en la escala 1 son el doble de las posiciones en la escala 2 ya que tiene es
	el doble en tamaño.
*/
int relocatePeaksAtAnotherScale(double *newScale, int newScaleLength, double *currentScale, int currentPos, int sw, int scaleFactor, int isBiggerScale){
	int newPos = (isBiggerScale) ? currentPos * scaleFactor : currentPos / scaleFactor;
	double currentValue = currentScale[currentPos];

	int leftIndex = ( newPos - sw < 0 ) ? 0 : newPos - sw;
	int rightIndex = ( newPos + sw > newScaleLength - 1 ) ? newScaleLength - 1 : newPos + sw;
	
	int peakPos;
	
	// Si era un pico positivo en la escala 2 busca un máximo, si era negativo busca un mínimo
	if( currentValue >= 0){
		peakPos = getArrayMaximum(newScale, leftIndex, rightIndex);
	}
	else{
		peakPos = getArrayMinimum(newScale, leftIndex, rightIndex);
	}

	return peakPos;
}



// Search Q peak as the minimum value (in absolute value in the scale 1) between the n.first and n.pre peaks
// Search S peak as the minimum value (in absolute value in the scale 1) between the n.post and n.last peaks
double * getQpeaks_Speaks(double *scale1, int lengthScale1, double *scale2, double *npre_npost, double *nfirst_nlast, int numBeats){
	double * returnQpeakSpeak = (double *) malloc(sizeof(double) * 2 * numBeats );
	int returnPos = 0;
	int nprePos;
	int npostPos;
	int nfirstPos;	
	int nlastPos;

	int sw = 2;
	int i;
	int cont_npre_npost = 0;
	int cont_nfirst_nlast = 0;
	int scaleFactor;
	int isBiggerScale;
	for(i = 0; i < numBeats; i++){
		nprePos = npre_npost[cont_npre_npost++];
		npostPos = npre_npost[cont_npre_npost++] ;

		nfirstPos = nfirst_nlast[cont_nfirst_nlast++];
		nlastPos = nfirst_nlast[cont_nfirst_nlast++];

		// Como puede haber 2 muestras de desfase entre la escala 1 y 2, recoloca los picos como máximos locales en el abs
		nprePos = relocatePeaksAtAnotherScale( scale1, lengthScale1, scale2, nprePos, sw, scaleFactor = 2, isBiggerScale = 1 );
		npostPos = relocatePeaksAtAnotherScale( scale1, lengthScale1, scale2, npostPos, sw, scaleFactor = 2, isBiggerScale = 1 );
		nfirstPos = relocatePeaksAtAnotherScale( scale1, lengthScale1, scale2, nfirstPos, sw, scaleFactor = 2, isBiggerScale = 1 );
		nlastPos = relocatePeaksAtAnotherScale( scale1, lengthScale1, scale2, nlastPos, sw, scaleFactor = 2, isBiggerScale = 1 );
		
		// Get Q and S peaks
		returnQpeakSpeak[returnPos++] = getMinimumInAbsArray( scale1, nfirstPos, nprePos); // Get Q peak
		returnQpeakSpeak[returnPos++] = getMinimumInAbsArray( scale1, npostPos, nlastPos); // Get S peak
		// Save results to return
	}	
	return returnQpeakSpeak;
}

double * getQRS_onset_end_prueba(double *scale2, int lengthScale2, double *nfirst_nlast, int *nqrsPositions, int numQrsPositions){
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

	while( i < numQrsPositions ){
		// Calculate thresholds
		nFirstPos = nfirst_nlast[contPos++];
		nFirstValue = scale2[nFirstPos];
		nLastPos = nfirst_nlast[contPos++];
		nLastValue = scale2[nLastPos];
		thr_qrs_onset = ( nFirstValue > 0.0 ) ? 0.05 * fabs(nFirstValue) : 0.07 * fabs(nFirstValue);
		thr_qrs_end = ( nLastValue > 0.0 ) ? 0.125 * fabs(nLastValue) : 0.71 * fabs(nLastValue);

		returnQRS_onset_end[returnPos++] = getOnset(scale2, lengthScale2, nqrsPositions[i], nFirstPos, sw, thr_qrs_onset );
		returnQRS_onset_end[returnPos++] = getEnd(scale2, lengthScale2, nqrsPositions[i], nLastPos, sw, thr_qrs_end );

		i++;
	}
	return returnQRS_onset_end;
}



double * getQRS_onset_end(double *scale2, int lengthScale2, double *nfirst_nlast, int *nqrsPositions, int numQrsPositions){
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
	int sign_nFirst;
	int sign_nLast;
	int onsetPos_criteria1;
	int onsetPos_criteria2;
	int endPos_criteria1;
	int endPos_criteria2;
	int sw = 3;
	double *returnedPeak;
	double *scale2Abs = getArrayAbs(scale2, lengthScale2);
	int searchForward;
	int searchForMinimum;
	while( i < numQrsPositions ){
		// Calculate thresholds
		nFirstPos = nfirst_nlast[contPos++];
		nFirstValue = scale2[nFirstPos];
		nLastPos = nfirst_nlast[contPos++];
		nLastValue = scale2[nLastPos];
		if( nFirstValue > 0.0 ){
			thr_qrs_onset = 0.05 * fabs(nFirstValue);
			sign_nFirst = 1;
		}
		else{
			thr_qrs_onset = 0.07 * fabs(nFirstValue);	
			sign_nFirst = -1;
		}
		if( nLastValue > 0.0 ){
			thr_qrs_end = 0.125 * fabs(nLastValue);			
			sign_nLast = 1;
		}
		else{
			thr_qrs_end = 0.71 * fabs(nLastValue);				
			sign_nLast = -1;
		}

		// Buscar onset desde nFirst según primer criterio
		onsetPos_criteria1 = nFirstPos;
		while( fabs(scale2[onsetPos_criteria1]) > thr_qrs_onset ){			
			// Si existe un cambio de signo guarda esa pos. como válida, porque bajaría del umbral
			// O si llega al principio de la señal
			if( ( sign_nFirst < 0 && scale2[onsetPos_criteria1] >= 0) || ( sign_nFirst >= 0 && scale2[onsetPos_criteria1] < 0) || (onsetPos_criteria1 == 0) ){
				break;
			}
			onsetPos_criteria1--;
		}
		// Buscar onset desde nFirst segundo criterio 
		onsetPos_criteria2 = nFirstPos;
		if( onsetPos_criteria2 != 0){
			// Por si ya está en el cero, que no busque más hacia atrás
			returnedPeak = (double *) getLocalPeak(scale2Abs, lengthScale2, onsetPos_criteria2, sw, searchForward = 0, searchForMinimum = 1);
			onsetPos_criteria2 = returnedPeak[0];
			free(returnedPeak);
		}
		// De las posiciones dadas por los 2 criterios tomar la mas cercana a N QRS
		if( abs( nqrsPositions[i] - onsetPos_criteria1 ) <= abs( nqrsPositions[i] - onsetPos_criteria2 ) ){
			returnQRS_onset_end[returnPos++] = onsetPos_criteria1;
		}
		else{
			returnQRS_onset_end[returnPos++] = onsetPos_criteria2;
		}

		// Buscar end desde nLast según primer criterio
		endPos_criteria1 = nLastPos;
		while( fabs(scale2[endPos_criteria1]) > thr_qrs_end ){
			if( ( sign_nLast < 0 && scale2[endPos_criteria1] >= 0) || ( sign_nLast >= 0 && scale2[endPos_criteria1] < 0) || (endPos_criteria1 >= (lengthScale2 -1))){
				break;
			}
			endPos_criteria1++;
		}
		// Buscar end desde nLast segundo criterio
		endPos_criteria2 = nLastPos;
		if( endPos_criteria2 != (lengthScale2 - 1 )){
			returnedPeak = (double *) getLocalPeak(scale2Abs, lengthScale2, endPos_criteria2, sw, searchForward = 1, searchForMinimum = 1);
			endPos_criteria2 = returnedPeak[0];
			free(returnedPeak);
		}
		// De las posiciones dadas por los 2 criterios toma la mas cercana a N QRS
		if( abs( nqrsPositions[i] - endPos_criteria1) <= abs(nqrsPositions[i] - endPos_criteria2)){
			returnQRS_onset_end[returnPos++] = endPos_criteria1;
		}
		else{
			returnQRS_onset_end[returnPos++] = endPos_criteria2;
		}

		i++;
	}

	free(scale2Abs);
	return returnQRS_onset_end;
}

void delineateQRS(double *scale1, double *scale2, int *lengthScale1, int *lengthScale2, int *nqrsPositions, int *numQrsPositions, int *qrsLength, 
				  int *intervalSize, int *returnOnsetPositions, int *returnEndPositions, int *returnQpeakPositions, int *returnSpeakPositions,
				  int *returnNprePositions, int *returnNpostPositions, int *returnNFirstPositions, int *returnNLastPositions ){	

	double * npre_npost = (double *) getNpreNpostPositions(scale2, *lengthScale2, nqrsPositions, *numQrsPositions, *qrsLength);
	double * nfirst_nlast = (double *) getNFirstNlast(scale2, *lengthScale2, npre_npost, *intervalSize, *numQrsPositions);	
	//double * nfirst_nlast = (double *) getNFirstNlast_prueba(scale2, *lengthScale2, npre_npost, *intervalSize, *numQrsPositions);	
	double * q_s_peaks = (double *)	getQpeaks_Speaks(scale1, *lengthScale1, scale2, npre_npost, nfirst_nlast, *numQrsPositions);
	//double * qrs_onset_end = (double *) getQRS_onset_end(scale2, *lengthScale2, nfirst_nlast, nqrsPositions, *numQrsPositions);
	double * qrs_onset_end = (double *) getQRS_onset_end_prueba(scale2, *lengthScale2, nfirst_nlast, nqrsPositions, *numQrsPositions);
	
	int i = 0;
	int j = 0;
	int cont = 0;	
	int tmp;
	int pos;

	while( i < *numQrsPositions){
		returnNprePositions[i] = ((int) npre_npost[cont]) * 2; // *2 Para pasarlo al valor de la escala 1
		returnNFirstPositions[i] = ((int) nfirst_nlast[cont]) * 2;
		returnOnsetPositions[i] =  ((int) qrs_onset_end[cont]) * 2; 
		returnQpeakPositions[i] =  (int) q_s_peaks[cont];
		i++;
		cont += 2;
	}
	i = 0;
	j = 0;
	cont = 1;
	while(i < *numQrsPositions){
		returnNpostPositions[i] = ((int) npre_npost[cont]) * 2;
		returnNLastPositions[i] = ((int) nfirst_nlast[cont]) * 2;
		returnEndPositions[i] = ((int) qrs_onset_end[cont]) * 2;
		returnSpeakPositions[i] =  (int) q_s_peaks[cont];
		i++;
		cont += 2;
	}

	free(npre_npost);
	free(nfirst_nlast);
	free(q_s_peaks);
	free(qrs_onset_end);
}




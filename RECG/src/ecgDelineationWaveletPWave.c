#include "commonFunctions.h"
#include "ecgDelineationWaveletGetMML.h"
#include "ecgDelineationWaveletGetOnsetEnd.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
 

void getPOnsetAndEndScale( double *scale, int lengthScale, int pPeak, int pPre, int pPost, int intervalSizeToSearch, int *onsetPos, int *endPos, int pPosIndex,
							int scaleFactor, int sw_beg, int sw_end ){
	double thr = 0.5 * fabs(scale[pPre]);
	onsetPos[pPosIndex] = getOnset(scale, lengthScale, pPeak, pPre, intervalSizeToSearch, thr) * scaleFactor;
	thr = 0.9 * fabs(scale[pPost]);
	endPos[pPosIndex] = getEnd(scale, lengthScale, pPeak, pPost, intervalSizeToSearch, thr) * scaleFactor;
}


void getPOnsetAndEnd(double *scale4, int lengthScale4, double *scale5, int lengthScale5, int pPeak, int modulus1, int modulus2, 
					 int *onsetPos, int *endPos, int pPosIndex, int scale, int sw_beg, int sw_end){	
	int scaleFactor = pow(2, scale) - pow(2, scale -1);
	int pPre = (modulus1 < modulus2) ? modulus1 : modulus2;
	int pPost = (modulus1 > modulus2) ? modulus1 : modulus2;
	int pPreScale = pPre / scaleFactor;
	int pPostScale = pPost / scaleFactor;
	int pPeakScale = pPeak / scaleFactor;
	int intervalSizeToSearch = 3;	


	if(scale == 4){
		getPOnsetAndEndScale(scale4, lengthScale4, pPeakScale, pPreScale, pPostScale, intervalSizeToSearch, onsetPos, endPos, pPosIndex, scaleFactor, sw_beg, sw_end); 
	}
	if(scale == 5){
		getPOnsetAndEndScale(scale5, lengthScale5, pPeakScale, pPreScale, pPostScale, intervalSizeToSearch, onsetPos, endPos, pPosIndex, scaleFactor, sw_beg, sw_end);
	}
}

/*
	Busca el pico de la onda P en un intervalo, desde left_index hasta right_index. 
	Primero hace la búsqueda en la escala 4, sino encuentra un pico válido en esa escala lo busca en la escala 5. Si en la escala 5 tampoco encuentra
	un pico válido entonces devuelve cero como posición del pico P, lo que significa que no hay onda P
	El algoritmo de búsqueda está basado en los artículos de J. Martínez et al. y el de Nicolas Boichat et al.		
	La posición del pico de la onda P se situa en la escala 3. Se busca el cruce por cero entre el MML en la escala 3 y en caso de que no se encuentre en
	la escala 3, se busca en la escala en la que se buscó el MML, es decir en la 4 o la 5.
*/
int * getPWaveFromInterval(double *signal, int lengthSignal, int left_index, int right_index, int qrs_onset, int scale){
	int pPeakPos = 0;
	int pr1 = 0;
	int pr2 = 0;
	int scaleFactor = pow(2, scale) - pow(2, scale -1);
	// Busca el pico P 
	int leftIndex = (ceil(left_index / scaleFactor) < 0) ? 0 : ceil(left_index / scaleFactor);
	int rightIndex = (round(right_index / scaleFactor) > (lengthSignal -1) ? (lengthSignal - 1) : round(right_index / scaleFactor) );
	int qrsOnset = round(qrs_onset / scaleFactor);
	if( rightIndex >= qrsOnset ){
		rightIndex = qrsOnset - 1;
	}
	//int searchWindow = (scale == 3) ? 5 : 3;
	int searchWindow = 3;	
	// buscar maximum modulus lines
	int *mmlPositions = getMML_interval(signal, lengthSignal, leftIndex, rightIndex, searchWindow);
	int maxPos = mmlPositions[0];
	int minPos = mmlPositions[1];
	free(mmlPositions);	
	// comprobar que los MML superan el umbral y si es así buscar el pico P en el cruce por cero
	double absMaxValue = fabs(signal[maxPos]);
	double absMinValue = fabs(signal[minPos]);
	double maxAbsValue = 0.0;
	double thr = 0.0;
	//if( signal[maxPos] > 0 && signal[minPos] < 0 && minPos != 0 && maxPos != 0 ){ // Si es un máximo positivo y un mínimo negativo
	if( minPos != 0 && maxPos != 0 ){ 
		maxAbsValue = ( absMaxValue >= absMinValue) ? absMaxValue : absMinValue;
		thr = 0.125 * maxAbsValue;
		if( absMaxValue > thr && absMinValue > thr ){ // Si ambos superan el umbral buscar el pico P
			if( minPos < maxPos ){
				pPeakPos = getMinimumInAbsArray(signal, minPos, maxPos);	
			}
			else{
				pPeakPos = getMinimumInAbsArray(signal, maxPos, minPos);
			}			
			pPeakPos = pPeakPos * scaleFactor;
			maxPos = maxPos * scaleFactor;
			minPos = minPos * scaleFactor;				
		}
	}
	// prueba --borrar
	/*if( minPos < left_index || maxPos < left_index || minPos > right_index || maxPos > right_index ){
		printf("\nLimite superado -- minPos:%d maxPos:%d leftIndex:%d rightIndex:%d left_index:%d right_index:%d scale:%d scaleFactor:%d  pPeakPos:%d", 
				minPos, maxPos, leftIndex * scaleFactor, rightIndex * scaleFactor, left_index, right_index, scale, scaleFactor, pPeakPos);
	}*/
	//printf("\nminPos: %d, maxPos: %d, signal[minPos]:%f, signal[maxPos]:%f, thr:%f, pPos: %d", minPos, maxPos, signal[minPos], signal[maxPos], thr, pPeakPos / 2);
	//printf("\nescala 1 minPos: %d, maxPos: %d, pPos: %d", minPos *8, maxPos*8, pPeakPos*8);
	int * toret = malloc(sizeof(int) *3);
	toret[0] = pPeakPos;
	toret[1] = maxPos;
	toret[2] = minPos;
	return toret;
}

void savePPosition(int *pWave, int *pPositions_return, int *maxPos_return, int *minPos_return, int pPosIndex, int sw_beg, int sw_end, int *leftIndex_return, 
					int *rightIndex_return){
	pPositions_return[pPosIndex] = pWave[0];
	maxPos_return[pPosIndex] = pWave[1];
	minPos_return[pPosIndex] = pWave[2];
	leftIndex_return[pPosIndex] = sw_beg;
	rightIndex_return[pPosIndex] = sw_end;
}

void placePAtScale(double *waveletScale, int lengthScale, int sw_beg, int sw_end, int qrsOnsetPos, int * pPositions_return, 
					int *maxPos_return, int *minPos_return, int pPosIndex, int scale, int *leftIndex_return, int *rightIndex_return){
	int *pWave = getPWaveFromInterval(waveletScale, lengthScale, sw_beg, sw_end, qrsOnsetPos, scale = 3);
	if( pWave[0] != 0){
		savePPosition(pWave, pPositions_return, maxPos_return, minPos_return, pPosIndex, sw_beg, sw_end, leftIndex_return, rightIndex_return );
	}
	free(pWave);
}

void delineatePWave( double *waveletScale3, double *waveletScale4, double *waveletScale5, int *lengthScale3, int *lengthScale4, int *lengthScale5, 
					int *rPositions, int *lengthRPositions, int *qrsOnsetPositions, int *initialRRMean, int *rrSize,
					int *pPositions_return, int *maxPos_return, int *minPos_return, int *leftIndex_return, int *rightIndex_return,
					int *pOnsetPos_return, int *pEndPos_return){

	int rCont = 0;
	int min = 0;
	int isw_beg = 0;
	int isw_end = 0;
	int nsw_beg = 0;
	int nsw_end = 0;
	int initRRMean = *initialRRMean;
	int rrArraySize = *rrSize;
	int rrArray[rrArraySize];
	int rrMean;	
	int arrayPrSize = 10;	
	int pr1Mean = 0;
	int arrayPr1[arrayPrSize];
	int pr2Mean = 0;
	int arrayPr2[arrayPrSize];
	int arrayPrCont = 0;
	int rPos = 0;
	int prevRPos = 0;
	int *pWave;
	int pPosIndex = 0;
	int arrayPrIndex = 0;
	int arrayPr_rightIndex = 0;
	int lostPWave = 0;	
	int scale = 0;
	int *currentPOnsetEnd;	
	// Busca la primera onda P. sucesivamente hasta que la encuentra 
	//getFirstPWave(initRRMean, qrsOnsetPositions );
	while(1){
		min = (85 < (initRRMean / 2)) ? 85 : (initRRMean / 2);
		isw_beg = (qrsOnsetPositions[rCont] - min); 
		isw_end = (qrsOnsetPositions[rCont] - 2);

		rPos = rPositions[rCont];
		rrArray[rCont % rrArraySize] = rPos;
		prevRPos = rPos;	
		// Buscar primera onda P
		pWave = getPWaveFromInterval(waveletScale4, *lengthScale4, isw_beg, isw_end, qrsOnsetPositions[rCont], scale = 4);
		if( pWave[0] == 0){ // Si no lo encuentra en la escala 4, lo busca en la 5
			free(pWave);
			pWave = getPWaveFromInterval(waveletScale5, *lengthScale5, isw_beg, isw_end, qrsOnsetPositions[rCont], scale = 5);
		}	
		rCont++;	
		if(pWave[0] != 0){ // Cuando encuentra, guarda todo y sale del bucle
			savePPosition(pWave, pPositions_return, maxPos_return, minPos_return, pPosIndex, isw_beg, isw_end, leftIndex_return, rightIndex_return);
			// Buscar onset y end
			getPOnsetAndEnd(waveletScale4, *lengthScale4, waveletScale5, *lengthScale5, pWave[0], pWave[1], pWave[2], pOnsetPos_return, pEndPos_return, pPosIndex, scale,
													isw_beg, isw_end);	
			free(pWave);
			// Buscar la onda P en la escala 3 y si existe lo marca ahí
			placePAtScale(waveletScale3, *lengthScale3, isw_beg, isw_end, qrsOnsetPositions[rCont], pPositions_return, 
							maxPos_return, minPos_return, pPosIndex, scale = 3, leftIndex_return, rightIndex_return);	

			

			arrayPr1[arrayPrIndex] = ( minPos_return[pPosIndex] < maxPos_return[pPosIndex]) ? rPos - minPos_return[pPosIndex] : rPos - maxPos_return[pPosIndex];
			arrayPr2[arrayPrIndex] = ( minPos_return[pPosIndex] < maxPos_return[pPosIndex]) ? rPos - maxPos_return[pPosIndex] : rPos - minPos_return[pPosIndex];

			pPosIndex++;
			arrayPrCont++;
			
			break;
		}
		else{
			free(pWave);
		}		
	}

	//printf("\nScale %d, p:%d maxPos:%d minPos:%d leftIndex:%d rightIndex:%d",((isScale5) ? 5 : 4), pWave[0], pWave[3], pWave[4], leftIndex_return[pPosIndex], rightIndex_return[pPosIndex]  );
	
	int i = 0;
	while( rCont < *lengthRPositions - 1){
	//while( rCont < 10){		
		arrayPr_rightIndex = ( arrayPrCont < arrayPrSize ) ? arrayPrCont - 1 : arrayPrSize - 1;				
		pr1Mean = 0.25 * arrayPr1[arrayPrIndex] + 0.75 * ceil(getArrayAvg(arrayPr1, 0, arrayPr_rightIndex));
		pr2Mean = 0.25 * arrayPr2[arrayPrIndex] + 0.75 * ceil(getArrayAvg(arrayPr2, 0, arrayPr_rightIndex));		
				
		rPos = rPositions[rCont];
		rrArray[rCont % rrArraySize] = rPos - prevRPos;
		prevRPos = rPos;		

		if( rCont < rrArraySize ){
			min = (85 < (initRRMean / 2)) ? 85 : (initRRMean / 2);	
		}
		else{
			rrMean = (int) getArrayAvg(rrArray, 0, rrArraySize - 1);
			min = (85 < (rrMean / 2)) ? 85 : (rrMean / 2);
		}
		//printf("\n\npr1Mean:%d  pr2Mean:%d pr1:%d pr2:%d  rrMean:%d", pr1Mean, pr2Mean, arrayPr1[arrayPrIndex], arrayPr2[arrayPrIndex], rrMean);
		/*printf("\n rrArraySize:%d rrArray: ", rrArraySize);
		for( i = 0; i < rrArraySize; i++ ){
			printf(" %d", rrArray[i]);
		}*/
		
		isw_beg = (qrsOnsetPositions[rCont] - min);
		isw_end = (qrsOnsetPositions[rCont] - 2);
		//nsw_beg = (isw_beg < (rPos - 1.5 * pr1Mean)) ? isw_beg : (rPos - 1.5 * pr1Mean);
		nsw_beg = (isw_beg > (rPos - 1.5 * pr1Mean)) ? isw_beg : (rPos - 1.5 * pr1Mean);
		nsw_end = (isw_end < (rPos - 0.6 * pr2Mean)) ? isw_end : (rPos - 0.6 * pr2Mean);
		//nsw_end = (isw_end > (rPos - 0.6 * pr2Mean)) ? isw_end : (rPos - 0.6 * pr2Mean);

		//printf("\nisw_beg:%d isw_end:%d nsw_beg:%d nsw_end:%d rCont:%d rPos:%d min:%d pr1Mean:%d pr2Mean:%d", 
		//		isw_beg, isw_end, nsw_beg, nsw_end, rCont, rPos, min, pr1Mean, pr2Mean);

				
		pWave = getPWaveFromInterval(waveletScale4, *lengthScale4, nsw_beg, nsw_end,qrsOnsetPositions[rCont], scale = 4);
		if( pWave[0] == 0){ // Si no lo encuentra en la escala 4, lo busca en la 5			
			free(pWave);
			pWave = getPWaveFromInterval(waveletScale5, *lengthScale5, nsw_beg, nsw_end, qrsOnsetPositions[rCont], scale = 5);
			if(pWave[0] == 0){ // Si tampoco lo encuentra en la escala 5, vuelve a buscar en la 4 con un intervalo mayor
				free(pWave);
				nsw_beg = (isw_beg > (rPos - 2.0 * pr1Mean)) ? isw_beg : (rPos - 2.0 * pr1Mean);
				nsw_end = (isw_end < (rPos - 0.5 * pr2Mean)) ? isw_end : (rPos - 0.5 * pr2Mean);
				pWave = getPWaveFromInterval(waveletScale4, *lengthScale4, nsw_beg, nsw_end, qrsOnsetPositions[rCont], scale = 4);
			}
		}
		
		//printf("\n isScale5:%d		isw_beg:%d isw_end:%d nsw_beg:%d nsw_end:%d rCont:%d rPos:%d", isScale5, isw_beg, isw_end, nsw_beg, nsw_end, rCont, rPos);
		
		if( pWave[0] == 0){
			//printf("\n------------------no encontró PWave rPos:%d  scale:%d",rPos, scale);
			lostPWave++;
			free(pWave);
		}
		else{
			savePPosition(pWave, pPositions_return, maxPos_return, minPos_return, pPosIndex, nsw_beg, nsw_end, leftIndex_return, rightIndex_return);
			// Buscar onset y end
			getPOnsetAndEnd(waveletScale4, *lengthScale4, waveletScale5, *lengthScale5, pWave[0], pWave[1], pWave[2], pOnsetPos_return, pEndPos_return, pPosIndex, scale,
													nsw_beg, nsw_end);
			free(pWave);
			// Buscar la onda P en la escala 3 y si existe lo marca ahí			
			placePAtScale(waveletScale3, *lengthScale3, nsw_beg, nsw_end, qrsOnsetPositions[rCont], pPositions_return, maxPos_return, 
							minPos_return, pPosIndex, scale = 3, leftIndex_return, rightIndex_return);

			
			
			//printf("\n P position:%d \n", pWave[0]);

			arrayPrIndex = arrayPrCont % arrayPrSize;
			arrayPr1[arrayPrIndex] = ( minPos_return[pPosIndex] < maxPos_return[pPosIndex]) ? rPos - minPos_return[pPosIndex] : rPos - maxPos_return[pPosIndex];
			arrayPr2[arrayPrIndex] = ( minPos_return[pPosIndex] < maxPos_return[pPosIndex]) ? rPos - maxPos_return[pPosIndex] : rPos - minPos_return[pPosIndex];
			pPosIndex++;	
			arrayPrCont++;
		}
				
		rCont++;
	}
	printf("\nNº lostPWave:%d",lostPWave);
}

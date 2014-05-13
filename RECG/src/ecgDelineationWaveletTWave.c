#include "commonFunctions.h"
#include "ecgDelineationWaveletGetMML.h"
#include "ecgDelineationWaveletGetOnsetEnd.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



void getTOnsetAndEndScale( double *scale, int lengthScale, int tPeak, int tPre, int tPost, int intervalSizeToSearch, int *onsetPos, int *endPos, int tPosIndex,
							int scaleFactor ){
	double thr = 0.5 * fabs(scale[tPre]);
	onsetPos[tPosIndex] = getOnset(scale, lengthScale, tPeak, tPre, intervalSizeToSearch, thr) * scaleFactor;
	thr = 0.9 * fabs(scale[tPost]);
	endPos[tPosIndex] = getEnd(scale, lengthScale, tPeak, tPost, intervalSizeToSearch, thr) * scaleFactor;
}


void getTOnsetAndEnd(double *scale4, int lengthScale4, double *scale5, int lengthScale5, int tPeak, int modulus1, int modulus2, 
					  int *onsetPos, int *endPos, int tPosIndex, int scale){	
	int scaleFactor = pow(2, scale) - pow(2, scale -1);
	int tPre = (modulus1 < modulus2) ? modulus1 : modulus2;
	int tPost = (modulus1 > modulus2) ? modulus1 : modulus2;
	int tPreScale = tPre / scaleFactor;
	int tPostScale = tPost / scaleFactor;
	int tPeakScale = tPeak / scaleFactor;	
	int intervalSizeToSearch = 3;

	if(scale == 4){
		getTOnsetAndEndScale(scale4, lengthScale4, tPeakScale, tPreScale, tPostScale, intervalSizeToSearch, onsetPos, endPos, tPosIndex, scaleFactor); 
	}
	if(scale == 5){
		getTOnsetAndEndScale(scale5, lengthScale5, tPeakScale, tPreScale, tPostScale, intervalSizeToSearch, onsetPos, endPos, tPosIndex, scaleFactor);
	}
}




int * getTWaveFromInterval(double *signal, int lengthSignal, int left_index, int right_index, int scale){
	int scaleFactor = pow(2, scale) - pow(2, scale -1);
	int tPeakPos = 0;

	int leftIndex = (ceil(left_index / scaleFactor) < 0) ? 0 : ceil(left_index / scaleFactor);
	int rightIndex = (round(right_index / scaleFactor) > (lengthSignal -1) ? (lengthSignal - 1) : round(right_index / scaleFactor) );
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
	
	if( minPos != 0 && maxPos != 0 ){ 
		maxAbsValue = ( absMaxValue >= absMinValue) ? absMaxValue : absMinValue;
		thr = 0.125 * maxAbsValue;
		if( absMaxValue >= thr && absMinValue >= thr ){ // Si ambos superan el umbral buscar el pico T
			if(minPos < maxPos) {
				tPeakPos = getMinimumInAbsArray(signal, minPos, maxPos);				
			}
			else{
				tPeakPos = getMinimumInAbsArray(signal, maxPos, minPos);	
			}
			tPeakPos = tPeakPos * scaleFactor;
			maxPos = maxPos * scaleFactor;
			minPos = minPos * scaleFactor;		
		}
	}

	/*if(isScale5){
		printf("\nbusqueda escala 5 - leftIndex:%d rightIndex:%d", leftIndex, rightIndex);
		printf("\nminPos: %d, maxPos: %d, signal[minPos]:%f, signal[maxPos]:%f, thr:%f, tPos: %d", minPos, maxPos, signal[minPos], signal[maxPos], thr, tPeakPos / 2);
	}
	else{
		printf("\nbusqueda escala 4 - leftIndex:%d  rightIndex:%d", leftIndex, rightIndex);
		printf("\nminPos: %d, maxPos: %d, signal[minPos]:%f, signal[maxPos]:%f, thr:%f, tPos: %d", minPos, maxPos, signal[minPos], signal[maxPos], thr, tPeakPos);
	}*/

	int * toret = malloc(sizeof(int) *3);
	toret[0] = tPeakPos;
	toret[1] = maxPos;
	toret[2] = minPos;

	return(toret);
}

void saveTPosition(int *tWave, int *tPositions_return, int *maxPos_return, int *minPos_return, int tPosIndex, int sw_beg, int sw_end, int *leftIndex_return, 
					int *rightIndex_return){
	tPositions_return[tPosIndex] = tWave[0];
	maxPos_return[tPosIndex] = tWave[1];
	minPos_return[tPosIndex] = tWave[2];
	leftIndex_return[tPosIndex] = sw_beg;
	rightIndex_return[tPosIndex] = sw_end;
}

void placeTAtScale(double *waveletScale3, int lengthScale3, int sw_beg, int sw_end, int qrsOnsetPos, int * tPositions_return, 
					int *maxPos_return, int *minPos_return, int tPosIndex, int scale, int *leftIndex_return, int *rightIndex_return){
	int *tWave = getTWaveFromInterval(waveletScale3, lengthScale3, sw_beg, sw_end, scale = 3);
	if( tWave[0] != 0){
		saveTPosition(tWave, tPositions_return, maxPos_return, minPos_return, tPosIndex, sw_beg, sw_end, leftIndex_return, rightIndex_return );
	}
	free(tWave);
}

void delineateTWave(double *waveletScale3, double *waveletScale4, double *waveletScale5, int *lengthScale3, int *lengthScale4, int *lengthScale5,
					int *rPositions, int *lengthRPositions, int *qrsOnsetPositions, int *qrsEndPositions, int *initialRRMean, int *rrSize,
					int *tPositions_return, int *maxPos_return, int *minPos_return, int *leftIndex_return, int *rightIndex_return,
					int *tOnsetPos_return, int *tEndPos_return){
	
	int rrArraySize = *rrSize;
	int rrArray[rrArraySize];
	int arrayRRIndex = 0;
	int rrMean = *initialRRMean;
	int rCont = 0;
	int rPos = 0;
	int prevRR = 0;
	int prevRPos = 0;
	int * qtcArray = malloc(sizeof(int) * rrArraySize);
	int qtcMax = 0;

	int initRRMean = *initialRRMean;
	
	int sw_beg = 0;
	int sw_end = 0;
	int min = 0;

	int scale = 0;
	int tPosIndex = 0;
	int lostTWave = 0;
	int *tWave;
	int tDuration = 100 * 250 / 1000; // 100 ms

	// Busca la primera onda T sucesivamente hasta que la encuentra
	while(1){
		rPos = rPositions[rCont];
		arrayRRIndex = rCont % rrArraySize;
		prevRR = rPos - prevRPos;
		rrArray[arrayRRIndex] = prevRR;
		prevRPos = rPos;

		sw_beg = qrsEndPositions[rCont] + 15;
		min = (87 < (rrMean / 2 + 2)) ? 87 : (rrMean / 2 + 2);
		sw_end = qrsOnsetPositions[rCont + 1] - min;

		
		tWave = getTWaveFromInterval(waveletScale4, *lengthScale4, sw_beg, sw_end, scale = 4);		
		if( tWave[0] == 0){ // Si no lo encuentra en la escala 4, lo busca en la 5
			free(tWave);
			tWave = getTWaveFromInterval(waveletScale5, *lengthScale5, sw_beg, sw_end, scale = 5);
		}
		rCont++;
		if( tWave != 0 ){
			saveTPosition(tWave, tPositions_return, maxPos_return, minPos_return, tPosIndex, sw_beg, sw_end, leftIndex_return, rightIndex_return);
			
			// Buscar onset y end
			getTOnsetAndEnd(waveletScale4, *lengthScale4, waveletScale5, *lengthScale5, tWave[0], tWave[1], tWave[2], tOnsetPos_return, tEndPos_return, tPosIndex, scale);
			// Buscar la onda T en la escala 3 y si existe la marca ahí			
			free(tWave);

			placeTAtScale(waveletScale3, *lengthScale3, sw_beg, sw_end, qrsOnsetPositions[rCont], tPositions_return, 
						maxPos_return, minPos_return, tPosIndex, scale = 3, leftIndex_return, rightIndex_return);

			tPosIndex++;
			//qtcArray[arrayRRIndex] = ( tPositions_return[tPosIndex-1] + 10 - qrsOnsetPositions[rCont-1] ) / sqrt(rrMean);		
			qtcArray[arrayRRIndex] = ( tPositions_return[tPosIndex-1] - qrsOnsetPositions[rCont-1] ) / sqrt(prevRR);		
			qtcMax = qtcArray[getArrayMaximumInteger(qtcArray, 0, rCont - 1)];
			
			break;
		}
		else{
			free(tWave);	
		}
		
	}

	

	//printf("\nDespues de buscar primera posicion. T: %d max:%d min:%d leftIndex:%d rightIndex:%d", tPositions_return[tPosIndex- 1],
	//			maxPos_return[tPosIndex -1], minPos_return[tPosIndex -1], leftIndex_return[tPosIndex -1], rightIndex_return[tPosIndex-1] );

	//while( rCont < *lengthRPositions - 1){
	while( rCont < *lengthRPositions - 2){
	//while( rCont < 10){
		// Calcular RR mean 
		rPos = rPositions[rCont];
		arrayRRIndex = rCont % rrArraySize;
		prevRR = rPos - prevRPos;
		rrArray[arrayRRIndex] = prevRR;
		prevRPos = rPos;

		if( rCont < rrArraySize ){
			rrMean = initRRMean;
		}
		else{
			rrMean = (int) getArrayAvg(rrArray, 0, rrArraySize - 1);
		}

		sw_beg = qrsEndPositions[rCont] + 15;
		min = (87 < (rrMean / 2 + 2)) ? 87 : (rrMean / 2 + 2);

		// Limita la ventana de búsqueda por la derecha
		sw_end = ( qrsOnsetPositions[rCont] + (1.5 * qtcMax * sqrt(rrMean)) < (qrsOnsetPositions[rCont + 1] - min) ) ?
				   qrsOnsetPositions[rCont] + (1.5 * qtcMax * sqrt(rrMean)) : (qrsOnsetPositions[rCont + 1] - min);

	   // Prueba
		if(sw_beg + tDuration >= sw_end){
			//printf("\nAmpliando SW, sw_beg:%d  sw_end:%d", sw_beg, sw_end);
			sw_end = sw_beg + tDuration;
		}
		
		//sw_end = round( qrsOnsetPositions[rCont] + (1.5 * qtcMax * sqrt(rrMean)) ) ;

		/*if(rCont < 30){
			printf("\nsw_end1: %d sw_end2:%d",(qrsOnsetPositions[rCont] + (1.5 * qtcMax * sqrt(rrMean))), ((qrsOnsetPositions[rCont + 1] - min)) );
		}*/

				
		if( sw_beg >= sw_end){
			printf("\n\n\n ERROR: sw_end:%d > sw_beg:%d", sw_end, sw_beg);
			printf("\nqtcMax:%d", qtcMax);		
			printf("\n qtcArray:");
			int i = 0;
			for(; i < rrArraySize; i++ ){
				printf(" %d",qtcArray[i] );
			}
			printf("\ninitRRMean: %d", initRRMean);
			printf("\nAntes de buscar T Wave, rCont:%d rrMean:%d  sw_beg:%d  sw_end:%d", rCont, rrMean, sw_beg, sw_end);
			printf("\nqrs.end:%d qrs.onset:%d", qrsEndPositions[rCont], qrsOnsetPositions[rCont] );
		}
		else{
			tWave = getTWaveFromInterval(waveletScale4, *lengthScale4, sw_beg, sw_end, scale = 4);		
			if( tWave[0] == 0){ // Si no lo encuentra en la escala 4, lo busca en la 5
				free(tWave);
				tWave = getTWaveFromInterval(waveletScale5, *lengthScale5, sw_beg, sw_end, scale = 5);			
			}		

			if( tWave[0] == 0){
				//printf("\n------------------no encontró T Wave rPos:%d  scale:%d", rPos, scale);
				lostTWave++;
				free(tWave);
			}
			else{
				saveTPosition(tWave, tPositions_return, maxPos_return, minPos_return, tPosIndex, sw_beg, sw_end, leftIndex_return, rightIndex_return);
				
				// Buscar onset y end
				getTOnsetAndEnd(waveletScale4, *lengthScale4, waveletScale5, *lengthScale5, tWave[0], tWave[1], tWave[2], tOnsetPos_return, tEndPos_return, tPosIndex, scale);
				// Buscar la onda T en la escala 3 y si existe la marca ahí
				free(tWave);
				placeTAtScale(waveletScale3, *lengthScale3, sw_beg, sw_end, qrsOnsetPositions[rCont], tPositions_return, 
						maxPos_return, minPos_return, tPosIndex, scale = 3, leftIndex_return, rightIndex_return);

				// Calcular QTc
				// El final de la onda T se supone 10 muestras despues del pico T
				qtcArray[arrayRRIndex] = ( tEndPos_return[tPosIndex] - qrsOnsetPositions[rCont] ) / sqrt(prevRR);
				if( rCont < 10 ){
					qtcMax = qtcArray[getArrayMaximumInteger(qtcArray, 0, rCont)];				
				}
				else{
					qtcMax = qtcArray[getArrayMaximumInteger(qtcArray, 0, rrArraySize - 1)];
				}
				tPosIndex++;
			}

			// Prueba para ver la ventana de búsqueda en las primeras posiciones
			/*if(rCont < 10 ){
				printf("\nsw_beg:%d  sw_end:%d  t_end:%d qrsOnset:%d prevRR:%d", sw_beg, sw_end, tEndPos_return[tPosIndex - 1], qrsOnsetPositions[rCont], prevRR );				
			}*/

			
		}		

		/*printf("\nrCont:%d T: %d max:%d min:%d leftIndex:%d rightIndex:%d  qtcMax:%f", rCont, tPositions_return[tPosIndex- 1],
				maxPos_return[tPosIndex -1], minPos_return[tPosIndex -1], leftIndex_return[tPosIndex -1], rightIndex_return[tPosIndex-1],
				qtcMax );*/		

		rCont++;
	}
	free(qtcArray);
	// Falta detectar la última onda P, usando el mismo tamaño de intervalo que en la penúltima	

	/*int sqrtRR = ceil(sqrt(initRRMean));
	int QTc = ( initialTPos - qrsOnsetPositions[0] ) / sqrtRR ;
	sw_beg = qrsEndPositions[1] + 15;
	sw_end = qrsOnsetPositions[1] + ceil(1.5 * QTc * sqrtRR);*/
}
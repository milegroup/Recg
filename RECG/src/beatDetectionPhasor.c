#include "commonFunctions.h"



void getMaxMin(double *signal, int beginning, int end, double *max, double *min){
	

	for( int i = beginning; i < end; i++){
		if( signal[i] > *max ){
			*max = signal[i];
		}

		if( signal[i] < *min){
			*min = signal[i];
		}
	}
}

void getNormalizedSignal(double *signal, int *signalSize, int *windowSize, double *normalizedSignal){
	double max = -99999.0;
	double min = 99999.0;

	for(int i = 0; i < *signalSize; i++){		
		max = -99999.0;
		min = 99999.0;		

		if( i + *windowSize >= *signalSize ){ // que calcule el final de la señal sin seguir reduciendo la ventana
			int begPos = (*signalSize - *windowSize);
			getMaxMin(signal, begPos, *signalSize, &max, &min);
			normalizedSignal[i] = ( (2 * (signal[i] - min) )  / (max - min) ) - 1;
			
		}		
		else{
			int endPos = (i + *windowSize);
			getMaxMin(signal, i, endPos, &max, &min);
			normalizedSignal[i] = ( (2 * (signal[i] - min) )  / (max - min) ) - 1;
		}
	}
}

void getMaxMagnitude( double *magnitude, int leftPos, int rightPos, int *maxPos, double *maxHeight  ){
	
	*maxHeight = -99999.0;
	*maxPos = 0;
	for(int i = leftPos; i < rightPos ; i++){
		if( magnitude[i] > *maxHeight ){
			*maxHeight = magnitude[i];
			*maxPos = i;		
		}
	}	
}




void phasorAlg( double *phase, double *magnitude, double *thr, int *arrayLength, int *resultPeakPositions, double *resultPeakHeights, int *samplingFreq,
				int *maxQRSWindow){
	int ARRAYSIZE = 8;
	
	int leftPos = 0;
	int rightPos = 0;
	int peakPos = 0;
	int maxPos = 0;
	double maxHeight = 0.0;
	double maxMagnitude = 0.0;
	int maxMagnitudePos = 0;

	// Intervalos RR
	int RRPos = 0;
	double currentRRInterval = 300.0;
	double RRInterval[ARRAYSIZE];
	double RRMedian = 300.0;
	
	double refractoryPeriod200 = *samplingFreq * 0.2; //200ms
	int maxRRInterval = 400;


	// Picos R
	int currentPeakP = 0;
	double currentPeakH = 0.0;
	int lastPeakP = 0;
	
	double lastPeaks[ARRAYSIZE];
	int lastPeakPos = 0;
	
	// Umbrales
	double adptThr = *thr;
	double newThr = *thr;
	
	int i = 0;
	while(i < *arrayLength){
		
		if( phase[i] >= adptThr ){
			leftPos = i;			
			while( phase[i] > adptThr ){ // sigue recorriendo hasta que no supera el umbral
				i++;
			}	
			// Utilizando la duración máxima que puede tener un complejo QRS no se permie coger más muestra aunque superen el umbral
			if( (rightPos - leftPos) < *maxQRSWindow){
				rightPos = i;	
			}
			else{
				rightPos = leftPos + *maxQRSWindow;
				i = rightPos;
			}

			// Busca el pico en la máxima magnitud
			getMaxMagnitude( magnitude, leftPos, rightPos, &maxPos, &maxHeight);
			
			// guarda el pico
			resultPeakPositions[peakPos] = maxPos;
			resultPeakHeights[peakPos] = maxHeight;
			
			

			currentPeakP = resultPeakPositions[ peakPos ];
			if( peakPos >= 1 ){
				lastPeakP = resultPeakPositions[ peakPos -1 ];
				currentRRInterval = currentPeakP - lastPeakP;				
			}

					

			// Umbral adaptativo
			if( lastPeakPos < ARRAYSIZE ){ //if array not full
				lastPeaks[lastPeakPos] = phase[ currentPeakP ]; // valor del pico en la fase
				lastPeakPos++;										
				adptThr = median( lastPeaks, &lastPeakPos ) - 0.001;				
			}	
			else{ // array full
				lastPeaks[ lastPeakPos % ARRAYSIZE ] = phase[ currentPeakP ];
				lastPeakPos++;
				adptThr = median( lastPeaks, &ARRAYSIZE ) - 0.001;
			}

			
			// Si no se detecta un pico en cierto tiempo, considero como pico el valor de máxima magnitud entre ambos picos detectados
			if( currentRRInterval > (1.5 * RRMedian)){
				// mantener el pico actual
				resultPeakPositions[peakPos + 1] = resultPeakPositions[peakPos];
				resultPeakHeights[peakPos +1 ] = resultPeakHeights[peakPos];

				// guardar el nuevo pico
				resultPeakPositions[peakPos] = maxMagnitudePos;
				resultPeakHeights[peakPos] = maxMagnitude;
				maxMagnitude = 0.0;
				maxMagnitudePos = 0;
				peakPos++;
				currentPeakP = resultPeakPositions[ peakPos];
				lastPeakP = resultPeakPositions[ peakPos - 1];
				currentRRInterval = currentPeakP - lastPeakP;
			}
			else{
				maxMagnitude = 0.0;
				maxMagnitudePos = 0;
			}

			// Remove False Positives
			if( ( peakPos >= 1 ) && (currentRRInterval < 0.4 * RRMedian || currentRRInterval < refractoryPeriod200 )){
				
			
				if( magnitude[currentPeakP] >= magnitude[lastPeakP] ){ // Eliminar pico anterior
					resultPeakPositions[ peakPos - 1 ] = resultPeakPositions[ peakPos ];
					resultPeakHeights[ peakPos - 1 ] = resultPeakHeights[ peakPos ];
					currentRRInterval = resultPeakPositions[ peakPos - 1 ] - resultPeakPositions[ peakPos - 2 ];
					peakPos--;
				}
				else{
					if( magnitude[lastPeakP] > magnitude[currentPeakP] ){ // Eliminar pico actual
						currentRRInterval = resultPeakPositions[ peakPos - 1 ] - resultPeakPositions[ peakPos - 2 ];
						peakPos--; // Esto provoca que se sobreescriba la proxima vez
					}
				}
			}



			
			
			
			
			// RR Intervals
			if(  RRPos < ARRAYSIZE ){
				RRInterval[RRPos] = currentRRInterval;
				RRPos++;
				RRMedian = median(RRInterval, &RRPos); 
			}
			else{
				RRInterval[RRPos % ARRAYSIZE ] = currentRRInterval;
				RRPos++;
				RRMedian = median(RRInterval, &ARRAYSIZE);
			}

			peakPos++;
		}
		else{
			// Va guardando el valor de máxima magnitud de los que no superan el umbral
			if( maxMagnitude < magnitude[i]){
				maxMagnitudePos = i;
				maxMagnitude = magnitude[i];
			}
			i++;
		}



	}
}
#include <stdlib.h>
#include <math.h>

/*
	Compares two values in order to know which is greater than the other
*/
int compare (const void * a, const void * b){	
	  return ( *(double*)a - *(double*)b );
}

/*
	Calculates the array's median
*/
double median(double *array, int *elements){
	double first;
	double second;
	qsort (array, *elements, sizeof(double), compare);

	int i;	

	if( *elements % 2 == 0){ // even
		first = array[*elements/2 -1];
		second = array[*elements/2];
		return( (first + second)/2 );
	}
	else{	// odd
		return( array[*elements/2] );
	}
}


/*
	Returns 1 when the current peak is a T-Wave, 0 otherwise
	The current peak is a T-Wave if the distance with the previous peak is less than 200 ms or if the distance with the previous peak is between 200ms and 360ms and the current peak's slope is less than half the previous peak's slope. The slope is obteined from the derivate signal.
*/
int isTWave( int *samplingFrequency, int *previousPeakPosition, int *currentPeakPosition, double *derivateSignal, int *DELAY){
	double refractoryPeriod200 = *samplingFrequency * 0.2; //200ms
	double refractoryPeriod360 = *samplingFrequency * 0.36; //360ms

	if( ( *currentPeakPosition - *previousPeakPosition ) < refractoryPeriod200 )
		return(1);
	else{		
		//if( ( ( *currentPeakPosition - *previousPeakPosition ) > refractoryPeriod200) && ( ( *currentPeakPosition - *previousPeakPosition ) < refractoryPeriod360 ) ){
		//	if( ( derivateSignal[ *currentPeakPosition - *DELAY ] * 2 ) < ( derivateSignal[ *previousPeakPosition - *DELAY]) ) 
		//		return(1);
		//	else
		//		return(0);
		//}
		//else
			return(0);
	}
}


/*
	This function traverses all the signal and detects all the peaks (local maximum).
	A peak is considered when their neighbours in an interval are smaller in height. The interval size is a parameter.
	The bigger the interval size is, the less peaks are detected.

*/
void peakDetector(double *signal, int *peakPositions, double *peakHeights, int *arraySize, int *intervalSize){
	int i = *intervalSize;
	int peakCont = 0;
	int isPeak;
	while( i < (*arraySize - *intervalSize) ){ 
		isPeak = 1; // == true Suppose is a peak

		for(int j = (i - *intervalSize) ; j < (i+ *intervalSize); j++){ // if all positions are smaller, is a peak
			if(j == i)
				continue;
			if( signal[j] > signal[i] ){  // otherwise not
				isPeak = 0;
				break;
			}
		}

		if( isPeak ) // save peak position
		{			
			peakHeights[ peakCont ] = signal[i]; 
			peakPositions[ peakCont ] = i + 1; // Se suma 1 para compensar con R
			peakCont++;
		}
		i++;
	}
}

/*
	This function traverses all the signal and detects all the peaks (local maximum).
	A peak is considered when their neighbours in an interval are smaller in height. The interval size is a parameter.
	The bigger the interval size is, the less peaks are detected.

*/
void peakDetector_SinR(double *signal, int *peakPositions, double *peakHeights, int *arraySize, int *intervalSize){
	int i = *intervalSize;
	int peakCont = 0;
	int isPeak;
	while( i < (*arraySize - *intervalSize) ){ 
		isPeak = 1; // == true Suppose is a peak

		for(int j = (i - *intervalSize) ; j < (i+ *intervalSize); j++){ // if all positions are smaller, is a peak
			if(j == i)
				continue;
			if(  signal[j] > signal[i] ){  // otherwise not
				isPeak = 0;
				break;
			}
		}

		if( isPeak ) // save peak position
		{			
			peakHeights[ peakCont ] = signal[i]; 
			peakPositions[ peakCont ] = i; 
			peakCont++;
		}
		i++;
	}
}


/*
Calcula el root mean square de un array, desde leftIndex a rightIndex
*/
double getRMS(double *array, int *leftIndex, int *rightIndex){
	int currentPos = *leftIndex;
	int arraySize = *rightIndex - *leftIndex;
	double sum = 0;

	while(currentPos <= *rightIndex){
		sum = sum + (array[currentPos] * array[currentPos]);
		currentPos++;
	}
	return sqrt(sum / arraySize);
}

// Recibe como parámetro un array y devuelve el mismo array pero en valor absoluto
double * getArrayAbs(double * array, int arrayLength){
	double * returnArrayAbs = (double *) malloc(sizeof(double) * arrayLength);
	int i = 0;
	while( i < arrayLength){
		returnArrayAbs[i] = fabs(array[i]);
		i++;
	}
	return returnArrayAbs;
}

int getMinimumInAbsArray(double *array, int leftIndex, int rightIndex){
	double minValue = fabs(array[leftIndex]);
	int minPos = leftIndex;
	leftIndex++;	
	while(leftIndex <= rightIndex){	
		if(fabs(array[leftIndex]) < minValue){
			minValue = fabs(array[leftIndex]);
			minPos = leftIndex;
		}
		leftIndex++;
	}
	return minPos;
}

int getMaximumInAbsArray(double *array, int leftIndex, int rightIndex){
	double maxValue = fabs(array[leftIndex]);
	int maxPos = leftIndex;
	leftIndex++;	
	while(leftIndex <= rightIndex){	
		if(fabs(array[leftIndex]) > maxValue){
			maxValue = fabs(array[leftIndex]);
			maxPos = leftIndex;
		}
		leftIndex++;
	}
	return maxPos;
}

int getArrayMinimum(double *array, int leftIndex, int rightIndex){
	double minValue = array[leftIndex];
	int minPos = leftIndex;
	while(leftIndex <= rightIndex){
		if(array[leftIndex] < minValue){
			minValue = array[leftIndex];
			minPos = leftIndex;
		}
		leftIndex++;
	}
	return minPos;
}

int getArrayMaximum(double *array, int leftIndex, int rightIndex){
	double maxValue = array[leftIndex];
	int maxPos = leftIndex;
	while(leftIndex <= rightIndex){
		if(array[leftIndex] > maxValue){
			maxValue = array[leftIndex];
			maxPos = leftIndex;
		}
		leftIndex++;
	}
	return maxPos;
}

int getArrayMaximumInteger(int *array, int leftIndex, int rightIndex){
	int maxValue = array[leftIndex];
	int maxPos = leftIndex;
	while(leftIndex <= rightIndex){
		if(array[leftIndex] > maxValue){
			maxValue = array[leftIndex];
			maxPos = leftIndex;
		}
		leftIndex++;
	}
	return maxPos;
}


double getArrayAvg(int *array, int leftIndex, int rightIndex){
	double avg = 0;
	int sum = 0;	
	int cont = 0;
	while(leftIndex <= rightIndex){
		sum += array[leftIndex];
		leftIndex++;
		cont++;
	}
	avg = sum / cont;
	return avg;
}


/**
	Recorre todos los picos guardados de una señal recolocando aquellos que no estaban colocados exactamente en el pico por un par de muestras de
	desfase.
*/
void relocatePeakPositions(double *signal, int *peakPositions, int *numberOfPeaks, int *resultPeakPositions){
	int i, j;
	int windowSize = 2; // comprueba las 2 muestras vecinas al modulus line
	int currentPos; 
	int maxPos;
	double max;
	for( i = 0; i < *numberOfPeaks; i++){
		currentPos = peakPositions[i];
		maxPos = currentPos;
		max = fabs(signal[currentPos]);
		// Recorre las muestras vecinas al pico actual y si hai un pico mayor lo guarda
		for(j = currentPos - windowSize; j < currentPos + windowSize; j++){
			if( fabs(signal[j]) > max ){
				max = fabs(signal[j]);
				maxPos = j;
			}
		}
		resultPeakPositions[i] = maxPos;
	}
}



/**
	Busca todos los máximos locales de un intervalo y devuelve sus posiciones.
	Recorre todo el intervalo desde leftIndex hasta rightIndex, y para cada posicion de ese intervalo comprueba en otro intervalo de búsqueda si
	es un máximo. El tamaño del intervalo de busqueda se pasa en la variable searchIntervalSize.
	Devuelve un array con todos los picos encontrados. La primera posicion del array es el tamaño del array, y a continuación están los picos
*/
int * getAllLocalMaximums_interval(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize){
	int initialLeft = leftIndex;
	int leftInside;
	int rightInside;
	int isPeak = 0;	
	int *maximumPeaks_tmp = malloc(sizeof(int) * (rightIndex - leftIndex));
	int i;
	for(i = 0; i < (rightIndex - leftIndex); i++){
		maximumPeaks_tmp[i] = 0;
	}
	int peakCont = 0;

	//printf("\nAntes - leftIndex: %d  rightIndex:%d",leftIndex, rightIndex);
	while( leftIndex <= rightIndex){
		leftInside = ((leftIndex - searchIntervalSize / 2) < 0) ? 0 : (leftIndex - searchIntervalSize / 2);
		rightInside = ((leftIndex + searchIntervalSize / 2) > signalLength - 1 ) ? signalLength - 1: (leftIndex + searchIntervalSize / 2);
		//printf("\nleftIndex:%d leftInside:%d, rightInside:%d, rightIndex:%d",leftIndex, leftInside, rightInside, rightIndex);
		isPeak = 1; // de partida se supone que es un pico válido
		while(leftInside <= rightInside){
			if( leftInside == leftIndex ){
				leftInside++;
				continue;
			}
			if( signal[leftInside] > signal[leftIndex] ){
				isPeak = 0;
				break;
			}
			leftInside++;
		}
		if( isPeak ){
			//printf("\nValid peak. PeakPos:%d",leftIndex);
			maximumPeaks_tmp[peakCont++] = leftIndex;
		}
		leftIndex++;
	}

	//printf("\nDespues - leftIndex: %d  rightIndex:%d",leftIndex, rightIndex);

	// Si no encuentra ningún pico, devuelve el máximo del intervalo entre leftIndex y rightIndex
	if( peakCont == 0){
		maximumPeaks_tmp[peakCont++] = getArrayMaximum(signal, initialLeft, rightIndex);		
	//	printf("\nMaximum - Desesperada - Numero  de peaks:%d", peakCont);
	}
	//else{
	//	printf("\nMaximum - Numero  de peaks:%d", peakCont);	
	//}

	
	int *maximumPeaks_return = malloc(sizeof(int) * (peakCont + 1));
	maximumPeaks_return[0] = peakCont;
	for(i = 1; i < peakCont + 1; i++){
		maximumPeaks_return[i] = maximumPeaks_tmp[i - 1];
	}
	free(maximumPeaks_tmp);

	return(maximumPeaks_return);
}

/**
	Busca todos los mínimos locales de un intervalo y devuelve sus posiciones.
	Recorre todo el intervalo desde leftIndex hasta rightIndex, y para cada posicion de ese intervalo comprueba en otro intervalo de búsqueda si
	es un máximo. El tamaño del intervalo de busqueda se pasa en la variable searchIntervalSize.
	Devuelve un array con todos los picos encontrados. La primera posicion del array es el tamaño del array, y a continuación están los picos
*/
int * getAllLocalMinimums_interval(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize){
	int initialLeft = leftIndex;
	int leftInside;
	int rightInside;
	int isPeak = 0;	
	int *minimumPeaks_tmp = malloc(sizeof(int) * (rightIndex - leftIndex));
	int i;
	for(i = 0; i < (rightIndex - leftIndex); i++){
		minimumPeaks_tmp[i] = 0;
	}
	int peakCont = 0;

	//printf("\nAntes - leftIndex: %d  rightIndex:%d",leftIndex, rightIndex);

	while( leftIndex <= rightIndex){
		leftInside = ((leftIndex - searchIntervalSize / 2) < 0) ? 0 : (leftIndex - searchIntervalSize / 2);
		rightInside = ((leftIndex + searchIntervalSize / 2) > signalLength - 1 ) ? signalLength - 1: (leftIndex + searchIntervalSize / 2);		
		isPeak = 1; // de partida se supone que es un pico válido
		while(leftInside <= rightInside){
			if( leftInside == leftIndex ){
				leftInside++;
				continue;
			}
			if( signal[leftInside] < signal[leftIndex] ){
				isPeak = 0;
				break;
			}
			leftInside++;
		}
		if( isPeak ){
			//printf("\nValid peak. PeakPos:%d",leftIndex);
			minimumPeaks_tmp[peakCont++] = leftIndex;
		}
		leftIndex++;
	}

	//printf("\nDespues - leftIndex: %d  rightIndex:%d",leftIndex, rightIndex);
	// Si no encuentra ningún pico, devuelve el mínimo del intervalo entre leftIndex y rightIndex
	if( peakCont == 0){
		minimumPeaks_tmp[peakCont++] = getArrayMinimum(signal, initialLeft, rightIndex);		
	//	printf("\nMinimum - Desesperada -  Numero  de peaks:%d", peakCont);	
	}
	//else{
	//	printf("\nMinimum - Numero  de peaks:%d", peakCont);	
	//}

	
	int *minimumPeaks_return = malloc(sizeof(int) * (peakCont + 1));
	minimumPeaks_return[0] = peakCont;
	for(i = 1; i < peakCont + 1; i++){
		minimumPeaks_return[i] = minimumPeaks_tmp[i - 1];
	}
	free(minimumPeaks_tmp);

	return(minimumPeaks_return);
}


/**
	Comprueba si en un intervalo un mínimo (o máximo) no es el único. Si el mínimo (máximo) no es el único,
	selecciona el mínimo (máximo) más próximo al máximo (mínimo) de referencia.
	Si era el único devuelve el pico a evaluar que recibe
*/
int isMoreThanOnePeak(double *array, int leftIndex, int rightIndex, int peakToEval, int referencePeak){
	while( leftIndex <= rightIndex){
		// Si en el array existe otro pico (máximo o mínimo) de igual valor pero más cercano al pico de referencia, lo selecciona
		if(array[leftIndex] == array[peakToEval] && leftIndex != peakToEval && ( abs(leftIndex - referencePeak) < abs(peakToEval - referencePeak)  ) ){				
				peakToEval = leftIndex;
		}
		leftIndex++;
	}
	return peakToEval;
}
#include "commonFunctions.h"
#include <math.h>
#include <stdlib.h>



int areDifferentSign(double *signal, int peakPos1, int peakPos2){
	return ( (signal[peakPos1] > 0 && signal[peakPos2] < 0) ||
			 (signal[peakPos1] < 0 && signal[peakPos2] > 0) );
}

int applyCuiweiAlgorithm(double *signal, int peakPos1, int peakPos2, int referencePeak){
	double factor = 1.2;
	double a1 = fabs(signal[peakPos1]);
	double a2 = fabs(signal[peakPos2]);
	double l1 = fabs( referencePeak - peakPos1 );
	double	l2 = fabs( referencePeak - peakPos2 );		


	if( (a1 / l1) > (factor * a2 / l2) ){				
		return peakPos1;
	}
	else{
		if( (a2 / l2) > (factor * a1 / l1) ){
			return peakPos2;
		}
		else{			
			if( (peakPos1 < referencePeak && peakPos2 < referencePeak) || 				
				(peakPos1 > referencePeak && peakPos2 > referencePeak)   ){
				// Si están del mismo lado el mas lejano es el redundante
				return ( abs(referencePeak - peakPos1) < abs(referencePeak - peakPos2)) ? peakPos1 : peakPos2;
			}
			else{
				// Si están en distinto lado el que sigue a referencePeak es el redundante	
				return ( peakPos2 > referencePeak ) ? peakPos1 : peakPos2;
			}			
		}
	}
}



/**
	Recibe dos picos adyacentes al MML y elimina el redundante
*/
int compareTwoPeaksAndRemoveRedundant(double *signal, int peakPos1, int peakPos2, int referencePeak){

	double slope1 = fabs((signal[peakPos1] - signal[referencePeak]) / abs(peakPos1 - referencePeak));
	double slope2 = fabs((signal[peakPos2] - signal[referencePeak]) / abs(peakPos2 - referencePeak));

	// Si peakPos1 y peakPos2 son de signos opuestos, elimina el de mismo signo que referencePeak
	if( areDifferentSign(signal, peakPos1, peakPos2) ){
		if( signal[referencePeak] > 0 ){
			if(signal[peakPos1] > 0){
				// Devolver pico 2, a menos que su pendiente sea 1.2 veces mayor que la pendiente del pico 1
				if(slope1 > 1.2 * slope2){
					return peakPos1;
				}
				else{
					return peakPos2;
				}
			}
			else{
				// Devolver pico 1, a menos que su pendiente sea 1.2 veces mayor que la pendiente del pico 2
				if(slope2 > 1.2 * slope1){
					return peakPos2;
				}
				else{
					return peakPos1;
				}
			}
			//return (signal[peakPos1] > 0) ? peakPos2 : peakPos1;
		}
		else{
			if(signal[peakPos1] < 0){
				// Devolver pico 2, a menos que su pendiente sea 1.2 veces mayor que la pendiente del pico 1
				if(slope1 > 1.2 * slope2){
					return peakPos1;
				}
				else{
					return peakPos2;
				}
			}
			else{
				// Devolver pico 1, a menos que su pendiente sea 1.2 veces mayor que la pendiente del pico 2
				if(slope2 > 1.2 * slope1){
					return peakPos2;
				}
				else{
					return peakPos1;
				}
			}
			//return (signal[peakPos1] < 0) ? peakPos2 : peakPos1;
		}
	}
	else{ // Si son del mismo signo, aplica el algoritmo de Cuiwei Li et al.
		return applyCuiweiAlgorithm(signal, peakPos1, peakPos2, referencePeak);
	}
}





/**
	Recibe un pico y un array de picos que son posibles adyacentes para formar un MML.
	De todos los picos adyacentes selecciona uno aplicando el algoritmo de C. Li
*/
int selectCorrectAdjacentPeakFromArray(double *signal, int *possibleAdjacentPeaks, int referencePeak){
	int validPeakPosition = 0;
	int numPeaks = possibleAdjacentPeaks[0]; // numero de picos
	int possiblePeaks[numPeaks];

	int i;
	for( i = 0; i < numPeaks; i++ ){
		possiblePeaks[i] = possibleAdjacentPeaks[i + 1];
	}
	i = 0;
	validPeakPosition = possiblePeaks[i++];
	while(i < numPeaks){
		validPeakPosition = compareTwoPeaksAndRemoveRedundant(signal, validPeakPosition, possiblePeaks[i], referencePeak);
		i++;
	}	
	return(validPeakPosition);
}



/*
	Comprueba para cada mínimo todos los máximos y para cada máximo todos los mínimos
*/
/*void checkAllPossibleMML(double *signal, int * arrayLocalMinimums, int *arrayLocalMaximums){
	// Recorrer todos los mínimos comprobando para cada mínimo todos los máximos


	// Recorrer todos los máximos comprobando para cada máximo todos los mínimos
}

int * getMML_interval_prueba(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize){

	int * arrayLocalMinimums = getAllLocalMinimums_interval(signal, signalLength, leftIndex, rightIndex, searchIntervalSize);
	int * arrayLocalMaximums = getAllLocalMaximums_interval(signal, signalLength, leftIndex, rightIndex, searchIntervalSize);

	// Buscar el máximo y mínimo del array
	int minPos = getArrayMinimum(signal, leftIndex, rightIndex);
	int maxPos = getArrayMaximum(signal, leftIndex, rightIndex);

	minPos = isMoreThanOnePeak(signal, leftIndex, rightIndex, minPos, maxPos);
	maxPos = isMoreThanOnePeak(signal, leftIndex, rightIndex, maxPos, minPos);

	// Si hay más de un mínimo o más de un máximo con el mismo valor, selecciona el más próximo al otro mínimo o máximo


	// Buscar para ese máximo y mínimo el pico adyacente para obtener un MML
	int adjacentMax = selectCorrectAdjacentPeakFromArray(signal, arrayLocalMaximums, minPos);
	int adjacentMin = selectCorrectAdjacentPeakFromArray(signal, arrayLocalMinimums, maxPos);
	// De los 2 MML seleccionar el correcto
	int* mmlPositions = selectCorrectMML(signal, maxPos, adjacentMin, minPos, adjacentMax);

	free(arrayLocalMinimums);
	free(arrayLocalMaximums);
	return(mmlPositions);
}*/


/**
	Recibe 2 máximos y 2 mínimos. Son 2 posibles MML: el max1 con el min1 y el max2 con el min2
	Se pueden dar 4 casos: 
		- Que los 2 picos max1 y min1 coincidan con max2 y min2
		- Que el mínimo coincida y los máximos sean diferentes
		- Que el máximo coincida y los mínimos sean diferentes
		- Que sean 2 MML distintos
	De entre todos los posibles casos tiene que seleccionar 1 solo MML como correcto
*/
int * selectCorrectMML(double *signal, int maxPos1, int minPos1, int minPos2, int maxPos2){
	int *toret = malloc(sizeof(int) * 2);

	if( minPos1 == minPos2 && maxPos1 == maxPos2 ){
		toret[0] = maxPos1;
		toret[1] = minPos1;
	}
	if( minPos1 == minPos2 && maxPos1 != maxPos2 ){
		toret[0] = compareTwoPeaksAndRemoveRedundant(signal, maxPos1, maxPos2, minPos1);
		toret[1] = minPos1;
	}
	if( minPos1 != minPos2 && maxPos1 == maxPos2 ){
		toret[0] = maxPos1;
		toret[1] = compareTwoPeaksAndRemoveRedundant(signal, minPos1, minPos2, maxPos1);
	}
	if( minPos1 != minPos2 && maxPos1 != maxPos2 ){		
		// Selecciona el ultimo MML
		if( maxPos1 > maxPos2 ){
			toret[0] = maxPos1;
			toret[1] = minPos1;
		}
		else{
			toret[0] = maxPos2;
			toret[1] = minPos2;
		}
		/*
		double slope1 = fabs((signal[maxPos1] - signal[minPos1]) / abs(maxPos1 - minPos1));
		double slope2 = fabs((signal[maxPos2] - signal[minPos2]) / abs(maxPos2 - minPos2));
		//printf("\n ----Condicion 4 selectCorrectMML  minPos1:%d [%f] maxPos1:%d [%f] minPos2:%d [%f]  maxPos2:%d [%f] slope1:%f slope2:%f", minPos1, signal[minPos1],
		//			maxPos1, signal[maxPos1],  minPos2, signal[minPos2],  maxPos2, signal[maxPos2], slope1, slope2);

		// Si la pendiente de uno de los MML es 1.8 veces mayor que la del otro lo selecciona. Sino selecciona el más próximo al pico R, que 
		// es el que sus posiciones son mayores
		if(slope1 >= 1.8 * slope2){
			toret[0] = maxPos1;
			toret[1] = minPos1;
		}
		else{
			if( slope2 >= 1.8 * slope1){
				toret[0] = maxPos2;
				toret[1] = minPos2;	
			}
			else{
				// Sino selecciona el ultimo MML
				if( maxPos1 > maxPos2 ){
					toret[0] = maxPos1;
					toret[1] = minPos1;
				}
				else{
					toret[0] = maxPos2;
					toret[1] = minPos2;
				}
				
			}
			
		}*/
	}
	return toret;
}


/**
	Busca todos los posibles MML que existen en la señal desde leftIndex hasta rightIndex. 
	Para buscar los MML, primero busca todos los máximos y mínimos locales.
	Después busca combinaciones de esos máximos con los mínimos, para crear MML
	Finalmente, selecciona como válidos aquellos MML con la mayor pendiente
*/
int * getMML_interval(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize){

	int * arrayLocalMinimums = getAllLocalMinimums_interval(signal, signalLength, leftIndex, rightIndex, searchIntervalSize);
	int * arrayLocalMaximums = getAllLocalMaximums_interval(signal, signalLength, leftIndex, rightIndex, searchIntervalSize);

	// Buscar el máximo y mínimo del array
	int minPos = getArrayMinimum(signal, leftIndex, rightIndex);
	int maxPos = getArrayMaximum(signal, leftIndex, rightIndex);

	minPos = isMoreThanOnePeak(signal, leftIndex, rightIndex, minPos, maxPos);
	maxPos = isMoreThanOnePeak(signal, leftIndex, rightIndex, maxPos, minPos);

	// Si hay más de un mínimo o más de un máximo con el mismo valor, selecciona el más próximo al otro mínimo o máximo


	// Buscar para ese máximo y mínimo el pico adyacente para obtener un MML
	int adjacentMax = selectCorrectAdjacentPeakFromArray(signal, arrayLocalMaximums, minPos);
	int adjacentMin = selectCorrectAdjacentPeakFromArray(signal, arrayLocalMinimums, maxPos);
	// De los 2 MML seleccionar el correcto
	int* mmlPositions = selectCorrectMML(signal, maxPos, adjacentMin, minPos, adjacentMax);


	/*if( mmlPositions[0] < leftIndex || mmlPositions[1] < leftIndex || mmlPositions[1] > rightIndex || mmlPositions[1] > rightIndex ){
		printf("\nLimite superado -- minPos:%d maxPos:%d leftIndex:%d rightIndex:%d ", 
				mmlPositions[0], mmlPositions[1], leftIndex, rightIndex);
	}*/
	
	/*printf("\n\nleftIndex:%d rightIndex:%d, minPos:%d [%f], maxPos:%d [%f]", leftIndex,rightIndex, minPos, signal[minPos], maxPos, signal[maxPos]);
	printf("\nadjacentMax:%d [%f] adjacentMin:%d [%f]", adjacentMax, signal[adjacentMax], adjacentMin, signal[adjacentMin]);
	printf("\nMaximum: ");
	int i;
	for(i = 1; i <= arrayLocalMaximums[0]; i ++ ){
		printf(" %d [%f]", arrayLocalMaximums[i], signal[arrayLocalMaximums[i]]);
	}
	printf("\nMinimum: ");	
	for(i = 1; i <= arrayLocalMinimums[0]; i ++ ){
		printf(" %d [%f]", arrayLocalMinimums[i], signal[arrayLocalMinimums[i]]);
	}
	printf("\n MML final -- maximo:%d minimo:%d",mmlPositions[0], mmlPositions[1]);*/

	free(arrayLocalMinimums);
	free(arrayLocalMaximums);
	return(mmlPositions);
}





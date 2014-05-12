#include "commonFunctions.h"
#include <math.h>

/*
	Obtiene todos los maximum modulus lines de una escala wavelet.
	Recibe como parámetros la señal wavelet, su tamaño y todos los picos locales (pueden ser máximos o mínimos) detectados en esa escala. 
	Entonces recorre todos los picos buscando otro pico con signo opuesto en un intervalo. Si ese pico tiene otro pico con signo opuesto en un intervalo
	y además superan el umbral, entonces se considera un maximum modulus line y guarda su valor.
	El umbral se calcula utilizando las fórmulas del artículo de Martinez et. al, para un intervalo de "intervalSize" muestras
	El pico de signo opuesto se busca hacia delante y hacia atras y se descarta aquel que se considera redundante (segun el artículo de C. Li.)

*/ 
void getMaximumModulusLines(double *waveletSignal, int *signal_length, int *maximumModulusPositions, double *maximumModulusHeights, int *mMLength,
									int *intervalSize, int *resultModulusPosition, int *scaleToAnalyze, int *windowSize  ){
	
	int contModulus = 0;
	int i = 0;	
	int j = 0;
	int pos = 0;
	int cero = 0;
	//double * absHeights;
	double absHeights[*intervalSize];
	int leftIndex = 0;
	int rightIndex = *intervalSize;
	double *returnedPeak;
	double thrQRS;
	
	int peakPosition1; 
	int peakPosition2;
	int validPeakPosition;
	double a1;
	double a2;
	int l1;
	int l2;
	int leftArrayPos;
	int rigthArrayPos;

	//printf("\nscale: %d",*scaleToAnalyze);
	//int cond = 0;

	while( rightIndex < (*signal_length - 1)){
		// Calcular el Umbral
		pos = 0;		
		while( j < rightIndex){	
			absHeights[pos] = fabs(waveletSignal[j]);	
			pos++;
			j++;
		}
		pos--;
		if( *scaleToAnalyze == 4){
			thrQRS = 0.5 * getRMS(absHeights, &cero, &pos);	
		}
		else{
			thrQRS = getRMS(absHeights, &cero, &pos);	
		}	

		
		 // Recorre los picos máximos entre leftIndex y rightIndex. Para cada pico comprueba que sea un maximum modulus line
	  	while( maximumModulusPositions[i] < rightIndex && i < *mMLength ){
	  		// Solo se consideran los que superan el umbral
	  		if( fabs(waveletSignal[ maximumModulusPositions[i] ]) > thrQRS){	  	
	  			if( waveletSignal[ maximumModulusPositions[i] ] > 0){
	  				// Si es un máximo positivo, busca un minimo local antes y/o después
	  				leftArrayPos = ((maximumModulusPositions[i] - *windowSize) < 0) ? 0 : (maximumModulusPositions[i] - *windowSize);
	  				rigthArrayPos = maximumModulusPositions[i];
	  				peakPosition1 = getArrayMinimum(waveletSignal, leftArrayPos, rigthArrayPos); 
	  				leftArrayPos = maximumModulusPositions[i];
	  				rigthArrayPos = ((maximumModulusPositions[i] + *windowSize) > *signal_length) ? *signal_length : (maximumModulusPositions[i] + *windowSize);
	  				peakPosition2 = getArrayMinimum(waveletSignal, leftArrayPos, rigthArrayPos);
	  			}
	  			else{
	  				// Si es un mínimo negativo, busca un máximo local antes y/o después
	  				leftArrayPos = ((maximumModulusPositions[i] - *windowSize) < 0) ? 0 : (maximumModulusPositions[i] - *windowSize);
	  				rigthArrayPos = maximumModulusPositions[i];
	  				peakPosition1 = getArrayMaximum(waveletSignal, leftArrayPos, rigthArrayPos);
	  				leftArrayPos = maximumModulusPositions[i];
	  				rigthArrayPos = ((maximumModulusPositions[i] + *windowSize) > *signal_length) ? *signal_length : (maximumModulusPositions[i] + *windowSize);
	  				peakPosition2 = getArrayMaximum(waveletSignal, leftArrayPos, rigthArrayPos);
	  			}

	  			a1 = fabs(waveletSignal[peakPosition1]);
				a2 = fabs(waveletSignal[peakPosition2]);
				validPeakPosition = (a1 >= a2) ? peakPosition1 : peakPosition2;
				

	  			// Eliminar mínimo o máximo redundante
	  			// Aplica las 3 reglas del articulo de Cuiwei Li et al.
				/*l1 = abs( maximumModulusPositions[i] - peakPosition1 );
				l2 = abs( maximumModulusPositions[i] - peakPosition2 );				
				if( (a1 / l1) > (1.2 * a2 / l2) ){				
					validPeakPosition = peakPosition1;
					cond = 1;
				}
				else{
					if( (a2 / l2) > (1.2 * a1 / l1) ){
						validPeakPosition = peakPosition2;
						cond = 2;
					}
					else{	
						//validPeakPosition = (l1 <= l2) ? peakPosition1 : peakPosition2;	
						//validPeakPosition = (a1 >= a2) ? peakPosition1 : peakPosition2;						
						cond = 3;
						if( waveletSignal[ maximumModulusPositions[i] ] > 0 ){
							// Si era un máximo se queda con el siguiente pico
							validPeakPosition = peakPosition2;
						}
						else{
							// Si era un mínimo se queda con el pico anterior
							validPeakPosition = peakPosition1; 
						}
						
					}
				}*/

				/*if( i < 10 ){
					printf("\npeak: %d peak1: %d peak2: %d Validpeak: %d, cond: %d", maximumModulusPositions[i], peakPosition1, peakPosition2, validPeakPosition,cond);
					printf("\na1: %f l1: %d  a2: %f l2: %d", a1, l1, a2, l2);
				}*/

				if( fabs(waveletSignal[validPeakPosition]) > thrQRS ){
					// Si es mayor que el umbral es un MML válido y se guarda
					resultModulusPosition[contModulus] = maximumModulusPositions[i];
	  				contModulus++;
				}
	  		}
	  		i++;
	  	}

		// Update indexes
		if( rightIndex == *signal_length - 2 ){
			break;
		}
		leftIndex = rightIndex;
		rightIndex = rightIndex + *intervalSize;
		if( rightIndex >= *signal_length - 1 ){
			rightIndex = *signal_length - 2;
		}
	}
}



/*
	Recibe la posicion del maximum modulus line de la escala previa y busca en esta escala un maximum modulus line dentro del vecindario y devuelve
	el índice del array donde está ese MML. En caso de que no encuentre busca hasta el final del array o hasta que el siguiente MML es mayor que
	el MML de la escala previa + el intervalo de búsqueda.
*/
int searchModulusAcrossScales(int previousScaleModulusPos, int intervalSize, int index, int *waveletScale, int waveletScaleLength){
	int newModulusPos = waveletScale[index];
	while( newModulusPos < (previousScaleModulusPos + intervalSize) && index < (waveletScaleLength -1) ){
		if( abs(previousScaleModulusPos - newModulusPos) < intervalSize){
			break;
		}
		else{
			newModulusPos = waveletScale[++index];
		}
	}
	return index;
}

/*
	Recibe como parámetro las posiciones de los MML de las 4 escalas y comprueba cuales coinciden en las escalas segun el artículo de Boichat
	Recorre todos los MML de la escala 4 y para cada uno comprueba si existe un MML en la escala 3, luego en la 2 y luego en la 1. En caso
	de que coincidiesen en todas las escalas considera válido el MML.	
*/
void getFinalMaximumModulusLines_boichat(int *modulusPositionsScale1, int *modulusPositionsScale2, int *modulusPositionsScale3, int *modulusPositionsScale4,
										 int *numModulusAtScale1, int *numModulusAtScale2, int *numModulusAtScale3, int *numModulusAtScale4,
										 int *resultModulusPosition_return, double *rrMean ){
	int intervalSize = 20; // numero de muestras de posible diferencia entre un MML entre escalas
	double rr = *rrMean;
	int modulusScale4, modulusScale3, modulusScale2, modulusScale1;
	int indexReturn = 0;
	int indexScale4 = 0;
	int indexScale3 = 0;
	int indexScale2 = 0;
	int indexScale1 = 0;
	int isDetectedModulus = 0;
	int lastModulusPos = 0;
	
	while(indexScale4 < (*numModulusAtScale4 - 1)){
		isDetectedModulus = 0;
		modulusScale4 = modulusPositionsScale4[indexScale4++];
		indexScale3 = searchModulusAcrossScales(modulusScale4, intervalSize, indexScale3, modulusPositionsScale3, *numModulusAtScale3);
		modulusScale3 = modulusPositionsScale3[indexScale3];		
		if( abs(modulusScale4 - modulusScale3) < intervalSize ){
			indexScale2 = searchModulusAcrossScales(modulusScale3, intervalSize, indexScale2, modulusPositionsScale2, *numModulusAtScale2);
			modulusScale2 = modulusPositionsScale2[indexScale2];	
			/*if( indexScale4 < 10 ){
						printf("\nmScale4: %d, mScale3: %d, mScale2: %d", modulusScale4, modulusScale3, modulusScale2);
			}*/		
			if( abs(modulusScale3 - modulusScale2) < intervalSize){				
				indexScale1 = searchModulusAcrossScales(modulusScale2, intervalSize, indexScale1, modulusPositionsScale1, *numModulusAtScale1);
				modulusScale1 = modulusPositionsScale1[indexScale1];
				if( abs(modulusScale2 - modulusScale1) < intervalSize){
					resultModulusPosition_return[indexReturn++] = modulusScale1;
					lastModulusPos = modulusScale1;
					isDetectedModulus = 1;
				}
			}
			if( (1.5 * rr ) < (modulusScale3 - lastModulusPos) ){
				// Si no detecta un pico en 1.5 veces el intervalo rr
				// Entonces buscar un MML que coincida en la 3 y 4, entre lastModulusPos y modulusScale3
			}
			if(!isDetectedModulus){

			}
		}
	}

}







/*
	Recibe como parámetro las 4 posiciones del MML actual en cada una de las escalas y devuelve la menor
*/
int getSmallerPosition( int pos1, int pos2, int pos3, int pos4){
	if( pos1 < pos2 && pos1 < pos3 && pos1 < pos4){ // m1 smaller
		return 1;
	}
	else{
		if( pos2 < pos3 && pos2 < pos4){
			return 2;
		}
		else{
			if( pos3 < pos4){
				return 3;
			}
			else{
				return 4;
			}
		}
	}
}


/**
	Toma como posiciones de referencia los maximum modulus lines de cada una de las 4 escalas y considera como válidos aquellos que coinciden en por lo menos 3 escalas
	Esta función recorre los arrays de las posiciones de los MML de cada una de las 4 escalas buscando cuando coinciden, cuando coinciden guarda como resultado
	la posición en la escala 1, o en la escala 2 cuando coinciden en todas las escalas menos la 1
*/
void getFinalMaximumModulusLines(int *modulusPositionsScale1, int *modulusPositionsScale2, int *modulusPositionsScale3, int *modulusPositionsScale4,
								 int *numModulusAtScale1, int *numModulusAtScale2, int *numModulusAtScale3, int *numModulusAtScale4,
								 int *resultModulusPosition_return){	
	int currentModulusPosition;
	int pos1, pos2, pos3, pos4;
	int indexScale4 = 0;
	int indexScale3 = 0;
	int indexScale2 = 0;
	int indexScale1 = 0;
	int sizeNeigh = 20;
	int isValidModulus;
	int resultCont = 0;
	int comp1, comp2, comp3, comp4, comp5, comp6;

	while( indexScale4 < (*numModulusAtScale4 - 1 ) || 
		   indexScale3 < (*numModulusAtScale3 - 1 ) ||
		   indexScale2 < (*numModulusAtScale2 - 1 ) ||
		   indexScale1 < (*numModulusAtScale1 - 1 )    ){

		pos1 = modulusPositionsScale1[indexScale1];
		pos2 = modulusPositionsScale2[indexScale2];
		pos3 = modulusPositionsScale3[indexScale3];
		pos4 = modulusPositionsScale4[indexScale4];

		//printf("\nPos1: %d Pos2: %d Pos3: %d, Pos4: %d", pos1, pos2, pos3, pos4);

		comp1 = abs(pos1 - pos2);
		comp2 = abs(pos1 - pos3);
		comp3 = abs(pos1 - pos4);
		comp4 = abs(pos2 - pos3);
		comp5 = abs(pos2 - pos4);
		comp6 = abs(pos3 - pos4);

		// Si coincide en las 4 escalas
		if( comp1 < sizeNeigh && comp2 < sizeNeigh && comp3 < sizeNeigh && comp4 < sizeNeigh && comp5 < sizeNeigh && comp6 < sizeNeigh ){
			resultModulusPosition_return[resultCont++] = pos1;		
			indexScale1++;
			indexScale2++;
			indexScale3++;	
			indexScale4++;
		}
		else{
			// Si coincide en 3 escalas
			if( comp1 < sizeNeigh && comp2 < sizeNeigh && comp4 < sizeNeigh ){
				resultModulusPosition_return[resultCont++] = pos1;
				indexScale1++;
				indexScale2++;
				indexScale3++;
			}
			else{
				if( comp1 < sizeNeigh && comp3 < sizeNeigh && comp5 < sizeNeigh ){
					resultModulusPosition_return[resultCont++] = pos1;		
					indexScale1++;
					indexScale2++;
					indexScale4++;
				}
				else{
					if( comp2 < sizeNeigh && comp3 < sizeNeigh && comp6 < sizeNeigh ){
						resultModulusPosition_return[resultCont++] = pos1;		
						indexScale1++;
						indexScale3++;
						indexScale4++;
					}
					else{
						if( comp4 < sizeNeigh && comp5 < sizeNeigh && comp6 < sizeNeigh ){
							resultModulusPosition_return[resultCont++] = pos2;	
							indexScale2++;
							indexScale3++;
							indexScale4++;	
						}
						else{
							//printf("\n\n Aumentar menor \n \n");
							// aumentar el índice de la escala con posición menor
							switch( getSmallerPosition(pos1, pos2, pos3, pos4) ){
								case 1:
									indexScale1++;
									break;
								case 2:
									indexScale2++;
									break;
								case 3:
									indexScale3++;
									break;
								case 4:
									indexScale4++;
									break;
							}
						}
					}
				}
			}
		}

		
	}
		

}


/*
	Recorre las posiciones finales de los MML y coloca la marca fiducial en el cruce por cero.
	Como en el MML solo se guarda un pico busca el otro pico correspondiente para situar la marca fiducial en el cruce por cero.

*/
void getZeroCrossing(double *waveletSignal, int *signal_length, int *maximumModulusPositions, int *modulusPosLength, int *windowSize,
					 int *resultAnnotPos){
	int peakPosition1; 
	int peakPosition2;
	int validPeakPosition;
	double a1;
	double a2;
	int l1;
	int l2;
	int leftArrayPos;
	int rigthArrayPos;
	int i;
	for( i = 0; i < *modulusPosLength; i++ ){
		// Para cada modulus line, si es un mínimo busca el correspondiente máximo y viceversa
		if( waveletSignal[ maximumModulusPositions[i] ] > 0.0){
			leftArrayPos = ((maximumModulusPositions[i] - *windowSize) < 0) ? 0 : (maximumModulusPositions[i] - *windowSize);
			rigthArrayPos = maximumModulusPositions[i];
			peakPosition1 = getArrayMinimum(waveletSignal, leftArrayPos, rigthArrayPos); 
			leftArrayPos = maximumModulusPositions[i];
			rigthArrayPos = ((maximumModulusPositions[i] + *windowSize) > *signal_length) ? *signal_length : (maximumModulusPositions[i] + *windowSize);
			peakPosition2 = getArrayMinimum(waveletSignal, leftArrayPos, rigthArrayPos);
		}
		else{			
			leftArrayPos = ((maximumModulusPositions[i] - *windowSize) < 0) ? 0 : (maximumModulusPositions[i] - *windowSize);
			rigthArrayPos = maximumModulusPositions[i];
			peakPosition1 = getArrayMaximum(waveletSignal, leftArrayPos, rigthArrayPos);
			leftArrayPos = maximumModulusPositions[i];
			rigthArrayPos = ((maximumModulusPositions[i] + *windowSize) > *signal_length) ? *signal_length : (maximumModulusPositions[i] + *windowSize);
			peakPosition2 = getArrayMaximum(waveletSignal, leftArrayPos, rigthArrayPos);
		}

		a1 = fabs(waveletSignal[peakPosition1]);
		a2 = fabs(waveletSignal[peakPosition2]);
		
		validPeakPosition = (a1 >= a2) ? peakPosition1 : peakPosition2;

		// Eliminar mínimo o máximo redundante
		// Aplica las 3 reglas del articulo de Cuiwei Li et al.
		/*l1 = abs( maximumModulusPositions[i] - peakPosition1 );
		l2 = abs( maximumModulusPositions[i] - peakPosition2 );		
		if( (a1 / l1) > (1.2 * a2 / l2) ){				
			validPeakPosition = peakPosition1;
		}
		else{
			if( (a2 / l2) > (1.2 * a1 / l1) ){
				validPeakPosition = peakPosition2;
			}
			else{	
				//validPeakPosition = (l1 <= l2) ? peakPosition1 : peakPosition2;	
				//validPeakPosition = (a1 >= a2) ? peakPosition1 : peakPosition2;								
				if( waveletSignal[ maximumModulusPositions[i] ] > 0 ){
					// Si era un máximo se queda con el siguiente pico
					validPeakPosition = peakPosition2;
				}
				else{
					// Si era un mínimo se queda con el pico anterior
					validPeakPosition = peakPosition1; 
				}
			}
		}*/

		// Colocar el punto en el cruce por cero
		if( validPeakPosition < maximumModulusPositions[i] ){
			resultAnnotPos[i] = getMinimumInAbsArray(waveletSignal, validPeakPosition, maximumModulusPositions[i]);
		}
		else{
			resultAnnotPos[i] = getMinimumInAbsArray(waveletSignal, maximumModulusPositions[i], validPeakPosition);
		}
	}
}


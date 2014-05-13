#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include <Rmath.h> 



/*
	Saves in an array the end point of the first N segments
*/
void getFirstSegments(int *firstSegments, int *segmentSize, int *numSegments){
	int i = 0;		
	for( i = 0; i < *numSegments; i++){
		firstSegments[i] = *segmentSize * (i+1); 
	}
}








/*int getMaxPosition(double *array, int *leftIndex, int *rightIndex, double *thr, int *maxPositions, double *maxHeights, int *index){

	int i = *leftIndex;
	double maximum = 0.0;
	int maximum_pos;
	int maxPositions_index = *index;

	double abs_array = 0.0;
	

	while(i < *rightIndex){
		//printf("\nArray %d: %f",i,array[i]);

		// Pasar cada valor a abs
		if(array[i] < 0){
			abs_array = array[i] * -1;
		}
		else{
			abs_array = array[i];
		}


		if( abs_array > *thr ){
			while( abs_array > *thr ){
				if( abs_array > maximum){
					maximum_pos = i;
					maximum = abs_array;
				}
				i++;
				// Pasar cada valor a abs
				if(array[i] < 0){
					abs_array = array[i] * -1;
				}
				else{
					abs_array = array[i];
				}
			}
			//printf("\nMaximo %d: %d", maxPositions_index, maximum_pos);
			maxPositions[maxPositions_index] = maximum_pos;
			maxHeights[maxPositions_index] = maximum;
			maximum = 0.0;
			maxPositions_index++;
		}
		else{
			i++;	
		}

	}

	return maxPositions_index;

}*/


int getMaxPosition(double *array, int *leftIndex, int *rightIndex, double *thr, int *maxPositions, double *maxHeights, int *index){

	int i = *leftIndex;
	double maximum = 0.0;
	int maximum_pos;
	int maxPositions_index = *index;

	double abs_array = 0.0;

	
	

	while(i < *rightIndex){
		//printf("\nArray %d: %f",i,array[i]);

		// Pasar cada valor a abs
		if(array[i] < 0){
			abs_array = array[i] * -1;
		}
		else{
			abs_array = array[i];
		}


		if( abs_array > *thr ){
			while( abs_array > *thr ){
				if( abs_array > maximum){
					maximum_pos = i;
					maximum = abs_array;
				}
				i++;
				// Pasar cada valor a abs
				if(array[i] < 0){
					abs_array = array[i] * -1;
				}
				else{
					abs_array = array[i];
				}
			}
			//printf("\nMaximo %d: %d", maxPositions_index, maximum_pos);
			maxPositions[maxPositions_index] = maximum_pos;
			maxHeights[maxPositions_index] = maximum;
			maximum = 0.0;
			maxPositions_index++;
		}
		else{
			i++;	
		}

	}

	return maxPositions_index;

}





/*
Recibe como parámetro el módulo actual de cada una de las 4 escalas y busca y devuelve el menor. 
*/
int getSmaller( int *m1, int *m2, int *m3, int *m4){

	if( *m1 < *m2 && *m1 < *m3 && *m1 < *m4){ // m1 smaller
		return 1;
	}
	else{
		if( *m2 < *m3 && *m2 < *m4){
			return 2;
		}
		else{
			if( *m3 < *m4){
				return 3;
			}
			else{
				return 4;
			}
		}
	}

}

/*
Esta funcion recibe como parámetros un valor de cada una de las cuatro escalas y si el valor coincide en todas se considera válido.
Cuando un valor coincide en las 4 escalas, teniendo en cuenta un posible desfase de tamaño windowSize, entonces se considera como un módulo válido
en otro caso, ese módulo se descarta.
*/
/*int isValid_old( int *m1, int *m2, int *m3, int *m4, int *windowSize){

	//printf("\nInside ---M1 %d \n M2 %d \n M3 %d \n M4 %d \n windowSize %d\n", *m1, *m2, *m3, *m4, *windowSize);

	
	int comp1 = *m1 - *m2;
	int comp2 = *m1 - *m3;
	int comp3 = *m1 - *m4;
	int comp4 = *m2 - *m3;
	int comp5 = *m2 - *m4;
	int comp6 = *m3 - *m4;


	// Si un pico está presente en las 4 escalas es válido
	if( abs(comp1) <= *windowSize && abs(comp2) <= *windowSize && abs(comp3) <= *windowSize
		&& abs(comp4) <= *windowSize && abs(comp5) <= *windowSize && abs(comp6) <= *windowSize)
		return 1;

	// Si solo está presente en 3 de las 4 escalas

		// Si no está en la escala 1 pero si en las otras 3
	if( abs(comp4) <= *windowSize && abs(comp5) <= *windowSize && abs(comp6) <= *windowSize)
		return 1;

		// Si no está en la escala 2
	if( abs(comp2) <= *windowSize && abs(comp3) <= *windowSize && abs(comp6) <= *windowSize)
		return 1;

		// Si no está en la escala 3
	if( abs(comp1) <= *windowSize && abs(comp3) <= *windowSize && abs(comp5) <= *windowSize)
		return 1;

		// Si no está en la escala 4
	if( abs(comp1) <= *windowSize && abs(comp2) <= *windowSize && abs(comp4) <= *windowSize)
		return 1;

  	return 0;
}*/


int isValid( int *m1, int *m2, int *m3, int *m4, int *windowSize){

	//printf("\nInside ---M1 %d \n M2 %d \n M3 %d \n M4 %d \n windowSize %d\n", *m1, *m2, *m3, *m4, *windowSize);

	
	
	int comp = *m1 - *m2;
  	
	if( abs(comp) > *windowSize ){
		return 0;
	}

  	comp = *m1 - *m3;
  	if( abs(comp) > *windowSize){
  			return 0;
	}

  	comp = *m1 - *m4;
  	if( abs(comp) > *windowSize){
  			return 0;
	}
  	

  	comp = *m2 - *m3;
  	if( abs(comp) > *windowSize){
  			return 0;
	}

  	comp = *m2 - *m4;
  	if( abs(comp) > *windowSize){
  			return 0;
	}

  	comp = *m3 - *m4;
  	if( abs(comp) > *windowSize){
  			return 0;
	}

  	return 1;
}

/*
Compara los módulos de las cuatro escalas y descarta aquellos valores que no están presentes en las 4 escalas a la vez.
Va buscando la posición menor de cada escala para avanzar esa posición y mantener las otras, así hasta recorrer todas las posiciones y finalizar.


*/
void compareModulusScales(int *m1, double *m1Hei, int *m2, int *m3, int *m4,
						  int *m1_length, int *m2_length, int *m3_length, int *m4_length,
						  int *modulusPos, double *modulusHei, int *samplingFreq){
   
	//int i = 0;
	int m1_index = 0;
	int m2_index = 0;
	int m3_index = 0;
	int m4_index = 0;
	int modulus_index = 0;
	int smaller_v = 0;

	int refractoryPeriod200 = 200 * *samplingFreq / 1000;
	int errorWindowSize = 0.1 * *samplingFreq; // 100 ms 

   
	while( ( *m1_length - 1)  > m1_index 
			&& ( *m2_length - 1) > m2_index 
			&& ( *m3_length - 1 ) > m3_index
			&& ( *m4_length - 1) > m4_index  ){

		// Busca el menor elemento y avanza una posicion ese array
		smaller_v = getSmaller( &m1[m1_index], &m2[m2_index], &m3[m3_index], &m4[m4_index]);
		//printf("\nM1 %d \n M2 %d \n M3 %d \n M4 %d \n Smaller %d", m1[m1_index], m2[m2_index], m3[m3_index], m4[m4_index], smaller_v);

 		if( isValid(&m1[m1_index], &m2[m2_index], &m3[m3_index], &m4[m4_index], &errorWindowSize)    ){   // Si es un numero correcto se guarda su módulo y se avanzan todos los índices
 			//&& ( (m1_index > 1 && (modulusPos[modulus_index - 1] + *refractoryPeriod200) < m1[m1_index]) || m1_index == 0 || m1_index == 1)  ){ // Permite solo picos que superan el refractory blanking de 200 ms
 			//&& ( (m2_index > 0 && (modulusPos[modulus_index - 1] + *refractoryPeriod200) < m2[m2_index]) || m2_index == 0)  ){ 
	 			modulusPos[modulus_index] = m1[m1_index];
	 			modulusHei[modulus_index] = m1Hei[m1_index];
 				//modulusPos[modulus_index] = m2[m2_index];
 			    //modulusHei[modulus_index] = m1Hei[m2_index];
	 			m1_index++;
	 			m2_index++;
	 			m3_index++;
	 			m4_index++;
	 			modulus_index++;
 		}
 		else{ // en caso contrario solo se avanza el menor valor para seguir comprobando después
 			switch(smaller_v){ 
 				case 1:
 					m1_index++;
 					break;
 				case 2:
 					m2_index++;
 					break;
 				case 3:
 					m3_index++;
 					break;
 				case 4:
 					m4_index++;
 					break;
 			}
 		}

	}
}

/*

Un pico considerase válido cando está presente en 3 ou 4 detectores dentro da ventana de error windowSize


Se sólo está presente en 2 detectores, considerarase válido cando a distancia RR sea a correcta e nos outros 2 detectores os picos actuales sean 
equivalentes os picos siguientes de estes 2 detectores.
E se solo está presente en 1 detector, considerarase válido se a distancia RR é a correcta e o pico actual en cada un dos 3 detectores é equivalente
o siguiente de este pico

Devuelve 0 si no coinciden los picos o la posición del pico 
*/
int isValidPeak_old( int *p1, int *p2, int *p3, int *p4, int *windowSize,
				 int *p1_next, int *p2_next, int *p3_next, int *p4_next,
				 int *lastPeak, double *RRMedian){

	// Pico presente en los 4 detectores

	int comp1 = *p1 - *p2;
	int comp2 = *p1 - *p3;
	int comp3 = *p1 - *p4;
	int comp4 = *p2 - *p3;
	int comp5 = *p2 - *p4;
	int comp6 = *p3 - *p4;
	int currentRRDistance = 0;
	int compActSig1 = 0;
	int compActSig2 = 0;
	int compActSig3 = 0;
	int avgPeak = 0;
	

    // Si el pico coincide en 4 detectores
    avgPeak = (int)((*p1 + *p2 + *p3 + *p4) / 4);
	if( abs(comp1) <= *windowSize && abs(comp2) <= *windowSize && abs(comp3) <= *windowSize
	    && abs(comp4) <= *windowSize && abs(comp5) <= *windowSize && abs(comp6) <= *windowSize ) 
		return avgPeak;
		

	// Si el pico coincide en 3 detectores
	avgPeak = (int)((*p1 + *p2 + *p3) / 3);
	if( ( abs(comp1) <= *windowSize && abs(comp2) <= *windowSize) && abs(comp4) <= *windowSize )
		return avgPeak;
		
	avgPeak = (int)((*p1 + *p2 + *p4) / 3);
	if( ( abs(comp1) <= *windowSize && abs(comp3) <= *windowSize) && abs(comp5) <= *windowSize )
		return avgPeak;
		
	avgPeak = (int)((*p1 + *p3 + *p4) / 3);
	if( ( abs(comp2) <= *windowSize && abs(comp3) <= *windowSize) && abs(comp6) <= *windowSize ) 
		return avgPeak;
		
	avgPeak = (int)((*p2 + *p3 + *p4) / 3);
	if( ( abs(comp4) <= *windowSize && abs(comp5) <= *windowSize) && abs(comp6) <= *windowSize ) 
		return avgPeak;


	//Si un pico coincide en 2 detectores y la distancia RR es mayor que 1,5 * RRMedian
	avgPeak = (int)((*p1 + *p2) / 2);
    currentRRDistance = avgPeak - *lastPeak;
    //compActSig1 = *p3 - (int)((*p1_next + *p2_next) / 2);
    //compActSig2 = *p4 - (int)((*p1_next + *p2_next) / 2);
	if( abs(comp1) <= *windowSize && currentRRDistance <= (1.5 * *RRMedian) )
		//&& abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize )
		return avgPeak;

	



	avgPeak = (int)((*p1 + *p3) / 2);
	currentRRDistance = avgPeak - *lastPeak;
	//compActSig1 = *p2 - (int)((*p1_next + *p3_next) / 2 );
	//compActSig2 = *p4 - (int)((*p1_next + *p3_next) / 2 );
	if( abs(comp2) <= *windowSize && currentRRDistance <= (1.5 * *RRMedian) )
		//&& abs(compActSig1) <= *windowSize  && abs(compActSig2) <= *windowSize )
		return avgPeak;



 	avgPeak = (int)((*p1 + *p4) / 2);
	currentRRDistance = avgPeak - *lastPeak;
	//compActSig1 = *p2 - (int)((*p1_next + *p4_next) / 2);
	//compActSig2 = *p4 - (int)((*p1_next + *p4_next) / 2);
	if( (abs(comp3)) <= *windowSize && currentRRDistance <= (1.5 * *RRMedian) )
		//&& abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize )
		return avgPeak;

	avgPeak = (int)((*p2 + *p3) / 2);
	currentRRDistance = avgPeak - *lastPeak;
	//compActSig1 = *p1 - (int)((*p2_next + *p3_next) / 2);
	//compActSig2 = *p4 - (int)((*p2_next + *p3_next) / 2);
	if( abs(comp4) <= *windowSize && currentRRDistance <= (1.5 * *RRMedian) )
		//&& abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize )
		return avgPeak;

	avgPeak = (int)((*p2 + *p4) / 2);
	currentRRDistance = avgPeak - *lastPeak;
	//compActSig1 = *p1 - (int)((*p2_next + *p4_next) / 2);
	//compActSig2 = *p3 - (int)((*p2_next + *p4_next) / 2);
	if( abs(comp5) <= *windowSize && currentRRDistance <= (1.5 * *RRMedian) )
		//&& abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize )
		return avgPeak;
		
	avgPeak = (int)((*p3 + *p4) / 2);
	currentRRDistance = avgPeak - *lastPeak;
	//compActSig1 = *p1 - (int)((*p3_next + *p4_next) / 2);
	//compActSig2 = *p2 - (int)((*p3_next + *p4_next) / 2);
	if( abs(comp6) <= *windowSize && currentRRDistance <= (1.5 * *RRMedian) )
		//&& abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize )
		return avgPeak;


	// Si 1 pico no coincide con los otros detectores, pero la distancia RR es correcta. Y el siguiente pico en los otros detectores coincide con el 
	// siguiente pico de este detector, se considera correcto
	

	//compActSig1 = *p1 - *p2_next;
	//compActSig2 = *p1 - *p3_next;
	//compActSig3 = *p1 - *p4_next;
	currentRRDistance = *p1 - *lastPeak;
	/*if( abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize && abs(compActSig3) <= *windowSize
		&& currentRRDistance <= (1.5 * *RRMedian) )
		return *p1;*/

	if(currentRRDistance <= (1.5 * *RRMedian))
		return *p1;


	//compActSig1 = *p2 - *p1_next;
	//compActSig2 = *p2 - *p3_next;
	//compActSig3 = *p2 - *p4_next;
	currentRRDistance = *p2 - *lastPeak;
	/*if( abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize && abs(compActSig3) <= *windowSize
		&& currentRRDistance <= (1.5 * *RRMedian) )
		return *p2;*/

	if(currentRRDistance <= (1.5 * *RRMedian))
		return *p2;


	//compActSig1 = *p3 - *p1_next;
	//compActSig2 = *p3 - *p2_next;
	//compActSig3 = *p3 - *p4_next;
	currentRRDistance = *p3 - *lastPeak;
	/*if( abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize && abs(compActSig3) <= *windowSize
		&& currentRRDistance <= (1.5 * *RRMedian) )
		return *p3;*/

	if(currentRRDistance <= (1.5 * *RRMedian))
		return *p3;



	//compActSig1 = *p4 - *p1_next;
	//compActSig2 = *p4 - *p2_next;
	//compActSig3 = *p4 - *p3_next;
	currentRRDistance = *p4 - *lastPeak;
	/*if( abs(compActSig1) <= *windowSize && abs(compActSig2) <= *windowSize && abs(compActSig3) <= *windowSize
		&& currentRRDistance <= (1.5 * *RRMedian) )
		return *p4;*/

	if(currentRRDistance <= (1.5 * *RRMedian))
		return *p4;


	return 0; // En caso de no coincidencia
	
}


/*

Un pico considerase válido cando está presente en 3 ou 4 detectores dentro da ventana de error windowSize

Devuelve 0 si no coinciden los picos o la posición del pico 
*/
int isValidPeak( int *p1, int *p2, int *p3, int *p4, int *windowSize){

	// Pico presente en los 4 detectores

	int comp1 = *p1 - *p2;
	int comp2 = *p1 - *p3;
	int comp3 = *p1 - *p4;
	int comp4 = *p2 - *p3;
	int comp5 = *p2 - *p4;
	int comp6 = *p3 - *p4;
	int avgPeak = 0;
	

    // Si el pico coincide en 4 detectores
    avgPeak = (int)((*p1 + *p2 + *p3 + *p4) / 4);
	if( abs(comp1) <= *windowSize && abs(comp2) <= *windowSize && abs(comp3) <= *windowSize
	    && abs(comp4) <= *windowSize && abs(comp5) <= *windowSize && abs(comp6) <= *windowSize ) 
		return avgPeak;
		

	// Si el pico coincide en 3 detectores
	avgPeak = (int)((*p1 + *p2 + *p3) / 3);
	if( ( abs(comp1) <= *windowSize && abs(comp2) <= *windowSize) && abs(comp4) <= *windowSize )
		return avgPeak;
		
	avgPeak = (int)((*p1 + *p2 + *p4) / 3);
	if( ( abs(comp1) <= *windowSize && abs(comp3) <= *windowSize) && abs(comp5) <= *windowSize )
		return avgPeak;
		
	avgPeak = (int)((*p1 + *p3 + *p4) / 3);
	if( ( abs(comp2) <= *windowSize && abs(comp3) <= *windowSize) && abs(comp6) <= *windowSize ) 
		return avgPeak;
		
	avgPeak = (int)((*p2 + *p3 + *p4) / 3);
	if( ( abs(comp4) <= *windowSize && abs(comp5) <= *windowSize) && abs(comp6) <= *windowSize ) 
		return avgPeak;


	
	return 0; // En caso de no coincidencia
	
}



void comparePeaksAlgorithms(int *peaksPan, int *peaksHam, int *peaksPhasor, int *peaksWavelet,
							int *p1_length, int *p2_length, int *p3_length, int *p4_length,
							int *finalPositions, int *samplingFreq){
	int p1_index = 0;
	int p2_index = 0;
	int p3_index = 0;
	int p4_index = 0;
	int p1_next = 0;
	int p2_next = 0;
	int p3_next = 0;
	int p4_next = 0;
	int pfinal_index = 0;
	int smaller = 0;
	int errorWindowSize = 0.1 * *samplingFreq; // 100 ms
	int peakPos;	

	// Intervalos RR
	int ARRAYSIZE = 8;
	int RRPos = 0;
	double currentRRInterval = 300.0;
	double RRInterval[ARRAYSIZE];
	double RRMedian = 300.0;
	int lastPeak = 0;


	

   
	while( ( *p1_length - 1)  > p1_index 
			&& ( *p2_length - 1) > p2_index 
			&& ( *p3_length - 1 ) > p3_index
			&& ( *p4_length - 1) > p4_index  ){

		smaller = getSmaller( &peaksPan[p1_index], &peaksHam[p2_index], &peaksPhasor[p3_index], &peaksWavelet[p4_index]);


		
		if( (*p1_length - 2) <= p1_index )
			p1_next = peaksPan[p1_index + 1];
		if( (*p2_length - 2) <= p2_index )
			p2_next = peaksHam[p2_index + 1];
		if( (*p3_length - 2) <= p3_index)
			p3_next = peaksPhasor[p3_index + 1];
		if( (*p4_length - 2) <= p4_index)
			p4_next = peaksWavelet[p4_index + 1];

       	
		



		peakPos = isValidPeak(&peaksPan[p1_index], &peaksHam[p2_index], &peaksPhasor[p3_index], &peaksWavelet[p4_index], &errorWindowSize);

		if( (peakPos - lastPeak) > RRMedian ){ //Si pico perdido

		}

		//printf("\nPeakpos %d",peakPos);
		if( peakPos ){  
	 			finalPositions[pfinal_index] = peakPos;
	 			lastPeak = peakPos;
	 			p1_index++;
	 			p2_index++;
	 			p3_index++;
	 			p4_index++;



	 			if( pfinal_index >= 2){
					currentRRInterval = finalPositions[pfinal_index - 1] - finalPositions[pfinal_index - 2];
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

				//printf("\nCurrentRRInteval %f -- RRMedian %f --index %d  --peak %d", currentRRInterval, RRMedian, pfinal_index, peakPos);

	 			pfinal_index++;
 		}
 		else{ // en caso contrario solo se avanza el menor valor para seguir comprobando después
 			switch(smaller){ 
 				case 1:
 					p1_index++;
 					break;
 				case 2:
 					p2_index++;
 					break;
 				case 3:
 					p3_index++;
 					break;
 				case 4:
 					p4_index++;
 					break;
 			}
 		}
	}

}








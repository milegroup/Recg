#include <stdlib.h>



int getArrayMiddlePosition(int sw, int initialPos, int searchForward){
	return (searchForward) ? initialPos + (sw / 2): initialPos - (sw / 2);
}

int getArrayEnd(int sw, int middlePos, int initialPos, int searchForward ){
	if(searchForward){
		return middlePos + sw / 2 ;
	}
	else{
		return ( (middlePos + sw / 2) >= initialPos) ? initialPos : (middlePos + sw / 2);
	}
}

int getArrayBeginning(int sw, int middlePos, int initialPos, int searchForward ){
	if(searchForward){
		return ( (middlePos - sw / 2) <= initialPos) ? initialPos : (middlePos - sw / 2);
	}
	else{
		return middlePos - sw / 2;		
	}
}

int isAPeak(double *signal, int arrayBeginning, int arrayEnd, int middlePos, int searchForMinimum){
	int i = 0;
	for( i = arrayBeginning; i <= arrayEnd; i++){
		if( i == middlePos){
			continue;
		}
		
		if( searchForMinimum && signal[i] < signal[middlePos]){
			return 0;
		}
	
		if( ! searchForMinimum && signal[i] > signal[middlePos]){
			return 0;
		}		
	}
	return 1;
}

int isExceedLimit(int searchForward, int arrayBeginning, int arrayEnd, int lengthSignal){
	if(!searchForward && arrayBeginning <= 0){		
		return 1;
	}
	if(searchForward && arrayEnd >= lengthSignal - 1){
		return 1;
	}
	return 0;
}


double * getLocalPeak(double *signal, int lengthSignal, int initialPos, int sw, int searchForward, int searchForMinimum){

	// Fuerza que el tamaño del array sea siempre impar
	sw = (sw % 2 == 0) ? sw + 1 : sw;

	int middlePos = getArrayMiddlePosition(sw, initialPos, searchForward); // Posicion central del array
	int arrayBeginning;
	int arrayEnd;
	
	double * returnPeak = (double *) malloc(2 * sizeof(double));

	while( 1 ){		
		arrayBeginning = getArrayBeginning(sw, middlePos, initialPos, searchForward);
		arrayEnd = getArrayEnd(sw, middlePos, initialPos, searchForward);

		if( isAPeak(signal, arrayBeginning, arrayEnd, middlePos, searchForMinimum) ){ 
			returnPeak[0] = middlePos;
			returnPeak[1] = signal[middlePos];
			break;
		} 
		else{ 
			middlePos = (searchForward) ? middlePos + 1: middlePos - 1;
		}
		
		if(isExceedLimit(searchForward, arrayBeginning, arrayEnd, lengthSignal)){
			returnPeak[0] = 0;
			returnPeak[1] = 0;
			break;
		}
	}

	return returnPeak;
}

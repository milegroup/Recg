#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include <Rmath.h> 






/*
	Calculate the file size by placing the pointer in the last position and calculating the difference with the first
*/
void getFileSize(char **fileName, long int *fileSize) 
{	
	FILE *file = NULL;
	int SAMPLES_SIZE = 2;
	long int comienzo, final;


	file = fopen(*fileName,"rb");
	
	if( ! file ){
	    //printf( "Error (OT OPENED)\n" );
		fclose(file); 
		file = NULL;	
	}
	if( (comienzo=ftell( file )) < 0 ) {  
		//printf( "ERROR: ftell fails\n" );
		fclose(file); 
		file = NULL;
	}
	if( fseek( file, 0L, SEEK_END ) != 0 ){  // places the pointer at the end of the file
		//printf("Error in fseek function - in getFileSize (funciones.c)\n");
		fclose(file); 
		file = NULL;
	}
	final = ftell( file ); // save position. Number of bytes from the beginning to the end of the file

	if( fseek( file, 0L, SEEK_SET ) != 0 ){ // places it at the beginning
		//printf("Error in fseek function - in getFileSize (funciones.c)\n");		
		fclose(file); 
		file = NULL;
	}

	
	*fileSize = (final - comienzo); // entre el tamaño que ocupa cada muestra
	
	fclose(file); 
	file = NULL;
}



/*
  @param fileName Nombre + path del fichero que contiene el ECG
  @param vector
  @param fileSize Tamaño del fichero / 2 . Porque solo tiene el tamaño que ocupan las muestras del primer canal
*/
void readFile16(char **fileName, double *channel1, double *channel2, int *fileSize)
{	
		
	int i;	
	short int *tmpSamplesCh1, *tmpSamplesCh2;
	FILE *file = NULL;
	int readCh1 = 0;
	int readCh2 = 0;
	int numSamplesRead = 0; 
	int numBytesRead = 0;

	tmpSamplesCh1 = (short int *) calloc( *fileSize, sizeof(short int ) );
	tmpSamplesCh2 = (short int *) calloc( *fileSize, sizeof(short int ) );
	
	file=fopen(*fileName,"rb");
   	//printf("Reading: %s\n",*fileName);


	fseek( file, 0L, SEEK_SET ); //coloca puntero al inicio del file
	
	for (i=0; i < *fileSize; i++) {
		readCh1 = fread(&tmpSamplesCh1[i],2,1,file); //lee el primer canal. De 2 bytes en 2 bytes 
		readCh2 = fread(&tmpSamplesCh2[i],2,1,file); //lee el segundo canal. De 2 bytes en 2 bytes
		
		if( readCh1 == 0 || readCh2 == 0 ) //Si 0 no lee nada ==> sale del bucle
			break;

		

		if( readCh1 != 1 || readCh2 !=1 )  
		{
			//printf("Error reading the file (funciones.c)\n");
			//printf("Last sample (ch1) read from the file: %d ",readCh1);
			//printf("Last sample (ch2) read from the file: %d ",readCh2);
			fclose(file); 	
			file = NULL;
			free(tmpSamplesCh1); 
			free(tmpSamplesCh2);
			tmpSamplesCh1 = NULL;
			tmpSamplesCh2 = NULL;
		}
		else{					
			numSamplesRead++; 
			numBytesRead = numBytesRead + 4; 
		}
		
	}

	if( numBytesRead != *fileSize){ 
		//printf("Error reading file. \n File size: %d \n Samples read: %d ",*fileSize, numSamplesRead);
		fclose(file); 
		file = NULL;
	}
	else{
		for ( i = 0; i < numSamplesRead ; i++ ){ //que recorra hasta numSamplesRead
			channel1[i] =  (tmpSamplesCh1[i]);	
			channel2[i] = (tmpSamplesCh2[i]);
		}	
	}
	
	
	fclose(file); 	
	file = NULL;
	free(tmpSamplesCh1); 
	free(tmpSamplesCh2);
	tmpSamplesCh1 = NULL;
	tmpSamplesCh2 = NULL;	
}

/*
  @param fileName Nombre + path del fichero que contiene el ECG
  @param vector
  @param fileSize Tamaño del fichero / 2 . Porque solo tiene el tamaño que ocupan las muestras del primer canal
*/
void readFile212(char **fileName, double *channel1, double *channel2, int *fileSize)
{	
		
	int i;	
	short int *byte1, *byte2, *byte3;
	short int *byte2_1, *byte2_2;
	short int *samples1, *samples2;
	FILE *file = NULL;
	int b1 = 0;
	int b2 = 0;
	int b3 = 0;
	int numSamplesRead = 0; //cuenta bytes leidos. Si no se leen todos no coincide con tamaño file -> Error
	int numBytesRead = 0;

	byte1 = (short int *) calloc( *fileSize, sizeof(short int) );
	byte2 = (short int *) calloc( *fileSize, sizeof(short int) );
	byte3 = (short int *) calloc( *fileSize, sizeof(short int) );

	byte2_1 = (short int *) calloc( *fileSize, sizeof(short int));
	byte2_2 = (short int *) calloc( *fileSize, sizeof(short int));

	samples1 = (short int *) calloc( *fileSize, sizeof(short int));
	samples2 = (short int *) calloc( *fileSize, sizeof(short int));
	
	file = fopen(*fileName,"rb");


	fseek( file, 0L, SEEK_SET ); //coloca puntero al inicio del file
	
	for (i=0; i < *fileSize; i++) {
		b1 = fread(&byte1[i], 1, 1, file);  
		b2 = fread(&byte2[i], 1, 1, file); 
		b3 = fread(&byte3[i], 1, 1, file);

		byte2_1[i] = (byte2[i] & 0xF0) >> 4; // obtain the first 4 bits of the byte
		byte2_2[i] = (byte2[i] & 0x0F);      // obtain the last 4 bits of the byte

		samples1[i] = (byte2_1[i] << 8) | byte1[i]; // the sample is composed of 4 first bits of the second byte (as most significative) and first byte
		samples2[i] = (byte2_2[i] << 8) | byte3[i]; // the sample is composed of 4 second bits of the second byte (as most significative) and the third byte

		
		if( b1 == 0 || b2 == 0 || b3 == 0 ) //Si 0 == no lee nada ==> sale del bucle
			break;		

		if( b1 != 1 || b2 != 1 || b3 != 1 )  
		{
			fclose(file); 	
			file = NULL;
			free(byte1); 
			free(byte2);
			free(byte3);
			free(byte2_1);
			free(byte2_2);
			free(samples1);
			free(samples2);
			byte1 = NULL;
			byte2 = NULL;
			byte3 = NULL;
			byte2_1 = NULL;
			byte2_2 = NULL;
			samples1 = NULL;
			samples2 = NULL;
		}
		else{					
			numSamplesRead++; 
			numBytesRead = numBytesRead + 3;
		}

		
	}

	if( numBytesRead  != *fileSize){ // en cada iteración se leyeron 2 muestras
		fclose(file); 	
		file = NULL;
		free(byte1); 
		free(byte2);
		free(byte3);
		free(byte2_1);
		free(byte2_2);
		free(samples1);
		free(samples2);
		byte1 = NULL;
		byte2 = NULL;
		byte3 = NULL;
		byte2_1 = NULL;
		byte2_2 = NULL;
		samples1 = NULL;
		samples2 = NULL;
	}
	else{
		for ( i = 0; i < numSamplesRead ; i++ ){ //que recorra hasta numSamplesRead
			channel1[i] =  (samples1[i]);	
			channel2[i] = (samples2[i]);
		}	
	}
	
	
	fclose(file);  	
	file = NULL;
	free(byte1); 
	free(byte2);
	free(byte3);
	free(byte2_1);
	free(byte2_2);
	free(samples1);
	free(samples2);
	byte1 = NULL;
	byte2 = NULL;
	byte3 = NULL;
	byte2_1 = NULL;
	byte2_2 = NULL;
	samples1 = NULL;
	samples2 = NULL;
}


/*
	The low-pass suppresses noise
*/
void lowPassFilter(double *vectorLPF, double *initialSamples, int *channelSize)
{
	int contLPF = 12;
	
	while( contLPF < *channelSize )
	{
		vectorLPF[ contLPF ]= 2 * vectorLPF[ contLPF - 1 ] - vectorLPF[ contLPF - 2 ] + initialSamples[ contLPF ] - 2 * initialSamples[ contLPF - 6 ] 
					+ initialSamples[ contLPF - 12 ];		
		contLPF++;
	}

}



/*
	Applies the high pass filter
	The high-pass filter attenuates the P and T waves and the baseline drift
*/
void highPassFilter(double *vectorHPF, double *vectorLPF, int *channelSize)
{
	
	int contHPF = 32;
	
	while( contHPF < *channelSize )
	{
		vectorHPF[ contHPF ]= 32 * vectorLPF[ contHPF - 16 ] - ( 1.0 / 32.0 ) * ( ( vectorHPF[ contHPF - 1 ] + vectorLPF[ contHPF ] - vectorLPF[ contHPF - 32 ] ) );
      		contHPF++;
	}

}

/*
	Calculates the derivate to the result of the filtering steps
*/
void derivative(double *vectorDerivate, double *vectorHPF, int *channelSize)
{	
	int contDerivate = 2;
	
	while( contDerivate < *channelSize )
	{
		vectorDerivate[ contDerivate ] = ( 0.125 ) * ( ( -1 ) * vectorHPF[ contDerivate - 2 ] - 2 * vectorHPF[ contDerivate - 1 ] + 2 * vectorHPF[ contDerivate + 1 ] + vectorHPF[ contDerivate + 2 ]  ) ;
		contDerivate++;
	}
}




/*
	Applies the window integration function to the squared function
*/
void mwi(double *vectorMWI, double *vectorSquare, int *channelSize, double *windowSize)
{
	double suma;
	double n = *windowSize;
	int cont = *windowSize; //tengo que convertir a entero *windowSize porque el vector necesita trabajar con int
	int i = 0;
	while( cont < ( *channelSize -2 ) ) //suma el area de la ventana de integración
	{
		suma = 0;
					
		for( i = ( cont - n + 1 ) ; i < cont; i++ ) 
		{
			suma += vectorSquare[ i ]; //suma las últimas posiciones (el nº de posiciones depende de windowSize) y la actual del vector square
		}

		//printf("Suma: %d ",suma);
		vectorMWI[ cont ] = suma; //va guardando la suma de los anteriores + el actual

		vectorMWI[ cont ] = ( 1.0 / n ) * vectorMWI[ cont ];
		cont++;
	}
}

/*
	This function traverses all the signal and detects all the peaks (local maximum).
	A peak is considered when their neighbours in an interval are smaller in height. The interval size is a parameter.
	The bigger the interval size is, the less peaks are detected.

*/
void peakDetector(double *signal, int *peakPositions, double *peakHeights, int *fileSize, int *intervalSize){
	int i = *intervalSize;
	int peakCont = 0;
	int isPeak;
	while( i < (*fileSize - *intervalSize) ){ 
		isPeak = 1; // == true Suppose is a peak

		for(int j = (i - *intervalSize) ; j < (i+ *intervalSize); j++){ // if all positions are smaller, is a peak
			if(j == i)
				continue;
			if( ! ( signal[j] < signal[i] )){  // otherwise not
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
	Saves in an array the end point of the first N segments
*/
void getFirstSegments(int *firstSegments, int *segmentSize, int *numSegments){
	int i = 0;		
	for( i = 0; i < *numSegments; i++){
		firstSegments[i] = *segmentSize * (i+1); 
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
	Adaptative thresolding function used by the Pan & Tompkins algorithm, see the paper (A Real-Time QRS Detection Algorithm) for more information. 
	This function recives as parameters the initial peaks detected by the initial peak detector, and applying thresholds is able to get the final peaks in the signal. It is used for both, the mwi and the derivate function (peaks) and gets the final peaks in both signals.  
	It uses two thresholds, one is for signal peaks (SPKI) and the other for noise peaks (NPKI). 
	Two RR Interval means are calculated, one is for normal rithm (last 8 beats between certain bounds) and the other is the last 8 beats. The reason of these two different RR Intervals is to adapt to quickly changing or irregular heart rates (reducing by half the thrs)
	Also a Search Back is done when a beat is not found in a certain time. 
	And finally a T-Wave remove technique is applied to discard False + predictions (optional)
*/
void panTompkinsAlg( double *derivateSignal, int *samplingFrequency, int *peakPositions, double *peakHeights, double *thr, int *allPeakPositions, double *allPeakHeights, 
			  int *allPeakArraySize, int *signalPeakPositions, double *signalPeakHeights, double * SPKI, double * NPKI, int *windowSize){	
	int RR_ARRAY_SIZE = 8;
	double refractoryPeriod200 = *samplingFrequency * 0.2; // 200 ms 
	double refractoryPeriod360 = *samplingFrequency * 0.36; // 360 ms

	//******	Initialization		******
	signalPeakPositions[0] = peakPositions[0]; // save 2 first R peaks
	signalPeakPositions[1] = peakPositions[1];
	signalPeakHeights[0] = peakHeights[0];
	signalPeakHeights[1] = peakHeights[1];
	double RRAverage1 = peakPositions[1] - peakPositions[0]; // RR means
	double RRAverage2 = peakPositions[1] - peakPositions[0];
	double RRLowLimit = 0.92 * RRAverage2;
	double RRHighLimit = 1.16 * RRAverage2;
	double RRMissedLimit = 1.66 * RRAverage2;
	double recentRRBeats [RR_ARRAY_SIZE]; // last 8 beats
	double normalRecentRRBeats[RR_ARRAY_SIZE]; // last 8 normal beats
	recentRRBeats[0] = peakPositions[0]; 
	recentRRBeats[1] = peakPositions[1];
	normalRecentRRBeats[0] = peakPositions[0];
	normalRecentRRBeats[1] = peakPositions[1]; 

	double thr2 = 0.0;	
	*SPKI = ( peakHeights[0] + peakHeights[1] ) / 2; // Signal thr. Starts with the mean of 2 first peaks
	*NPKI = 0.0; // Noise peaks. Starts with the all noise peaks from the beginning to the second signal peak

	int i = 0;
	int bound = peakPositions[1]; 
	int value = allPeakPositions[i];
	// initializing NPKI
	while( value < bound ){ // from the beginning to the second signal peak
		
		if( allPeakHeights[i] < *SPKI ){ 	
			*NPKI = 0.125 * allPeakHeights[i] + 0.875 * *NPKI;
		}
		i++;
		value = allPeakPositions[i]; // next peak
	}

	// initializing thrs
	*thr = *NPKI + 0.25 * ( *SPKI - *NPKI);
	thr2 = *thr / 2;

	
	//******	Detection	******
	
	int peakIndex = 2; // 2 first peaks are already saved
	int avg1Index = 2; 
	int avg2Index = 2;
	int lastPeakPosition = peakPositions[1]; // needed in Search-back
	int maxPosition = 0; // in case of Search-back
	int maxHeight = 0;
	double sum = 0.0;

	while( i < *allPeakArraySize){
		  // Removing TWaves
		if( allPeakHeights[i] > *thr ){ //si supera umbral => pico de señal
			
			int previous = signalPeakPositions[ peakIndex - 1 ];
			int current = allPeakPositions[i];		
	
			if( isTWave( samplingFrequency,  &previous, &current, derivateSignal, windowSize ) ){ //entonces es ruido
				*NPKI = 0.125 * allPeakHeights[i] + 0.875 * *NPKI;
				if( ( allPeakHeights[i] > maxHeight ) && ( allPeakHeights[i] > thr2)  ){ //el mayor pico que supera el umbral 2
					maxHeight = allPeakHeights[i];
					maxPosition = allPeakPositions[i];
				} 				
			}
			else{ //registrar posicion
				*SPKI = 0.125 * allPeakHeights[i] + 0.875 * *SPKI;
				signalPeakPositions[ peakIndex ] = allPeakPositions[i];
				signalPeakHeights[ peakIndex ] = allPeakHeights[i];
				lastPeakPosition = allPeakPositions[i];
				peakIndex++;
				maxPosition = 0;
				maxHeight = 0;
			}			
		}
		else{ //sino => pico de ruido
			*NPKI = 0.125 * allPeakHeights[i] + 0.875 * *NPKI;

			if( ( allPeakHeights[i] > maxHeight ) && ( allPeakHeights[i] > thr2)  ){ //el mayor pico que supera el umbral 2
				maxHeight = allPeakHeights[i];
				maxPosition = allPeakPositions[i];
			} 
		}
		
		
		// Without removing TWaves
		/*if( allPeakHeights[i] > *thr ){ // signal peak
			*SPKI = 0.125 * allPeakHeights[i] + 0.875 * *SPKI;
			signalPeakPositions[ peakIndex ] = allPeakPositions[i];
			signalPeakHeights[ peakIndex ] = allPeakHeights[i];
			lastPeakPosition = allPeakPositions[i];
			peakIndex++;
			maxPosition = 0;
			maxHeight = 0;
			
		}
		else{ // noise peak
			*NPKI = 0.125 * allPeakHeights[i] + 0.875 * *NPKI;

			if( ( allPeakHeights[i] > maxHeight ) && ( allPeakHeights[i] > thr2)  ){ // the highest peak that exceeds the 2 thresholds
				maxHeight = allPeakHeights[i];
				maxPosition = allPeakPositions[i];
			} 
		}
		*/
		// Search-Back
		if( ( ( allPeakPositions[i] - lastPeakPosition ) > RRMissedLimit ) && ( maxPosition != 0 ) ){ // in case of a lost beat
			*SPKI = 0.25 * maxHeight + 0.75 * *SPKI;
			signalPeakPositions[ peakIndex ] = maxPosition;
			signalPeakHeights[ peakIndex ] = maxHeight;
			lastPeakPosition = maxPosition;
			peakIndex++;
			maxPosition = 0;
			maxHeight = 0;
		}

		// adjusting thrs
		*thr = *NPKI + 0.25 * ( *SPKI - *NPKI);
		thr2 = *thr / 2;
	
		// adjusting RR intervals	
		if( ( (allPeakPositions[i] - lastPeakPosition) > RRLowLimit ) && ( (allPeakPositions[i] - lastPeakPosition) < RRHighLimit) ){ //ritmo normal
			normalRecentRRBeats[ avg2Index % RR_ARRAY_SIZE ] = allPeakPositions[i]; //va guardando en el array sobreescribiendo	
			for ( int j = 0 ; j < RR_ARRAY_SIZE; j++)
				sum += normalRecentRRBeats[j];					
			RRAverage2 = 0.125 * sum;
			avg2Index++;
		}
		else{ // irregular heart rate 
			*thr /=2;
			thr2 /=2;
		}			
		recentRRBeats[ avg1Index % RR_ARRAY_SIZE ] = allPeakPositions[ i ];
		for ( int j = 0 ; j < RR_ARRAY_SIZE; j++)
				sum += recentRRBeats[j];	
		RRAverage1 = 0.125 * sum;
		avg1Index++;
				
		

		RRLowLimit = 0.92 * RRAverage2;
		RRHighLimit = 1.16 * RRAverage2;
		RRMissedLimit = 1.66 * RRAverage2;

		i++;

	}	
}



/*
	This function takes the peaks in the mwi and derivate signals and decided that is a final peak if the peak position is the same in both signals. 
	It uses an error factor that allows a certain error in the comparision. And also takes into account the Delay between the mwi and derivate signals (half of the mwi window size)
	So it returns as a result the final peaks where the peaks are in both signals.
*/
void samePeaks( int *mwiPeakPositions, double *mwiPeakHeights, int *derivatePeakPositions, double *derivatePeakHeights,  int *mwiLength, int *derivateLength, int *resultPeakPositions, double *resultPeakHeights, int *windowSize){
	
	int errorFactor = 50; // num. samples of possible error
	int signalPos = 0;
	double signalH = 0.0;
	int rIndex = 0;
	int DELAY = *windowSize/2; 

	// Find the smallest array and traveses it, comparing with the other
	if( *mwiLength > *derivateLength){ 		
		for(int i = 0; i <  *derivateLength; i++){
			signalPos = derivatePeakPositions[i] + DELAY;
			for( int j = 0; ( mwiPeakPositions[j] - errorFactor) <  signalPos ; j++)
			{
				if( ( abs( signalPos - mwiPeakPositions[j] ) < errorFactor ) ){
					resultPeakPositions[rIndex] = mwiPeakPositions[j];
					resultPeakHeights[rIndex] = mwiPeakHeights[j];
					rIndex++;
				}
			}
		}
	}
	else{		
		for(int i = 0; i < *mwiLength ; i++){
			signalPos = mwiPeakPositions[i];
			signalH = mwiPeakHeights[i];
			for( int j = 0; ( (derivatePeakPositions[j]+ DELAY) - errorFactor) <  signalPos ; j++)
			{
				if( ( abs( signalPos - (derivatePeakPositions[j]+DELAY) ) < errorFactor ) ){
					resultPeakPositions[rIndex] = signalPos;
					resultPeakHeights[rIndex] = signalH;
					rIndex++;
				}
			}		
		}
	}	
}


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
	After the peak detection method, this function decides which peaks are signal peaks and discards the others.
	It uses two thresholds to determine the peaks, one is the threshold used to the normal detection, and the other is used in case of search back
	The search back is applied when the current RR Interval is higher than the 150% of the last ARRAYSIZE median intervals
	Three arrays are used to maintein information about the signal. One is the last peaks, other the last noise peaks and finally the last RR Interval values
	To apply the Search Back, the maximal peak (that exceed the lower threshold) is saved, and this position is saved as a peak when a SB is applied
	
*/
void hamiltonPeakDecision( double *derivateSignal, int *peakPositions, double *peakHeights, int *allPeakPositions, double *allPeakHeights, int *allPeakArraySize, 
			   int *signalPeakPositions, double *signalPeakHeights, int *samplingFrequency, int *windowSize){
	
	int ARRAYSIZE = 10;
	int RR_ARRAYSIZE = 8;
	double lastPeaks[ARRAYSIZE];	
	double noisePeaks[ARRAYSIZE];
	double RRInterval[RR_ARRAYSIZE];
	double DT; 
	double QRSPL;
	double NPL;
	double SBThr; // search back thr
	double TC = 0.133; // Threshold coefficient
	double refractoryPeriod = *samplingFrequency * 0.2; // 200 ms 
	
	//******	Initialization		******
	lastPeaks[0] = peakHeights[0];
	lastPeaks[1] = peakHeights[1];
	int lastPeakPos = 2;
	RRInterval[0] = lastPeaks[1] - lastPeaks[0];
	int RRPos = 1;
	double RRMissedLimit = 0.0;
	int noisePeakPos = 0;

	signalPeakHeights[0] = peakHeights[0];
	signalPeakHeights[1] = peakHeights[1];
	signalPeakPositions[0] = peakPositions[0];
	signalPeakPositions[1] = peakPositions[1];
	int peakIndex = 2;

	
	
	double currentPeakHeight = 0.0;
	int currentPeakPosition = 0;
	int bound = peakPositions[1];
	//******	Detection	******
	
	int maxPosition = 0; // in case of Search-back
	int maxHeight = 0;

	// Initializing noise peak levels
	int i = 0;
	int value = allPeakPositions[i];
	//Prueba
	DT = median(lastPeaks, &lastPeakPos);	
	DT = 0.3 * DT;
	while( value < bound ){	// from the beginning to the second signal peak	
		if( allPeakHeights[i] < DT ){
			if( noisePeakPos < ARRAYSIZE ){
				noisePeaks[ noisePeakPos ]  = allPeakHeights[i];
			}
			else{
				noisePeaks[ noisePeakPos % ARRAYSIZE ] = allPeakHeights[i];
			}
			noisePeakPos++;
		}	
		i++;
		value = allPeakPositions[i]; // next peak
	}

	QRSPL = median( lastPeaks, &lastPeakPos );
	if( noisePeakPos < ARRAYSIZE )
		NPL = median( noisePeaks, &noisePeakPos);
	else
		NPL = median( noisePeaks, &ARRAYSIZE );		
	DT = NPL + TC * ( QRSPL - NPL );
	SBThr = 0.3 * DT;


	int lastPeakPosition = peakPositions[1];
	while( i < *allPeakArraySize){ 
		
		currentPeakHeight = allPeakHeights[i];
		currentPeakPosition = allPeakPositions[i];

		if( currentPeakHeight > DT //){ // if signal peak
		     && ( currentPeakPosition - lastPeakPosition ) > refractoryPeriod  ){
		    //&& ! isTWave( samplingFrequency,  &lastPeakPosition, &currentPeakPosition, derivateSignal, windowSize ) ){ // && not TWave 	
		
			if( lastPeakPos < ARRAYSIZE ){ //if array not full
				lastPeaks[lastPeakPos] = currentPeakHeight;
				lastPeakPos++;										
				QRSPL = median( lastPeaks, &lastPeakPos );				
			}	
			else{ // array full
				lastPeaks[ lastPeakPos % ARRAYSIZE ] = currentPeakHeight;
				lastPeakPos++;
				QRSPL = median( lastPeaks, &ARRAYSIZE );
			}
			
			
			//save signal 
			signalPeakPositions[peakIndex] = currentPeakPosition;
			signalPeakHeights[peakIndex] = currentPeakHeight;
			peakIndex++;
			maxPosition = 0;
			maxHeight = 0;

			//update RRInteval
			if( RRPos < RR_ARRAYSIZE ){
				RRInterval[RRPos] = currentPeakPosition - lastPeakPosition;
				RRPos++;
				RRMissedLimit = median(RRInterval, &RRPos) * 1.5; // 150 % del RRInterval
			}
			else{
				RRInterval[RRPos % RR_ARRAYSIZE ] = currentPeakPosition - lastPeakPosition;
				RRPos++;
				RRMissedLimit = median(RRInterval, &RR_ARRAYSIZE) * 1.5;
			}			
			
			lastPeakPosition = currentPeakPosition;
			
		}
		else{
			// update NPL
			if( noisePeakPos < ARRAYSIZE ){ 
				noisePeaks[ noisePeakPos ]  = allPeakHeights[i];
				noisePeakPos++;
				NPL = median( noisePeaks, &noisePeakPos);
			}
			else{
				noisePeaks[ noisePeakPos % ARRAYSIZE ] = allPeakHeights[i];	
				noisePeakPos++;
				NPL = median( noisePeaks, &ARRAYSIZE );				
			}
			

			if( ( currentPeakHeight > maxHeight ) && ( currentPeakHeight > SBThr) //){ // the highest peak that exceeds the 2 thresholds
				 && ( currentPeakPosition - lastPeakPosition ) > refractoryPeriod  ){
		    		//&& ! isTWave( samplingFrequency,  &lastPeakPosition, &currentPeakPosition, derivateSignal, windowSize ) ){ // && not TWave 	
				maxHeight = currentPeakHeight;
				maxPosition = currentPeakPosition;
			} 
		}

		// Apply Search Back
		if( RRMissedLimit  < ( currentPeakPosition - lastPeakPosition) ){ 
			
			// Update QRS peak levels
			if( lastPeakPos < ARRAYSIZE ){
				lastPeaks[lastPeakPos] = maxHeight;
				lastPeakPos++;										
				QRSPL = median( lastPeaks, &lastPeakPos );		
			}
			else{
				lastPeaks[ lastPeakPos % ARRAYSIZE ] = maxHeight;
				lastPeakPos++;
				QRSPL = median( lastPeaks, &ARRAYSIZE );
			}
			
			
			//save the peak positions 
			signalPeakPositions[peakIndex] = maxPosition;
			signalPeakHeights[peakIndex] = maxHeight;
			peakIndex++;
			
			// Update RR missed limit level
			if( RRPos < RR_ARRAYSIZE ){
				RRInterval[RRPos] = maxPosition - lastPeakPosition;
				RRPos++;
				RRMissedLimit = median(RRInterval, &RRPos) * 1.5; // 150 % del RRInterval
			}
			else{
				RRInterval[RRPos % RR_ARRAYSIZE ] = maxPosition - lastPeakPosition;
				RRPos++;
				RRMissedLimit = median(RRInterval, &RR_ARRAYSIZE) * 1.5;
			}			
			
			// Update variables
			lastPeakPosition = maxPosition;
			maxPosition = 0;
			maxHeight = 0;
		}

		// update thresholds
		DT = NPL + TC * ( QRSPL - NPL );
		SBThr = 0.3 * DT;
		
		i++;
	}
	
}


/*
	Place the fiducial mark according to the Hamilton algorithm. Starting from the signal positions placed in the mwi, it looks back in an interval,
	searching for the largest peak in the derivate signal.
	The fiducial mark is placed in the maximun value in the interval even when the derivate is negative
*/
void placeFiducialMark( double *signal, int *mwiPeakPositions, int *mwiPeakArraySize, int *resultPeakPositions, 
			double *resultPeakHeights, int *DELAY, int *halfWindowSize){
	int intervalBeginning = 0;
	int intervalEnd = 0;
	int maxPos = 0;
	double maxValue = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int isPeak = 1;	

	for( i=0; i < *mwiPeakArraySize; i++){  // traverses all the peak positions
		
		intervalBeginning = mwiPeakPositions[i] - *DELAY - *halfWindowSize;
		if( intervalBeginning <= 0 ){
			intervalBeginning = 0;
		}
		intervalEnd = mwiPeakPositions[i] - *DELAY + *halfWindowSize; 
		maxValue = -999999999; // because the signal can be negative
		maxPos = 0;
			
		for( j = intervalBeginning; j < intervalEnd; j++){ // for each position traverses the interval to locate the max.
			isPeak = 1; // == true Suppose is a peak
			
			for( k = intervalBeginning; k < intervalEnd ; k++){ // traverses this position neighbours. If all positions are smaller, is a peak
				if( j == k )
					continue;
				if( signal[k] >= signal[j] ){
					isPeak = 0;	
					break;
				}
			}
			if( isPeak && signal[j] > maxValue ){
				maxPos = j;
				maxValue = signal[j];
			}			
		}	
		

		if( maxPos == 0 ){ // si no hay un pico, que registre la máxima posicion
			for( j = intervalBeginning; j < intervalEnd; j++){ // for each position traverses the interval to locate the max.
				if( signal[j] > maxValue ){
					maxPos = j;
					maxValue = signal[j];
				}			
			}
	
		}



		//save pos
		resultPeakPositions[i] = maxPos;
		resultPeakHeights[i] = maxValue;
		
	}
}

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

void convertToPhysicalUnits(int *dataSize, int *ADCGain, int *ADCZero, double *channelData){
	int i = 0;
	double gain = (1 /(double) *ADCGain);
	while(i < *dataSize){
		channelData[i] = ((double) (channelData[i] - *ADCZero)) * gain;
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


	while(currentPos < *rightIndex){
		sum = sum + (array[currentPos] * array[currentPos]);
		currentPos++;
	}


	return sqrt(sum / arraySize);

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

/*
	A partir de la posicion n.pre busca la posicion n.first
	N.first es el máximo o mínimo local que precede a n.pre
	Para localizarlo empieza a buscar desde la posición de n.pre hacia atrás hasta encontrar un máximo o mínimo local

	Para localizar un máximo local se utiliza un array y se considera que se ha detectado cuando el valor de la posición central del array es mayor que
	cualquier otro valor del array.
	Tomando como posicion central del array nPrePosition, se calcula la posición de la mitad del array (arrayMiddlePosition)
	y entonces se va comprobando si arrayMiddlePosition es la posición más grande del array. Cuando así sea, es porque se ha encontrado un máximo local	
*/
void getNFirst(double *waveletSignal, int *nPrePosition, int *returnNFirstPosition, double *returnNFirstHeight  ){

	int arraySize = 6; // Tamaño del array en numero de muestras
	int positionNpre = *nPrePosition - 1; // Porque en R los indices empiezan en 1 y en C empiezan en 0
	//int arrayMiddlePosition = positionNpre - (arraySize / 2); 
	int arrayMiddlePosition = positionNpre; // La posición central empieza en n.pre 
	int isPeakDetected = 0;
	int isAValidPeak;
	int arrayBeggining;
	int arrayEnd;

	double n_preValue = waveletSignal[positionNpre];



	while( ! isPeakDetected ){

		isAValidPeak = 1; // Siempre supone que es un pico válido

		arrayBeggining = (arrayMiddlePosition - (arraySize / 2));
		arrayEnd = (arrayMiddlePosition + (arraySize / 2));
		for( int j = arrayBeggining ; j < arrayEnd; j++){
			if( j == arrayMiddlePosition){
				continue;
			}

			if( n_preValue <= 0){  // Si n.pre es negativo busca un máximo local. Si existe una posicion en el array mayor que la posicion central ya no es un máximo, sale del bucle
				if( ( waveletSignal[j] >= waveletSignal[arrayMiddlePosition] ) ){ // Si es mayor, ya no es un máximo local. Por tanto no será un pico válido
					isAValidPeak = 0;
					break;
				}	
			}
			else{ // Si n.pre es positivo busca un mínimo local. Si existe una posicion en el array menor que la posicion central ya no es un mínimo local, sale del bucle
				if(  ( waveletSignal[j] <= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
				}	
			}					
		}

		// Si sale del bucle siendo un pico válido, entonces se guarda el pico detectado (mínimo o máximo) y sale del bucle
 		if(isAValidPeak){ 
			isPeakDetected = 1;
			returnNFirstPosition[0] = arrayMiddlePosition + 1; // se suma 1 para compensar con R
			returnNFirstHeight[0] = waveletSignal[arrayMiddlePosition];
		} 
		else{ // Sino comprueba la posición anterior
			arrayMiddlePosition = arrayMiddlePosition - 1; 
		}
	}
}


/*
	A partir de la posicion n.post busca la posicion n.last
	N.last es el máximo o mínimo local a continuación a n.post
	Para localizarlo empieza a buscar desde la posición de n.post hacia delante hasta encontrar un máximo o mínimo local

	Para localizar un máximo local se utiliza un array y se considera que se ha detectado cuando el valor de la posición central del array es mayor que
	cualquier otro valor del array.
	Tomando como posicion central del array nPostPosition, se calcula la posición de la mitad del array (arrayMiddlePosition)
	y entonces se va comprobando si arrayMiddlePosition es la posición más grande del array. Cuando así sea, es porque se ha encontrado un máximo local	
*/
void getNLast(double *waveletSignal, int *nPostPosition, int *returnNLastPosition, double *returnNLastHeight ){

	int arraySize = 6; // Tamaño del array en numero de muestras
	int positionNpost = *nPostPosition - 1; // Porque en R los indices empiezan en 1 y en C empiezan en 0
	//int arrayMiddlePosition = positionNpost + (arraySize / 2); 
	int arrayMiddlePosition = positionNpost; // La posición central empieza en n.post 
	int isPeakDetected = 0;
	int isAValidPeak;
	int arrayBeggining;
	int arrayEnd;

	double n_postValue = waveletSignal[positionNpost]; // Valor del pico n.post


	/*
		A partir de la posición n.post busca hacia delante un máximo o mínimo local. Utilizando un tamaño de array, donde comprueba que la posición central
		sea el mínimo o el máximo local. Mientras no encuentra ese mínimo o máximo local continúa en el bucle buscando hacia delante
	*/
	while( ! isPeakDetected ){

		isAValidPeak = 1; // Siempre supone que es un pico válido

		arrayBeggining = (arrayMiddlePosition - (arraySize / 2));
		arrayEnd = (arrayMiddlePosition + (arraySize / 2));
		for( int j = arrayBeggining ; j < arrayEnd; j++){ // Recorre el array
			if( j == arrayMiddlePosition){
				continue;
			}


			if( n_postValue <= 0){ // Si n.post es negativo busca un máximo local. Si existe una posicion en el array mayor que la posicion central ya no es un máximo, sale del bucle
				if( ( waveletSignal[j] >= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
				}	
			}
			else{ // Si n.post es positivo busca un mínimo local. Si existe una posicion en el array menor que la posicion central ya no es un mínimo local, sale del bucle
				if(  ( waveletSignal[j] <= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
				}	
			}

					
		}

		// Si sale del bucle siendo un pico válido, entonces se guarda el pico detectado (mínimo o máximo) y sale del bucle
		if(isAValidPeak){ 
			isPeakDetected = 1;
			returnNLastPosition[0] = arrayMiddlePosition + 1;  // se suma 1 para compensar con R
			returnNLastHeight[0] = waveletSignal[arrayMiddlePosition];
		} 
		else{ // Sino comprueba la posición siguiente
			arrayMiddlePosition = arrayMiddlePosition + 1; 
		}
	}
}


void getLocalMinimaBeforeCurrentPosition(double *waveletSignal, int *currentPosition, int *returnMinimaPosition, double *returnMinimaHeight){

	int position = *currentPosition - 1; // Porque en R los índices empiezan en 1 y en C en 0
	int arraySize = 6; // Tamaño del array en número de muestras
	int arrayMiddlePosition = position - (arraySize / 2); // Posicion central del array
	int isPeakDetected = 0;
	int isAValidPeak;
	int arrayBeggining;
	int arrayEnd;

	/*
		
	*/
	while( ! isPeakDetected ){

		isAValidPeak = 1; // Siempre supone que es un pico válido

		arrayBeggining = (arrayMiddlePosition - (arraySize / 2));
		arrayEnd = (arrayMiddlePosition + (arraySize / 2));
		for( int j = arrayBeggining ; j < arrayEnd; j++){ // Recorre el array para comprobar si la posición central es un minimo local
			if( j == arrayMiddlePosition){
				continue;
			}

			// Si existe una posición en el array menor que la central, ya no es un mínimo. Sale del bucle (no sigue buscando)
			if(  ( waveletSignal[j] <= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
			}						
		}

		// Si sale del bucle siendo un pico válido, entonces se guarda el pico detectado y sale del bucle
		if(isAValidPeak){ 
			isPeakDetected = 1;
			returnMinimaPosition[0] = arrayMiddlePosition + 1;  // se suma 1 para compensar con R
			returnMinimaHeight[0] = waveletSignal[arrayMiddlePosition];
		} 
		else{ // Sino comprueba la posición anterior
			arrayMiddlePosition = arrayMiddlePosition - 1; 
		}
	}

}


void getLocalMinimaAfterCurrentPosition(double *waveletSignal, int *currentPosition, int *returnMinimaPosition, double *returnMinimaHeight){

	int position = *currentPosition - 1; // Porque en R los índices empiezan en 1 y en C en 0
	int arraySize = 3; // Tamaño del array en número de muestras
	int arrayMiddlePosition = position + (arraySize / 2); // Posicion central del array
	int isPeakDetected = 0;
	int isAValidPeak;
	int arrayBeggining;
	int arrayEnd;

	/*
		
	*/
	while( ! isPeakDetected ){

		isAValidPeak = 1; // Siempre supone que es un pico válido

		arrayBeggining = (arrayMiddlePosition - (arraySize / 2));
		arrayEnd = (arrayMiddlePosition + (arraySize / 2));
		for( int j = arrayBeggining ; j < arrayEnd; j++){ // Recorre el array para comprobar si la posición central es un minimo local
			if( j == arrayMiddlePosition){
				continue;
			}

			// Si existe una posición en el array menor que la central, ya no es un mínimo. Sale del bucle (no sigue buscando)
			if(  ( waveletSignal[j] <= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
			}						
		}

		// Si sale del bucle siendo un pico válido, entonces se guarda el pico detectado y sale del bucle
		if(isAValidPeak){ 
			isPeakDetected = 1;
			returnMinimaPosition[0] = arrayMiddlePosition + 1;  // se suma 1 para compensar con R
			returnMinimaHeight[0] = waveletSignal[arrayMiddlePosition];
		} 
		else{ // Sino comprueba la posición siguiente
			arrayMiddlePosition = arrayMiddlePosition + 1; 
		}
	}
}


void getLocalMaximaBeforeCurrentPosition(double *waveletSignal, int *currentPosition, int *returnMaximaPosition, double *returnMaximaHeight){

	int position = *currentPosition - 1; // Porque en R los índices empiezan en 1 y en C en 0
	int arraySize = 3; // Tamaño del array en número de muestras
	int arrayMiddlePosition = position - (arraySize / 2); // Posicion central del array
	int isPeakDetected = 0;
	int isAValidPeak;
	int arrayBeggining;
	int arrayEnd;

	/*
		
	*/
	while( ! isPeakDetected ){

		isAValidPeak = 1; // Siempre supone que es un pico válido

		arrayBeggining = (arrayMiddlePosition - (arraySize / 2));
		arrayEnd = (arrayMiddlePosition + (arraySize / 2));
		for( int j = arrayBeggining ; j < arrayEnd; j++){ // Recorre el array para comprobar si la posición central es un minimo local
			if( j == arrayMiddlePosition){
				continue;
			}

			// Si existe una posición en el array mayor que la central, ya no es un máximo. Sale del bucle (no sigue buscando)
			if(  ( waveletSignal[j] >= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
			}						
		}

		// Si sale del bucle siendo un pico válido, entonces se guarda el pico detectado y sale del bucle
		if(isAValidPeak){ 
			isPeakDetected = 1;
			returnMaximaPosition[0] = arrayMiddlePosition + 1;  // se suma 1 para compensar con R
			returnMaximaHeight[0] = waveletSignal[arrayMiddlePosition];
		} 
		else{ // Sino comprueba la posición anterior
			arrayMiddlePosition = arrayMiddlePosition - 1; 
		}
	}

}

void getLocalMaximaAfterCurrentPosition(double *waveletSignal, int *currentPosition, int *returnMaximaPosition, double *returnMaximaHeight){

	int position = *currentPosition - 1; // Porque en R los índices empiezan en 1 y en C en 0
	int arraySize = 6; // Tamaño del array en número de muestras
	int arrayMiddlePosition = position + (arraySize / 2); // Posicion central del array
	int isPeakDetected = 0;
	int isAValidPeak;
	int arrayBeggining;
	int arrayEnd;

	/*
		
	*/
	while( ! isPeakDetected ){

		isAValidPeak = 1; // Siempre supone que es un pico válido

		arrayBeggining = (arrayMiddlePosition - (arraySize / 2));
		arrayEnd = (arrayMiddlePosition + (arraySize / 2));
		for( int j = arrayBeggining ; j < arrayEnd; j++){ // Recorre el array para comprobar si la posición central es un minimo local
			if( j == arrayMiddlePosition){
				continue;
			}

			// Si existe una posición en el array menor que la central, ya no es un máximo. Sale del bucle (no sigue buscando)
			if(  ( waveletSignal[j] >= waveletSignal[arrayMiddlePosition] ) ){
					isAValidPeak = 0;
					break;
			}						
		}

		// Si sale del bucle siendo un pico válido, entonces se guarda el pico detectado y sale del bucle
		if(isAValidPeak){ 
			isPeakDetected = 1;
			returnMaximaPosition[0] = arrayMiddlePosition + 1;  // se suma 1 para compensar con R
			returnMaximaHeight[0] = waveletSignal[arrayMiddlePosition];
		} 
		else{ // Sino comprueba la posición siguiente
			arrayMiddlePosition = arrayMiddlePosition + 1; 
		}
	}
}

/*
	Detecta los maximum modulus lines en la escala que se pasa como parámetro. Descartando ya aquellos picos aislados (isolated) y redundantes


	Params: 
	 	- waveletScale: Señal wavelet de una determinada escala
	 	- lengthWaveletScale: Tamaño de la waveletScale
	 	- samplingFreq: Frecuencia de muestreo de la señal
	 	- scale: Entero que indica en que escala de wavelet se busca los modulus lines. La escala 4 utiliza la mitad del umbral
	 	- returnModulusPosition: array que devuelve las posiciones de los modulus lines
	 	- returnModulusHeight: array que devuelve las alturas de los modulus lines

void getMaximumModulus(double *waveletScale, int *lengthWaveletScale, int *samplingFreq,
					   int *scale, int *returnModulusPosition, double *returnModulusHeight){
	int sizeThresholdUpdate = 2^16;



	int cont = 0; 
	int contThresholdUpdate = 0;
	double thr = 0.0;
	int leftIndex = 0;
	int rightIndex = sizeThresholdUpdate;




	// Recorrer la señal buscando máximos locales que superal el umbral
	while( cont < *lengthWaveletScale ){

		// Actualizar el umbral
		if(contThresholdUpdate >= sizeThresholdUpdate){ // Cada sizeThresholdUpdate muestras se actualiza el umbral
			thr = getRMS(waveletScale, &leftIndex, &rightIndex);
			if( *scaleNum == 4){
				thr = thr * 0.5;
			}
			// Mover los indices del array, para calcular después el nuevo umbral
			leftIndex = rightIndex;
			rightIndex = rightIndex + sizeThresholdUpdate;
			if(rightIndex > lengthWaveletScale){ // Para que evitar arrayIndexOutOfBoundException
				rightIndex = lengthWaveletScale;
			}
			contThresholdUpdate = 0;
		}

		



		




		cont = cont + 1;
		contThresholdUpdate = contThresholdUpdate + 1;
	}
}

*/


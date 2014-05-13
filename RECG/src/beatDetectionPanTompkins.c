


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
					//resultPeakPositions[rIndex] = mwiPeakPositions[j];
					//resultPeakHeights[rIndex] = mwiPeakHeights[j];
					resultPeakPositions[rIndex] = derivatePeakPositions[i];
					resultPeakHeights[rIndex] = derivatePeakHeights[i];
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
					//resultPeakPositions[rIndex] = signalPos;
					//resultPeakHeights[rIndex] = signalH;
					resultPeakPositions[rIndex] = derivatePeakPositions[j];
					resultPeakHeights[rIndex] = derivatePeakHeights[j];
					rIndex++;
				}
			}		
		}
	}	
}
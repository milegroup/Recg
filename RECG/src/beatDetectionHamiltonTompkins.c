#include "commonFunctions.h"

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

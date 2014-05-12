


double median(double *array, int *elements);

int isTWave( int *samplingFrequency, int *previousPeakPosition, int *currentPeakPosition, double *derivateSignal, int *DELAY);

void peakDetector(double *signal, int *peakPositions, double *peakHeights, int *arraySize, int *intervalSize);

void peakDetector_SinR(double *signal, int *peakPositions, double *peakHeights, int *arraySize, int *intervalSize);

double getRMS(double *array, int *leftIndex, int *rightIndex);

double * getArrayAbs(double * array, int arrayLength);

int getMinimumInAbsArray(double *array, int leftIndex, int rightIndex);

int getMaximumInAbsArray(double *array, int leftIndex, int rightIndex);

int getArrayMinimum(double *array, int leftIndex, int rightIndex);

int getArrayMaximum(double *array, int leftIndex, int rightIndex);

int getArrayMaximumInteger(int *array, int leftIndex, int rightIndex);

double getArrayAvg(int *array, int leftIndex, int rightIndex);

void relocatePeakPositions(double *signal, int *peakPositions, int *numberOfPeaks, int *resultPeakPositions);

int * getAllLocalMaximums_interval(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize);

int * getAllLocalMinimums_interval(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize);

int isMoreThanOnePeak(double *array, int leftIndex, int rightIndex, int peakToEval, int referencePeak);
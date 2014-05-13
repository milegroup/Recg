int applyCuiweiAlgorithm(double *signal, int peakPos1, int peakPos2, int referencePeak);

int areDifferentSign(double *signal, int peakPos1, int peakPos2);

int compareTwoPeaksAndRemoveRedundant(double *signal, int peakPos1, int peakPos2, int referencePeak);

int selectCorrectAdjacentPeakFromArray(double *signal, int *possibleAdjacentPeaks, int currentPeak);

int * selectCorrectMML(double *signal, int maxPos1, int minPos1, int minPos2, int maxPos2);

int * getMML_interval(double *signal, int signalLength, int leftIndex, int rightIndex, int searchIntervalSize);
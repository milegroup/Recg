WaveletDetector <- function(ECGData, channel = 1, ... ){
  # Performs a Wavelet transform QRS detection algorithm
  # First the wavelet transformation is applyed, then in each scale the maximum modulus lines are searched, as the maximum values exceeding
  # a threshold. Next, the isolated maximum lines are discarded, only the maximum lines that appear in all scales are taken in account.
  # Finally, the QRS position is set in the zero crossing between a positive maximum and a negative minimum.
  # 
  # Args:
  #   ECGData: ECG data model
  #   channel: In which channel the algorithm is applied
  #
  # Returns:
  #   The data model with the QRS positions added

  # Apply Wavelet transform
  library(waveslim)
  signal <- ECGData$lead[[channel]]$data  
  #wt <- mra(signal, wf="la8", J=5, method="modwt")
  wt <- mra(signal, wf="haar", J=5, method="modwt") 
  
  

  # Obtain maximum modulus lines for each scale, saving only those positions present in all scales 
  maximum.modulus <- WaveletGetMaximumModulus(wt, ECGData$samplingFreq)

  
  positions <- maximum.modulus$positions
  heights <- maximum.modulus$heights
  modulus <- data.frame(positions, heights)


  # Place the fiducial mark in the zero crossing, between the minimum and maximum points around the maximum modulus position
  peaks <- WaveletGetPeaks(wt$D1, modulus, ECGData$samplingFreq)  
  #peaks <- WaveletGetPeaks(wt$D2, modulus, ECGData$samplingFreq)


  peaks <- data.frame(positions, heights)
  return(modulus)
}

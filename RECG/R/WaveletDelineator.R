WaveletDelineator <- function(ECGData, channel = 1, ... ){
  # Performs the ECG delineation based on a Waveform transform 
  # 
  # Args:
  #   ECGData: ECG data model
  #   channel: In which channel the algorithm is applied
  #
  # Returns:
  #   The data model with the ECG signal delineated 

  # Apply the wavelet transform
  library(waveslim)
  signal <- ECGData$lead[[channel]]$data
  
  #wt <- mra(signal, wf="la8", J=5, method="modwt")
  wt <- mra(signal, wf="haar", J=5, method="modwt") 


  # QRS delineation
  #WaveletDelineatorQRS(ECGData, wt$D2, wt$D1)    
  ECGData <- WaveletDelineatorQRS(ECGData, wt$D2, wt$D1)  
  #ECGData <- WaveletDelineatorQRS(ECGData, wt$D3, wt$D1)  
  #ECGData <- WaveletDelineatorTWave(ECGData, wt$D4, wt$D5)

  return(ECGData)  
}
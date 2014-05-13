BeatDetectorWavelet <- function(ECGData, channel = 1, ... ){
  # 
  # 
  # Args:
  #   ECGData: ECG data model
  #   channel: In which channel the algorithm is applied
  #
  # Returns:
  #   The data model with the QRS positions added

  # Apply the wavelet transform
  ECGData <- WaveletTransform(ECGData,channel)  

  # Get all the maximum modulus lines for each wavelet scale
  ECGData <- BeatDetectorWaveletGetAllMML(ECGData)

  # Check which MML of each scale match and gets the fiducial mark
  ECGData <- BeatDetectorWaveletGetFiducialPositions(ECGData, channel)


  # Prueba, cambiar las posiciones a frecuencia de muestreo 360
  if( ECGData$samplingFreq == 250){
    #cat("\nCambiando posiciones a samplingFreq: 360")    
    #ECGData$annot$pos <- round(ECGData$annot$pos * 360 / 250)
  }


  return(ECGData)
}
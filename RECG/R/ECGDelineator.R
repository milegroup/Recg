ECGDelineator <- function(ECGData, algorithm, channel = 1, ...){
  # Performs the beat delineation, according to the selected algorithm
  # 
  # Args:
  #   ECGData: The ECG data model
  #   channel: In which channel apply the delineation
  #
  # Returns:
  #   The data model with the delineation annotations added

  if (missing(ECGData)){
    stop("ECGData param is mandatory")
  }

  if (missing(algorithm)){
    stop("Algorithm param is mandatory")
    #algorithm <- SelectQRSAlgorithm()
  }
  if (algorithm != 1 && algorithm != 2){
    stop("You have selected a non valid algorithm number\n")
  }

  if(algorithm == 1){
    ECGData <- ECGDelineatorWavelet(ECGData, channel)
  }
  if(algorithm == 2){
    ECGDelineatorPhasor(ECGData, channel)
  }

  return(ECGData)
}
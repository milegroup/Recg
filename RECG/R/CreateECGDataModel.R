CreateECGDataModel <- function(Verbose = FALSE, ...) {
  # Creates the data model
  # 
  # Args:
  #   Verbose: Enable verbose mode (True or False)
  #
  # Returns:
  #   The created data model 

  if (Verbose) {
    cat("** Creating data model **\n")
  }

  ECGData <- list()

    ECGData$numLeads <- NULL # Number of leads
    ECGData$samplingFreq <- NULL # Sampling frequency
    ECGData$samplesPerChannel <- NULL # Number of samples for the entire ECG file
    ECGData$date <- NULL
    ECGData$time <- NULL
    ECGData$lead <- list() # Each lead information. List containing:
      # Name: Lead name
      # Data: Lead ECG signal data, samples in mv
    ECGData$annot <- list()
    	ECGData$annot$pos <- NULL # Array containing annotation positions, measured in samples
      ECGData$annot$height <- NULL # Array containing annotation heights
    	ECGData$annot$type <- NULL # String array containing the type of annotation
    	ECGData$annot$lead <- NULL # In which lead is annotated
    	ECGData$annot$value <- NULL # Real value
    	ECGData$annot$comment <- NULL # String

    ECGData$delineation <- list()
      ECGData$delineation$qrs.onset.positions <- NULL
      ECGData$delineation$qrs.end.positions <- NULL
      ECGData$delineation$q.peak.positions <- NULL
      ECGData$delineation$s.peak.positions <- NULL
      ECGData$delineation$p.peak.positions <- NULL

    ECGData$modulus <- list()
      ECGData$modulus$scale1 <- NULL
      ECGData$modulus$scale2 <- NULL
      ECGData$modulus$scale3 <- NULL
      ECGData$modulus$scale4 <- NULL

  ECGData$Verbose <- Verbose
  
  if (Verbose) {
    cat("   Data model created\n")
  }

  return(ECGData)
}


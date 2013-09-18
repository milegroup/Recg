CreateECGData <- function(Verbose = FALSE, ...) {
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
      ECGData$delineation$n.pre.position <- NULL
      ECGData$delineation$n.pre.height <- NULL
      ECGData$delineation$n.post.position <- NULL
      ECGData$delineation$n.post.height <- NULL
      ECGData$delineation$n.first.position <- NULL
      ECGData$delineation$n.first.height <- NULL
      ECGData$delineation$n.last.position <- NULL
      ECGData$delineation$n.last.height <- NULL
      ECGData$delineation$qrs.onset.position <- NULL
      ECGData$delineation$qrs.onset.height <- NULL
      ECGData$delineation$qrs.end.position <- NULL
      ECGData$delineation$qrs.end.height <- NULL
      ECGData$delineation$q.peak.position <- NULL
      ECGData$delineation$q.peak.height <- NULL
      ECGData$delineation$s.peak.position <- NULL
      ECGData$delineation$s.peak.height <- NULL


      # Prueba borrar ############
      ECGData$delineation$qrs.onset.position.criteria1
      ECGData$delineation$qrs.onset.height.criteria1
      ECGData$delineation$qrs.onset.position.criteria2
      ECGData$delineation$qrs.onset.heightcriteria2
      ECGData$delineation$qrs.end.position.criteria1
      ECGData$delineation$qrs.end.height.criteria1
      ECGData$delineation$qrs.end.position.criteria2
      ECGData$delineation$qrs.end.height.criteria2
      ############

  ECGData$Verbose <- Verbose
  
  if (Verbose) {
    cat("   Data model created\n")
  }

  return(ECGData)
}


BeatDetector <- function(ECGData, algorithm, channel = 1, ...){
  # Performs the beat (QRS) detection, according to the selected algorithm
  # 
  # Args:
  #   ECGData: The ECG data model
  #   algorithm: Number of the selected algorithm
  #   channel: In which channel apply the beat detection
  #
  # Returns:
  #   The data model with the QRS positions added

  if (missing(ECGData)){
    stop("ECGData param is mandatory")
  }

  if (missing(algorithm)){
    stop("Algorithm param is mandatory")
  }
  if (algorithm != 1 && algorithm != 2 && algorithm != 3 && algorithm != 4 && algorithm != 5 ){
    stop("You have selected a non valid algorithm number\n")
  }

  windowSize = 150 # ms

  # Convert window size from ms to samples
  ms.to.seconds = 1000
  windowSize.samples = trunc( windowSize * ECGData$samplingFreq ) / ms.to.seconds # 


  # Preprocessing the signal  
  preproInfo <- Preprocessor.PanTompkins(ECGData, channel, windowSize.samples)


  # Beat detection
  channel.size <- length(ECGData$lead[[channel]]$data)

  if (algorithm == 1 || algorithm == 2){
    
    if (algorithm == 1){
      peaks <- PanTompkins(ECGData, preproInfo, channel.size, windowSize.samples, channel)  
    }
    if (algorithm == 2){
      peaks <- HamiltonTompkins(ECGData, preproInfo, channel.size, windowSize.samples, channel)
    }
  }
  else{
    if(algorithm == 3){
      peaks <- Phasor(ECGData, channel)
    }
    else{
      if (algorithm == 4){
        peaks <- WaveletDetector(ECGData, channel)
      }
      else {
        if (algorithm == 5){ # combinacion algoritmos anteriores PRUEBA

          peaksPan <- PanTompkins(ECGData, preproInfo, channel.size, windowSize.samples, channel) 
          peaksHam <- HamiltonTompkins(ECGData, preproInfo, channel.size, windowSize.samples, channel)
          peaksPhasor <- Phasor(ECGData, channel)
          peaksWavelet <-  WaveletDetector(ECGData, channel)

          length.higher <- max(length(peaksPan$positions), length(peaksHam$positions), length(peaksPhasor$positions), length(peaksWavelet$positions))
          finalPositions <- mat.or.vec(length.higher, 1)

          out <- .C("comparePeaksAlgorithms", as.integer(peaksPan$positions), as.integer(peaksHam$positions), as.integer(peaksPhasor$positions), as.integer(peaksWavelet$positions), 
                    as.integer(length(peaksPan$positions)), as.integer(length(peaksHam$positions)), as.integer(length(peaksPhasor$positions)), as.integer(length(peaksWavelet$positions)), 
                    finalPositions = as.integer(finalPositions), as.integer(ECGData$samplingFreq) )
	  
          positions <- out$finalPositions
          #heights <- out$finalPositions
	        positions <- positions[which(positions !=0 )]
	        #heights <- heights[which(heights != 0 )]
          heights <- mat.or.vec(length(positions),1)

          peaks <- data.frame(positions, heights)
          
        }
      }
    }
  }

 
  

  # Save QRS positions in the data model
  ECGData$annot$pos <- peaks$positions  
  ECGData$annot$height <- peaks$heights
  ECGData$annot$lead <- ECGData$lead[[channel]]$name

  if (ECGData$Verbose){
    cat("   Beat positions detected \n")
  }

  return(ECGData)
}

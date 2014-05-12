BeatDetectorPanTompkins <- function(ECGData, preproInfo, channel.size, windowSize, channel, ...){
  # Performs the Pan & Tompkins QRS detection algorithm
  # 
  # Args:
  #   ECGData: ECG data model
  #   preproInfo: Contains the derivative and integrated signals from the preproccesing stage
  #   windowSize: Size of the window used in the preproccesing stage 
  #   channel.size: Length of the array that contains the signal on that channel
  #   channel: In which channel the algorithm is applied
  #
  # Returns:
  #   The data model with the QRS positions added


  if (missing(preproInfo)){
    stop("   preproInfo is a mandatory param")
  }

  if (missing(ECGData)){
    stop("   ECGData is a mandatory param")
  }

  if (ECGData$Verbose == TRUE){
    cat("   Applying Pan & Tompkins algorithm \n")
  }
  # Detection in the derivative signal
  all.peaks.derivative        <- PeakDetector(preproInfo$derivative, ECGData$samplingFreq, windowSize = 222)
  #thr.initial.derivative      <- GetInitialThreshold(preproInfo$derivative, ECGData$samplingFreq, coef = 0.8)
  thr.initial.derivative <- Rms(preproInfo$derivative[1:2^16])
  two.first.peaks.derivative  <- GetTwoFirstPeaks(preproInfo$derivative, thr.initial.derivative, preproInfo$derivative, ECGData$samplingFreq)
  

  # Detection in the mwi signal
  all.peaks.mwi       <- PeakDetector(preproInfo$mwi, ECGData$samplingFreq, windowSize = 222)
  #thr.initial.mwi     <- GetInitialThreshold(preproInfo$mwi, ECGData$samplingFreq, coef = 0.35)
  thr.initial.mwi <- Rms(preproInfo$mwi[1:2^16])
  two.first.peaks.mwi <- GetTwoFirstPeaks(preproInfo$mwi, thr.initial.mwi, preproInfo$derivative, ECGData$samplingFreq)


  #######       Apply Pan & Tompkins algorithm  #######

  # Get QRS positions from derivative signal
  derivative <- preproInfo$derivative
  allPeaksArraySize <- length(all.peaks.derivative$positions)
  signalPeakPositions <- mat.or.vec(channel.size, 1)
  signalPeakHeights <- mat.or.vec(channel.size, 1)
  SPKI <- 0.0
  NPKI <- 0.0

  out <- .C("panTompkinsAlg", as.double(derivative), as.integer(ECGData$samplingFreq), as.integer(two.first.peaks.derivative$positions),
            as.double(two.first.peaks.derivative$heights), as.double(thr.initial.derivative), as.integer(all.peaks.derivative$positions),
            as.double(all.peaks.derivative$heights), as.integer(allPeaksArraySize), signalPeakPositions = as.integer(signalPeakPositions),
            signalPeakHeights = as.double(signalPeakHeights), SPKI = as.double(SPKI), NPKI = as.double(NPKI), as.integer(windowSize) )
  
  positions <- out$signalPeakPositions[which(out$signalPeakPositions != 0)]
  heights   <- out$signalPeakHeights[which(out$signalPeakPositions != 0)]
  peaks.derivative <- data.frame(positions, heights)

  # Get QRS positions from integrated signal
  allPeaksArraySize <- length(all.peaks.mwi$positions)
  signalPeakPositions <- mat.or.vec(channel.size, 1)
  signalPeakHeights <- mat.or.vec(channel.size, 1)
  SPKI <- 0.0
  NPKI <- 0.0

  out <- .C("panTompkinsAlg", as.double(derivative), as.integer(ECGData$samplingFreq), as.integer(two.first.peaks.mwi$positions), 
            as.double(two.first.peaks.mwi$heights), as.double(thr.initial.mwi), as.integer(all.peaks.mwi$positions), 
            as.double(all.peaks.mwi$heights), as.integer(allPeaksArraySize), signalPeakPositions = as.integer(signalPeakPositions),
            signalPeakHeights = as.double(signalPeakHeights), SPKI = as.double(SPKI), NPKI = as.double(NPKI), as.integer(windowSize))
  positions <- out$signalPeakPositions[which(out$signalPeakPositions != 0)]
  heights   <- out$signalPeakHeights[which(out$signalPeakPositions != 0)]
  peaks.mwi <- data.frame(positions,heights)

  
  # Discard those positions which are not in both (derivative and integrated) signals
  peaks.positions <- mat.or.vec(channel.size, 1)  
  peaks.heights   <- mat.or.vec(channel.size, 1)

  out <- .C("samePeaks", as.integer(peaks.mwi$positions), as.double(peaks.mwi$heights), as.integer(peaks.derivative$positions),
            as.double(peaks.derivative$heights), as.integer(length(peaks.mwi$positions)), as.integer(length(peaks.derivative$positions)), 
            peaks.positions = as.integer(peaks.positions), peaks.heights = as.double(peaks.heights), as.integer(windowSize))

  positions <- out$peaks.positions[which(out$peaks.positions != 0)]
  heights   <- out$peaks.heights[which(out$peaks.positions != 0)]


  if ( length(positions) != length(heights)){
    message <- paste("Number of final peak positions: ", length(positions), "\n Number of final peak heights", length(heights), "\n", sep=" ")
    stop(message)
  }

  

  # Place QRS positions according to the DELAY produced in the preproccesing stage
  #halfWS <- trunc(windowSize / 2)
  DELAY  <- 6 + 16 #+ 2 #+ halfWS # 6 (1/2 lpf), 16 (1/2 hpf), 2 (derivative), halfWS (1/2 mwi)
  #DELAY <- max(windowSize || 32) # 32 é o desplazamento máximo (highpassFilter), se a windowSize que se utiliza na mwi é mayor ese debe ser o delay
  positions <- positions - DELAY

  # Place the fiducial point in the R peak, searching the maxima
  window.qrs <- 120 # ms
  window.qrs.samples <- trunc( window.qrs * ECGData$samplingFreq / 1000)
  for (i in 1:length(positions)){       
    p.ini <- positions[i] - window.qrs.samples 
    p.end <- positions[i] + window.qrs.samples 
    if (p.ini < 0){
      p.ini <- 0
    }
    heights[i] <- max(ECGData$lead[[channel]]$data[p.ini:p.end])
    positions[i] <- which(max(ECGData$lead[[channel]]$data[p.ini:p.end]) == heights[i])[1] + positions[i]
  }


  # Save final QRS positions
  final.peaks <- data.frame(positions, heights)

  return(final.peaks)    
}
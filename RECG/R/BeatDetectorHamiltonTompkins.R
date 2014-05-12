BeatDetectorHamiltonTompkins <- function(ECGData, preproInfo, channel.size, windowSize, channel , ... ){
  # Performs the Hamilton & Tompkins QRS detection algorithm
  # 
  # Args:
  #   ECGData: ECG data model
  #   preproInfo: Contains the derivative and integrated signals from the preproccesing stage
  #   channel.size: Length of the array that contains the signal on that channel
  #   windowSize: Size of the window used in the preproccesing stage 
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

  # Detection in the mwi signal
  all.peaks.mwi       <- PeakDetector(preproInfo$mwi, ECGData$samplingFreq, windowSize = 222)
  #thr.initial.mwi     <- GetInitialThreshold(preproInfo$mwi, ECGData$samplingFreq, coef = 0.2)
  thr.initial.mwi <- Rms(preproInfo$mwi[1:2^16])
  two.first.peaks.mwi <- GetTwoFirstPeaks(preproInfo$mwi, thr.initial.mwi, preproInfo$derivative, ECGData$samplingFreq)

  #######       Apply Hamilton & Tompkins algorithm  #######
  signalPeakPositions <- mat.or.vec(channel.size, 1)
  signalPeakHeights   <- mat.or.vec(channel.size, 1)
  out <- .C("hamiltonPeakDecision", as.double(preproInfo$derivative), as.integer(two.first.peaks.mwi$positions), as.double(two.first.peaks.mwi$heights),
            as.integer(all.peaks.mwi$positions), as.double(all.peaks.mwi$heights), as.integer(length(all.peaks.mwi$positions)), 
            signalPeakPositions = as.integer(signalPeakPositions), signalPeakHeights = as.double(signalPeakHeights), as.integer(ECGData$samplingFreq), 
            as.integer(windowSize) )

  positions <- out$signalPeakPositions[which(out$signalPeakPositions != 0)]
  heights   <- out$signalPeakHeights[which(out$signalPeakPositions != 0 )]
  peaks     <- data.frame(positions, heights)


  # Place the fiducial mark in the derivative signal (according to authors)
  resultPeakPositions <- mat.or.vec(length(peaks$positions), 1)
  resultPeakHeights   <- mat.or.vec(length(peaks$positions), 1)
  DELAY <- trunc(windowSize / 2)

  window.qrs <- 200 # ms
  window.qrs.samples <- trunc( window.qrs * ECGData$samplingFreq / 1000)
  out  <- .C("placeFiducialMark", as.double(preproInfo$derivative), as.integer(peaks$positions),
            as.integer(length(peaks$positions)), resultPeakPositions = as.integer(resultPeakPositions), resultPeakHeights = as.double(resultPeakHeights),
            as.integer(DELAY), as.integer(window.qrs.samples))

  positions <- out$resultPeakPositions
  heights   <- out$resultPeakHeights

  # Place QRS positions according to the DELAY produced in the preproccesing stage
  DELAY <- 6 + 16 + 2
  positions <- positions - DELAY

  # Place the fiducial point in the R peak, searching the maxima
  window.qrs <- 100 # ms
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

  final.peaks <- data.frame(positions, heights)

}
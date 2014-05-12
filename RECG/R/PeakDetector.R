PeakDetector <- function(signal, samplingFreq, windowSize = 222, ...){
  # Detects local maxima peaks in the signal. The maxima is considered when is the higher peak in a window
  # 	
  # Args:
  #   signal: Signal where peaks are detected
  #   samplingFreq: Signal sampling frequency
  #   windowSize: The window size (ms) where the maxima is searched
  #   
  # Returns:
  #   A data frame containing the peaks positions and heights
  
  windowSize.samples <- windowSize / 1000 * samplingFreq # Convert from ms to samples

  signal.size   <- length(signal)
  positions <- mat.or.vec(signal.size, 1)
  heights   <- mat.or.vec(signal.size, 1) 
  
  out <- .C("peakDetector", as.double(signal), peakPositions = as.integer(positions),
            peakHeights = as.double(heights), as.integer(signal.size), as.integer(windowSize.samples) )

  positions <- out$peakPositions[which(out$peakPositions != 0)]
  heights   <- out$peakHeights[which(out$peakHeights != 0)]

  peaks <- data.frame(positions, heights)
  return(peaks)	
}
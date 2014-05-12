GetInitialThreshold <- function(signal, samplingFreq, coef, ...){
  # Stablish the initial threshold according to the first maxima and minima peaks
  # 
  # Args:
  #   signal: ECG signal where the threshold is stablished
  #   samplingFreq: Signal sampling frequency
  #   coef: A ratio indicating how to raise the threshold according to the difference between minima and maxima peaks
  #
  # Returns:
  #   The threshold
    
  # Calculate the segment size in samples for 3 seconds duration 
  segment.size <- samplingFreq * 3
  segment.size <- trunc(segment.size)
  number.of.segments <- 8

  # Calculate the distance between segments
  first.segments <- mat.or.vec(number.of.segments, 1)
  out <- .C("getFirstSegments", firstSegments = as.integer(first.segments), as.integer(segment.size), as.integer(number.of.segments))
  first.segments <- out$firstSegments

  # Maxima in those segments
  max.first.segments <- mat.or.vec(number.of.segments, 1)
  max.first.segments.pos <- mat.or.vec(number.of.segments, 1)

  # Minima in those segments
  min.first.segments <- mat.or.vec(number.of.segments, 1)
  min.first.segments.pos <- mat.or.vec(number.of.segments, 1)

  # For each segment gets the maxima and minima
  left.side <- 1
  for (i in 1: number.of.segments){
    max.first.segments[i] <- max(signal[left.side:first.segments[i]])
    max.first.segments.pos[i] <- left.side + which(max.first.segments[i] == signal[left.side:first.segments[i]])[1] # in case there are many maxima, only get the first one

    min.first.segments[i] <- min(signal[left.side:first.segments[i]])
    min.first.segments.pos[i] <- left.side + which(min.first.segments[i] == signal[left.side:first.segments[i]])[1] # in case there are many minima, only get the first one

    left.side = first.segments[i]
  }

  # Calculate threshold
  max.median <- median(max.first.segments)
  min.median <- median(min.first.segments)
  thr <- min.median + coef * (max.median - min.median)
  
  return(thr)
}
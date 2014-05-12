GetTwoFirstPeaks <- function(signal, thr, derivative, samplingFreq, ...){
  # Get the two first peaks from all detected peaks that exceed the threshold
  # 
  # Args:
  #   signal: ECG signal where to detect the peaks
  #   thr: Threshold
  #   derivative: Derivative signal (from preprocessing), used to remove false QRS detection (usually T waves)
  #
  # Returns:
  #   A data frame containing the two first peaks 

  if (missing(signal)){
    stop("   Signal is a mandatory param")
  }
  if (missing(thr)){
    stop("   thr is a mandaroty param")
  }
  if (missing(derivative)){
    stop("   derivative is a mandaroty param")
  }
  if (missing(samplingFreq)){
    stop("   samplingFreq is a mandaroty param")
  }


  greater.than.thr <- (signal > thr) # save positions that exceed the threshold
  differences <- diff(greater.than.thr, differences = 1) # differences between adjacent positions. 

  left <- which(differences == 1) # First positions exceeding the threshold
  right <- which(differences == -1) # Last positions exceeding the threshold

  peak.height1 <- max(signal[left[1]:right[1]]) # first peak
  peak.height2 <- max(signal[left[2]:right[2]]) # second peak

  peak.position1 <- which(signal[left[1]:right[1]] == peak.height1) # get the peak position
  peak.position1 <- peak.position1 + left[1] # add the offset

 
  pos <- 2
  peak.position2 <- which(signal[left[pos]:right[pos]] == peak.height2) # get the peak position
  peak.position2 <- peak.position2 + left[pos] # add the offset

  # Remove while is T Wave
  while (IsTWave(derivative, peak.position1, peak.position2, samplingFreq)){
    pos <- pos + 1
    peak.height2 <- max(signal[left[pos]:right[pos]])
    peak.position2 <- which(signal[left[pos]:right[pos]] == peak.height2)
    peak.position2 <- peak.position2 + left[pos]
  }

  # Seek the maxima value in the two first intervals exceeding the threshold
  windowQRS <- 100 # ms
  windowQRS.samples <- trunc(windowQRS * samplingFreq / 1000)
  p.ini.1 <- peak.position1 - windowQRS.samples
  p.end.1 <- peak.position1 + windowQRS.samples
  p.ini.2 <- peak.position2 - windowQRS.samples
  p.end.2 <- peak.position2 + windowQRS.samples
  if (p.ini.1 < 0){
    p.ini.1 <- 0
  }
  peak.height1 <- max(signal[p.ini.1:p.end.1])
  peak.height2 <- max(signal[p.ini.2:p.end.2])
  peak.position1 <- which( max(signal[p.ini.1:p.end.1]) == peak.height1) + p.ini.1
  peak.position2 <- which( max(signal[p.ini.2:p.end.2]) == peak.height2) + p.ini.2

  # Save and return two first peaks
  positions <- c(peak.position1, peak.position2)
  heights <- c(peak.height1, peak.height2)
  peaks <- data.frame(positions, heights)

}
IsTWave <- function(derivative, previous.peak.position, current.peak.position, samplingFreq, ...){
  # Checks if a peak position is a T wave, according to Pan & Tompkins
  # 
  # Args:
  #   derivative: Derivative signal from preproccesing stage
  #   previous.peak.position: The peak before the current
  #   current.peak.position: Current peak position
  #   samplingFreq: Signal sampling frequency
  #
  # Returns:
  #   Whether the peak is a T wave or not


  refractory.period.200 <- samplingFreq * 0.2  # 200 ms
  refractory.period.360 <- samplingFreq * 0.36 # 360 ms

  if ((current.peak.position - previous.peak.position) < refractory.period.200){
    return (TRUE)
  }
  if (((current.peak.position - previous.peak.position) > refractory.period.200) && ((current.peak.position - previous.peak.position) < refractory.period.360)){
    if((derivative[current.peak.position] * 2) < (derivative[previous.peak.position])){
      return (TRUE)
    }
    else{
      return (FALSE)
    }
  }
  else{
    return (FALSE)
  }

}
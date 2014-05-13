BeatDetectorPhasor <- function(ECGData, channel =1,  Rv = 0.001){
  # Performs the Phasor trasnform QRS detection algorithm
  # 
  # Args:
  #   ECGData: ECG data model
  #   channel: In which channel the algorithm is applied
  #   Rv: A value that determines the degree with which ECG waves are enhanced in the phasorial signal
  #
  # Returns:
  #   The data model with the QRS positions added

  library(signal)

  # Normalization
  window.size <- 1000
  signal.size <- length(ECGData$lead[[channel]]$data)
  normalized <- mat.or.vec(signal.size, 1)

  out <- .C("getNormalizedSignal", as.double(ECGData$lead[[channel]]$data), as.integer(signal.size),
            as.integer(window.size), normalized = as.double(normalized))
  normalized <- out$normalized

  filter.order <- 5
  filter.frequency <- 0.003
  bf <- butter(filter.order, filter.frequency, type = "high")
  #filtered.signal <- filtfilt(bf, ECGData$lead[[channel]]$data)
  filtered.signal <- filtfilt(bf, normalized)

  phase <- atan(abs(filtered.signal) / Rv)
  magnitude <- sqrt(Rv ** 2 + filtered.signal ** 2)

  # Detection threshold
  threshold <- ( (pi / 2) - 0.003)

  window.qrs <- 200 # ms
  window.qrs.samples <- trunc( window.qrs * ECGData$samplingFreq / 1000)
  peak.positions <- mat.or.vec(length(phase), 1)
  peak.heights   <- mat.or.vec(length(phase), 1)
  out <- .C("phasorAlg", as.double(phase), as.double(magnitude), as.double(threshold), 
            as.integer(length(phase)), peak.positions = as.integer(peak.positions),
            peak.heights = as.double(peak.heights), as.integer(ECGData$samplingFreq), as.integer(window.qrs.samples))

  positions <- out$peak.positions[which(out$peak.positions != 0)]
  heights   <- out$peak.heights[which(out$peak.positions != 0)]

  peaks <- data.frame(positions, heights)
  return(peaks)
}
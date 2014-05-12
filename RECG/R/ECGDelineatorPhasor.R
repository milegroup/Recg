ECGDelineatorPhasor <- function(ECGData, channel = 1, ... ){
  # Performs the ECG delineation based on the Phasor algorithm 
  # 
  # Args:
  #   ECGData: ECG data model
  #   channel: In which channel the algorithm is applied
  #
  # Returns:
  #   The data model with the ECG signal delineated 
    

  n.qrs <- ECGData$annot$pos

  # Aplicar transformada Phasor
  library(signal)
  Rv <- 0.001
  filter.order <- 5
  filter.frequency <- 0.003
  bf <- butter(filter.order, filter.frequency, type = "high")
  filtered.signal <- filtfilt(bf, ECGData$lead[[channel]]$data)

  phase <- atan(abs(filtered.signal) / Rv)  
  magnitude <- sqrt(Rv ** 2 + filtered.signal ** 2)

  thr <- (pi / 2)

  plot(phase[1:5000],type='l')
  abline(thr,0,col="blue")
  abline(0.25 * thr, 0, col="green")


}
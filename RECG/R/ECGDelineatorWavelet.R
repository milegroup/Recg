ECGDelineatorWavelet <- function(ECGData, channel = 1, ... ){
  # Performs the ECG delineation based on a Waveform transform 
  # 
  # Args:
  #   ECGData: ECG data model
  #   channel: In which channel the algorithm is applied
  #
  # Returns:
  #   The data model with the ECG signal delineated 

  # Apply the wavelet transform
  ECGData <- WaveletTransform(ECGData,channel)

  # QRS delineation
  #ECGData <- ECGDelineatorWaveletQRS(ECGData)

  scale1 <- ECGData$wt$W1
  scale2 <- ECGData$wt$W2
  n.qrs <- ECGData$annot$pos
  length.qrs <- 0.12 * ECGData$samplingFreq
  interval.size <- 512

  qrs.onset.positions <- mat.or.vec(length(n.qrs), 1) 
  qrs.end.positions <- mat.or.vec(length(n.qrs), 1)
  q.peak.positions <- mat.or.vec(length(n.qrs), 1)
  s.peak.positions <- mat.or.vec(length(n.qrs), 1)

  npre.positions <- mat.or.vec(length(n.qrs),1)
  npost.positions <- mat.or.vec(length(n.qrs),1)
  nfirst.positions <- mat.or.vec(length(n.qrs),1)
  nlast.positions <- mat.or.vec(length(n.qrs),1)

  out <- .C("delineateQRS", as.double(scale1), as.double(scale2), as.integer(length(scale1)), as.integer(length(scale2)), 
                as.integer(n.qrs), as.integer(length(n.qrs)), as.integer(length.qrs), as.integer(interval.size), 
                qrs.onset.positions = as.integer(qrs.onset.positions), qrs.end.positions = as.integer(qrs.end.positions), 
                q.peak.positions =  as.integer(q.peak.positions), s.peak.positions = as.integer(s.peak.positions),
                npre.positions = as.integer(npre.positions), npost.positions = as.integer(npost.positions),
                nfirst.positions = as.integer(nfirst.positions), nlast.positions = as.integer(nlast.positions) )
  
  ECGData$delineation$qrs.onset.positions <- as.integer(out$qrs.onset.positions) #  ( out$qrs.onset.positions + 1 ) # Suma 1 para compensar con C
  ECGData$delineation$qrs.end.positions <- as.integer(out$qrs.end.positions) #( out$qrs.end.positions + 1) 
  ECGData$delineation$q.peak.positions <- as.integer(out$q.peak.positions) #( out$q.peak.positions + 1)
  ECGData$delineation$s.peak.positions <- as.integer(out$s.peak.positions) #( out$s.peak.positions + 1)

  ECGData$delineation$npre.positions <- as.integer(out$npre.positions) #(out$npre.positions + 1)
  ECGData$delineation$npost.positions <- as.integer(out$npost.positions) #(out$npost.positions + 1)
  ECGData$delineation$nfirst.positions <- as.integer(out$nfirst.positions) #(out$nfirst.positions + 1)
  ECGData$delineation$nlast.positions <- as.integer(out$nlast.positions) #(out$nlast.positions + 1)

  ECGData <- ECGDelineatorWaveletPWave(ECGData)
  ECGData <- ECGDelineatorWaveletTWave(ECGData)


  return(ECGData)  
}
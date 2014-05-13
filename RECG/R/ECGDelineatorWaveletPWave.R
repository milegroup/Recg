ECGDelineatorWaveletPWave <- function(ECGData){
  # 
  # 
  # Args:
  #   ECGData: ECG data model 
  #
  # Returns:
  #   The ECG data model with the delineation positions added

  scale3 <- ECGData$wt$W3
  scale4 <- ECGData$wt$W4
  scale5 <- ECGData$wt$W5
  r.positions <- ECGData$annot$pos
  qrs.onset.positions <- ECGData$delineation$qrs.onset.positions

  rr.array.size <- 10
  r.interval <- r.positions[1:rr.array.size]
  rr.mean <- mean(diff(r.interval, 1))

  p.positions <- mat.or.vec(length(r.positions), 1)
  maxPos <- mat.or.vec(length(r.positions), 1)
  minPos <- mat.or.vec(length(r.positions), 1)
  leftIndex <- mat.or.vec(length(r.positions), 1)
  rightIndex <- mat.or.vec(length(r.positions), 1)
  onsetPos <- mat.or.vec(length(r.positions), 1)
  endPos <- mat.or.vec(length(r.positions), 1)

  out <- .C("delineatePWave", as.double(scale3), as.double(scale4), as.double(scale5), as.integer(length(scale3)), as.integer(length(scale4)), 
                              as.integer(length(scale5)), as.integer(r.positions), as.integer(length(r.positions)), as.integer(qrs.onset.positions), 
                              as.integer(rr.mean), as.integer(rr.array.size), p.positions = as.integer(p.positions),
                              maxPos = as.integer(maxPos), minPos = as.integer(minPos), leftIndex = as.integer(leftIndex), rightIndex = as.integer(rightIndex),
                              onsetPos = as.integer(onsetPos), endPos = as.integer(endPos) )

  cat("\n P wave delineada")

  ECGData$delineation$p.peak.positions <- out$p.positions[which(out$p.positions != 0)]
  ECGData$delineation$p.maxPos.peak.positions <- out$maxPos[which(out$maxPos != 0)]
  ECGData$delineation$p.minPos.peak.positions <- out$minPos[which(out$minPos != 0)]
  ECGData$delineation$p.leftIndex <- out$leftIndex[which(out$leftIndex != 0)]
  ECGData$delineation$p.rightIndex <- out$rightIndex[which(out$rightIndex != 0)]
  ECGData$delineation$p.onset <- out$onsetPos[which(out$onsetPos != 0)]
  ECGData$delineation$p.end <- out$endPos[which(out$endPos != 0)]


  return(ECGData)
}
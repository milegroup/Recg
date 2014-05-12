ECGDelineatorWaveletTWave <- function(ECGData){
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
  qrs.end.positions <- ECGData$delineation$qrs.end.positions

  rr.array.size <- 10
  r.interval <- r.positions[1:rr.array.size]
  rr.mean <- mean(diff(r.interval, 1))


  t.positions <- mat.or.vec(length(r.positions), 1)
  maxPos <- mat.or.vec(length(r.positions), 1)
  minPos <- mat.or.vec(length(r.positions), 1)
  leftIndex <- mat.or.vec(length(r.positions), 1)
  rightIndex <- mat.or.vec(length(r.positions), 1)
  onsetPos <- mat.or.vec(length(r.positions), 1)
  endPos <- mat.or.vec(length(r.positions), 1)

  out <- .C("delineateTWave", as.double(scale3), as.double(scale4), as.double(scale5), as.integer(length(scale3)), as.integer(length(scale4)), 
                              as.integer(length(scale5)), as.integer(r.positions), as.integer(length(r.positions)), as.integer(qrs.onset.positions), 
                              as.integer(qrs.end.positions),
                              as.integer(rr.mean), as.integer(rr.array.size), t.positions = as.integer(t.positions),
                              maxPos = as.integer(maxPos), minPos = as.integer(minPos), leftIndex = as.integer(leftIndex), rightIndex = as.integer(rightIndex),
                              onsetPos = as.integer(onsetPos), endPos = as.integer(endPos))

  ECGData$delineation$t.peak.positions <- out$t.positions[which(out$t.positions != 0)]
  ECGData$delineation$t.maxPos.peak.positions <- out$maxPos[which(out$maxPos != 0)]
  ECGData$delineation$t.minPos.peak.positions <- out$minPos[which(out$minPos != 0)]
  ECGData$delineation$t.leftIndex <- out$leftIndex[which(out$leftIndex != 0)]
  ECGData$delineation$t.rightIndex <- out$rightIndex[which(out$rightIndex != 0)]
  ECGData$delineation$t.onset <- out$onsetPos[which(out$onsetPos != 0)]
  ECGData$delineation$t.end <- out$endPos[which(out$endPos != 0)]




  return(ECGData)
}
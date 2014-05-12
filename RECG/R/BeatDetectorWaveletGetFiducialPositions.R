BeatDetectorWaveletGetFiducialPositions <- function(ECGData, channel, ...){

  m.pos.s1 <- ECGData$modulus$scale1
  m.pos.s2 <- ECGData$modulus$scale2
  m.pos.s3 <- ECGData$modulus$scale3
  m.pos.s4 <- ECGData$modulus$scale4


  bigger <- max( length(m.pos.s4), length(m.pos.s3), length(m.pos.s2), length(m.pos.s1) )
  modulus.result <- mat.or.vec( bigger, 1)

  # Para facilitar la comprobación en C se pasa el tamaño en la escala 1
  m.pos.s4 <- m.pos.s4 * 8
  m.pos.s3 <- m.pos.s3 * 4
  m.pos.s2 <- m.pos.s2 * 2

  #rr.mean <- mean(abs(diff(m.pos.s3,1)))
  #out <- .C("getFinalMaximumModulusLines_boichat", as.integer(m.pos.s1), as.integer(m.pos.s2), as.integer(m.pos.s3), as.integer(m.pos.s4),
   #                                                as.integer(length(m.pos.s1)), as.integer(length(m.pos.s2)), as.integer(length(m.pos.s3)), 
    #                                               as.integer(length(m.pos.s4)), modulus.result = as.integer(modulus.result), as.double(rr.mean) )

  out <- .C("getFinalMaximumModulusLines", as.integer(m.pos.s1), as.integer(m.pos.s2), as.integer(m.pos.s3), as.integer(m.pos.s4),
                                               as.integer(length(m.pos.s1)), as.integer(length(m.pos.s2)), as.integer(length(m.pos.s3)),
                                               as.integer(length(m.pos.s4)), modulus.result = as.integer(modulus.result))

  modulus.result <- out$modulus.result[which(out$modulus.result != 0)]  
  out <- .C("relocatePeakPositions", as.double(ECGData$wt$W1), as.integer(modulus.result), as.integer(length(modulus.result)), 
            modulus.result = as.integer(modulus.result))

  ECGData$modulus$result <- out$modulus.result[which(out$modulus.result != 0)]
  
  annot.pos <- mat.or.vec(length(ECGData$modulus$result), 1)

  #peakInterval <- 7
  #out <- .C("getZeroCrossing", as.double(ECGData$wt$W1), as.integer(length(ECGData$wt$W1)), as.integer(ECGData$modulus$result), 
   #         as.integer(length(ECGData$modulus$result)), as.integer(peakInterval), as.integer(ECGData$samplingFreq), annot.pos = as.integer(annot.pos) )
  
  search.window.size <- 0.12 * ECGData$samplingFreq
  window <- round(search.window.size) * 2
  out <- .C("getZeroCrossing", as.double(ECGData$wt$W1), as.integer(length(ECGData$wt$W1)), as.integer(ECGData$modulus$result), 
            as.integer(length(ECGData$modulus$result)), as.integer(window), annot.pos = as.integer(annot.pos) )

  ECGData$annot$pos <- out$annot.pos[which(out$annot.pos != 0)]

  signal <- ECGData$lead[[channel]]$data
  annot.pos <- mat.or.vec(length(ECGData$annot$pos),1)
  out <- .C("relocatePeakPositions", as.double(signal), as.integer(ECGData$annot$pos), as.integer(length(ECGData$annot$pos)), 
            annot.pos = as.integer(annot.pos))

  ECGData$annot$pos <- out$annot.pos[which(out$annot.pos != 0)]

  return(ECGData)  
}
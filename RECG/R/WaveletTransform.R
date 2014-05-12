WaveletTransform <- function(ECGData, channel = 1, ... ){
  signal <- ECGData$lead[[channel]]$data

  # Prueba utilizar base de wavelets como articulo J. Martínez
  wt <- list()
  wt$W1 <- mat.or.vec(length(signal),1)
  wt$S1 <- mat.or.vec(length(signal),1)
  wt$W2 <- mat.or.vec(length(signal)/2,1)
  wt$S2 <- mat.or.vec(length(signal)/2,1)
  wt$W3 <- mat.or.vec(length(signal)/4,1)
  wt$S3 <- mat.or.vec(length(signal)/4,1)
  wt$W4 <- mat.or.vec(length(signal)/8,1)
  wt$S4 <- mat.or.vec(length(signal)/8,1)
  wt$W5 <- mat.or.vec(length(signal)/16,1)
  wt$S5 <- mat.or.vec(length(signal)/16,1)

  out <- .C("getQuadraticSplineWavelet250", as.double(signal), as.integer(length(signal)), W1 = as.double(wt$W1), S1 = as.double(wt$S1),
            W2 = as.double(wt$W2), S2 = as.double(wt$S2), W3 = as.double(wt$W3), S3 = as.double(wt$S3), W4 = as.double(wt$W4), 
            S4 = as.double(wt$S4), W5 = as.double(wt$W5), S5 = as.double(wt$S5))

  ECGData$wt$W1 <- out$W1
  ECGData$wt$S1 <- out$S1
  ECGData$wt$W2 <- out$W2
  ECGData$wt$S2 <- out$S2
  ECGData$wt$W3 <- out$W3
  ECGData$wt$S3 <- out$S3
  ECGData$wt$W4 <- out$W4
  ECGData$wt$S4 <- out$S4
  ECGData$wt$W5 <- out$W5
  ECGData$wt$S5 <- out$S5

  return(ECGData)
}
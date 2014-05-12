BeatDetectorWaveletGetAllMML <- function(ECGData, ... ){
  samplingFreq <- ECGData$samplingFreq  
  size <- 512
  #size <- 2 ^ 16
  
  # ------------------------ Detectar modulus lines escala 4
  scale <- 4
  result <- BeatDetectorWaveletGetMMLAtScale(ECGData$wt$W4, samplingFreq, scale, size)  
  peaks.s4 <- result$peaks.positions
  modulus.pos.s4 <- result$maximum.modulus.positions
  # ------------------------ Detectar modulus lines escala 3
  scale <- 3
  result <- BeatDetectorWaveletGetMMLAtScale(ECGData$wt$W3, samplingFreq, scale, size)
  peaks.s3 <- result$peaks.positions
  modulus.pos.s3 <- result$maximum.modulus.positions
  # ------------------------ Detectar modulus lines escala 2
  scale <- 2
  result <- BeatDetectorWaveletGetMMLAtScale(ECGData$wt$W2, samplingFreq, scale, size)
  peaks.s2 <- result$peaks.positions
  modulus.pos.s2 <- result$maximum.modulus.positions

  # ------------------------ Detectar modulus lines escala 1
  scale <- 1
  result <- BeatDetectorWaveletGetMMLAtScale(ECGData$wt$W1, samplingFreq, scale, size)
  peaks.s1 <- result$peaks.positions
  modulus.pos.s1 <- result$maximum.modulus.positions

 

  ECGData$modulus$scale4 <- modulus.pos.s4
  ECGData$modulus$scale3 <- modulus.pos.s3
  ECGData$modulus$scale2 <- modulus.pos.s2
  ECGData$modulus$scale1 <- modulus.pos.s1

  # Prueba borrar
  ECGData$modulus$peaks4 <- peaks.s4
  ECGData$modulus$peaks3 <- peaks.s3
  ECGData$modulus$peaks2 <- peaks.s2
  ECGData$modulus$peaks1 <- peaks.s1


  return(ECGData)
}
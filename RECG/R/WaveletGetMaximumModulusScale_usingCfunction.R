WaveletGetMaximumModulusScale_usingCfunction <- function(wt, size = 2^16, samplingFreq, scale, ...){
  # Receives as a parameter one Wavelet scale and extracts its maximum modulus lines using the C function peakDetector.
  # 
  # 
  # Args:
  #   wt: Wavelet transform signal
  #   size: Number of samples using to update the threshold
  #   samplingFreq: Signal sampling frequency
  #   scale: In which scale search maximum modulus. Scale 4 needs a different threshold
  #
  # Returns:
  #   An array containing the maximum modulus lines positions and heights

  # Suponiendo una longitud del QRS de 100 ms
  length.qrs <- 0.2 * samplingFreq

  refractoryPeriod200 <- 0.2 * samplingFreq
  refractoryPeriod360 <- 0.36 * samplingFreq
  
  left.index <- 1
  right.index <- size
  maximum <- mat.or.vec(length(wt), 1)
  final.maximum <- mat.or.vec(length(wt), 1)
  maximum.heights <- mat.or.vec(length(wt), 1)
  final.maximum.heights <- mat.or.vec(length(wt), 1)
  maximum.index <- 1

  out <- .C("peakDetector", as.double(abs(wt)), maximum = as.integer(maximum), maximum.heights = as.double(maximum.heights), 
            as.integer(length(wt)), as.integer(length.qrs))

  maximum <- out$maximum[which(out$maximum != 0)]  
  maximum.heights <- out$maximum.heights[which(out$maximum != 0)]  
  #maximum <- maximum + 1 # Porque las posiciones están detectadas en C donde los arrays tienen un índice menos


  loop.iter <- 1
  while( right.index < length(wt) ){

    
    # Update indexes and threshold
    if( loop.iter != 1){
      left.index <- right.index
      right.index <- right.index + size
    }
    if(right.index > length(wt)){
      right.index <- length(wt)
    }
    if( missing(scale)){
      thr.qrs <- Rms(abs(wt[left.index:right.index]))
      #thr.qrs <- Rms(wt[left.index:right.index])
    }
    else{
      if(scale != 4){
        thr.qrs <- Rms(abs(wt[left.index:right.index]))
        #thr.qrs <- Rms(wt[left.index:right.index])
      }
      else{
        thr.qrs <- 0.5 * Rms(abs(wt[left.index:right.index]))
        #thr.qrs <- 0.5 * Rms(wt[left.index:right.index])
      }
    }

    # De todos los máximos se queda con los que superan el umbral. Como el umbral se va actualizando, lo concatena
    tmp <- maximum[which( (maximum.heights > thr.qrs) & (maximum >= left.index) & (maximum <= right.index) ) ]
    tmp.heights <- maximum.heights[which( (maximum >= left.index) & (maximum <= right.index) & (maximum.heights > thr.qrs) )]
    final.maximum <- c(final.maximum,tmp)
    final.maximum.heights <- c(final.maximum.heights, tmp.heights)    


    loop.iter <- loop.iter + 1
  }
  # Cuando final.maximum y final.maximum.heights son inicializados, contienen muchos 0 que después se mantienen al concatenar. Aquí se eliminan.
  positions <- final.maximum[which(final.maximum != 0)]
  positions <- positions + 1 # Porque los máximos están detectadas en C donde los arrays tienen un índice menos
  heights <- final.maximum.heights[which(final.maximum !=0)]


  

  annot <- data.frame(positions, heights)
  return(annot)
}

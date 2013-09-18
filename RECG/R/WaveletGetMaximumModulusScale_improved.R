WaveletGetMaximumModulusScale_improved <- function(wt, size = 2^16, samplingFreq, scale, ...){
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

  # Suponiendo una longitud del QRS de 120 ms
  length.qrs <- 0.12 * samplingFreq

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
    }
    else{
      if(scale != 4){
        thr.qrs <- Rms(abs(wt[left.index:right.index]))
      }
      else{
        thr.qrs <- 0.5 * Rms(abs(wt[left.index:right.index]))
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
  modulus.positions <- final.maximum[which(final.maximum != 0)]
  modulus.positions <- modulus.positions + 1 # Porque los máximos están detectadas en C donde los arrays tienen un índice menos
  modulus.heights <- final.maximum.heights[which(final.maximum !=0)]


  # Remove isolated maximum modulus lines
  #df.returned <- RemoveIsolatedMaximumModulusLines(wt, modulus.positions, modulus.heights, thr.qrs, samplingFreq)
  # Remove redundant maximum modulus lines
  #positions <- df.returned$final.modulus.position
  #heights <- df.returned$final.modulus.height

  positions <- modulus.positions
  heights <- modulus.heights

  #dev.new()
  #pos <- 1 
  #samplesToShow = 1000
  #while( pos < length(wt)){
    #to.show <- wt[ pos: (pos + samplesToShow)]
    #plot(to.show, col = "black", type = "l")
    #posit <- modulus.positions[which(modulus.positions > pos & modulus.positions < (pos + samplesToShow - 1))] - pos
    #heig <- modulus.heights[which(modulus.positions > pos & modulus.positions < (pos + samplesToShow - 1))]
    #points(posit, heig, col="red")

    #abline(a=thr.qrs, b=0, col="blue")
    #thr2 = (max(modulus.heights) - thr.qrs) * 0.2 + thr.qrs # Un 20% más de la diferencia entre el máximo y el umbral
    #abline(a=thr2, b=0,col="green")

    #posit <- positions[which(positions > pos & positions < (pos + samplesToShow - 1))] - pos
    #heig <- heights[which(positions > pos & positions < (pos + samplesToShow - 1))]
    #points(posit, heig, col="green")

    #pos <- pos + samplesToShow
    #readline("Press return to continue") 
  #}



  annot <- data.frame(positions, heights)
  return(annot)
}

WaveletGetMaximumModulusScale <- function(wt, size = 2^16, samplingFreq, scale, ...){
  # Receives as a parameter one Wavelet scale and extracts its maximum modulus lines. This implementation is based completely in R language
  # 
  # 
  # Args:
  #   wt: Wavelet transform signal at one scale
  #   size: Number of samples using to update the threshold
  #   samplingFreq: Signal sampling frequency
  #   scale: In which scale search maximum modulus. Scale 4 needs a different threshold
  #
  # Returns:
  #   An array containing the maximum modulus lines positions and heights

  # Suponiendo una longitud del QRS de 100 ms
  length.qrs <- 0.3 * samplingFreq
  refractoryPeriod200 <- 0.2 * samplingFreq
  refractoryPeriod360 <- 0.36 * samplingFreq
  


  left.index <- 1
  right.index <- size
  maximum.array <- mat.or.vec(length(wt), 1)
  maximum.array.heights <- mat.or.vec(length(wt), 1)
  maximum.array.index <- 1

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

    # Buscar los puntos que superan el umbral
    higher <- NULL 
    higher <- which(abs(wt[left.index:right.index]) > thr.qrs) + left.index - 1 # guarda en un array los trozos de señal que superan el umbral

    i <- 1
    left <- higher[i]
    array <- mat.or.vec(length(higher), 1)
    array.index <- 1    
    while( i < length(higher)){ # recorre todo el array higher (de los valores que superan el umbral)   
      array <- NULL
      array.index <- 1
      left <- higher[i] 

      # Recorre el array higher, que contiene el trozo de señal que supera el umbral
      # Recorre desde la parte izquierda del array hasta que la distancia desde la posicion izquierda es mayor que la posible longitud del qrs (legnth.qrs)
      while( higher[i] < (left + length.qrs) && (i < length(higher))){ 
        array[array.index] <- higher[i] 
        i <- i + 1
        array.index <- array.index + 1
      }
      array <- array[ which(array != 0)]
    
      maxPos <- which(max(abs(wt[array])) == abs(wt[array[1]:array[array.index - 1]]))[1] + (array[1] - 1) 
      maxHei <- max(abs(wt[array]))

      # Refractory period
      #if( (maximum.array.index > 1 && (maximum.array[maximum.array.index - 1] + refractoryPeriod200) <  maxPos  ) || maximum.array.index == 1 ){
        maximum.array[maximum.array.index] <- maxPos
          maximum.array.heights[maximum.array.index] <- maxHei
          maximum.array.index <- maximum.array.index + 1        
      #}

      
      
    }


    loop.iter <- loop.iter + 1
  }
  positions <- maximum.array[which(maximum.array != 0)]
  heights <- maximum.array.heights[which(maximum.array != 0)]


  annot <- data.frame(positions, heights)
  return(annot)
}

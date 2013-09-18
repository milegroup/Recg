WaveletGetPeaks <- function( wtD1, modulus, samplingFreq, size = 2^16, ... ){
  # Gets the R peaks from the wavelet decomposition level 1. Departing from each maximum modulus line position search a minimum and a maximum and finally
  # stablish the fiducial point as the closer value to zero between the minimum and the maximum previous located.
  # 
  # Args:
  #   wtD1: signal from wavelet decomposition level 1
  #   size: number of samples used to get the threshold
  #   samplingFreq: sampling frequency
  #
  # Returns:
  #   An array containing the R peaks
  


  # Suponiendo una longitud del QRS de 100 ms
  length.qrs <- 0.1 * samplingFreq

  positions <- modulus$positions
  heights <- modulus$heights

  qrs.positions <- mat.or.vec(length(positions),1)
  qrs.heights <- mat.or.vec(length(positions),1)
  
  

  cont = 1
  contPerdida = 0
  # Recorre los picos del máximum modulus line de la escala 1, que pueden ser positivos o negativos y los coloca como mínimos o máximos
  for(i in positions){ 
    if( wtD1[i] == heights[cont]){ # es un máximo
      maximo <- heights[cont]
      maxPos <- i
     

      # buscar el mínimo
      leftIndex <- maxPos - (length.qrs / 2)
      rightIndex <- maxPos + (length.qrs / 2)
      if( leftIndex < 0 ){
        leftIndex <- 0
      }
      if( rightIndex > length(wtD1)){
        rightIndex <- length(wtD1)
      }
      minimo <- min(wtD1[leftIndex : rightIndex]) # busca el mínimo
      minPos <- which( minimo == wtD1[leftIndex:rightIndex])[1] + (leftIndex - 1)

      # Finalmente busca el punto intermedio entre el mínimo y el máximo más próximo a cero. Que es aquel valor cuyo valor absoluto es el menor.
      if( minimo < 0 && maximo > 0){
      	if(minPos <= maxPos){
      	qrs.positions[cont] <- which(min(abs(wtD1[minPos:maxPos])) == abs(wtD1[leftIndex:rightIndex]))[1] + leftIndex
      	}
      	else{
      	qrs.positions[cont] <- which(min(abs(wtD1[maxPos:minPos])) == abs(wtD1[leftIndex:rightIndex]))[1] + leftIndex
      	}
      }
      else{
         cat("\nPoint discarded. It is not between a positive maximum and a negative minimum")
      }

    }
    else{
      if( wtD1[i] == (heights[cont] * -1) ){ # es un mímimo
        minimo <- (heights[cont] * -1)
        minPos <- i

        # buscar el máximo
	      leftIndex <- minPos - (length.qrs / 2)
        rightIndex <- minPos + (length.qrs / 2)
        if( leftIndex < 0 ){
          leftIndex <- 0
        }
        if( rightIndex > length(wtD1)){
	       rightIndex <- length(wtD1)
        }
        maximo <- max(wtD1[leftIndex:rightIndex]) # busca el máximo
        maxPos <- which( maximo == wtD1[leftIndex:rightIndex])[1] + (leftIndex - 1)
        
    	  # Finalmente busca el punto intermedio entre el mínimo y el máximo más próximo a cero. Que es aquel valor cuyo valor absoluto es el menor.
        if( minimo < 0 && maximo > 0){
      		if(minPos <= maxPos){
      		  qrs.positions[cont] <- which(min(abs(wtD1[minPos:maxPos])) == abs(wtD1[leftIndex:rightIndex]))[1] + leftIndex
      		}
      		else{
      		  qrs.positions[cont] <- which(min(abs(wtD1[maxPos:minPos])) == abs(wtD1[leftIndex:rightIndex]))[1] + leftIndex
      		}
            }
            else{
    	         cat("\nPoint discarded. It is not between a positive maximum and a negative minimum")
          	}

      }
      else{
        cat("\nLost position:")
        cat(contPerdida)
        contPerdida = contPerdida + 1
      }
    }
    cont <- cont + 1
  }

  
  


  positions <- qrs.positions
  heights <- qrs.heights
  

  qrs.array <- data.frame(positions, heights)
  return(qrs.array)

}

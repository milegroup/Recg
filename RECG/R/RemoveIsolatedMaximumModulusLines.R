RemoveIsolatedMaximumModulusLines <- function(wt, modulus.position, modulus.height, thr, samplingFreq){
  # Receives as a parameter one Wavelet scale and its modulus maximum positions and heights
  # The modulus maximum recieved is either a maximum or a minimum local. So first it is determined if it is a minimum or a maximum.
  # Next it is checked that this peak is not isolated, if it is isolated is discarded. If it is a valid peak, and the peak is a maximum (minimum)
  # is searched a minimum (maximum) adjacent within a valid interval (120 ms)
  # 
  # Args:
  #   wt: Wavelet transform signal at one scale
  #   modulus.position: The position of the peaks, either local maximum or minimum
  #   modulus.height: The height of this peaks in abs value
  #   thr: The threshold that the peaks should exceed to be considered a valid peak
  #   samplingFreq: sampling frequency
  #
  # Returns:
  #   An array containing the valid maximum modulus lines position and heights
  

  final.modulus.position <- mat.or.vec(length(modulus.position),1)
  final.modulus.height <- mat.or.vec(length(modulus.position), 1)
  final.modulus.cont <- 1


  qrs.interval <- 0.12 * samplingFreq # 120 ms

  # Recorre todas los picos (modulus.position) para ver si son picos aislados
  cont <- 1
  while( cont < length(modulus.position)){

    # Se establede un umbral, un 20% por encima del umbral
    # En los picos que no superan ese umbral se comprueba si son picos aislados
    thr2 = (max(modulus.height) - thr) * 0.2 + thr 
    if( wt[modulus.position[cont]] > thr2){
      final.modulus.position[final.modulus.cont] <- modulus.position[cont]
      final.modulus.height[final.modulus.cont] <- wt[modulus.position[cont]]
      final.modulus.cont <- final.modulus.cont + 1
    }
    else{
        # Determinar si es un mínimo o un máximo
      if( wt[modulus.position[cont]] > 0){ # Máximo
        max.position <- modulus.position[cont]
        max.height<- wt[modulus.position[cont]]
        # Busca los minimos adyacentes por ambos lados del máximo
        min1.pos <- mat.or.vec(1,1)
        min1.hei <- mat.or.vec(1,1)
        min2.pos <- mat.or.vec(1,1)
        min2.hei <- mat.or.vec(1,1)
        out <- .C("getLocalMinimaBeforeCurrentPosition", as.double(wt), as.integer(max.position), min1.pos = as.integer(min1.pos), min1.hei = as.double(min1.hei) )
        min1.pos <- out$min1.pos[1]
        min1.hei <- out$min1.hei[1]
        # Si alguno de los 2 minimos supera el umbral en valor absoluto se guarda la posición. Sino se descarta por ser un pico aislado
        # Si el minimo 1 ya supera el umbral, ya no se calcula el mínimo 2
        if( abs(abs(wt[min1.pos]) >= thr)){
          final.modulus.position[final.modulus.cont] <- max.position
          final.modulus.height[final.modulus.cont] <- max.height
          final.modulus.cont <- final.modulus.cont + 1
        }
        else{
          out <- .C("getLocalMinimaAfterCurrentPosition", as.double(wt), as.integer(max.position), min2.pos = as.integer(min2.pos), min2.hei = as.double(min2.hei))
          min2.pos <- out$min2.pos[1]
          min2.hei <- out$min2.hei[1]
          if( abs(wt[min2.pos]) >= thr ){
            final.modulus.position[final.modulus.cont] <- max.position
            final.modulus.height[final.modulus.cont] <- max.height
            final.modulus.cont <- final.modulus.cont + 1
          }
        }

      }
      else{ # Minimo
        min.position <- modulus.position[cont]
        min.height <- wt[modulus.position[cont]]

        # Busca los maximos adyacentes por ambos lados del minimo
        max1.pos <- mat.or.vec(1,1)
        max1.hei <- mat.or.vec(1,1)
        max2.pos <- mat.or.vec(1,1)
        max2.hei <- mat.or.vec(1,1)
        out <- .C("getLocalMaximaBeforeCurrentPosition", as.double(wt), as.integer(min.position), max1.pos = as.integer(max1.pos), max1.hei = as.double(max1.hei) )
        max1.pos <- out$max1.pos[1]
        max1.hei <- out$max1.hei[1]
        # Si alguno de los 2 maximos supera el umbral en valor absoluto se guarda la posición. Sino se descarta por ser un pico aislado
        # Si el maximo 1 ya supera el umbral, ya no se calcula el máximo 2
        if( abs(wt[max1.pos]) >= thr ){
          final.modulus.position[final.modulus.cont] <- min.position
          final.modulus.height[final.modulus.cont] <- min.height
          final.modulus.cont <- final.modulus.cont + 1
        }
        else{
          out <- .C("getLocalMaximaAfterCurrentPosition", as.double(wt), as.integer(min.position), max2.pos = as.integer(max2.pos), max2.hei = as.double(max2.hei))
          max2.pos <- out$max2.pos[1]
          max2.hei <- out$max2.hei[1]
          if( abs(wt[max2.pos]) >= thr ){
            final.modulus.position[final.modulus.cont] <- min.position
            final.modulus.height[final.modulus.cont] <- min.height
            final.modulus.cont <- final.modulus.cont + 1
          }
        }      
        
      }
    }

    
    cont <- cont + 1
  }

  toret <- data.frame(final.modulus.position, final.modulus.height)
 
  return(toret)

}
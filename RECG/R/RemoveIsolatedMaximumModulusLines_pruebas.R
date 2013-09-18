RemoveIsolatedMaximumModulusLines_pruebas <- function(wt, modulus.position, modulus.height, thr){
  # Receives as a parameter one Wavelet scale and its modulus maximum positions and heights
  # The modulus maximum recieved is either a maximum or a minimum local. So first it is determined if is a minimum or a maximum.
  # Next is checked that this peak is not isolated, if it is isolated is discarded. If it is a valid peak, and the peak is a maximum (minimum)
  # is searched a minimum (maximum) adjacent within a valid interval (120 ms)
  # 
  # Args:
  #   wt: Wavelet transform signal at one scale
  #   modulus.position: The position of the peaks, either local maximum or minimum
  #   modulus.height: The height of this peaks in abs value
  #   thr: The threshold that the peaks should exceed to be considered a valid peak
  #
  # Returns:
  #   An array containing the valid maximum modulus lines position and heights
  

  # Prueba, guardando los puntos máximos y minimos para hacer un plot
  maximos.position <- mat.or.vec(length(modulus.position),1)
  maximos.height <- mat.or.vec(length(modulus.position),1)
  maximos.cont <- 1
  minimos.position <- mat.or.vec(length(modulus.position),1)
  minimos.height <- mat.or.vec(length(modulus.position),1)
  minimos.cont <- 1


  # Recorre todas los picos (modulus.position) para ver si son picos aislados
  cont <- 1
  while( cont < length(modulus.position)){
    # Determinar si es un mínimo o un máximo
    if( wt[modulus.position[cont]] > 0){ # Máximo
      maximos.position[maximos.cont] <- modulus.position[cont]
      maximos.height[maximos.cont] <- wt[modulus.position[cont]]
      maximos.cont <- maximos.cont + 1
    }
    else{ # Minimo
      minimos.position[minimos.cont] <- modulus.position[cont]
      minimos.height[minimos.cont] <- wt[modulus.position[cont]]
      minimos.cont <- minimos.cont + 1
    }

    cont <- cont + 1
  }

  # Eliminar los eltos del array que contienen un 0
  maximos.position <- maximos.position[which(maximos.position != 0)]
  maximos.height <- maximos.height[which(maximos.position != 0)]
  minimos.position <- minimos.position[which(minimos.position != 0)]
  minimos.height <- minimos.height[which(minimos.position != 0)]




  pos <- 1 
  samplesToShow = 1000
  while( pos < length(wt)){
    to.show <- wt[ pos: (pos + samplesToShow)]
    plot(to.show, col = "black", type = "l")

    # Pintar máximos
    positions <- maximos.position[which(maximos.position > pos & maximos.position < (pos + samplesToShow - 1))] - pos
    heights <- maximos.height[which(maximos.position > pos & maximos.position < (pos + samplesToShow - 1))] 
    points(positions, heights, col="red")

    # Pintar mínimos
    positions <- minimos.position[which(minimos.position > pos & minimos.position < (pos + samplesToShow - 1))] - pos
    heights <- minimos.height[which(minimos.position > pos & minimos.position < (pos + samplesToShow - 1))] 
    points(positions, heights, col="blue") 

    abline(a=thr, b=0, col="blue")

    pos <- pos + samplesToShow
    readline("Press return to continue") 
  }

}
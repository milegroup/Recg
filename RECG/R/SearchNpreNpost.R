SearchNpreNpost <- function(ECGData, wtD2){
  # Suponemos una longitud máxima del QRS de 120 ms
  length.qrs <- 0.24 * ECGData$samplingFreq
  n.qrs <- ECGData$annot$pos

  n.pre <- mat.or.vec(length(n.qrs),1)
  n.pre.height <- mat.or.vec(length(n.qrs),1)
  n.post <- mat.or.vec(length(n.qrs),1)
  n.post.height <- mat.or.vec(length(n.qrs),1)

  cont <- 1
  # Buscar todos los n.pre y todos los n.post
  # A partir de la posicion del QRS busca el mínimo y el máximo que le rodean
  for(position in n.qrs){

    # buscar el mínimo y máximo
    leftIndex <- position - (length.qrs / 2)
    rightIndex <- position + (length.qrs / 2)

    if( leftIndex < 0 ){
      leftIndex <- 0
    }
    if( rightIndex > length(wtD2)){
      rightIndex <- length(wtD2)
    }
    minimo <- min(wtD2[leftIndex : rightIndex]) # busca el mínimo
    minPos <- which( minimo == wtD2[leftIndex:rightIndex])[1] + leftIndex 
    maximo <- max(wtD2[leftIndex:rightIndex]) # busca el máximo
    maxPos <- which( maximo == wtD2[leftIndex:rightIndex])[1] + leftIndex 

    if(minimo < 0 && maximo > 0){ # debe estar entre un mínimo negativo y un máximo positivo
      if( minPos < maxPos ){ # Si el mínimo está primero, entonces n.pre es el mínimo
        n.pre[cont] <- minPos
        n.pre.height[cont] <- minimo
        n.post[cont] <- maxPos
        n.post.height[cont] <- maximo
      }
      else{ # Si el máximo está primero entonces n.pre es el máximo
        n.pre[cont] <- maxPos
        n.pre.height[cont] <- maximo
        n.post[cont] <- minPos
        n.post.height[cont] <- minimo
      }

      cont <- cont + 1
    }    
  }

  toret <- data.frame(n.pre, n.pre.height, n.post, n.post.height)
  return(toret)
}
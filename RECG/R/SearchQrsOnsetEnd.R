SearchQrsOnsetEnd <- function(wtD2, n.qrs, n.first.position, n.first.height, n.last.position, n.last.height, qrs.on, qrs.end ){
  # Busca el qrs.onset basandose en 2 criterios. En el primer criterio se establece un umbral y busca hacia atrás hasta encontrar una posición por debajo
  # de ese umbral. Y en el segundo criterio busca desde n.first hacia atrás hasta encontrar un mínimo local
  qrs.onset.position <- mat.or.vec(length(n.first.position),1)
  qrs.onset.height <- mat.or.vec(length(n.first.position),1)



  # Prueba borrar ############
  qrs.onset.position.criteria1 <- mat.or.vec(length(n.first.position),1)
  qrs.onset.height.criteria1 <- mat.or.vec(length(n.first.position),1)
  qrs.onset.position.criteria2 <- mat.or.vec(length(n.first.position),1)
  qrs.onset.height.criteria2 <- mat.or.vec(length(n.first.position),1)

  qrs.end.position.criteria1 <- mat.or.vec(length(n.first.position),1)
  qrs.end.height.criteria1 <- mat.or.vec(length(n.first.position),1)
  qrs.end.position.criteria2 <- mat.or.vec(length(n.first.position),1)
  qrs.end.height.criteria2 <- mat.or.vec(length(n.first.position),1)

  #############



  pos.2 <- mat.or.vec(1,1) # Vectores usado como params en la llamda a la funcion C del segundo criterio
  hei.2 <- mat.or.vec(1,1)
  i <- 1
  while( i < length(n.first.position) ){ # Recorre todos los n.first, busca los qrs.onset en ambos criterios y selecciona el más cercano a n.qrs
    # Primer criterio
    pos.1 <- n.first.position[i]
    hei.1 <- abs(n.first.height[i])
    while( hei.1 > qrs.on[i] ){ # Busca hacia atrás hasta que la altura es menor que el umbral
      pos.1 <- pos.1 - 1
      hei.1 <- abs(wtD2[pos.1])
      if(pos.1 == 1){ # Si llega al inicio de la señal
        break;
      }
    }
    # Segundo criterio
    nf <- n.first.position[i]
    out <- .C("getLocalMinimaBeforeCurrentPosition", as.double(abs(wtD2)), as.integer(nf), pos.2 = as.integer(pos.2), hei.2 = as.double(hei.2) )
    pos.2 <- out$pos.2[1]
    hei.2 <- out$hei.2[1]  

    # De las posiciones obtenidas por los 2 criterios. Toma como qrs.onset la posicion más cercana a n.qrs
    if( abs(n.qrs[i] - pos.1) < abs(n.qrs[i] - pos.2) ){
      qrs.onset.position[i] <- pos.1
      qrs.onset.height[i] <- hei.1
    }
    else{
      qrs.onset.position[i] <- pos.2
      qrs.onset.height[i] <- hei.2
    }

    # Prueba borrar ############
    qrs.onset.position.criteria1[i] <- pos.1
    qrs.onset.height.criteria1[i] <- hei.1
    qrs.onset.position.criteria2[i] <- pos.2
    qrs.onset.height.criteria2[i] <- pos.2
    ############


    i <- i + 1
  }
  

  # Busca el qrs.end basandose en 2 criterios. En el primer criterio se establece un umbral y busca hacia delante hasta encontrar una posición por debajo
  # de ese umbral. Y en el segundo criterio busca desde n.last hacia delante hasta encontrar un mínimo local
  qrs.end.position <- mat.or.vec(length(n.last.position), 1)
  qrs.end.height <- mat.or.vec(length(n.last.position), 1)
  pos.2 <- mat.or.vec(1,1)
  hei.2 <- mat.or.vec(1,1)
  i <- 1
  while( i < length(n.last.position)){ # Recorre todos los n.last
    # Primer criterio
    pos.1 <- n.last.position[i]
    hei.1 <- abs(n.last.height[i])
    while( hei.1 > qrs.end[i] ){ # Busca hacia delante hasta que la altura es menor que el umbral
      pos.1 <- pos.1 + 1
      hei.1 <- abs(wtD2[pos.1])
      if( pos.1 >= length(n.last.position)){ # Si llega al final de la señal
        break;
      }
    }
    
    # Segundo criterio
    nl <- n.last.position[i]
    out <- .C("getLocalMinimaAfterCurrentPosition", as.double(abs(wtD2)), as.integer(nl), pos.2 = as.integer(pos.2), hei.2 = as.double(hei.2))
    pos.2 <- out$pos.2[1]
    hei.2 <- out$hei.2[1]

    # De las posiciones obtenidas por los 2 criterios. Toma como qrs.end la posicion más cercana a n.qrs
    if( abs(pos.1 - n.qrs[i]) < abs(pos.2 - n.qrs[i]) ){
      qrs.end.position[i] <- pos.1
      qrs.end.height[i] <- hei.1
    }
    else{
      qrs.end.position[i] <- pos.2
      qrs.end.height[i] <- hei.2
    }

    # Prueba borrar ############
    qrs.end.position.criteria1[i] <- pos.1
    qrs.end.height.criteria1[i] <- hei.1
    qrs.end.position.criteria2[i] <- pos.2
    qrs.end.height.criteria2[i] <- pos.2
    ############

    i <- i + 1
  }

  #toret <- data.frame(qrs.onset.position, qrs.onset.height, qrs.end.position, qrs.end.height)
  # Prueba borrar ############
  toret <- data.frame(qrs.onset.position, qrs.onset.height, qrs.end.position, qrs.end.height, 
                      qrs.onset.position.criteria1, qrs.onset.height.criteria1, qrs.onset.position.criteria2, qrs.onset.height.criteria2,
                      qrs.end.position.criteria1, qrs.end.height.criteria1, qrs.end.position.criteria2, qrs.end.height.criteria2)
  return(toret)

}
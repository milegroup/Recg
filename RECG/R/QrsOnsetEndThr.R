QrsOnsetEndThr <- function(n.first.position, n.first.height, n.last.position, n.last.height){

  # Determinar los umbrales
  qrs.on <- mat.or.vec(length(n.first.position),1)
  qrs.end <- mat.or.vec(length(n.last.position),1)
  # Si tienen el mismo numero de elementos hace un solo bucle
  if( length(n.first.position) == length(n.last.position) ){
    i <- 1
    while( i < length(n.first.position)){
      if(n.first.height[i] > 0){
        qrs.on[i] <- 0.05 * abs(n.first.height[i])
      }
      else{
        qrs.on[i] <- 0.07 * abs(n.first.height[i])
      }
      if(n.last.height[i] > 0){
        qrs.end[i] <- 0.125 * abs(n.last.height[i])
      }
      else{
        qrs.end[i] <- 0.71 * abs(n.last.height[i])
      }
      i <- i + 1
    }
  }
  else{
    cat("\nNumero de elementos distinto, hace 2 bucles")
    i <- 1
    while( i < length(n.first.position)){
      if(n.first.height[i] > 0){
        qrs.on[i] <- 0.05 * abs(n.first.height[i])
      }
      else{
        qrs.on[i] <- 0.07 * abs(n.first.height[i])
      }
      i <- i + 1
    }
    i <- 1
    while( i < length(n.last.position)){
      if(n.last.height[i] > 0){
        qrs.end[i] <- 0.125 * abs(n.last.height[i])
      }
      else{
        qrs.end[i] <- 0.71 * abs(n.last.height[i])
      }
      i <- i + 1
    }
  }

  toret <- data.frame(qrs.on, qrs.end)
  return(toret)
}
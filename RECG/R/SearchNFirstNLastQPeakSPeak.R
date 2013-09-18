SearchNFirstNLastQPeakSPeak <- function(wtD2, wtD1, n.pre, n.post, size){
  # Inicializacion variables n.first
  n.first.position <- mat.or.vec(length(n.pre),1)
  n.first.height <- mat.or.vec(length(n.pre),1)
  n.first.cont <- 1
  n.first.pos <- mat.or.vec(1,1)
  n.first.hei <- mat.or.vec(1,1)

  # Inicializacion variables n.last
  n.last.position <- mat.or.vec(length(n.post),1)
  n.last.height <- mat.or.vec(length(n.post),1)
  n.last.cont <- 1  
  n.last.pos <- mat.or.vec(1,1)
  n.last.hei <- mat.or.vec(1,1)

  # Inicializacion variables q.peak
  q.peak <- mat.or.vec(length(n.pre), 1)
  q.peak.height <- mat.or.vec(length(n.pre),1)
  cont.q.peak <- 1

  # Inicializacion variables s.peak
  s.peak <- mat.or.vec(length(n.pre), 1)
  s.peak.height <- mat.or.vec(length(n.pre),1)
  cont.s.peak <- 1

  left.index <- 1
  right.index <- size
  loop.iter <- 1




  # Buscar máximos significativos en la escala 2
  while( right.index < length(wtD2) ){

    # Update indexes 
    if( loop.iter != 1){
      left.index <- right.index
      right.index <- right.index + size
    }
    if(right.index > length(wtD2)){
      right.index <- length(wtD2)
    }

  

    # Update thresholds
    thr.qrs.pre <- 0.06 * max(abs(wtD2[left.index:right.index]))
    thr.qrs.post <- 0.09 * max(abs(wtD2[left.index:right.index]))

    # Obtain n.pre and n.post values in the current interval
    interval.n.pre <- n.pre[which(n.pre >= left.index & n.pre <= right.index)]
    interval.n.post <- n.post[which(n.post >= left.index & n.post <= right.index)]

    # Traverse all n.pre and n.post searching for significant maximum modulus (n.first y n.post)
    inside.loop.iter <- 1
    while(inside.loop.iter < length(interval.n.pre) ){

      # Search n.first before n.pre
      out <- .C("getNFirst", as.double(wtD2), as.integer(interval.n.pre[inside.loop.iter]), n.first.pos = as.integer(n.first.pos), n.first.hei = as.double(n.first.hei))
      n.first.position[n.first.cont] <- out$n.first.pos[1]
      n.first.height[n.first.cont] <- out$n.first.hei[1]
      n.first.cont <- n.first.cont + 1

      # Search n.last after n.post
      out <- .C("getNLast", as.double(wtD2), as.integer(interval.n.post[inside.loop.iter]), n.last.pos = as.integer(n.last.pos), n.last.hei = as.double(n.last.hei))
      n.last.position[n.last.cont] <- out$n.last.pos[1]
      n.last.height[n.last.cont] <- out$n.last.hei[1]
      n.last.cont <- n.last.cont + 1

     
      # Search q.peak as the minimum value (in absolute value in the scale 1) between the n.first and n.pre peaks
      n.pre.position <- interval.n.pre[inside.loop.iter] 
      first <- n.first.position[n.first.cont - 1]
      excerpt <- wtD1[first:n.pre.position] 
      q.peak[cont.q.peak] <- which(min(abs(excerpt)) == abs(excerpt)  )[1] + (first - 1)
      q.peak.height[cont.q.peak] <- min(abs(excerpt))
      cont.q.peak <- cont.q.peak + 1
      

      # Search s.peak as the minimum value (in absolute value in the scale 1) between the n.post and n.last peaks
      n.post.position <- interval.n.post[inside.loop.iter] 
      last <- n.last.position[n.last.cont -1]      
      excerpt <- wtD1[n.post.position:last] 
      s.peak[cont.s.peak] <- which(min(abs(excerpt)) == abs(excerpt) )[1] + (n.post.position - 1)
      s.peak.height[cont.s.peak] <- min(abs(excerpt))
      cont.s.peak <- cont.s.peak + 1
      

      # Increment loop variable
      inside.loop.iter <- inside.loop.iter + 1
    }


    loop.iter <- loop.iter + 1
  }

  # Hai un problema, que os ultimos valores de n.first e n.last son 0
  # De forma temporal está asi, despois hai que solucionar este problema
  n.first.position <- n.first.position[which(n.first.position != 0)]
  n.first.height <- n.first.height[which(n.first.position != 0)]
  n.last.position <- n.last.position[which(n.last.position != 0)]
  n.last.height <- n.last.height[which(n.last.position != 0)]
  q.peak <- q.peak[which(q.peak != 0)]
  q.peak.height <- q.peak.height[which(q.peak != 0)]
  s.peak <- s.peak[which(s.peak != 0)]
  s.peak.height <- s.peak.height[which(s.peak != 0)]
  ##########   Borrar temporal




  toret <- data.frame(n.first.position, n.first.height, n.last.position, n.last.height, q.peak, q.peak.height, s.peak, s.peak.height)
  return(toret)

}
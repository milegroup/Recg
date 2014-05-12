PlotDelineationResults2 <- function(ECGData, wtD2, wtD1){
  n.qrs <- ECGData$annot$pos
  qrs.onset.position <- ECGData$delineation$qrs.onset.position
  qrs.onset.height <- ECGData$delineation$qrs.onset.height
  qrs.end.position <- ECGData$delineation$qrs.end.position
  qrs.end.height <- ECGData$delineation$qrs.end.height
  q.peak.position <- ECGData$delineation$q.peak.position
  q.peak.height <- ECGData$delineation$q.peak.height
  s.peak.position <- ECGData$delineation$s.peak.position
  s.peak.height <- ECGData$delineation$s.peak.height

  n.pre.position <- ECGData$delineation$n.pre.position
  n.pre.height <- ECGData$delineation$n.pre.height
  n.post.position <- ECGData$delineation$n.post.position
  n.post.height <- ECGData$delineation$n.post.height
  n.first.position <- ECGData$delineation$n.first.position
  n.first.height <- ECGData$delineation$n.first.height
  n.last.position <- ECGData$delineation$n.last.position
  n.last.height <- ECGData$delineation$n.last.height


  pos <- 1
  samplesToShow <- 2000
  #while( pos < length(wtD1)){
    plot(ECGData$lead[[1]]$data[pos:(pos+samplesToShow)],type='l')
    #plot(wtD1[pos:(pos+samplesToShow)],type='l')
    posit <- q.peak.position[which( q.peak.position >= pos & q.peak.position <= (pos + samplesToShow - 1)) ] - pos
    heig <-  q.peak.height[which(q.peak.position >= pos & q.peak.position <= (pos + samplesToShow - 1))]   
    points(posit, heig, col="green")

    posit <- s.peak.position[which( s.peak.position >= pos & s.peak.position <= (pos + samplesToShow - 1)) ] - pos
    heig <-  s.peak.height[which( s.peak.position >= pos & s.peak.position <= (pos + samplesToShow -1 ))]
    points(posit, heig, col="green")

    # Pintar los n.first y n.last
    posit <- n.first.position[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ] - pos
    heig <- n.first.height[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ]
    points(posit, heig, col="red")

    posit <- n.last.position[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))] - pos
    heig <- n.last.height[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))]
    points(posit, heig, col="red")

    # Pintar los qrs.onset y qrs.end
    posit <- qrs.onset.position[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))] - pos
    heig <- qrs.onset.height[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))]
    points(posit, heig, col="blue")

    posit <- qrs.end.position[which( qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] - pos
    heig <- qrs.end.height[which(qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] 
    points(posit, heig, col="blue")

    #pos <- pos + samplesToShow
    #readline("Press return to continue") 
  #}

  dev.new()


  # Hace el plot de wtD2 con n.pre y n.post
  pos <- 1
  samplesToShow <- 2000
  #while( pos < length(wtD2)){
    plot(wtD2[pos:(pos+samplesToShow)],type='l')


    # Pintar umbrales
    thr <- Rms(wtD2[1:2^16])
    thr.qrs.pre <- 0.06 * max(abs(wtD2[1:2^16]))
    thr.qrs.post <- 0.09 * max(abs(wtD2[1:2^16]))
    abline(thr,0,col="red")
    abline(thr.qrs.pre, 0 , col="green")
    abline(thr.qrs.post, 0 , col="blue")

    # Pintar los n.qrs (posiciones del latido)
    #posit <- n.qrs[which(n.qrs >= pos & n.qrs < (pos + samplesToShow - 1))] - pos
    #heig <- mat.or.vec(length(posit),1)
    #points(posit, heig, col="orange")


    # Pintar los n.pre y n.post
    posit <- n.pre.position[which( n.pre.position >= pos & n.pre.position <= (pos + samplesToShow - 1)) ] - pos
    heig <- n.pre.height[which( n.pre.position >= pos & n.pre.position <= (pos + samplesToShow -1 ))]
    points(posit, heig, col="orange")    

    posit <- n.post.position[which( n.post.position >= pos & n.post.position <= (pos + samplesToShow - 1)) ] - pos
    heig <- n.post.height[which( n.post.position >= pos & n.post.position <= (pos + samplesToShow -1 ))]
    points(posit, heig, col="orange")

    # Pintar los n.first y n.last

    posit <- n.first.position[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ] - pos
    heig <- n.first.height[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ]
    points(posit, heig, col="red")

    posit <- n.last.position[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))] - pos
    heig <- n.last.height[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))]
    points(posit, heig, col="red")

    # Pintar los picos q y s

    posit <- q.peak.position[which( q.peak.position >= pos & q.peak.position <= (pos + samplesToShow - 1)) ] - pos
    heig <-  q.peak.height[which(q.peak.position >= pos & q.peak.position <= (pos + samplesToShow - 1))]   
    points(posit, heig, col="green")

    posit <- s.peak.position[which( s.peak.position >= pos & s.peak.position <= (pos + samplesToShow - 1)) ] - pos
    heig <-  s.peak.height[which( s.peak.position >= pos & s.peak.position <= (pos + samplesToShow -1 ))]
    points(posit, heig, col="green")

    # Pintar los qrs.onset y qrs.end
    posit <- qrs.onset.position[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))] - pos
    heig <- qrs.onset.height[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))]
    points(posit, heig, col="blue")

    posit <- qrs.end.position[which( qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] - pos
    heig <- qrs.end.height[which(qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] 
    points(posit, heig, col="blue")

    #pos <- pos + samplesToShow
    #readline("Press return to continue") 
  #}
  


}
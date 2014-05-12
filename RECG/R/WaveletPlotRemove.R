WaveletPlotRemove <- function(wt, modulus1, modulus2, modulus3, modulus4, isFinal=FALSE, size=2^16){
    
  thr.qrs.1 <- Rms(wt$D1[1:size])
  thr.qrs.2 <- Rms(wt$D2[1:size])
  thr.qrs.3 <- Rms(wt$D3[1:size])
  thr.qrs.4 <- 0.5 * Rms(wt$D4[1:size])

  # Plotting the results
  #plot(wt$D1[1:5000], type="l")
  #abline(a=thr.qrs.1, b=0, col="blue")
  #dev.new()
  #plot(wt$D2[1:5000], type="l")
  #abline(a=thr.qrs.2, b=0, col="blue")
  #dev.new()
  #plot(wt$D3[1:5000], type="l")
  #abline(a=thr.qrs.3, b=0, col="blue")
  #dev.new()
  #plot(wt$D4[1:5000], type="l")
  #abline(a=thr.qrs.4, b=0, col="blue")



  # Plotting modulus
  if( isFinal ){
    dev.new()
    plot(abs(wt$D1[1:5000]), type='l')
    a <- modulus1$positions[which(modulus1$positions < 5000)]
    b <- modulus1$heights[which(modulus1$positions < 5000)]
    points(a, b)
    abline(a=thr.qrs.1, b=0, col="blue")
  }
  else{
    plot(abs(wt$D1[1:5000]), type='l')
    a <- modulus1$positions[which(modulus1$positions < 5000)]
    b <- modulus1$heights[which(modulus1$positions < 5000)]
    points(a, b)
    abline(a=thr.qrs.1, b=0, col="blue")

    #dev.new()
    plot(abs(wt$D2[1:5000]), type='l')
    a <- modulus2$positions[which(modulus2$positions < 5000)]
    b <- modulus2$heights[which(modulus2$positions < 5000)]
    points(a, b)
    abline(a=thr.qrs.2, b=0, col="blue")

    #dev.new()
    plot(abs(wt$D3[1:5000]), type='l')
    a <- modulus3$positions[which(modulus3$positions < 5000)]
    b <- modulus3$heights[which(modulus3$positions < 5000)]
    points(a, b)
    abline(a=thr.qrs.3, b=0, col="blue")

    #dev.new()
    plot(abs(wt$D4[1:5000]), type='l')
    a <- modulus4$positions[which(modulus4$positions < 5000)]
    b <- modulus4$heights[which(modulus4$positions < 5000)]
    points(a, b)
    abline(a=thr.qrs.4, b=0, col="blue")
  }

  dev.new()
  pos <- 1 
  samplesToShow = 1000
  while( pos < length(wt$D2)){
    to.show <- wt$D2[ pos: (pos + samplesToShow)]
    plot(to.show, col = "black", type = "l")
    positions <- modulus2$positions[which(modulus2$positions > pos & modulus2$positions < (pos + samplesToShow - 1))] - pos
    heights <- modulus2$heights[which(modulus2$positions > pos & modulus2$positions < (pos + samplesToShow - 1))]
    points(positions, heights, col="red")
    abline(a=thr.qrs.2, b=0, col="blue")

    pos <- pos + samplesToShow
    readline("Press return to continue") 
  }
  
  
}
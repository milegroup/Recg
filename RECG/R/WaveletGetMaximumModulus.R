WaveletGetMaximumModulus <- function(wt, samplingFreq, ...){
  # Get the maximum modulus lines from the first 4 scales of the wavelet transform, rejecting isolated ones
  # 
  # Args:
  #   wt: Wavelet transform signal
  #   samplingFreq: Signal sampling frequency
  #
  # Returns:
  #   The maximum modulus lines at WT scale 1


 
  # Obtain the maximum modulus lines for each scale

  #modulus1 <- WaveletGetMaximumModulusScale(wt$D1, samplingFreq = samplingFreq )
  #modulus2 <- WaveletGetMaximumModulusScale(wt$D2, samplingFreq = samplingFreq )  
  #modulus3 <- WaveletGetMaximumModulusScale(wt$D3, samplingFreq = samplingFreq )  
  #modulus4 <- WaveletGetMaximumModulusScale(wt$D4, samplingFreq = samplingFreq, scale=4 )

  #modulus1 <- WaveletGetMaximumModulusScale_usingCfunction(wt$D1, samplingFreq = samplingFreq )
  #modulus2 <- WaveletGetMaximumModulusScale_usingCfunction(wt$D2, samplingFreq = samplingFreq )  
  #modulus3 <- WaveletGetMaximumModulusScale_usingCfunction(wt$D3, samplingFreq = samplingFreq )  
  #modulus4 <- WaveletGetMaximumModulusScale_usingCfunction(wt$D4, samplingFreq = samplingFreq, scale=4 )

  modulus1 <- WaveletGetMaximumModulusScale_improved(wt$D1, samplingFreq = samplingFreq )
  modulus2 <- WaveletGetMaximumModulusScale_improved(wt$D2, samplingFreq = samplingFreq )  
  modulus3 <- WaveletGetMaximumModulusScale_improved(wt$D3, samplingFreq = samplingFreq )  
  modulus4 <- WaveletGetMaximumModulusScale_improved(wt$D4, samplingFreq = samplingFreq, scale=4 )




  modulusPos <- mat.or.vec(length(wt$D1),1)
  modulusHei <- mat.or.vec(length(wt$D1),1)
  

  #WaveletPlotRemove(wt, modulus1, modulus2, modulus3, modulus4, isFinal=FALSE)

  m1 <- modulus1$positions
  m1.length <- length(m1) 
  m1Hei <- modulus1$heights
  m2 <- modulus2$positions
  m2Hei <- modulus2$heights
  m2.length <- length(m2)
  m3 <- modulus3$positions
  m3.length <- length(m3)
  m4 <- modulus4$positions
  m4.length <- length(m4)
  
  # Compare the modulus lines in all scales to discard isolated modulus lines
  out <- .C("compareModulusScales", as.integer(m1), as.double(m1Hei), as.integer(m2), as.integer(m3), as.integer(m4),
            as.integer(m1.length), as.integer(m2.length), as.integer(m3.length), as.integer(m4.length), modulusPos = as.integer(modulusPos),
            modulusHei = as.double(modulusHei), as.integer(samplingFreq))
  

  positions <- out$modulusPos
  positions <- positions[which(positions != 0)]
  heights <- out$modulusHei
  heights <- heights[which(positions != 0)]


  #pos <- 1
  #samplesToShow <- 5000
  #while( pos < length(wt$D1)){
   # plot(abs(wt$D1[pos:(pos+samplesToShow)]),type='l')
    #posit <- positions[which( positions >= pos & positions <= (pos + samplesToShow - 1)) ] - pos
    #heig <- heights[which(positions >= pos & positions <= (pos + samplesToShow - 1))] 

    #abline(a=thr.qrs,b=0,col="blue")
    #points(posit, heig, col="red")
    #pos <- pos + samplesToShow
    #readline("Press return to continue") 
  #}
  
  
  modulus <- data.frame(positions, heights)
  
  return(modulus)
}
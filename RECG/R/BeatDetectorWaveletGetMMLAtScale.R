BeatDetectorWaveletGetMMLAtScale <- function(waveletSignal, samplingFreq, scale, thr.window = 512){
  

  # Suponiendo una longitud del QRS de 120 ms
  search.window.size <- 0.12 * samplingFreq  
  search.window.size <- search.window.size / (2 ^ (scale - 1))
  
  peaks.positions <- mat.or.vec(length(waveletSignal), 1)
  peaks.heights <- mat.or.vec(length(waveletSignal), 1)
  array.size <- length(waveletSignal)

  window <- round(search.window.size) * 2
  out <- .C("peakDetector_SinR", as.double(abs(waveletSignal)), peaks.positions = as.integer(peaks.positions), peaks.heights = as.double(peaks.heights), 
            as.integer(array.size), as.integer(window))

  peaks.positions <- out$peaks.positions[which(out$peaks.positions != 0)]  
  peaks.heights <- out$peaks.heights[which(out$peaks.heights != 0)]  


  maximum.modulus.positions <- mat.or.vec(length(peaks.positions), 1)

  out <- .C("getMaximumModulusLines", as.double(waveletSignal), as.integer(length(waveletSignal)), as.integer(peaks.positions), as.double(peaks.heights), 
            as.integer(length(peaks.positions)), as.integer(thr.window), maximum.modulus.positions = as.integer(maximum.modulus.positions), 
            as.integer(scale), as.integer(round(search.window.size)) )
 
  
  maximum.modulus.positions <- out$maximum.modulus.positions[which(out$maximum.modulus.positions != 0)]


  toret <- list()
  toret$peaks.positions <- peaks.positions
  toret$maximum.modulus.positions <- maximum.modulus.positions  
  return(toret)  
}
Preprocessor.PanTompkins <- function(ECGData, channel, windowSize, ...){
  # Performs the preprocessor stage according to Pan & Tompkins algorithm
  # 
  # Args:
  #   ECGData: The ECG data model
  #   channel: In which channel apply the preprocessing
  #   windowSize: Size of the window used in the moving window integrator step
  #
  # Returns:
  #   A list with the preprocessed signal. E.g. The derivative and integrated signals
  
  if (missing(ECGData)){
    stop("   ECGData is a mandatory param")
  }
  if (missing(channel)){
    stop("   channel is a mandatory param")
  }

  # Contains the info about preproccesing
  preproInfo <- list()


  if (ECGData$Verbose){
    message <- paste("   Preprocessing channel", channel, "\n", sep = " ")
    cat(message)
  }

   

  channel.size <- length(ECGData$lead[[channel]]$data)

  # Normalization
  ECGData$lead[[channel]]$data <- ECGData$lead[[channel]]$data / max(abs(ECGData$lead[[channel]]$data))



  # Low pass filter
  lpf <- mat.or.vec(channel.size, 1)
  out <- .C("lowPassFilter", lpf.array = as.double(lpf), as.double(ECGData$lead[[channel]]$data), channel.size)
  lpf <- out$lpf.array

  # High pass filter
  hpf <- mat.or.vec(channel.size, 1)
  out <- .C("highPassFilter", hpf.array = as.double(lpf), as.double(lpf), channel.size)
  hpf <- out$hpf.array

  # Derivative function
  derivative <- mat.or.vec(channel.size, 1)
  out <- .C("derivative", derivative.array = as.double(derivative), as.double(hpf), channel.size)
  derivative <- out$derivative.array

  # Squaring fuction
  square <- derivative * derivative

  # Moving-window integrator
  mwi <- mat.or.vec(channel.size, 1)
  out <- .C("mwi", mwi.array = as.double(mwi), as.double(square), channel.size, windowSize)
  mwi <- out$mwi.array
  mwi <- mwi / max(abs(mwi)) # Normalization

  # Save signals
  preproInfo$derivative <- derivative
  preproInfo$mwi <- mwi


  return(preproInfo)
}
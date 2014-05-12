PlotDelineation <- function(ECGData, scale = 0, showNqrs = TRUE, showQRSDelineation = TRUE, showNPreNPost = TRUE, showNLastNFirst = TRUE, 
                            showPDelineation = TRUE, showTDelineation = TRUE, samplesToShow=5000, plotType = "l", ...){
  # Plot continuously the signal with the Delineation positions
  #   
  # Args:
  #   ECGData: ECG data model
  #   signalToPlot: the signal to be plotted
  #   showNqrs: whether to show qrs fiducial marks
  #   showQRSDelineation: whether to show QRS delineation
  #   samplesToShow: Number of samples to show in each iteration
  #


  if (missing(ECGData)){
    stop("ECGData param is mandatory")
  }
 
  if(scale == 1){
    signalToPlot <- ECGData$wt$W1
  }
  if(scale == 2){
    signalToPlot <- ECGData$wt$W2 
  }
  if(scale == 3){
    signalToPlot <- ECGData$wt$W3
  }
  if(scale == 4){
    signalToPlot <- ECGData$wt$W4
  }
  if(scale == 5){
    signalToPlot <- ECGData$wt$W5
  }
  if( scale == 0){ # Plots the signal at derivation lead 1
    signalToPlot <- ECGData$lead[[1]]$data
    scale <- 1
  }
  if(scale >5 || scale < 0 ){
    stop("\nOnly scales 0 to 5 are available ")
  }


  divider <- (2 ^ scale - 2 ^(scale -1) ) 
  n.qrs <- (ECGData$annot$pos / divider ) + 1

  # Save the data in variables with shorter names
  qrs.onset.positions <- (ECGData$delineation$qrs.onset.positions / divider ) + 1 # +1 para compensar indices con C
  qrs.end.positions <- (ECGData$delineation$qrs.end.positions / divider ) + 1
  q.peak.positions <- (ECGData$delineation$q.peak.positions / divider ) + 1
  s.peak.positions <- (ECGData$delineation$s.peak.positions / divider ) + 1

  npre.positions <- (ECGData$delineation$npre.positions / divider ) + 1
  npost.positions <- (ECGData$delineation$npost.positions / divider ) + 1
  nfirst.positions <- (ECGData$delineation$nfirst.positions / divider ) + 1
  nlast.positions <- (ECGData$delineation$nlast.positions / divider ) + 1

  p.peak.positions <- (ECGData$delineation$p.peak.positions / divider ) + 1
  p.maxPos.positions <- (ECGData$delineation$p.maxPos.peak.positions / divider ) + 1
  p.minPos.positions <- (ECGData$delineation$p.minPos.peak.positions / divider ) + 1
  p.leftIndex <- ( ECGData$delineation$p.leftIndex / divider  ) + 1
  p.rightIndex <- ( ECGData$delineation$p.rightIndex / divider ) + 1
  p.onset <- (ECGData$delineation$p.onset / divider ) + 1
  p.end <- (ECGData$delineation$p.end / divider ) + 1

  t.peak.positions <- (ECGData$delineation$t.peak.positions / divider ) + 1
  t.maxPos.positions <- (ECGData$delineation$t.maxPos.peak.positions / divider ) + 1
  t.minPos.positions <- (ECGData$delineation$t.minPos.peak.positions / divider ) + 1
  t.leftIndex <- ( ECGData$delineation$t.leftIndex / divider  ) + 1
  t.rightIndex <- ( ECGData$delineation$t.rightIndex / divider ) + 1
  


  # Loop that plots the signal continuously
  pos <- 1 
  while( pos < length(signalToPlot) ){
    to.show <- signalToPlot[ pos: (pos + samplesToShow)]
    plot(to.show, col = "black", type = plotType)

    if(showNqrs){
      addPoints( signalToPlot, n.qrs, pos, samplesToShow, colour = "green", ... )
    }
    if(showNPreNPost){
      addPoints( signalToPlot, npre.positions, pos, samplesToShow, colour = "darkgreen")
      addPoints( signalToPlot, npost.positions, pos, samplesToShow, colour = "darkgreen")
    }
    if(showNLastNFirst){
      addPoints( signalToPlot, nfirst.positions, pos, samplesToShow, colour = "darkmagenta")
      addPoints( signalToPlot, nlast.positions, pos, samplesToShow, colour = "darkmagenta")
    }
    if(showQRSDelineation){
      addPoints( signalToPlot, qrs.onset.positions, pos, samplesToShow, colour = "blue")
      addPoints( signalToPlot, qrs.end.positions, pos, samplesToShow, colour = "blue")
      
      addPoints( signalToPlot, q.peak.positions, pos, samplesToShow, colour = "red")
      addPoints( signalToPlot, s.peak.positions, pos, samplesToShow, colour = "red")
    }
    if(showPDelineation){
      addPoints( signalToPlot, p.peak.positions, pos, samplesToShow, colour="green")
      addPoints( signalToPlot, p.maxPos.positions, pos, samplesToShow, colour="red")
      addPoints( signalToPlot, p.minPos.positions, pos, samplesToShow, colour="red")
      addPoints( signalToPlot, p.leftIndex, pos, samplesToShow, colour="blue")
      addPoints( signalToPlot, p.rightIndex, pos, samplesToShow, colour="blue")   
      addPoints( signalToPlot, p.onset, pos, samplesToShow, colour="darkmagenta")   
      addPoints( signalToPlot, p.end, pos, samplesToShow, colour="darkmagenta")   
    }
    if(showTDelineation){
      addPoints( signalToPlot, t.peak.positions, pos, samplesToShow, colour="green")
      addPoints( signalToPlot, t.maxPos.positions, pos, samplesToShow, colour="red")
      addPoints( signalToPlot, t.minPos.positions, pos, samplesToShow, colour="red")
      addPoints( signalToPlot, t.leftIndex, pos, samplesToShow, colour="blue")
      addPoints( signalToPlot, t.rightIndex, pos, samplesToShow, colour="blue")      
    }

    message <- "\nPress return to continue  "
    message <- paste(message, pos, " ", pos + samplesToShow)
    readline(message) 
    pos <- pos + samplesToShow
    
  }

}


addPoints <- function( signalToPlot, array, currentPos, samplesToShow, colour = "red", ... ){
        posit <- array[which(array >= currentPos & array < (currentPos + samplesToShow - 1))] - currentPos + 1
        heig <- mat.or.vec(length(posit),1)
        cont <- 1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + currentPos - 1]
          cont <- cont + 1
        }        
        points(posit, heig, col = colour)
}
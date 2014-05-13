BeatDetectorWaveletPlot <- function(signalToPlot, peakPositions, samplesToShow = 800, ...){

	# ------------------------
	  peakPositions <- peakPositions[which(peakPositions != 0)]	+ 1 # suma 1 para compensar con C  

	  #dev.new()
	  pos <- 1 
	  while( pos < length(signalToPlot)){
	    to.show <- signalToPlot[ pos: (pos + samplesToShow)]
	    plot(to.show, col = "black", type = "ol")
	    posit <- peakPositions[which(peakPositions > pos & peakPositions < (pos + samplesToShow - 1))] - pos 
	    heig <- mat.or.vec(length(posit),1)
	    cont <-1
	    for( position in posit){ # For each position takes the height from the signal to plot
	      heig[cont] <- signalToPlot[position + pos]
	      cont <- cont + 1
	    }
	    points(posit, heig, col="green")

	    
	    message <- "Press return to continue  "
	    message <- paste(message, pos, " ", pos + samplesToShow)
	    readline(message)
	    pos <- pos + samplesToShow
	  }

	  # ------------------------

}
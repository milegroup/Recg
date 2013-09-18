PlotECG <- function( ECGData, channel = 1, showAnnot = FALSE , samplesToShow = 5000, ...){
  # Plot continuously the signal
  # 	
  # Args:
  #   ECGData: ECG data model
  #   channel: which channel show
  #   showAnnot: whether to show annotations
  #   samplesToShow: Number of samples to show in each iteration
  #

  if (missing(ECGData)){
    stop("ECGData param is mandatory")
  }

  pos <- 1	
  # Plot continuously the signal
  while (pos < length(ECGData$lead[[channel]]$data)) {
    to.show <- ECGData$lead[[channel]]$data[ pos: (pos + samplesToShow)]
    plot(to.show, col = "red", type = "l", xlab = "Original signal")
    if( showAnnot){
      #points.ini <- which(ECGData$annot$pos >= pos)[1]  # first point
      #points.end <- which(ECGData$annot$pos <= samplesToShow)[length(which(ECGData$annot$pos <= samplesToShow))]  # last point

      positions <- ECGData$annot$pos[which(ECGData$annot$pos >= pos & ECGData$annot$pos <= (pos + samplesToShow - 1)) ] - pos
      heights <- ECGData$annot$height[which(ECGData$annot$pos >= pos & ECGData$annot$pos <= (pos + samplesToShow - 1))] 

      points(positions, heights) 
    }
    #axis(1, at = c(pos:(pos + samplesToShow)), labels = c(pos:(pos + samplesToShow)), las = 2)
	  pos <- pos + samplesToShow
	  readline("Press return to continue")
  }

}

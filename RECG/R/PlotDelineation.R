PlotDelineation <- function(ECGData, signalToPlot, showNqrs = TRUE, showQRSDelineation= TRUE, showNPreNpost=TRUE, showNLastNFirst = TRUE, pointsOverTheSignal = TRUE, samplesToShow=5000, ...){
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
  if (missing(signalToPlot)){ # If no signal is passed takes the signal at derivation lead 1
    signalToPlot <- ECGData$lead[[1]]$data
  }

  # Save the data in variables with shorter names
  n.qrs <- ECGData$annot$pos
  qrs.onset.position <- ECGData$delineation$qrs.onset.position
  qrs.onset.height <- ECGData$delineation$qrs.onset.height
  qrs.end.position <- ECGData$delineation$qrs.end.position
  qrs.end.height <- ECGData$delineation$qrs.end.height

  # Prueba borrar ############
  qrs.onset.position.criteria1 <- ECGData$delineation$qrs.onset.position.criteria1
  qrs.onset.height.criteria1 <- ECGData$delineation$qrs.onset.height.criteria1
  qrs.onset.position.criteria2 <- ECGData$delineation$qrs.onset.position.criteria2
  qrs.onset.height.criteria2 <- ECGData$delineation$qrs.onset.height.criteria2

  qrs.end.position.criteria1 <- ECGData$delineation$qrs.end.position.criteria1
  qrs.end.height.criteria1 <- ECGData$delineation$qrs.end.height.criteria1
  qrs.end.position.criteria2 <- ECGData$delineation$qrs.end.position.criteria2
  qrs.end.height.criteria2 <- ECGData$delineation$qrs.end.height.criteria2
  ############


  n.pre.position <- ECGData$delineation$n.pre.position
  n.pre.height <- ECGData$delineation$n.pre.height
  n.post.position <- ECGData$delineation$n.post.position
  n.post.height <- ECGData$delineation$n.post.position
  n.first.position <- ECGData$delineation$n.first.position
  n.first.height <- ECGData$delineation$n.first.height
  n.last.position <- ECGData$delineation$n.last.position
  n.last.height <- ECGData$delineation$n.last.height


  # Loop that plot the signal continuously
  pos <- 1 
  while( pos < length(signalToPlot)){
    to.show <- signalToPlot[ pos: (pos + samplesToShow)]
    plot(to.show, col = "black", type = "l")

    if(pointsOverTheSignal){ # Plot the points height over the signal
      if(showNqrs){
        posit <- n.qrs[which(n.qrs >= pos & n.qrs < (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <- 1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        }        
        points(posit, heig, col="black")
      }
      if(showQRSDelineation){
        posit <- qrs.onset.position[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <-1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        } 
        #points(posit, heig, col="blue")

        posit <- qrs.end.position[which( qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <-1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        } 
        #points(posit, heig, col="blue")


        # Prueba borrar ############
        posit <- qrs.onset.position.criteria1[which( qrs.onset.position.criteria1 >= pos & qrs.onset.position.criteria1 <= (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <-1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        } 
        points(posit, heig, col="blue")

        posit <- qrs.onset.position.criteria2[which( qrs.onset.position.criteria2 >= pos & qrs.onset.position.criteria2 <= (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <-1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        } 
        points(posit, heig, col="orange")

        posit <- qrs.end.position.criteria1[which( qrs.end.position.criteria1 >= pos & qrs.end.position.criteria1 <= (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <-1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        } 
        points(posit, heig, col="blue")

        posit <- qrs.end.position.criteria2[which( qrs.end.position.criteria2 >= pos & qrs.end.position.criteria2 <= (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <-1
        for( position in posit){ # For each position takes the height from the signal to plot
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        } 
        points(posit, heig, col="orange")
        ############




      }

      if(showNPreNpost){
        posit <- n.pre.position[which( n.pre.position >= pos & n.pre.position <= (pos + samplesToShow - 1)) ] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <- 1
        for( position in posit){
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        }
        points(posit, heig, col="green")    

        posit <- n.post.position[which( n.post.position >= pos & n.post.position <= (pos + samplesToShow - 1)) ] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <- 1
        for( position in posit){
          heig[cont] <- signalToPlot[position + pos - 1]
          cont <- cont + 1
        }
        points(posit, heig, col="green")
      }

      if(showNLastNFirst){
        posit <- n.first.position[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <- 1
        for( position in posit){
          heig[cont] <- signalToPlot[position + pos]# - 1]
          cont <- cont + 1
        }
        points(posit, heig, col="red")

        posit <- n.last.position[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))] - pos
        heig <- mat.or.vec(length(posit),1)
        cont <- 1
        for( position in posit){
          heig[cont] <- signalToPlot[position + pos]# - 1]
          cont <- cont + 1
        }
        points(posit, heig, col="red")
      }

    }
    else{
      if(showNqrs){
        posit <- n.qrs[which(n.qrs >= pos & n.qrs < (pos + samplesToShow - 1))] - pos
        heig <- mat.or.vec(length(posit),1)
        points(posit, heig, col="black")
      }
      if(showQRSDelineation){
        posit <- qrs.onset.position[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))] - pos
        heig <- qrs.onset.height[which( qrs.onset.position >= pos & qrs.onset.position <= (pos + samplesToShow - 1))]
        points(posit, heig, col="blue")

        posit <- qrs.end.position[which( qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] - pos
        heig <- qrs.end.height[which(qrs.end.position >= pos & qrs.end.position <= (pos + samplesToShow - 1))] 
        points(posit, heig, col="blue")
      }
      if(showNPreNpost){
        posit <- n.pre.position[which( n.pre.position >= pos & n.pre.position <= (pos + samplesToShow - 1)) ] - pos
        heig <- n.pre.height[which( n.pre.position >= pos & n.pre.position <= (pos + samplesToShow -1 ))]
        points(posit, heig, col="green")    

        posit <- n.post.position[which( n.post.position >= pos & n.post.position <= (pos + samplesToShow - 1)) ] - pos
        heig <- n.post.height[which( n.post.position >= pos & n.post.position <= (pos + samplesToShow -1 ))]
        points(posit, heig, col="green")
      }
      if(showNLastNFirst){
        posit <- n.first.position[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ] - pos
        heig <- n.first.height[which( n.first.position >= pos & n.first.position <= (pos + samplesToShow -1)) ]
        points(posit, heig, col="red")

        posit <- n.last.position[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))] - pos
        heig <- n.last.height[which( n.last.position >= pos & n.last.position <= (pos + samplesToShow -1))]
        points(posit, heig, col="red")
      }
    }

    pos <- pos + samplesToShow
    readline("Press return to continue") 
  }

}
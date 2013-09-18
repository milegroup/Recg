ReadECG.WFDB.dat <- function(ECGData, datFile, format){
  # Read the ECG signal from an .dat file (Physionet format)
  # 
  # Args:
  #   ECGData: ECG data model
  #   datFile: Dat file name, including the path
  #   format: Which is the file format
  #
  # Returns:
  #   The data model with the ECG information added

  if (missing(datFile)){
    stop("   datFile is a mandatory param")
  }
  if (missing(format)){
    stop("   format is a mandatory param")
  }

  if (ECGData$Verbose){
    message <- paste("   Reading dat file ->", datFile, "\n", sep = " ")
    cat(message)
  } 

  

  out <- .C("getFileSize", datFile, fileSize = as.integer(0))

  file.size <- out$fileSize  


  # Create channels arrays
  samplesCh1 <- mat.or.vec(ECGData$samplesPerChannel, 1)
  samplesCh2 <- mat.or.vec(ECGData$samplesPerChannel, 1)

  
  if (format == 16){
    out <- .C("readFile16", datFile, samplesCh1 = as.double(samplesCh1), 
              samplesCh2 = as.double(samplesCh2), file.size)
  }
  else{
    if (format == 212){
       out <- .C("readFile212", datFile, samplesCh1 = as.double(samplesCh1), 
              samplesCh2 = as.double(samplesCh2), file.size)
    }
  }


  ECGData$lead[[1]]$data <- out$samplesCh1
  ECGData$lead[[2]]$data <- out$samplesCh2

  return(ECGData)
}
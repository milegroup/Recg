ReadECG.WFDB.header <- function(headerFile, format, Verbose = FALSE, ...){
  # Reads the header file
  #   
  # Args:
  #   headerFile: Header file name, including the path
  #   format: Which is the file format
  #   Verbose: Enable verbose mode (True or False)
  #
  # Returns:
  #   A list with the readed info.
  
  if (Verbose){
    message <- paste("   Reading header file ->", headerFile, "\n", sep = " ")
    cat(message)
  }  

  if (missing(headerFile)){
    stop("   headerFile is a mandatory param")
  }


  con <- file(headerFile)
  row1 <- readLines(con, 1)
  close(con)

  headerInfo <- list()
    
  splitRow                <- strsplit(row1, " ")[[1]] 
  headerInfo$name         <- splitRow[1]
  headerInfo$numLeads      <- as.numeric(splitRow[2])
  headerInfo$samplingFreq <- as.numeric(splitRow[3])
  headerInfo$samplesPerChannel   <- as.numeric(splitRow[4])  


  channels <- read.table(headerFile, skip = 1) # Skips the first line, which contain general info.

  # For each lead creates another list 
  for ( i in 1:headerInfo$numLeads){
    headerInfo$lead[[i]] <- list()
    headerInfo$lead[[i]]$filePathName <- channels[[1]][i]
    if ( missing(format)){
        headerInfo$lead[[i]]$format <- as.numeric(channels[[2]][i])
        format <- headerInfo$lead[[i]]$format
    }    
    headerInfo$lead[[i]]$ADCGain <- as.numeric(channels[[3]][i])
    headerInfo$lead[[i]]$ADCBitResolution <- as.numeric(channels[[4]][i])
    headerInfo$lead[[i]]$ADCZero <- as.numeric(channels[[5]][i])
    headerInfo$lead[[i]]$InitialValue <- as.numeric(channels[[6]][i])
    headerInfo$lead[[i]]$checkSum <- as.numeric(channels[[7]][i])
    headerInfo$lead[[i]]$blockSize <- as.numeric(channels[[8]][i])
    headerInfo$lead[[i]]$name <- channels[[9]][i]
  }

  if ( format != 16 && format != 212){
    stop("Sorry, but currently only format 16 and 212 are available")
  }

  return(headerInfo)
}
ReadECG.WFDB <- function(ECGData, headerFile, format, path = "./", ADCGain = 200, ...){
  # Reads an ECG file in Physionet format. Header and dat file must be in the same path
  # 	
  # Args:
  #   ECGData: ECG data model
  #   headerFile: Header's file name
  #   format: Which is the file format
  #   path: Where the file is
  #   ADCGain
  #
  # Returns:
  #   The data model with the added ECG information

  if (missing(ECGData)){
    stop("ECGData param is mandatory")
  }
  if (missing(headerFile)){
    stop("headerFile param is mandatory")
  }

  if (is.null(ECGData)){
    stop("ECGData model is not created. Use CreateECGData function in order 
          to create it")
  }

  hFile <- paste(path, headerFile, sep = "")

  # Check if header file exists
  if (!file.exists(hFile)){
    stop("Header file does not exist")
  }

  # Takes the .dat file from the header file name
  datFile <- strsplit(headerFile, ".", fixed = TRUE)[[1]][1]
  datFile <- paste(path, datFile, ".dat", sep = "") # concatenates the path
                                                    # and name

  if (!file.exists(datFile)){
    stop("Dat file does not exists, or is not in the same path as header file")
  }

  # Read header file and get info
  headerInfo <- ReadECG.WFDB.header(headerFile = hFile, format, ECGData$Verbose)

  ECGData$numLeads     <- headerInfo$numLeads
  ECGData$samplingFreq <- headerInfo$samplingFreq
  ECGData$samplesPerChannel   <- headerInfo$samplesPerChannel

  if ( missing(format)){
    format <- headerInfo$lead[[1]]$format
  }

  for ( i in 1:headerInfo$numLeads){
    ECGData$lead[[i]] <- list()
    ECGData$lead[[i]]$name <- headerInfo$lead[[i]]$name
  }

  # Read dat file and get info
  ECGData <- ReadECG.WFDB.dat(ECGData, datFile, format)
  
  ECGData <- ConvertToPhysicalUnits(ECGData, headerInfo)

  return(ECGData)
}

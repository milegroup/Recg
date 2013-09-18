ConvertToPhysicalUnits <- function(ECGData, headerInfo){
  


  

  # De momento para un solo canal
  i <- 0 
  while (i < headerInfo$numLeads){
    ADCGain <- headerInfo$lead[[i+1]]$ADCGain 
    ADCZero <- headerInfo$lead[[i+1]]$ADCZero

    data.size <- length(ECGData$lead[[i+1]]$data)



    out <- .C("convertToPhysicalUnits", as.integer(data.size), as.integer(ADCGain), as.integer(ADCZero), data = as.double(ECGData$lead[[i+1]]$data) )
    ECGData$lead[[i+1]]$data <- out$data
    
    i <- i + 1
  }

  

  return(ECGData)
}
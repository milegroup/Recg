ExportAscii <- function(ECGData, fileName, path = "./", ...){
  # Save the QRS positions in an ASCII file (txt file)
  # 
  # Args:
  #   ECGData: Data model containing QRS positions
  #   fileName: Name of the file to be written
  #   path: Where to save the file
  #

  if (missing(ECGData)){
    stop("   ECGData is a mandatory param")
  }    
  if (missing(fileName)){
    stop("   fileName is a mandatory param")
  }

  output.file <- paste(path, fileName, sep = "") # Concatenates the path and file name

  if (ECGData$Verbose){
    message <- paste("   Saving QRS positions in ", output.file, "\n", sep=" ")
    cat(message)
  }  

  # Save in file
  write(ECGData$annot$pos, file = output.file, ncolumns = 1, append = FALSE, sep = " " )
}
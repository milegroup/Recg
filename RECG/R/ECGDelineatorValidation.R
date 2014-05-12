ECGDelineatorValidation <- function(ECGData, reference.annotation.file, path="./", mnemonic = "N", ...){      
  match.window <- 0.15 * ECGData$samplingFreq # 0.15 s de ventana
  file <- paste(path, reference.annotation.file, sep = "")
  reference.annotations <- read.table(file)

  algorithm.annotations <- getECGDataAnnotations(ECGData, mnemonic)
  algorithm.annotations <- filterECGDataAnnotations( reference.annotations, algorithm.annotations )
  
  reference.annotations.peaks <- reference.annotations$V2[which(reference.annotations$V3 == mnemonic)]
  reference.annotations.onsets <- reference.annotations$V2[which(reference.annotations$V3 == mnemonic) - 1]
  reference.annotations.ends <- reference.annotations$V2[which(reference.annotations$V3 == mnemonic) + 1]

  mnemonics.onsets <- reference.annotations$V3[which(reference.annotations$V3 == mnemonic) - 1]

  number.of.annotations <- length(reference.annotations.peaks)
  validation <- createValidationList(number.of.annotations)

  validation <- applyValidation( number.of.annotations, algorithm.annotations$peaks, reference.annotations.peaks, algorithm.annotations$onsets, 
                                 reference.annotations.onsets, algorithm.annotations$ends, reference.annotations.ends, mnemonics.onsets,
                                 match.window, validation )
  
  if( mnemonic == "N"){    
    ECGData$validation$qrs <- validation
  }
  if( mnemonic == "p"){   
    ECGData$validation$p <- validation
  }
  if( mnemonic == "t"){   
    ECGData$validation$t <- validation
  }

  return(ECGData)
}

getECGDataAnnotations <- function(ECGData, mnemonic){
  algorithm.annotations <- list()
  if( mnemonic == "N" ){
    algorithm.annotations$peaks <- ECGData$annot$pos
    algorithm.annotations$onsets <- ECGData$delineation$qrs.onset.positions
    algorithm.annotations$ends <- ECGData$delineation$qrs.end.positions
  }
  if( mnemonic == "p" ){
    algorithm.annotations$peaks <- ECGData$delineation$p.peak.positions
    algorithm.annotations$onsets <- ECGData$delineation$p.onset
    algorithm.annotations$ends <- ECGData$delineation$p.end
  }

  if( mnemonic == "t"){
    algorithm.annotations$peaks <- ECGData$delineation$t.peak.positions
    algorithm.annotations$onsets <- ECGData$delineation$t.onset
    algorithm.annotations$ends <- ECGData$delineation$t.end
  }

  return(algorithm.annotations)
}

# Obtener la primera y última posicion del archivo anotado de referencia. Seleccionar las posiciones P válidas
filterECGDataAnnotations <- function( reference.annotations, algorithm.annotations ){    
  first.annotation <- reference.annotations$V2[1]
  last.annotation <- reference.annotations$V2[length(reference.annotations$V2)]
  valid.positions <- which( algorithm.annotations$peaks > first.annotation & algorithm.annotations$peaks < last.annotation )

  algorithm.annotations$peaks <- algorithm.annotations$peaks[valid.positions]
  algorithm.annotations$onsets <- algorithm.annotations$onsets[valid.positions]
  algorithm.annotations$ends <- algorithm.annotations$ends[valid.positions]

  return(algorithm.annotations)
}

applyValidation <- function(number.of.annotations, peak.positions, peak.positions.ref, onset.positions, onset.positions.ref,
                            end.positions, end.positions.ref, mnemonics.onsets, match.window, validation ){
  pos <- 1
  ref.pos <- 1
  cont.validas <- 1
  # Recorre todas las anotaciones del achivo archivo de referencia
  while( ref.pos <= number.of.annotations){  
    peak.pos <- peak.positions[pos]
    peak.pos.ref <- peak.positions.ref[ref.pos]
    is.valid.mnemonic = mnemonics.onsets[ref.pos] == "("
    if( is.valid.mnemonic ){  # Por si no existe, caso de la onda T
      onset.pos <- onset.positions[pos]    
      onset.pos.ref <- onset.positions.ref[ref.pos]
    }     
    end.pos <- end.positions[pos]
    end.pos.ref <- end.positions.ref[ref.pos]

    # Avanzar índice del pico menor
    #msg <- paste("\nDebugging: \npeak.pos", peak.pos, "\nmatch.window", match.window, "\npeak.pos.ref", peak.pos.ref)
    #cat(msg)
    if( peak.pos + match.window > peak.pos.ref ){
      ref.pos <- ref.pos + 1
    }
    if( peak.pos.ref + match.window > peak.pos ){
      pos <- pos + 1
    }
    

    # Si los peak hacen match compara la delineacion
    if( abs(peak.pos - peak.pos.ref) < match.window ){
      if( is.valid.mnemonic ){ # Por si no existe, caso de la onda T
        validation$onset.differences[cont.validas] <- onset.pos - onset.pos.ref # abs(onset.pos - onset.pos.ref)
      }      
      validation$end.differences[cont.validas] <- end.pos - end.pos.ref #abs(end.pos - end.pos.ref)      
      validation$peak.differences[cont.validas] <- peak.pos - peak.pos.ref # abs(peak.pos - peak.pos.ref)

      # Guardar posiciones de los picos (Para hacer pruebas)
      validation$annot[cont.validas] <- peak.pos
      validation$annot.ref[cont.validas] <- peak.pos.ref
      if( is.valid.mnemonic ){ # Por si no existe, caso de la onda T
        validation$annot.onset[cont.validas] <- onset.pos
        validation$annot.onset.ref[cont.validas] <- onset.pos.ref
      }
      validation$annot.end[cont.validas] <- end.pos
      validation$annot.end.ref[cont.validas] <- end.pos.ref
      cont.validas <- cont.validas + 1
    }

    
  }

  validation$number.of.matchs <- cont.validas - 1  
  validation$number.total <- number.of.annotations

  return(validation)
}

createValidationList <- function(number.of.annotations){
  validation <- list()

  # Pruebas
  validation$annot <- mat.or.vec(number.of.annotations, 1)
  validation$annot.ref <- mat.or.vec(number.of.annotations, 1)
  validation$annot.onset <- mat.or.vec(number.of.annotations, 1)
  validation$annot.onset.ref <- mat.or.vec(number.of.annotations, 1)
  validation$annot.end <- mat.or.vec(number.of.annotations, 1)
  validation$annot.end.ref <- mat.or.vec(number.of.annotations, 1)


  # Result arrays
  validation$onset.differences <- mat.or.vec(number.of.annotations, 1)
  validation$end.differences <- mat.or.vec(number.of.annotations, 1)
  return(validation)
}
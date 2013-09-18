WaveletDelineatorQRS <- function(ECGData, wtD2, wtD1, size=2^16){
  # Departing from the QRS annotation positions, delineates each QRS complex indicating its onset and offset
  # The qrs fiducial mark (n.qrs) is flanked by a pair of maximum modulus lines with opposite sign at scale 2 (n.pre and n.post)
  # n.pre and n.post positions are searched as a minimum negative and a maximum positive near the fiducial mark. Then the first peak
  # found is named n.pre and the other n.post
  # Next the delineator looks before n.pre and after n.post for significant maxima accounting for adjacent slopes (n.first and n.last)
  # Then q and s peaks are placed between n.first and n.pre, n.post and n.last
  # Finally qrs.onset and qrs.end are located before n.first and n.post
  # 
  # Args:
  #   ECGData: ECG data model 
  #   wtD2: The wavelet transform at scale decomposition level 2
  #   wtD1: The wavelet transform at scale decomposition level 1
  #   size: Number of samples used to update the threshold each time
  #
  # Returns:
  #   The ECG data model with the delineation positions added

  n.qrs <- ECGData$annot$pos

  # Buscar n.pre y n.post
  df.returned <- SearchNpreNpost(ECGData, wtD2)
  n.pre.position <- df.returned$n.pre
  n.pre.height <- df.returned$n.pre.height
  n.post.position <- df.returned$n.post
  n.post.height <- df.returned$n.post.height

  


  # Busca los máximos significativos antes de n.pre (n.first) y después de n.post (n.last)
  df.returned <- SearchNFirstNLastQPeakSPeak(wtD2, wtD1, n.pre.position, n.post.position, size)
  n.first.position <- df.returned$n.first.position
  n.first.height <- df.returned$n.first.height
  n.last.position <- df.returned$n.last.position
  n.last.height <- df.returned$n.last.height


  ECGData$delineation$q.peak.position <- df.returned$q.peak
  ECGData$delineation$q.peak.height <- df.returned$q.peak.height
  ECGData$delineation$s.peak.position <- df.returned$s.peak
  ECGData$delineation$s.peak.height <- df.returned$s.peak.height



  # Determine qrs.onset and qrs.end thresholds
  df.returned <- QrsOnsetEndThr(n.first.position, n.first.height, n.last.position, n.last.height)
  thr.qrs.on <- df.returned$qrs.on
  thr.qrs.end <- df.returned$qrs.end

  # Search qrs.onset and qrs.end positions
  df.returned <- SearchQrsOnsetEnd(wtD2, n.qrs, n.first.position, n.first.height, n.last.position, n.last.height, thr.qrs.on, thr.qrs.end )
  ECGData$delineation$qrs.onset.position <- df.returned$qrs.onset.position
  ECGData$delineation$qrs.onset.height <- df.returned$qrs.onset.height
  ECGData$delineation$qrs.end.position <- df.returned$qrs.end.position
  ECGData$delineation$qrs.end.height <- df.returned$qrs.end.height

  # Prueba borrar ############
  ECGData$delineation$qrs.onset.position.criteria1 <- df.returned$qrs.onset.position.criteria1
  ECGData$delineation$qrs.onset.height.criteria1 <- df.returned$qrs.onset.height.criteria1
  ECGData$delineation$qrs.onset.position.criteria2 <- df.returned$qrs.onset.position.criteria2
  ECGData$delineation$qrs.onset.height.criteria2 <- df.returned$qrs.onset.height.criteria2

  ECGData$delineation$qrs.end.position.criteria1 <- df.returned$qrs.end.position.criteria1
  ECGData$delineation$qrs.end.height.criteria1 <- df.returned$qrs.end.height.criteria1
  ECGData$delineation$qrs.end.position.criteria2 <- df.returned$qrs.end.position.criteria2
  ECGData$delineation$qrs.end.height.criteria2 <- df.returned$qrs.end.height.criteria2
  ############


  ECGData$delineation$n.pre.position <- n.pre.position
  ECGData$delineation$n.pre.height <- n.pre.height
  ECGData$delineation$n.post.position <- n.post.position
  ECGData$delineation$n.post.height <- n.post.height
  ECGData$delineation$n.first.position <- n.first.position
  ECGData$delineation$n.first.height <- n.first.height
  ECGData$delineation$n.last.position <- n.last.position
  ECGData$delineation$n.last.height <- n.last.height



  # PlotDelineationResults2(ECGData, wtD2, wtD1)

  return(ECGData)  
}
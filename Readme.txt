This is the RECG software package, created by Mile group (www.milegroup.net). This package is intended to be used by researchers in the area of signal processing, in particular the ECG processing. 
This is not a stable version of the package, it is a version still under construction. This means that it may contain errors and that there is some functions that are still under tests, so it may not produce the expected results.
In a few months we expect to release a new (stable) version, correcting the errors and adding more functionality.

This package was created and tested under the 3.0.2 R version. It contains C functions that are used to reduce the execution time in loops or time-consuming tasks. 

How to install and use the package?

1 - Install the package:
	
	* install.packages("RECG_1.1.tar.gz")
	or
	* R CMD install RECG_1.1.tar.gz

2 - Once the package is installed, you should load the package
	
	* library(RECG)

3 - Next, create a data model. The data model is an structure to save the ECG signal and processing information.
	
	* dm <- CreateECGDataModel()

4 - Then, read the ECG file. At this moment the package only reads files with the WFDB format (http://physionet.org/physiobank/database/#ecg). Currently the package reads the 16 and 212 format, but in some cases it has problems with the 212 format. It would be advisable to modify the file format from 212 to 16 using the xform function from WFDB (http://www.physionet.org/physiotools/wag/).

	* dm <- ReadECG.WFDB(dm, headerFile = "my_file", path = "my_path")

	Note: The results are saved in the data model. The file passed to the function should be the header file (.hea extension), and it looks for the data file (.dat extension) in the same path. You should place both files in the same path.

5 - Apply the detector algorithm.
	The package contains, currently, 4 different QRS detection algorithms.

	* dm <- BeatDetector(dm, algorithm = 4)


6 - Apply the delieation algorithm.
	The package contains, currently, 1 delineation algorithm. In some cases it fails seeking the T wave, this algorithm is also under construction.	

	* dm <- ECGDelineator(dm,1)

7 - Plotting the results
	The package contains a function to plot the results, depending on the different parameters the plot can be modified. You should take into account that this function was implemented for testing, while the package is under construction, so it is not the purpose of this package to show the results with a good and user-friendly GUI.

	* PlotDelineation(dm, scale=0, samplesToShow = 5000, showNqrs = FALSE,  showQRSDelineation = TRUE , showNPreNPost = TRUE, showNLastNFirst = TRUE, showPDelineation=FALSE, plotType='ol', showTDelineation=FALSE)
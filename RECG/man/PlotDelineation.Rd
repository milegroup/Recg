\name{PlotDelineation}
\alias{PlotDelineation}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
PlotDelineation
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
  Plots the signal containing the delineation positons annotated. It takes as parameters, which signal to plot, whether to show the QRS fiducial
  marks, whether to show the QRS delineation positions and the number of samples to show in each iteration.
  According to the parameters it receives, plot the signal with the corresponding annotations. 
}
\usage{
  PlotDelineation(ECGData, scale = 0, showNqrs = TRUE, showQRSDelineation= TRUE, showNPreNPost=TRUE, showNLastNFirst = TRUE, showPDelineation = TRUE, 
    showTDelineation = TRUE, samplesToShow=5000, plotType = "l", ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
  ECG data model
}
  
  \item{scale}{
    The wavelet scale to plot the result
  }
  \item{showNqrs}{
    whether to show or not qrs fiducial marks
  }
  \item{showQRSDelineation}{
    whether to show or not QRS delineation
  }
  \item{showNPreNPost}{
    whether to show or not Npre and Npost annotations
  }
  \item{showNLastNFirst}{
    whether to show or not NFirst and NLast annotations
  }
  \item{showPDelineation}{
    whether to show or not P delineation
  }
  \item{showTDelineation}{
   whether to show or not T delineation 
  }
  \item{samplesToShow}{
    Number of samples that are shown in each iteration
  }
  \item{plotType}{
    It defines the type of plot
  }
  \item{\dots}{
%%     ~~Describe \code{\dots} here~~
}
}
%\details{
%%  ~~ If necessary, more details than the description above ~~
%}
%\value{
%%  ~Describe the value returned
%%  If it is a LIST, use
%%  \item{comp1 }{Description of 'comp1'}
%%  \item{comp2 }{Description of 'comp2'}
%% ...
%}
%\references{
%% ~put references to the literature/web site here ~
%}
\author{
%%  ~~who you are~~
  Raúl Alonso Álvarez
}
%\note{
%%  ~~further notes~~
%}

\encoding{UTF-8}

%% ~Make other sections like Warning with \section{Warning }{....} ~

%\seealso{
%% ~~objects to See Also as \code{\link{help}}, ~~~
%}
%\examples{

%}
% Add one or more standard keywords, see file 'KEYWORDS' in the
% R documentation directory.
\keyword{ ~kwd1 }
\keyword{ ~kwd2 }% __ONLY ONE__ keyword per line

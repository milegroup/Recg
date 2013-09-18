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
  PlotDelineation(ECGData, signalToPlot, showNqrs = TRUE, showQRSDelineation= TRUE, showNPreNpost=TRUE, showNLastNFirst = TRUE, pointsOverTheSignal = TRUE, samplesToShow=5000, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
  ECG data model
}
  \item{signalToPlot}{
  the signal to be plotted
}
  \item{showNqrs}{
    whether to show qrs fiducial marks
  }
  \item{showQRSDelineation}{
    whether to show QRS delineation
  }
  \item{showNPreNpost}{
    whether to show n.pre and n.post positions
  }
  \item{showNLastNFirst}{
    whether to show n.first and n.last.positions
  }
  \item{pointsOverTheSignal}{
    If it is true, show the height of the points over the signal to be plotted
  }
  \item{samplesToShow}{
    Number of samples that are shown in each iteration
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

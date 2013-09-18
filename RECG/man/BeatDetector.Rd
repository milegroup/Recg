\name{BeatDetector}
\alias{BeatDetector}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
BeatDetector
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
  Detectes the QRS positions in an ECG signal
}
\usage{
  BeatDetector(ECGData, algorithm, channel = 1, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
    ECG data model
  } 
  \item{algorithm}{
    Select an algorithm, numeric value (from 1 to 3) is not mandatory
  }
  \item{channel}{
    In which channel the detection is applied
  }
  \item{\dots}{
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
\references{
J. Pan and W. J. Tompkins 1985, "A real-time QRS detection algorithm", \emph{Biomedical Engineering, IEEE Transactions}, vol \bold{BME-32}

P. S. Hamilton and W. J. Tompkins 1986, "Quantitative investigation of QRS detection rules using the MIT/BIH arrhythmia database", \emph{Biomedical Engineering, IEEE Transactions}, vol \bold{BME-33}
}
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

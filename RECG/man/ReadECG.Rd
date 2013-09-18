\name{ReadECG.WFDB}
\alias{ReadECG.WFDB}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
ReadECG.WFDB
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
  Read an ECG from a Physionet file
}
\usage{
  ReadECG.WFDB(ECGData, headerFile, format, path = "./", ADCGain = 200, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
    ECG data model
  }
  \item{headerFile}{
    Header file name
  }
  \item{format}{
    One of physionet file formats
  }
  \item{path}{
    Path where the .header and .dat files are located. It must be the same for both
  }
  \item{ADCGain}{
    ADCGain
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

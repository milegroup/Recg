\name{ECGDelineatorValidation}
\alias{ECGDelineatorValidation}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
ECGDelineatorValidation
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
  Writes the QRS positions in an ascii file
}
\usage{
  ECGDelineatorValidation(ECGData, reference.annotation.file, path="./", mnemonic = "N", ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
    ECG data model
  }
  \item{path}{
    The file's path
  }
  \item{reference.annotation.file}{
    File with the annotated positions
  }
  \item{mnemonic}{
    The type of the mnemonic used in the reference file
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
%\keyword{ ~kwd1 }
%\keyword{ ~kwd2 }% __ONLY ONE__ keyword per line

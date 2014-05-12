\name{PlotECG}
\alias{PlotECG}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
PlotECG
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
  Plots an array recursively. The array contain the ECG signal
}
\usage{
  PlotECG( ECGData, channel = 1, showAnnot = FALSE , samplesToShow = 5000, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
%%     ~~Describe \code{signal} here~~
  ECG data model
}
  \item{channel}{
%%     ~~Describe \code{samplesToShow} here~~
  Which channel to plot
}
  \item{showAnnot}{
    Whether to show annotations, boolean value
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

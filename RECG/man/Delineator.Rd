\name{Delineator}
\alias{Delineator}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
%%  ~~function to do ... ~~
Delineator
}
\description{
%%  ~~ A concise (1-5 lines) description of what the function does. ~~
  Applies a delineation based on a wavelet algorithm
}
\usage{
  Delineator(ECGData, algorithm, channel = 1, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{ECGData}{
    ECG data model
  } 
  \item{algorithm}{
    The algorithm number 
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
J. P. Martinez, R. Almeida, S. Olmos, A. P. Rocha and P. Laguna 2004, "A Wavelet-Based ECG Delineator: Evaluation on Standard Databases", \emph{Biomedical Engineering, IEEE Transactions}

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

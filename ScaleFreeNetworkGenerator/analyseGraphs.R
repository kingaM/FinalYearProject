library(ggplot2)
require(plyr)

plotHistogramDegrees <- function(x=NULL) {
  setwd("~/Documents/GitHub/FinalYearProject/ScaleFreeNetworkGenerator")
  values <- read.table("degrees.csv", sep=",")
  if (!x) {
    df <- data.frame(table(unlist(as.list(values))))
  } else {
    df <- data.frame(table(unlist(as.list(values[x,]))))
  }
  df <- mutate(df, relFreq = prop.table(Freq), Cumulative_Freq = cumsum(Freq),
    Cumulative_Relative_Freq = cumsum(relFreq))
  ggplot(df, aes(x=Var1, y=relFreq), stat="identity") +
    geom_histogram(stat="identity")
}

library(ggplot2)
require(plyr)

plotHistogramDegrees <- function(x=-1) {
  setwd("~/Documents/GitHub/FinalYearProject/ScaleFreeNetworkGenerator")
  values <- read.table("degrees.csv", sep=",")
  if (x == -1) {
    df <- data.frame(table(unlist(as.list(values))))
  } else {
    df <- data.frame(table(unlist(as.list(values[x,]))))
  }
  df <- mutate(df, relFreq = prop.table(Freq), Cumulative_Freq = cumsum(Freq),
    Cumulative_Relative_Freq = cumsum(relFreq))
  ggplot(df, aes(x=Var1, y=relFreq), stat="identity") +
    geom_histogram(stat="identity")
  ggsave(paste(paste("Degrees", toString(x), sep=" "), "png", sep="."), width=10)
}

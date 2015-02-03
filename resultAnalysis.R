require(omnetpp)
require(plyr)
require(parallel)

loadVector <- function(name) {
  vec <- loadDataset(name)
  val <- loadVectors(vec, NULL)
  return(val)
}

getPkts <- function(name) {
  sca <- loadDataset(name)
  gen <- subset(sca$scalars, name %in% c('dataGenerated:count'))$value
  rcvd <- subset(sca$scalars, name %in% c('recievedPackets:count'))$value
  return(rcvd/gen)
}

getKey <- function(vector, metricName) {
  if (nrow(subset(vector$vectors, name == metricName)) <= 0) {
    return(-1)
  }
  return(subset(vector$vectors, name == metricName)$resultkey)
}

getResults <- function (vector, metricName) {
  key <- getKey(vector, metricName)
  return(subset(vector$vectordata, resultkey %in% key))
}

roundTo100 <- function(x) {
  x = x - (x %% 100)
  return(x)
}

stepsOf100 <- function(vector) {
  vector$x = roundTo100(vector$x)
  return (aggregate(vector, by = list(vector$x), FUN = sum))
}

all <- function(name, metricName) {
  vec <- loadVector(name)
  res <- getResults(vec, metricName)
  if (nrow(res) <= 0) {
    res$Group.1 <- vector()
    return(res)
  }
  return (stepsOf100(res))
}

dcMaturity <- function(name) {
  fn = all(name, "falseNegative:vector")[,c("Group.1", "y")]
  fn = rename(fn, c("y"="fn"))
  fp = all(name, "falePositive:vector")[,c("Group.1", "y")]
  fp = rename(fp, c("y"="fp"))
  tp = all(name, "truePositive:vector")[,c("Group.1", "y")]
  tp = rename(tp, c("y"="tp"))
  tn = all(name, "trueNegative:vector")[,c("Group.1", "y")]
  tn = rename(tn, c("y"="tn"))
  tmp1 <- merge(fn, fp, by="Group.1", all=TRUE)
  tmp2 <- merge(tn, tp, by="Group.1", all=TRUE)
  final <- merge(tmp1, tmp2, by="Group.1", all=TRUE)
  final[is.na(final)] <- 0
  return(final)
}

filterClassification <- function(name) {
  fn = all(name, "falseNegativeFilter:vector")[,c("Group.1", "y")]
  fn = rename(fn, c("y"="fn"))
  fp = all(name, "falePositiveFilter:vector")[,c("Group.1", "y")]
  fp = rename(fp, c("y"="fp"))
  tp = all(name, "truePositiveFilter:vector")[,c("Group.1", "y")]
  tp = rename(tp, c("y"="tp"))
  tn = all(name, "trueNegativeFilter:vector")[,c("Group.1", "y")]
  tn = rename(tn, c("y"="tn"))
  tmp1 <- merge(fn, fp, by="Group.1", all=TRUE)
  tmp2 <- merge(tn, tp, by="Group.1", all=TRUE)
  final <- merge(tmp1, tmp2, by="Group.1", all=TRUE)
  final[is.na(final)] <- 0
  return(final)
}

dc <- function(name) {
  f <- dcMaturity(name)
  f$precision = f$tp / (f$tp + f$fp)
  f$recall = f$tp / (f$fn + f$tp)
  f$precision[f$tp == 0 & f$fp == 0] <- 1
  f$recall[f$tp == 0 & f$fn == 0] <- 1
  f$precision[is.na(f$precision)] <- 0
  f$recall[is.na(f$recall)] <- 0
  return(f)
}

filter <- function(name) {
  f <- filterClassification(name)
  f$precision = f$tp / (f$tp + f$fp)
  f$recall = f$tp / (f$fn + f$tp)
  f$precision[f$tp == 0 & f$fp == 0] <- 1
  f$recall[f$tp == 0 & f$fn == 0] <- 1
  f$precision[is.na(f$precision)] <- 0
  f$recall[is.na(f$recall)] <- 0
  return(f)
}

precision <- function() {
  setwd("~/Documents/GitHub/FinalYearProject/RandomNetworks/results")
  files = list.files(pattern = "RandomNetwork.*-0.vec")
  all <- mclapply(files, filter)
  tmp = mclapply(all, FUN = function(x) { sum(x$precision)/10 })
  return(unlist(tmp))
}

recall <- function() {
  setwd("~/Documents/GitHub/FinalYearProject/RandomNetworks/results")
  files = list.files(pattern = "RandomNetwork.*-0.vec")
  all <- mclapply(files, filter)
  tmp = mclapply(all, FUN = function(x) { sum(x$recall)/10 })
  return(unlist(tmp))
}

rcvPkts <- function() {
  setwd("~/Documents/GitHub/FinalYearProject/RandomNetworks/results")
  files = list.files(pattern = "RandomNetwork.*-0.sca")
  all <- mclapply(files, getPkts)
  return(unlist(all))
}

precision()
recall()
rcvPkts()
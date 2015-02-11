require(omnetpp)
require(plyr)
require(parallel)

directory <- "~/Documents/GitHub/FinalYearProject/RandomNetworks/"

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
  return (aggregate(vector, by = list(vector$x), FUN =  function(x){sum(as.numeric(x))}))
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
  return(precisionNameFilter("results"))
}

recall <- function() {
  return(recallNameFilter("results"))
}

rcvPktsName <- function(name) {
  setwd(paste(directory, name, sep=""))
  files = list.files(pattern = "RandomNetwork.*-0.sca")
  all <- mclapply(files, getPkts)
  return(changeListToDf(name, all, files))
}

changeListToDf <- function(name, list, files) {
  df <- data.frame(y = unlist(list), 
                   grp = rep(files[1:length(list)],
                             times = sapply(list,length)))
  df$grp = sub('(_[0-9])*-[0-9]\\.(sca|vec)', '', df$grp)
  df <- summarySE(df, measurevar="y", groupvars="grp")
  df$net = name
  show(df)
  return(df)
}

plotOne <- function(dirName, fileName) {
  setwd(paste(directory, dirName, sep=""))
  files = list.files(pattern = fileName)
  f <- lapply(files, filter)
  d <- lapply(files, dc)
  show(f)
  ggplot() +
    geom_line(data = f[[1]], aes(x = Group.1, y = precision, color = "Precision Filter")) + 
    geom_line(data = f[[1]], aes(x = Group.1, y = recall, color = "Recall Filter")) +
    geom_line(data = d[[1]], aes(x = Group.1, y = precision, color = "Precision DC")) + 
    geom_line(data = d[[1]], aes(x = Group.1, y = recall, color = "Recall DC")) +
    ylim(0,1)
}

precisionName <- function(name, FUN) {
  setwd(paste(directory, name, sep=""))
  files = list.files(pattern = "RandomNetwork.*-0.vec")
  all <- lapply(files, FUN)
  tmp = lapply(all, FUN = function(x) { sum(as.numeric(x$precision))/nrow(x) })
  show(tmp)
  return(return(changeListToDf(name, tmp, files)))
}

precisionNameFilter <- function(name) {
  return(precisionName(name, filter))
}

recallNameFilter <- function(name) {
  return(recallName(name, filter))
}

precisionNameDc <- function(name) {
  return(precisionName(name, dc))
}

recallNameDc <- function(name) {
  return(recallName(name, dc))
}

recallName <- function(name, FUN) {
  setwd(paste(directory, name, sep=""))
  files = list.files(pattern = "RandomNetwork.*-0.vec")
  all <- lapply(files, FUN)
  tmp = lapply(all, FUN = function(x) { mean(x$recall) })
  return(changeListToDf(name, tmp, files))
}

rcvPkts <- function() {
  setwd(paste(directory, "results", sep=""))
  files = list.files(pattern = "RandomNetwork.*-0.sca")
  all <- lapply(files, getPkts)
  return(unlist(all))
}

plotAll <- function(FUN, name) {
  library(plyr)
  library(ggplot2)
  library(reshape2)
  setwd(directory)
  files = list.files(pattern="results_.*")
  list <- mclapply(files, FUN)
  df <- ldply(list, data.frame)
  df$Index <- ave( 1:nrow(df), factor( df$net), FUN=function(x) 1:length(x) )
  show(df)
  pd <- position_dodge(.1)
  p <- ggplot(df, aes(x = Index, y = y, group = net, color = net, ymin=y-se, ymax=y+se), position = pd) + 
    geom_line(position = pd) +
    geom_point(position = pd, size = 3) + 
    ggtitle(name) + ylim(0,1) +
    geom_errorbar(width=0.2, position = pd)
  ggsave(paste(name, ".png", sep=""), width = 10)
  return(p)
}

plotEverything <- function() {
  library(gridExtra)
  p1 <- plotAll(rcvPktsName, "Rcvd Packets")
  p2 <- plotAll(precisionNameFilter, "Precision - Filter")
  p3 <- plotAll(recallNameFilter, "Recall - Filter")
  p4 <- plotAll(precisionNameDc, "Precision - DC")
  p5 <- plotAll(recallNameDc, "Recall - DC")
  grid.arrange(p1, p2, p3, p4, p5)
}

plotAverage <- function(dirName) {
  library(reshape)
  library(ggplot2)
  setwd(paste(directory, dirName, sep=""))
  files = list.files(pattern = "RandomNetwork.*-0.vec")
  f <- lapply(files, filter)
  fm <- merge_all(f)
  d <- lapply(files, dc)
  dm <- merge_all(d)
  fm <- aggregate(fm, by = list(fm$Group.1), FUN = mean)
  dm <- aggregate(dm, by = list(dm$Group.1), FUN = mean)
  ggplot() +
    geom_line(data = fm, aes(x = Group.1, y = precision, color = "Precision Filter")) + 
    geom_line(data = fm, aes(x = Group.1, y = recall, color = "Recall Filter")) +
    geom_line(data = dm, aes(x = Group.1, y = precision, color = "Precision DC")) + 
    geom_line(data = dm, aes(x = Group.1, y = recall, color = "Recall DC")) +
    ylim(0,1)
}

plotAverageErrorBars <- function(dirName) {
  library(reshape)
  library(ggplot2)
  setwd(paste(directory, dirName, sep=""))
  files = list.files(pattern = "RandomNetwork.*-0.vec")
  f <- lapply(files, filter)
  fm <- merge_all(f)
  d <- lapply(files, dc)
  dm <- merge_all(d)
  precisionF <- summarySE(fm, measurevar="precision", groupvars="Group.1")
  recallF <- summarySE(fm, measurevar="recall", groupvars="Group.1")
  precisionD <- summarySE(dm, measurevar="precision", groupvars="Group.1")
  recallD <- summarySE(dm, measurevar="recall", groupvars="Group.1")
  ggplot() + 
    geom_line(data=precisionF, aes(x=Group.1, y=precision, color="Precision Filter")) +
    geom_errorbar(data=precisionF, 
                  mapping=aes(ymin=precision-se, ymax=precision+se, x=Group.1, y=precision, color="Precision Filter"), 
                  width=10) +
    geom_line(data=recallF, aes(x=Group.1, y=recall, color="Recall Filter")) +
    geom_errorbar(data=recallF, 
                  mapping=aes(ymin=recall-se, ymax=recall+se, x=Group.1, y=recall, color="Recall Filter"), 
                  width=10) +
    geom_line(data=precisionD, aes(x=Group.1, y=precision, color="Precision DC")) +
    geom_errorbar(data=precisionD, 
                  mapping=aes(ymin=precision-se, ymax=precision+se, x=Group.1, y=precision, color="Precision DC"), 
                  width=10) +
    geom_line(data=recallD, aes(x=Group.1, y=recall, color="Recall DC")) +
    geom_errorbar(data=recallD, 
                  mapping=aes(ymin=recall-se, ymax=recall+se, x=Group.1, y=recall, color="Recall DC"), 
                  width=10) +
    ggtitle(dirName) +
    theme(legend.title=element_blank()) +
    ylab("") + xlab("Time") +
    ylim(0,1)
  ggsave(paste(dirName, "_plot.png", sep=""), width = 10)
}

# Taken from: http://www.cookbook-r.com/Manipulating_data/Summarizing_data/
summarySE <- function(data=NULL, measurevar, groupvars=NULL, na.rm=FALSE,
                      conf.interval=.95, .drop=TRUE) {
  require(plyr)
  
  # New version of length which can handle NA's: if na.rm==T, don't count them
  length2 <- function (x, na.rm=FALSE) {
    if (na.rm) sum(!is.na(x))
    else       length(x)
  }
  
  # This does the summary. For each group's data frame, return a vector with
  # N, mean, and sd
  datac <- ddply(data, groupvars, .drop=.drop,
                 .fun = function(xx, col) {
                   c(N    = length2(xx[[col]], na.rm=na.rm),
                     mean = mean   (xx[[col]], na.rm=na.rm),
                     sd   = sd     (xx[[col]], na.rm=na.rm)
                   )
                 },
                 measurevar
  )
  
  # Rename the "mean" column    
  datac <- rename(datac, c("mean" = measurevar))
  
  datac$se <- datac$sd / sqrt(datac$N)  # Calculate standard error of the mean
  
  # Confidence interval multiplier for standard error
  # Calculate t-statistic for confidence interval: 
  # e.g., if conf.interval is .95, use .975 (above/below), and use df=N-1
  ciMult <- qt(conf.interval/2 + .5, datac$N-1)
  datac$ci <- datac$se * ciMult
  
  return(datac)
}

# precision()
# recall()
# rcvPkts()
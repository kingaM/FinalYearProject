require(omnetpp)
require(plyr)
require(parallel)

directory <- "~/FinalYearProject/RandomNetworks/"
networkName <- "RandomNetwork.*-0"
# directory <- "~/FinalYearProject/ScaleFreeNetworks/"
# networkName <- "ScaleFreeNetwork.*-0"

glob.vectors <- list()
currFolder <- ""

loadVector <- function(name) {
  return(glob.vectors[[currFolder]][[name]])
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

rcvdPktVec <- function(name) {
  rcv = all(name, "recievedPackets:vector")[,c("Group.1", "y")]
  rcv = rename(rcv, c("y"="rcv"))
  gen = all(name, "dataGenerated:vector")[,c("Group.1", "y")]
  gen = rename(gen, c("y"="gen"))
  tmp <- merge(gen, rcv, by="Group.1", all=TRUE)
  tmp[is.na(tmp)] <- 0
  tmp$ratio <- tmp$rcv / tmp$gen
  return(tmp)
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
  files = list.files(pattern = paste(networkName, "sca", sep="."))
  all <- lapply(files, getPkts)
  return(changeListToDf(name, all, files))
}

changeListToDf <- function(name, list, files) {
  show(list)
  df <- data.frame(y = unlist(list),
                   grp = rep(files[1:length(list)],
                             times = sapply(list,length)))
  if (networkName == "RandomNetwork.*-0") {
    df$grp = sub('(_[0-9]+)*-[0-9]+\\.(sca|vec)', '', df$grp)
  } else {
    df$grp = sub('(_[0-9]+)*-[0-9]+\\.(sca|vec)', '', df$grp)
    df$grp = sub('ScaleFreeNetwork', '', df$grp)
    df$grp = strtoi(df$grp)
    df$grp = df$grp %% 2
    df$grp[df$grp == 1] <- "major"
    df$grp[df$grp == 0] <- "minor"
  }
  df.group <- summarySE(df, measurevar="y", groupvars="grp")
  df.group$net = name
  df$net = name
  return(list(df.group, df))
}

plotOne <- function(dirName, fileName) {
  setwd(paste(directory, dirName, sep=""))
  currFolder <<- dirName
  files = list.files(pattern = fileName)
  f <- lapply(files, filter)
  d <- lapply(files, dc)
  ggplot() +
    geom_line(data = f[[1]], aes(x = Group.1, y = precision, color = "Precision Filter")) +
    geom_line(data = f[[1]], aes(x = Group.1, y = recall, color = "Recall Filter")) +
    geom_line(data = d[[1]], aes(x = Group.1, y = precision, color = "Precision DC")) +
    geom_line(data = d[[1]], aes(x = Group.1, y = recall, color = "Recall DC")) +
    ylim(0,1)
}

precisionName <- function(name, FUN) {
  setwd(paste(directory, name, sep=""))
  currFolder <<- name
  files = list.files(pattern = paste(networkName, "vec", sep="."))
  all <- lapply(files, FUN)
  tmp = lapply(all, FUN = function(x) { sum(as.numeric(x$precision))/nrow(x) })
  show(all)
  return(changeListToDf(name, tmp, files))
}

precisionNameAll <- function(name, FUN) {
  setwd(paste(directory, name, sep=""))
  currFolder <<- name
  files = list.files(pattern = paste(networkName, "vec", sep="."))
  all <- lapply(files, FUN)
  show(all)
  return(changeListToDf2(name, all, files, "precision"))
}

recallNameAll <- function(name, FUN) {
  setwd(paste(directory, name, sep=""))
  currFolder <<- name
  files = list.files(pattern = paste(networkName, "vec", sep="."))
  all <- lapply(files, FUN)
  show(all)
  return(changeListToDf2(name, all, files, "recall"))
}

rcvPktVecAll <- function(name) {
  setwd(paste(directory, name, sep=""))
  currFolder <<- name
  files = list.files(pattern = paste(networkName, "vec", sep="."))
  all <- lapply(files, rcvdPktVec)
  show(all)
  return(changeListToDf2(name, all, files, "ratio"))
}

changeListToDf2 <- function(name, list, files, measurevar) {
  df.group <- summarySE(ldply(list, data.frame), measurevar=measurevar, groupvars="Group.1")
  colnames(df.group)[3] <- "y"
  df.group$net = name
  df <- ldply(list, data.frame)
  colnames(df)[3] <- "y"
  df$net = name
  return(list(df.group, df))
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

precisionNameFilterTime <- function(name) {
  return(precisionNameAll(name, filter))
}

recallNameFilterTime <- function(name) {
  return(recallNameAll(name, filter))
}

precisionNameDcTime <- function(name) {
  return(precisionNameAll(name, dc))
}

recallNameDcTime <- function(name) {
  return(recallNameAll(name, dc))
}

recallName <- function(name, FUN) {
  setwd(paste(directory, name, sep=""))
  currFolder <<- name
  files = list.files(pattern = paste(networkName, "vec", sep="."))
  all <- lapply(files, FUN)
  tmp = lapply(all, FUN = function(x) { sum(as.numeric(x$recall))/nrow(x) })
  return(changeListToDf(name, tmp, files))
}

rcvPkts <- function() {
  setwd(paste(directory, "results", sep=""))
  files = list.files(pattern = paste(networkName, "sca", sep="."))
  all <- lapply(files, getPkts)
  return(unlist(all))
}

plotAll <- function(FUN, name) {
  library(plyr)
  library(ggplot2)
  library(reshape2)
  setwd(directory)
  show("In PLOT ALL")
  files = list.files(pattern="results_.*")
  list <- lapply(files, FUN=function(x) FUN(x)[1])
  csv <- lapply(files, FUN=function(x) FUN(x)[2])
  show("lapply finished")
  write.csv(ldply(csv, data.frame), file=paste(name, ".csv", sep=""))
  df <- ldply(list, data.frame)
  df$Index <- ave( 1:nrow(df), factor( df$net), FUN=function(x) 1:length(x) )
  show(df)
  pd <- position_dodge(.1)
  p <- ggplot(df, aes(x = Index, y = y, group = net, color = net, ymin=y-se, ymax=y+se), position = pd) +
    geom_line(position = pd) +
    geom_point(position = pd, size = 3) +
    ggtitle(name) + ylim(0,1) +
    geom_errorbar(width=0.2, position = pd)
  ggsave(paste(name, ".png", sep=""), width = 10, height=7)
  return(p)
}

plotAllTime <- function(FUN, name) {
  library(plyr)
  library(ggplot2)
  library(reshape2)
  setwd(directory)
  files = list.files(pattern="results_.*")
  list <- lapply(files, FUN=function(x) FUN(x)[1])
  csv <- lapply(files, FUN=function(x) FUN(x)[2])
  write.csv(ldply(csv, data.frame), file=paste(paste("Time", name, sep=" "), ".csv", sep=""))
  df <- ldply(list, data.frame)
  df$Index <- ave( 1:nrow(df), factor( df$net), FUN=function(x) 1:length(x) )
  df <- ldply(list, data.frame)
  df$Index <- ave( 1:nrow(df), factor( df$net), FUN=function(x) 1:length(x) )
  show(df)
  pd <- position_dodge(20)
  p <- ggplot(df, aes(x = Group.1, y = y, group = net, color = net, ymin=y-se, ymax=y+se), position = pd) +
    geom_line(position = pd) +
    geom_point(position = pd, size = 3) +
    ggtitle(name) + ylim(0,1) +
    geom_errorbar(width=30, position = pd)
  ggsave(paste(paste("Time", name, sep=" "), ".png", sep=""), width = 10, height=7)
  return(p)
}

plotEverything <- function() {
  library(gridExtra)
  p1 <- plotAll(rcvPktsName, "Rcvd Packets")
  p2 <- plotAll(precisionNameFilter, "Precision - Filter")
  p3 <- plotAll(recallNameFilter, "Recall - Filter")
  p4 <- plotAll(precisionNameDc, "Precision - DC")
  p5 <- plotAll(recallNameDc, "Recall - DC")
  png("All.png", width=1375, height=876)
  grid.arrange(p1, p2, p3, p4, p5)
  dev.off()
}

plotEverythingTime <- function() {
  library(gridExtra)
  p1 <- plotAllTime(rcvPktVecAll, "Rcvd Packets")
  p2 <- plotAllTime(precisionNameFilterTime, "Precision - Filter")
  p3 <- plotAllTime(recallNameFilterTime, "Recall - Filter")
  p4 <- plotAllTime(precisionNameDcTime, "Precision - DC")
  p5 <- plotAllTime(recallNameDcTime, "Recall - DC")
  png("All Time.png", width=1375, height=876)
  grid.arrange(p1, p2, p3, p4, p5)
  dev.off()
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

load("./RandomNetworks/results_scalefree_21/vectors.csv")
glob.vectors <<- list
plotEverything()
#plotEverythingTime()

                  


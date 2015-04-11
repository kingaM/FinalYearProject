library(agricolae)
library(ggplot2)

HSDTest <- function(file, one, two) {
  df <- read.csv(file, header=TRUE)
  show(file)
  if (grepl("Time", file)) {
    colnames(df)[colnames(df) == "Group.1"] <- "grp"
  }
  ajuste <- lm(df$y ~ df$net + df$grp)
  summary(ajuste)
  show(anova(ajuste))
  a1 <- aov(df$y ~ df$net + df$grp)
  posthoc <- TukeyHSD(x=a1, 'df$net', conf.levfileel=0.95)
  a <- HSD.test(ajuste, 'df$net')
  show(a)
  return(a)
}

KSTest <- function(file, x1, x2) {
  df <- read.csv(file, header=TRUE)
  one = subset(df, df$net == x1)$y
  two = subset(df, df$net == x2)$y
  show(mean(one))
  show(mean(two))
  show(mean(one) - mean(two))
  ks.test(one, two, alternative="two.sided")
}

KSTestSF <- function(file, attacker) {
  df <- read.csv(file, header=TRUE)
  df = subset(df, df$net == attacker)
  one = subset(df, df$grp == "major")$y
  two = subset(df, df$grp == "minor")$y
  show(mean(one))
  show(mean(two))
  show(mean(one) - mean(two))
  ks.test(one, two, alternative="two.sided")
}

KWTest <- function(file) {
  df <- read.csv(file, header=TRUE)
  df = subset(df, df$grp == "minor")
  show(file)
  if (grepl("Time", file)) {
    colnames(df)[colnames(df) == "Group.1"] <- "grp"
  }
  show(df)
  kruskal.test(df$y ~ df$net)
}

KSTestSNAIS <- function(file, attacker) {
  setwd("~/FinalYearProject/ScaleFreeNetworks/Plots/Final")
  dfd <- read.csv(file)
  setwd("~/FinalYearProject/ScaleFreeNetworks/Plots/FinalEnabled")
  dfe <- read.csv(file)
  e <- subset(dfe, dfe$net == attacker)$y
  d <- subset(dfd, dfd$net == attacker)$y 
  show(mean(e))
  show(mean(d))
  show(mean(e) - mean(d))
  ks.test(e, d, alternative="two.sided")
}

hist <- function(file, result) {
  show(file)
  show(result)
  df <- read.csv(file, header=TRUE)
  t = table(round(subset(df, df$net == result)$y, 2))
  plot(t, xlim=c(0,1))
}

setwd("~/FinalYearProject/ScaleFreeNetworks/Plots/FinalEnabled")
files = list.files(pattern="(P|R).*\\.csv")

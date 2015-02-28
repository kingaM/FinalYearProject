source('~/Documents/GitHub/FinalYearProject/resultAnalysis.R')

args <- commandArgs(trailingOnly = TRUE)
if (args[1] == "N") {
  if(args[2] == "R") {
    plotAll(rcvPktsName, "Rcvd Packets")
  }
  if(args[2] == "PF") {
    plotAll(precisionNameFilter, "Precision - Filter")
  }
  if(args[2] == "RF") {
    plotAll(recallNameFilter, "Recall - Filter")
  }
  if(args[2] == "PD") {
    plotAll(precisionNameDc, "Precision - DC")
  }
  if(args[2] == "RD") {
    plotAll(recallNameDc, "Recall - DC")
  }
} else {
  if(args[2] == "R") {
    plotAllTime(rcvPktVecAll, "Rcvd Packets")
  }
  if(args[2] == "PF") {
    plotAllTime(precisionNameFilterTime, "Precision - Filter")
  }
  if(args[2] == "RF") {
    plotAllTime(recallNameFilterTime, "Recall - Filter")
  }
  if(args[2] == "PD") {
    plotAllTime(precisionNameDcTime, "Precision - DC")
  }
  if(args[2] == "RD") {
    plotAllTime(recallNameDcTime, "Recall - DC")
  }
}
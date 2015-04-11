require(omnetpp)

directory <- "~/FinalYearProject/RandomNetworks/"
networkName <- "RandomNetwork.*-0"

loadVectorsAllFolders <- function() {
  setwd(directory)
  files = list.files(pattern="results_.*")
  list <- sapply(files, loadVectorsAllFiles, simplify = FALSE,USE.NAMES = TRUE)
  save(list, file="vectors.csv")
}

loadVectorsAllFiles <- function(folder) {
  setwd(paste(directory, folder, sep=""))
  files = list.files(pattern = paste(networkName, "vec", sep="."))
  all <- sapply(files, loadSingleVector, simplify = FALSE,USE.NAMES = TRUE)
  return (all)
}

loadSingleVector <- function(name) {
  vec <- loadDataset(name)
  val <- loadVectors(vec, NULL)
  return(val)
}

loadVectorsAllFolders()
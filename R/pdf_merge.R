#' Merge multiple PDF files into one
#'
#' @param input_paths Character vector of input PDF file paths
#' @param output_path Output PDF file path
#' @return (Invisibly) the output path
#' @details Uses the qpdf R package.
#' @export
pdf_merge <- function(input_paths, output_path) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  qpdf::pdf_combine(input_paths, output = output_path)
  invisible(output_path)
}

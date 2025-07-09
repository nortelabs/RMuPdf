#' Select specific pages from a PDF
#'
#' @param pdf_path Input PDF file
#' @param pages Integer vector of pages to keep (1-based)
#' @param output_path Output PDF file
#' @return (Invisibly) the output path
#' @details Uses the qpdf R package. No external system dependencies required.
#' @export
pdf_select_pages <- function(pdf_path, pages, output_path) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  n_pages <- qpdf::pdf_length(pdf_path)
  if (any(pages < 1 | pages > n_pages)) stop("Invalid page numbers.")
  qpdf::pdf_subset(pdf_path, pages = pages, output = output_path)
  invisible(output_path)
}

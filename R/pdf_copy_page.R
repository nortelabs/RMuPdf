#' Copy a page and append it to the end of the PDF
#'
#' @param pdf_path Input PDF file
#' @param page_num Page number to copy (1-based)
#' @param output_path Output PDF file
#' @return (Invisibly) the output path
#' @details Uses the qpdf R package. No external system dependencies required.
#' @export
pdf_copy_page <- function(pdf_path, page_num, output_path) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  n_pages <- qpdf::pdf_length(pdf_path)
  if (page_num < 1 || page_num > n_pages) stop("Invalid page number.")
  # Keep all pages, then append the selected page again
  pages <- c(seq_len(n_pages), page_num)
  qpdf::pdf_subset(pdf_path, pages = pages, output = output_path)
  invisible(output_path)
}

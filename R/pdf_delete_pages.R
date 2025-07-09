#' Delete pages from a PDF using qpdf
#'
#' @param pdf_path Path to input PDF
#' @param from_page First page to delete (integer)
#' @param to_page Last page to delete (integer)
#' @param output_path Path to save the new PDF
#' @return (Invisibly) the output path
#' @details Uses the qpdf R package. No external system dependencies required.
#' @export
pdf_delete_pages <- function(pdf_path, from_page, to_page, output_path) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  n_pages <- qpdf::pdf_length(pdf_path)
  all_pages <- seq_len(n_pages)
  keep <- setdiff(all_pages, from_page:to_page)
  if (length(keep) == 0) stop("Cannot delete all pages: at least one page must remain.")
  qpdf::pdf_subset(pdf_path, pages = keep, output = output_path)
  invisible(output_path)
}

#' Encrypt a PDF file with a password
#'
#' @param pdf_path Input PDF file path
#' @param output_path Output PDF file path
#' @param password Password to set for the PDF
#' @return (Invisibly) the output path
#' @details Uses the qpdf R package.
#' @export
pdf_encrypt <- function(pdf_path, output_path, password) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  qpdf::pdf_encrypt(pdf_path, output = output_path, upw = password, opw = password)
  invisible(output_path)
}

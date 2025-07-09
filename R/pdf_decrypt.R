#' Decrypt a password-protected PDF file
#'
#' @param pdf_path Input PDF file path
#' @param output_path Output PDF file path
#' @param password Password for the PDF
#' @return (Invisibly) the output path
#' @details Uses the qpdf R package.
#' @export
pdf_decrypt <- function(pdf_path, output_path, password) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  qpdf::pdf_decrypt(pdf_path, output = output_path, password = password)
  invisible(output_path)
}

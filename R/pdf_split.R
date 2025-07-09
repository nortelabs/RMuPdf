#' Split a PDF into separate files for each page
#'
#' @param pdf_path Input PDF file path
#' @param output_dir Directory to save the split PDFs
#' @return Character vector of output file paths
#' @details Uses the qpdf R package.
#' @export
pdf_split <- function(pdf_path, output_dir) {
  if (!requireNamespace("qpdf", quietly = TRUE)) {
    stop("The 'qpdf' package is required but not installed.")
  }
  n_pages <- qpdf::pdf_length(pdf_path)
  out_files <- character(n_pages)
  for (i in seq_len(n_pages)) {
    out_file <- file.path(output_dir, sprintf("page-%03d.pdf", i))
    qpdf::pdf_subset(pdf_path, pages = i, output = out_file)
    out_files[i] <- out_file
  }
  invisible(out_files)
}

# Basic tests for RMuPdf package
library(testthat)
library(RMuPdf)

# Helper: Use a sample PDF (should be included in inst/extdata for real packages)
sample_pdf <- system.file("extdata", "sample.pdf", package = "RMuPdf")

# 1. Test pdf_open and pdf_page_count
if (file.exists(sample_pdf)) {
  test_that("pdf_open returns a document pointer", {
    doc <- pdf_open(sample_pdf)
    expect_true(!is.null(doc))
  })

  test_that("pdf_page_count returns positive integer", {
    doc <- pdf_open(sample_pdf)
    n <- pdf_page_count(doc)
    expect_true(is.numeric(n) && n > 0)
  })
} else {
  test_that("sample PDF is present", {
    skip("No sample PDF found for testing.")
  })
}

# 2. Test pdf_extract_text
if (file.exists(sample_pdf)) {
  test_that("pdf_extract_text returns character vector", {
    doc <- pdf_open(sample_pdf)
    text <- pdf_extract_text(doc)
    expect_true(is.character(text))
  })
}

# 3. Test pdf_render_page (output file creation)
if (file.exists(sample_pdf)) {
  test_that("pdf_render_page creates an image file", {
    doc <- pdf_open(sample_pdf)
    out <- tempfile(fileext = ".png")
    pdf_render_page(doc, 1L, out)
    expect_true(file.exists(out))
    unlink(out)
  })
}

# 4. Test mutool-based page deletion (if mutool is available and PDF has >1 page)
mutool_path <- Sys.which("mutool")
if (nzchar(mutool_path) && file.exists(sample_pdf)) {
  test_that("pdf_delete_pages with mutool works", {
    doc <- pdf_open(sample_pdf)
    n_pages <- pdf_page_count(doc)
    if (n_pages < 2) {
      skip("Sample PDF must have at least 2 pages for deletion test.")
    }
    out <- tempfile(fileext = ".pdf")
    # Try deleting the last page (should always leave at least one page)
    result <- tryCatch({
      pdf_delete_pages(sample_pdf, n_pages, n_pages, out)
      TRUE
    }, error = function(e) {
      cat("mutool deletion error:\n", conditionMessage(e), "\n")
      FALSE
    })
    expect_true(result)
    expect_true(file.exists(out))
    unlink(out)
  })
} else {
  test_that("mutool is available for page deletion", {
    skip("mutool not available or no sample PDF.")
  })
}


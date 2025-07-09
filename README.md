# RMuPdf
R package to manage PDF documents.

## Example Usage

```r
# Load the RMuPdf package
library(RMuPdf)

# Open a PDF document
pdf <- pdf_open("/path/to/document.pdf")

# Get MuPDF library version
mupdf_version()

# Get the number of pages
pdf_page_count(pdf)

# Extract text from all pages
text <- pdf_extract_text(pdf)
cat(text[1])  # Print text from the first page

# Render a page to PNG
pdf_render_page(pdf, page_num = 1, path = "page1.png", dpi = 150)

# Get image xrefs from a page
xrefs <- pdf_page_image_xrefs(pdf, page_num = 1)

# Save the first image from the page
if (length(xrefs) > 0) {
  pdf_save_image(pdf, xrefs[1], path = "image1.png")
}
```


## Deleting Pages


To delete a range of pages from a PDF document:

```r
# Delete pages 9 to 14 (inclusive)
pdf_delete_pages("input.pdf", 9, 14, "deleted-pages.pdf")
```

To delete a single page from a PDF document:

```r
# Delete the 1st page of the document
pdf_delete_pages("input.pdf", 1, 1, "deleted-page-one.pdf")
```

## Copying Pages

You can copy a page and append it to the end of a PDF using the RMuPdf function `pdf_copy_page()` (powered by the qpdf R package):

```r
# Copy the 1st page and append it to the end
pdf_copy_page("input.pdf", 1, "copied-page.pdf")
```

## Selecting Pages

You can create a new PDF with only the pages you want using the RMuPdf function `pdf_select_pages()`:

```r
# Select the 1st and 2nd pages only
pdf_select_pages("input.pdf", c(1, 2), "just-page-one-and-two.pdf")
```

## PDF Merging

You can merge multiple PDF files into one using `pdf_merge()`:

```r
pdf_merge(c("file1.pdf", "file2.pdf"), "merged.pdf")
```

## PDF Splitting

You can split a PDF into separate files for each page using `pdf_split()`:

```r
pdf_split("input.pdf", "output_dir/")
# This will create output_dir/page-001.pdf, output_dir/page-002.pdf, etc.
```

## PDF Encryption

You can encrypt a PDF file with a password using `pdf_encrypt()`:

```r
pdf_encrypt("input.pdf", "encrypted.pdf", password = "secret")
```

## PDF Decryption

You can decrypt a password-protected PDF file using `pdf_decrypt()`:

```r
pdf_decrypt("encrypted.pdf", "decrypted.pdf", password = "secret")
```


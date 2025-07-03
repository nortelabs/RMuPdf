#ifndef RMUPDF_UTILS_H
#define RMUPDF_UTILS_H

#include <Rcpp.h>
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>

// RAII wrapper for MuPDF document and context
struct MuPDFDoc {
    fz_context *ctx{nullptr};
    fz_document *doc{nullptr};

    // Destructor for automatic cleanup, called by Rcpp::XPtr finalizer
    ~MuPDFDoc() {
        if (ctx && doc) {
            fz_drop_document(ctx, doc);
        }
        if (ctx) {
            fz_drop_context(ctx);
        }
    }
};

// Helper to safely get the MuPDFDoc pointer from the R external pointer
static inline MuPDFDoc* get_handle(SEXP xp) {
    Rcpp::XPtr<MuPDFDoc> ptr(xp);
    if (!ptr || !(ptr->ctx) || !(ptr->doc)) {
        Rcpp::stop("Invalid mupdf_document handle");
    }
    return ptr.get();
}

// Helper to safely cast fz_document to pdf_document
static inline pdf_document* get_pdf_document(MuPDFDoc* h) {
    pdf_document *pdf = pdf_specifics(h->ctx, h->doc);
    if (!pdf) {
        Rcpp::stop("This operation requires a PDF document.");
    }
    return pdf;
}

#endif // RMUPDF_UTILS_H

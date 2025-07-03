#include "utils.h"

// [[Rcpp::export]]
SEXP pdf_open(std::string path) {
    // Create context
    fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    if (!ctx) {
        Rcpp::stop("Could not create MuPDF context");
    }
    fz_register_document_handlers(ctx);

    fz_document *doc = NULL;
    try {
        doc = fz_open_document(ctx, path.c_str());
    } catch (...) {
        std::string err("Unknown error");
        if (ctx) err = fz_caught_message(ctx);
        fz_drop_context(ctx);
        Rcpp::stop("Failed to open document: " + err);
    }

    MuPDFDoc *handle = new MuPDFDoc{ctx, doc};
    Rcpp::XPtr<MuPDFDoc> xp(handle, true);
    xp.attr("class") = "mupdf_document";
    return xp;
}

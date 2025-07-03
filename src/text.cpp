#include "utils.h"

// [[Rcpp::export]]
int pdf_page_count(SEXP doc_xptr) {
    MuPDFDoc* h = get_handle(doc_xptr);
    return fz_count_pages(h->ctx, h->doc);
}

// [[Rcpp::export]]
Rcpp::CharacterVector pdf_extract_text(SEXP doc_xptr) {
    MuPDFDoc* h = get_handle(doc_xptr);
    int n = fz_count_pages(h->ctx, h->doc);
    Rcpp::CharacterVector pages(n);

    for (int i = 0; i < n; ++i) {
        fz_page *page = nullptr;
        fz_stext_page *text_page = nullptr;
        fz_device *dev = nullptr;
        try {
            page = fz_load_page(h->ctx, h->doc, i);
            fz_rect mediabox = fz_bound_page(h->ctx, page);
            text_page = fz_new_stext_page(h->ctx, mediabox);
            dev = fz_new_stext_device(h->ctx, text_page, nullptr);
            fz_run_page(h->ctx, page, dev, fz_identity, nullptr);
            fz_close_device(h->ctx, dev);
            fz_drop_device(h->ctx, dev);
            dev = nullptr;

            // Get text as utf8 string
            fz_buffer *buf = fz_new_buffer(h->ctx, 0);
            fz_output *out = fz_new_output_with_buffer(h->ctx, buf);
            fz_print_stext_page_as_text(h->ctx, out, text_page);
            fz_drop_output(h->ctx, out);
            pages[i] = std::string((char*)buf->data, buf->len);
            fz_drop_buffer(h->ctx, buf);
            fz_drop_stext_page(h->ctx, text_page);
            fz_drop_page(h->ctx, page);
        } catch (...) {
            if (dev) { fz_drop_device(h->ctx, dev); }
            if (text_page) { fz_drop_stext_page(h->ctx, text_page); }
            if (page) { fz_drop_page(h->ctx, page); }
            std::string msg = fz_caught_message(h->ctx);
            Rcpp::stop("Error extracting text: " + msg);
        }
    }
    return pages;
}

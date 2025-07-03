#include "utils.h"

// [[Rcpp::export]]
void pdf_render_page(SEXP doc_xptr, int page_num, std::string path, int dpi = 150) {
    MuPDFDoc* h = get_handle(doc_xptr);
    
    fz_page *page = nullptr;
    fz_device *dev = nullptr;
    fz_pixmap *pix = nullptr;
    fz_output *out = nullptr;

    fz_try(h->ctx)
    {
        if (page_num < 1 || page_num > fz_count_pages(h->ctx, h->doc)) {
            Rcpp::stop("Page number out of bounds.");
        }
        page = fz_load_page(h->ctx, h->doc, page_num - 1);

        // Create pixmap to render to
        fz_matrix transform = fz_scale((float)dpi / 72.0f, (float)dpi / 72.0f);
        fz_rect bounds = fz_transform_rect(fz_bound_page(h->ctx, page), transform);
        pix = fz_new_pixmap_with_bbox(h->ctx, fz_default_rgb(h->ctx, NULL), fz_round_rect(bounds), NULL, 1);
        fz_clear_pixmap_with_value(h->ctx, pix, 255);

        // Create a draw device and run the page into it
        dev = fz_new_draw_device(h->ctx, transform, pix);
        fz_run_page(h->ctx, page, dev, fz_identity, NULL);
        fz_close_device(h->ctx, dev);

        // Save pixmap as PNG
        out = fz_new_output_with_path(h->ctx, path.c_str(), 0);
        fz_write_pixmap_as_png(h->ctx, out, pix);
    }
    fz_always(h->ctx)
    {
        if (dev) fz_drop_device(h->ctx, dev);
        if (pix) fz_drop_pixmap(h->ctx, pix);
        if (page) fz_drop_page(h->ctx, page);
        if (out) fz_drop_output(h->ctx, out);
    }
    fz_catch(h->ctx)
    {
        Rcpp::stop("Failed to render page %d: %s", page_num, fz_caught_message(h->ctx));
    }
}

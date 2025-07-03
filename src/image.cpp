#include "utils.h"

// [[Rcpp::export]]
Rcpp::IntegerVector pdf_page_image_xrefs(SEXP doc_xptr, int page_num) {
    MuPDFDoc* h = get_handle(doc_xptr);
    pdf_document *pdf = get_pdf_document(h);

    if (page_num < 1 || page_num > fz_count_pages(h->ctx, h->doc)) {
        Rcpp::stop("Page number out of bounds.");
    }
    pdf_page *page = pdf_load_page(h->ctx, pdf, page_num - 1);
    Rcpp::IntegerVector xrefs;

    fz_try(h->ctx)
    {
        pdf_obj *resources = pdf_page_resources(h->ctx, page);
        if (resources) {
            pdf_obj *xobjects = pdf_dict_get(h->ctx, resources, PDF_NAME(XObject));
            if (xobjects) {
                int n = pdf_dict_len(h->ctx, xobjects);
                for (int i = 0; i < n; ++i) {
                    pdf_obj *val = pdf_dict_get_val(h->ctx, xobjects, i);
                    pdf_obj *obj = pdf_resolve_indirect(h->ctx, val);
                    pdf_obj *subtype = pdf_dict_get(h->ctx, obj, PDF_NAME(Subtype));
                    if (subtype && pdf_name_eq(h->ctx, subtype, PDF_NAME(Image))) {
                        xrefs.push_back(pdf_to_num(h->ctx, val));
                    }
                }
            }
            pdf_drop_obj(h->ctx, resources);
        }
    }
    fz_always(h->ctx)
    {
        pdf_drop_page(h->ctx, page);
    }
    fz_catch(h->ctx)
    {
        Rcpp::stop("Failed to get images for page %d: %s", page_num, fz_caught_message(h->ctx));
    }

    return xrefs;
}

// [[Rcpp::export]]
void pdf_save_image(SEXP doc_xptr, int xref, std::string path) {
    MuPDFDoc* h = get_handle(doc_xptr);
    pdf_document *pdf = get_pdf_document(h);
    
    fz_output *out = nullptr;
    fz_pixmap *pix = nullptr;
    fz_pixmap *rgb_pix = nullptr;
    fz_image *image = nullptr;
    pdf_obj *obj = nullptr;
    fz_default_colorspaces *default_cs = nullptr;

    fz_try(h->ctx)
    {
        obj = pdf_new_indirect(h->ctx, pdf, xref, 0);
        image = pdf_load_image(h->ctx, pdf, obj);
        
        pix = fz_get_pixmap_from_image(h->ctx, image, NULL, NULL, NULL, NULL);

        default_cs = fz_new_default_colorspaces(h->ctx);
        fz_colorspace *rgb_cs = fz_default_rgb(h->ctx, default_cs);

        if (pix->colorspace == rgb_cs) {
            rgb_pix = pix;
        } else {
            rgb_pix = fz_convert_pixmap(h->ctx, pix, rgb_cs, NULL, default_cs, fz_default_color_params, 1);
        }

        out = fz_new_output_with_path(h->ctx, path.c_str(), 0);
        fz_write_pixmap_as_png(h->ctx, out, rgb_pix);
    }
    fz_always(h->ctx)
    {
        if (rgb_pix && rgb_pix != pix) fz_drop_pixmap(h->ctx, rgb_pix);
        if (pix) fz_drop_pixmap(h->ctx, pix);
        if (image) fz_drop_image(h->ctx, image);
        if (obj) pdf_drop_obj(h->ctx, obj);
        if (out) fz_drop_output(h->ctx, out);
        if (default_cs) fz_drop_default_colorspaces(h->ctx, default_cs);
    }
    fz_catch(h->ctx)
    {
        Rcpp::stop("Failed to save image for xref %d: %s", xref, fz_caught_message(h->ctx));
    }
}

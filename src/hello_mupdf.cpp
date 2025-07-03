#include <Rcpp.h>
#include <mupdf/fitz.h>

// [[Rcpp::export]]
std::string mupdf_version() {
#ifdef FZ_VERSION
    return std::string(FZ_VERSION);
#else
    return std::string("unknown");
#endif
}
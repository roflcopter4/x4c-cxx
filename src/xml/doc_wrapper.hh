#ifndef X4C_XML_DOC_WRAPPER_HH_
#define X4C_XML_DOC_WRAPPER_HH_

#include "Common.hh"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>

#define ND [[nodiscard]]

namespace x4c::XML {

class doc_wrapper
{
        struct filename {
            private:
              alignas(128)
                std::string base_;
                std::string path_;
                std::string full_;
            public:
                explicit filename(const char *file);
                ND inline const std::string &base() const { return base_; }
                ND inline const std::string &path() const { return path_; }
                ND inline const std::string &full() const { return full_; }
        } fname_;
        xmlDoc        *doc_;
        xmlTextReader *rd_;

    public:
        explicit doc_wrapper(const char *file);
        ~doc_wrapper();

        /* Delete all dumb copy/move nonsense. Just use a damn pointer. */
        doc_wrapper(const doc_wrapper &o)             = delete;
        doc_wrapper(const doc_wrapper &&o)            = delete;
        doc_wrapper &operator=(const doc_wrapper &o)  = delete;
        doc_wrapper &operator=(const doc_wrapper &&o) = delete;

        ND inline const filename &fname() const { return fname_; };
        ND inline xmlDoc *doc()       { return doc_; };
        ND inline xmlTextReader *rd() { return rd_;  };
};

} // namespace x4c::XML

#undef ND
#endif /* doc_wrapper.hh */

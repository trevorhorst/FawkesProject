/**
 * \brief For more details on this, review this stack overflow thread:
 * https://stackoverflow.com/questions/4864866/c-c-with-gcc-statically-add-resource-files-to-executable-library/4910421#4910421
 * */
#ifndef FAWKES_EXAMPLE_RESOURCES_H
#define FAWKES_EXAMPLE_RESOURCES_H

#include "fawkes/core/resource.h"

namespace Resources
{
    extern "C" const char bundle_js[];
    extern "C" const unsigned int bundle_js_size;

    extern "C" const char index_html[];
    extern "C" const unsigned int index_html_size;
}

#endif // FAWKES_EXAMPLE_RESOURCES_H

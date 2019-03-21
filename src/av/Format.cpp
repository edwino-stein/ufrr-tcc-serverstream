#include "av/Format.hpp"

using namespace av;

Format::Format() : formatCtx(_formatCtx){
    this->_formatCtx = NULL;
}

Format::~Format(){}

void Format::dump(String label, bool isOutput) const {
    if(this->formatCtx == NULL) return;
    av_dump_format(this->formatCtx, 0, label.c_str(), isOutput ? 1 : 0);
}
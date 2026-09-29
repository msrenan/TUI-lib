#include "tui/Text.hpp"

#include "tui/detail/Canvas.hpp"
#include "tui/detail/Utf8.hpp"

namespace tui {

void Text::setString(const std::string& str) {
    original_ = str;
    std::string expanded;
    expanded.reserve(str.size());
    for (char ch : str) {
        if (ch == '\t')
            expanded += "    ";
        else
            expanded += ch;
    }
    chars_ = detail::utf8ToUtf32(expanded);
}

void Text::draw(detail::Canvas& canvas) const {
    int cx = x_;
    int cy = y_;
    for (char32_t ch : chars_) {
        if (ch == U'\n') {
            ++cy;
            cx = x_;
            continue;
        }
        if (hasBg_)
            canvas.put(cx, cy, ch, fg_, bg_);
        else
            canvas.put(cx, cy, ch, fg_);
        ++cx;
    }
}

} // namespace tui

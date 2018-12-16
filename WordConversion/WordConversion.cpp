#include "WordConversion.h"

CL_BEGIN

WordConversion::WordConversion(const std::wstring & font_family, Gdiplus::REAL em_size)
    :_em_size(em_size), _font_family(font_family.c_str()), _pen(Gdiplus::Color::Black, 1),
    _format(Gdiplus::StringFormat::GenericTypographic()), _path(nullptr)
{
    _format.SetAlignment(Gdiplus::StringAlignmentNear);
    _format.SetLineAlignment(Gdiplus::StringAlignmentNear);
}

Gdiplus::Rect WordConversion::begin_conversion(const std::wstring & s)
{
    _path = std::make_unique<Gdiplus::GraphicsPath>();
    _path->AddString(s.c_str(), s.size(), &_font_family, Gdiplus::FontStyleRegular, _em_size,
        Gdiplus::PointF(0.0, 0.0), &_format);
    Gdiplus::Rect rect;
    _path->GetBounds(&rect);
    return rect;
}

bool WordConversion::visible(INT x, INT y)
{
    if (_path == nullptr)
    {
        throw std::exception("invoke begin_conversion() first.");
    }
    return _path->IsOutlineVisible(x, y, &_pen);
}

void WordConversion::end_conversion()
{
    _path.reset();
}

WordConversion::~WordConversion()
{
    // prevent from forgetting end_conversion
    if (_path)
    {
        end_conversion();
    }
}

CL_END
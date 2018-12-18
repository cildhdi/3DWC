#pragma once

#include <App.h>

#include <windows.h>
#include <memory>
#include <string>

CL_BEGIN


class WordConversion
{
private:
    std::unique_ptr<Gdiplus::GraphicsPath> _path;
    Gdiplus::FontFamily _font_family;
    Gdiplus::REAL _em_size;
    Gdiplus::StringFormat _format;
    Gdiplus::Pen _pen;
public:
    WordConversion(Gdiplus::REAL em_size = 30, const std::wstring& font_family = L"Microsoft YaHei");
    // return the minimum width and height of string
    Gdiplus::Rect begin_conversion(const std::wstring& s);
    bool visible(INT x, INT y);
    void end_conversion();
    virtual ~WordConversion();
};

CL_END


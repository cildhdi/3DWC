#pragma once

#define CL_BEGIN namespace cl{
#define CL_END }

#include <glog/logging.h>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

CL_BEGIN

class App
{
private:
    Gdiplus::GdiplusStartupInput _gdi_startup_input;
    ULONG_PTR _gdi_token;
public:
    App(int* argc, char** argv)
    {
        google::InitGoogleLogging(argv[0]);
        google::SetStderrLogging(google::INFO);
        LOG(INFO) << "InitGoogleLogging.";

        Gdiplus::GdiplusStartup(&_gdi_token, &_gdi_startup_input, nullptr);
        LOG(INFO) << "GdiplusStartup.";
    }

    virtual ~App()
    {
        Gdiplus::GdiplusShutdown(_gdi_token);
    }
};

CL_END
#pragma once

#ifdef MYLIBRARY_EXPORTS  // This ensures proper import/export of functions
#define MYLIBRARY_API __declspec(dllexport)  // For exporting functions from the DLL
#else
#define MYLIBRARY_API __declspec(dllimport)  // For importing functions into another project
#endif

extern "C" __declspec(dllexport) void PrintComment();
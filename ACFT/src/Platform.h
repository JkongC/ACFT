#pragma once

#ifdef _WIN32

#ifdef ACFT_EXPORTS
#define ACFT_API __declspec(dllexport)
#else
#define ACFT_API __declspec(dllimport)
#endif // ACFT_EXPORTS

#else
#define ACFT_API

#endif // _WIN32

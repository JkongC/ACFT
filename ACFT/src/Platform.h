#pragma once

#if defined(_WIN32) && defined(ACFT_DLL)

#ifdef ACFT_EXPORTS
#define ACFT_API __declspec(dllexport)
#else
#define ACFT_API __declspec(dllimport)
#endif // ACFT_EXPORTS

#else
#define ACFT_API

#endif // _WIN32

#ifndef _ACFT_
#define _ACFT_

import ACFT;

// This must be implemented by user.
ACFT::Application* CreateApplication();

int main(int argc, char** argv)
{
	ACFT::Application* app = CreateApplication();
	int ret = app->Entry(argc, argv);
	return ret;
}

#endif

#ifndef _ACFT_
#define _ACFT_

import ACFT;

// This must be implemented by user.
ACFT::Application* CreateApplication();

int main(int argc, char** argv)
{
	ACFT::Ref<ACFT::Application> app = ACFT::Ref<ACFT::Application>(CreateApplication());
	int ret = app->Entry(argc, argv);
	return ret;
}

#endif

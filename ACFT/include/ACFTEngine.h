#pragma once

import ACFT;

ACFT::Application* CreateApplication();

int main(int argc, char** argv)
{
	ACFT::Application* app = CreateApplication();
	return app->Entry(argc, argv);
}

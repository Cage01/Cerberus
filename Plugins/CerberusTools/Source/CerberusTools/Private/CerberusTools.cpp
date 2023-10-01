// Copyright Epic Games, Inc. All Rights Reserved.

#include "CerberusTools.h"

#define LOCTEXT_NAMESPACE "FCerberusToolsModule"

void FCerberusToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	AddContentBrowserContextMenuExtender();
}

void FCerberusToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	RemoveContentBrowserContextMenuExtender();
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCerberusToolsModule, CerberusTools)
#ifdef __APPLE__

#include "processes.h"
#include <AppKit/NSRunningApplication.h>

process processes::get_active_process() {
    NSRunningApplication *app = NSWorkspace.sharedWorkspace.frontmostApplication;
    return {
            .pid = app.processIdentifier,
            .name = std::string(app.localizedName.UTF8String)
    };
}

#endif
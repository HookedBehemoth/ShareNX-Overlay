#include <tesla.hpp>    // The Tesla Header

#include "gui_main.hpp"
#include "gui_error.hpp"

class ShareOverlay : public tsl::Overlay {
public:
    ShareOverlay() { }
    ~ShareOverlay() { }
    
    tsl::Gui* onSetup() {
        tsl::Gui *next;
        Result rc = smInitialize();
        if (R_SUCCEEDED(rc)) {
            rc = capsaInitialize();
            if (R_SUCCEEDED(rc)) {
                next = new GuiMain();
            } else {
                next = new ErrorGui(rc, "Failed to init CapSrv!");
            }
            smExit();
        } else {
            next = new ErrorGui(rc, "Failed to init sm!");
        }
        return next;
    }

    virtual void onDestroy() {
        capsaExit();
    }

};


tsl::Overlay *overlayLoad() {
    return new ShareOverlay();
}
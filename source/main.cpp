#include <tesla.hpp>    // The Tesla Header

#include "gui_main.hpp"
#include "gui_error.hpp"

class ShareOverlay : public tsl::Overlay {
public:
    ShareOverlay() { }
    ~ShareOverlay() {
        smExit();
        socketExit();
        capsaExit();
    }
    
    tsl::Gui* onSetup() {
        Result rc = smInitialize();

        if (R_FAILED(rc)) {
            return new ErrorGui(rc, "Failed to init sm!");
        }

        SocketInitConfig sockConf = {
            .bsdsockets_version = 1,

            .tcp_tx_buf_size        = 0x800,
            .tcp_rx_buf_size        = 0x800,
            .tcp_tx_buf_max_size    = 0x25000,
            .tcp_rx_buf_max_size    = 0x25000,

            .udp_tx_buf_size = 0,
            .udp_rx_buf_size = 0,

            .sb_efficiency = 1,

            .num_bsd_sessions       = 0,
            .bsd_service_type       = BsdServiceType_Auto,
        };
        rc = socketInitialize(&sockConf);
        if (R_FAILED(rc)) {
            smExit();
            return new ErrorGui(rc, "Socket init failed!");
        }

        rc = capsaInitialize();
        if (R_FAILED(rc)) {
            socketExit();
            smExit();
            return new ErrorGui(rc, "Failed to init CapSrv!");
        }

        return new GuiMain();
    }

    virtual void onDestroy() {
        smExit();
        socketExit();
        capsaExit();
    }

};


tsl::Overlay *overlayLoad() {
    return new ShareOverlay();
}
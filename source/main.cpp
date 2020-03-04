/*
 * Copyright (c) 2020 Behemoth
 *
 * This file is part of ShareNX.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define TESLA_INIT_IMPL
#include "gui_error.hpp"
#include "gui_main.hpp"

#define R_INIT(cmd)     \
    rc = cmd;           \
    if (R_FAILED(rc)) { \
        msg = #cmd;     \
        return;         \
    }

constexpr SocketInitConfig sockConf = {
    .bsdsockets_version = 1,

    .tcp_tx_buf_size = 0x800,
    .tcp_rx_buf_size = 0x800,
    .tcp_tx_buf_max_size = 0x25000,
    .tcp_rx_buf_max_size = 0x25000,

    .udp_tx_buf_size = 0,
    .udp_rx_buf_size = 0,

    .sb_efficiency = 1,

    .num_bsd_sessions = 0,
    .bsd_service_type = BsdServiceType_Auto,
};

class ShareOverlay : public tsl::Overlay {
  public:
    virtual void initServices() override {
        R_INIT(socketInitialize(&sockConf));
        R_INIT(capsaInitialize());
    }
    virtual void exitServices() override {
        capsaExit();
        socketExit();
    }

    virtual void onShow() override {}
    virtual void onHide() override {}

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        if (R_SUCCEEDED(rc))
            return initially<MainGui>();
        else
            return initially<ErrorGui>(rc, msg);
    }

  private:
    Result rc = 0;
    const char *msg = "OK!";
};

int main(int argc, char **argv) {
    return tsl::loop<ShareOverlay>(argc, argv);
}

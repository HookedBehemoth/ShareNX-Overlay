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
#include "tesla.hpp"

#include "gui_main.hpp"

const SocketInitConfig sockConf = {
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

class ShareOverlay : public tsl::Overlay<GuiMain> {
public:
	virtual void initServices() override {
		tsl::hlp::doWithSmSession([] {
			socketInitialize(&sockConf);
			capsaInitialize();
		});
	}

	virtual void exitServices() override {
		capsaExit();
		socketExit();
	}
};

int main(int argc, char **argv) {
	return tsl::loop<ShareOverlay>(argc, argv);
}
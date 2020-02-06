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
#include <tesla.hpp>

#include "gui_error.hpp"
#include "gui_main.hpp"

class ShareOverlay : public tsl::Overlay {
public:
	ShareOverlay() {}
	~ShareOverlay() {
		smExit();
		socketExit();
		capsaExit();
	}

	tsl::Gui *onSetup() {
		Result rc = smInitialize();

		if (R_FAILED(rc)) {
			return new ErrorGui(rc, "Failed to init sm!");
		}

		SocketInitConfig sockConf = {
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
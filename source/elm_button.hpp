/**
 * Copyright (C) 2020 WerWolv
 *
 * This file is part of libtesla.
 *
 * libtesla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libtesla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libtesla.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <tesla.hpp>

class Button : public tsl::Element {
public:
	Button(u16 x, u16 y, u16 w, u16 h, const std::string &txt, std::function<bool(s64)> cb);
	~Button();

	tsl::Element *requestFocus(tsl::Element *oldFocus, FocusDirection direction) override;

	void draw(tsl::Screen *screen, u16 x, u16 y) override;
	void layout() override;
	bool onClick(s64 key) override;

private:
	std::string m_text;
	std::function<bool(s64)> m_onClick;
};
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
#include "elm_text.hpp"

Text::Text(u16 x, u16 y, u16 w, u16 h, const std::string &txt) : m_text(txt) {
	this->setBoundaries(x, y, w, h);
}

void Text::draw(tsl::gfx::Renderer *renderer) {
	const auto [x, y] = this->getPosition();

	renderer->drawString(this->m_text.c_str(), false, x + 20, y + 45, 23, a({0xF, 0xF, 0xF, 0xF}));
}

void Text::layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) {
}

void Text::setText(const std::string &txt) {
	this->m_text = txt;
}
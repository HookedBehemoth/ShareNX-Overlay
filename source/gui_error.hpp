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
#pragma once
#include <tesla.hpp>

class ErrorGui : public tsl::Gui {
  private:
    Result rc = 0;
    char errorCode[10];
    const char *msg = nullptr;

  public:
    ErrorGui(Result result, const char *message);
    ~ErrorGui();

    virtual tsl::elm::Element *createUI() override;

    virtual void update() override {}

    virtual bool handleInput(u64 down, u64 held, touchPosition pos, JoystickPosition left, JoystickPosition right) override;
};
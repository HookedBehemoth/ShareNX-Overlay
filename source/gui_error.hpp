#pragma once
#include <tesla.hpp>

class ErrorGui : public tsl::Gui {
private:
    Result rc;
    std::string msg;
public:
    ErrorGui(Result result, const std::string &message);
    ~ErrorGui();

    tsl::Element *createUI();
};
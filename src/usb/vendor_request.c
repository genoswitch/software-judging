#include "vendor_request.h"

#include "tusb.h"

const req_version version = {
    .bLength = 3,
    .bCode = CUSTOM_REQUEST_VERSION,
    .bVersion = 0x7b // 123
};

const req_unknown_value unknown = {
    .bLength = 2,
    .bCode = CUSTOM_REQUEST_UNKNOWN};

bool handle_custom_vendor_req(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request)
{
    switch (request->wValue)
    {
    case CUSTOM_REQUEST_VERSION:
        return tud_control_xfer(rhport, request, (void *)(uintptr_t)&version, version.bLength);
    default:
        // Unknown value
        return tud_control_xfer(rhport, request, (void *)(uintptr_t)&unknown, unknown.bLength);
    }
}
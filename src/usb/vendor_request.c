#include "vendor_request.h"

#include "tusb.h"

#include "git.h"

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
    case CUSTOM_REQUEST_BUILD_INFO:
    {
        // Add brackets to declare a variable as the first statement in a case (https://stackoverflow.com/a/2036822)
        const char *sha = git_CommitSHA1();
        const char *date = git_CommitDate();

        const req_build_info build_info = {
            .bLength = 67, // bLength (1), bCode (1), sha1 (40), date (25). We could also use strlen(sha) here?
            .bCode = CUSTOM_REQUEST_BUILD_INFO,
            // sha1 and date are set using strcpy.
        };
        // "arrays in C are not assignable, you need to use strcpy." https://stackoverflow.com/a/1292591
        strcpy(build_info.sha1, sha);
        strcpy(build_info.date, date);
        return tud_control_xfer(rhport, request, (void *)(uintptr_t)&build_info, build_info.bLength);
    }
    default:
        // Unknown value
        return tud_control_xfer(rhport, request, (void *)(uintptr_t)&unknown, unknown.bLength);
    }
}
// TU_ATTR_PACKED
#include "tusb.h"

enum
{
    VENDOR_REQUEST_CUSTOM = 3
};

// We use the request value (VENDOR_REQUEST_CUSTOM) for our messages, and use the value property to discern what is being requested (instead of using lots of request IDs)
// This allows for more efficient usage of vendor requests.
enum
{
    CUSTOM_REQUEST_UNKNOWN,
    CUSTOM_REQUEST_VERSION
};

// b prefix for byte value
typedef struct TU_ATTR_PACKED
{
    uint8_t bLength;
    uint8_t bCode;
    uint8_t bVersion;
} req_version;

typedef struct TU_ATTR_PACKED
{
    uint8_t bLength;
    uint8_t bCode;
} req_unknown_value;

bool handle_custom_vendor_req(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request);
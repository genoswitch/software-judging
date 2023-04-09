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
    CUSTOM_REQUEST_VERSION,
    CUSTOM_REQUEST_BUILD_INFO,
    CUSTOM_REQUEST_BOARD_ID,
    CUSTOM_REQUEST_FEATURE_SET
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

typedef struct TU_ATTR_PACKED
{
    uint8_t bLength;
    uint8_t bCode;
    char sha1[40];
    char date[25]; // Seems to be "YYYY-MM-DD HH:mm:ss +0000"
} req_build_info;

typedef struct TU_ATTR_PACKED
{
    uint8_t bLength;
    uint8_t bCode;
    uint8_t bId[8];
} req_board_id;

typedef struct TU_ATTR_PACKED
{
    uint8_t bLength;
    uint8_t bCode;
    char bFeatures[1];
} req_feature_set;

bool handle_custom_vendor_req(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request);
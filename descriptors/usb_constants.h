#define REQUEST_TYPE_HOST_TO_DEV 0
#define REQUEST_TYPE_DEV_TO_HOST (1 << 7)

#define REQUEST_TYPE_STANDARD 0
#define REQUEST_TYPE_CLASS (1 << 5)
#define REQUEST_TYPE_VENDOR (2 << 5)

#define REQUEST_TYPE_DEVICE 0
#define REQUEST_TYPE_INTERFACE 1
#define REQUEST_TYPE_ENDPOINT 2
#define REQUEST_TYPE_OTHER 3

#define REQUEST_GET_STATUS 0
#define REQUEST_CLEAR_FEATURE 1
#define REQUEST_SET_FEATURE 3
#define REQUEST_SET_ADDRESS 5
#define REQUEST_GET_DESCRIPTOR 6
#define REQUEST_SET_DESCRIPTOR 7
#define REQUEST_GET_CONFIGURATION 8
#define REQUEST_SET_CONFIGURATION 9

#define DESC_TYPE_DEVICE (1 << 8)
#define DESC_TYPE_CONFIG (2 << 8)
#define DESC_TYPE_STRING (3 << 8)
#define DESC_TYPE_INTERFACE (4 << 8)
#define DESC_TYPE_ENDPOINT (5 << 8)
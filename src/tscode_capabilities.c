#include "tscode_capabilities.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// maintain internal caps list:
static tscode_cap_details_list_t _cap_details = TSCODE_CAP_DETAILS_LIST_DEFAULT;

// maintain internal version info:
static tscode_device_vendor_details_t _vendor_details = TSCODE_DEVICE_VENDOR_DETAILS_DEFAULT;

void tscode_cap_details_list_append(tscode_cap_details_list_t *list, tscode_cap_channel_t *cap) {
    tscode_cap_details_node_t *node = malloc(sizeof(tscode_cap_details_node_t));
    node->cap = malloc(sizeof(tscode_cap_channel_t));
    *node->cap = *cap;
    node->next = NULL;

    // E_NO_MEM
    if (node == NULL) return;

    if (list->first == NULL) {
        list->first = node;
    }

    if (list->last != NULL) {
        list->last->next = node;
    }

    list->last = node;
}

void tscode_details_list_dispose(tscode_cap_details_list_t *list) {
    tscode_cap_details_node_t *ptr = list->first;
    while (ptr != NULL) {
        tscode_cap_details_node_t *tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
}

void tscode_register_channel(tscode_cap_channel_t* channel) {
    tscode_cap_details_list_append(&_cap_details, channel);
}

void tscode_register_vendor_details(tscode_device_vendor_details_t *details) {
    _vendor_details.device = details->device;
    _vendor_details.vendor = details->vendor;
    _vendor_details.version = details->version;
}

void tscode_generate_capstring(char *str, size_t str_len) {
    size_t cursor = 0;

    cursor += snprintf(str, str_len, "%s; %s; %s\n", 
        _vendor_details.vendor,
        _vendor_details.device,
        _vendor_details.version
    );

    tscode_cap_details_node_t *node = _cap_details.first;
    while (node != NULL && str_len > cursor) {
        tscode_cap_channel_t *channel = node->cap;
        int resolution = 0;

        switch (channel->range.unit) {
            case TSCODE_UNIT_DEGREES:
            case TSCODE_UNIT_PERCENTAGE:
                resolution = 1;
                break;

            case TSCODE_UNIT_INCH:
            case TSCODE_UNIT_METRIC:
                resolution = 2;
                break;

            default:
                resolution = 0;
        }

        cursor += snprintf(str + cursor, str_len - cursor, "%c[%d]=%.*f,%.*f%c\n", 
            'A' + channel->capability,
            channel->channel,
            resolution, channel->range.min, resolution, channel->range.max, tscode_unit_str[channel->range.unit]
        );

        node = node->next;
    }
}
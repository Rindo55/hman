#include "mongoose.h"

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev == MG_REQUEST_BEGIN) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        mg_http_send_response(nc, 200, 0, "Content-Type: text/plain\r\n\r\nHello, World!");
        nc->flags |= MG_F_SEND_AND_CLOSE;
    }
}

int main(void) {
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, ":8080", ev_handler);
    mg_set_protocol_http_websocket(nc);

    printf("Starting web server on port %s\n", mg_get_option(&mgr, "listening_port"));

    while (1) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);

    return 0;
}

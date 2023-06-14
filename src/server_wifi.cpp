////
//// Created by japru on 12/03/2023.
////
//
//#include "server_wifi.hpp"
//#include <esp_wifi.h>
//#include <esp_event.h>
//#include <esp_http_server.h>
//#include <esp_log.h>
//#include <sys/param.h>
//#include "esp_netif.h"
//#include "esp_eth.h"
//#include "esp_tls_crypto.h"
//
//
//#define WIFI_SSID "ESP_PRIME"
//#define WIFI_PASSWORD "01234567"
//
//esp_err_t get_handler(httpd_req_t *req) {
//    /* Send a simple response */
//    const char resp[] = "URI GET Response";
//    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
//    return ESP_OK;
//}
//
///* Our URI handler function to be called during POST /uri request */
//esp_err_t post_handler(httpd_req_t *req) {
//    /* Destination buffer for content of HTTP POST request.
//     * httpd_req_recv() accepts char* only, but content could
//     * as well be any binary data (needs type casting).
//     * In case of string data, null termination will be absent, and
//     * content length would give length of string */
//    char content[100];
//
//    /* Truncate if content length larger than the buffer */
//    size_t recv_size = MIN(req->content_len, sizeof(content));
//
//    int ret = httpd_req_recv(req, content, recv_size);
//    if (ret <= 0) {  /* 0 return value indicates connection closed */
//        /* Check if timeout occurred */
//        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//            /* In case of timeout one can choose to retry calling
//             * httpd_req_recv(), but to keep it simple, here we
//             * respond with an HTTP 408 (Request Timeout) error */
//            httpd_resp_send_408(req);
//        }
//        /* In case of error, returning ESP_FAIL will
//         * ensure that the underlying socket is closed */
//        return ESP_FAIL;
//    }
//
//    /* Send a simple response */
//    const char resp[] = "URI POST Response";
//    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
//    return ESP_OK;
//}
//
//httpd_uri_t uri_get = {
//        .uri      = "/uri",
//        .method   = HTTP_GET,
//        .handler  = get_handler,
//        .user_ctx = NULL
//};
//
///* URI handler structure for POST /uri */
//httpd_uri_t uri_post = {
//        .uri      = "/uri",
//        .method   = HTTP_POST,
//        .handler  = post_handler,
//        .user_ctx = NULL
//};
//
//httpd_handle_t start_webserver(void) {
//    /* Generate default configuration */
//    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
//
//    /* Empty handle to esp_http_server */
//    httpd_handle_t server = NULL;
//
//    /* Start the httpd server */
//    if (httpd_start(&server, &config) == ESP_OK) {
//        /* Register URI handlers */
//        httpd_register_uri_handler(server, &uri_get);
//        httpd_register_uri_handler(server, &uri_post);
//    }
//    /* If server failed to start, handle will be NULL */
//    return server;
//}
//
//void wifi_init_soft_ap() {
//    ESP_ERROR_CHECK(esp_netif_init());
//    const wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
//    const wifi_mode_t wifi_mode = WIFI_MODE_AP;
//    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
//    esp_event_loop_create_default();
//    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
//                                                        reinterpret_cast<esp_event_handler_t>(wifi_soft_ap_event_handler),
//                                                        nullptr, nullptr));
//    wifi_config_t wifi_config = {
//            .ap = {
//                    WIFI_SSID,
//                    WIFI_PASSWORD,
//                    .ssid_len = static_cast<uint8_t>(strlen(WIFI_SSID)),
//
//
//#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
//                    .authmode = WIFI_AUTH_WPA3_PSK,
//                    .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
//#else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
//                    .authmode = WIFI_AUTH_WPA2_PSK,
//#endif
//
//            },
//    };
//    if (strlen(WIFI_PASSWORD) == 0) {
//        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
//    }
//    ESP_ERROR_CHECK(esp_wifi_set_mode(wifi_mode));
//    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
//    ESP_ERROR_CHECK(esp_wifi_start());
//    esp_wifi_connect();
//    start_webserver();
//}
//
//static const char *TAG = "wifi softAP";
//int a = 0;
//
//void wifi_soft_ap_event_handler(void *arg, esp_event_base_t event_base,
//                                uint32_t event_id, void *event_data) {
//    esp_wifi_connect();
//    display_content = "N";
//    display_content += std::to_string(a);
//    a++;
//    display_content += std::to_string(event_id);
//    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
//        auto *event = (wifi_event_ap_staconnected_t *) event_data;
//        display_content = "NDCON";
////        ESP_LOGI(TAG, "station " MACSTR" join, AID=%d",
////                 MAC2STR(event->mac), event->aid);
//    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
//        auto *event = (wifi_event_ap_stadisconnected_t *) event_data;
//        display_content = "NDDIS";
////        ESP_LOGI(TAG, "station " MACSTR" leave, AID=%d",
////                 MAC2STR(event->mac), event->aid);
//    }
//
//}
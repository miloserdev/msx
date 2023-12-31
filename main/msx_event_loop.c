#include "msx_event_loop.h"


/* static  */xQueueHandle event_loop_queue;


/*
    very fucked up queue because in this case
    event_loop have a multiple events
    WIFI | ESPNOW | UART
*/
void event_loop(void *params)
{
    __MSX_PRINT__("event raise");

    msx_event_t *evt; //malloc( sizeof( msx_event_t ) );
    //memset( evt, 0, sizeof( msx_event_t ) );

    vTaskDelay(5000 / portTICK_RATE_MS);

    while( xQueueReceive( event_loop_queue, (msx_event_t *) &evt, portMAX_DELAY ) == pdTRUE )
    {
        switch( evt->id )
        {
            case MSX_ESP_NOW_INIT:
            {
                __MSX_PRINT__("MSX_ESP_NOW_INIT");
                break;
            }
            case MSX_ESP_NOW_DATA_SEND:
            {
                __MSX_PRINT__("MSX_ESP_NOW_DATA_SEND");

                //packet_t *pack = os_malloc(sizeof(packet_t));

                break;
            }
            case MSX_ESP_NOW_SEND_CB:
            {
                __MSX_PRINT__("MSX_ESP_NOW_SEND_CB");
                break;
            }
            case MSX_ESP_NOW_RECV_CB:
            {
                __MSX_PRINT__("MSX_ESP_NOW_RECV_CB");

            //    char *datas = /*may cause crash*/ (char *) os_malloc(evt.len);
            //    memcpy(datas, evt.data, evt.len);

            //    os_printf("MSX_ESP_NOW_RECV_CB >> wtf buf | size %d | buf %s \n", evt.len, datas);   // fucking memory leak

                //cJSON *pack = cJSON_Parse( (char *) evt.data );

                char *exec_data = exec_packet((char *) evt->data, evt->len);
                __MSX_DEBUGV__( os_free(exec_data)  );

                //cJSON_free(pack);
                //cJSON_Delete(pack);
                //os_free(pack);

                /* __MSX_DEBUGV__( os_free(evt->data)   ); */
                break;
            }

            case MSX_UART_DATA:
            {
                __MSX_PRINT__("MSX_UART_DATA");

                __MSX_PRINTF__("uart data is %.*s", evt->len, (char *) evt->data);
                //__uint8_t u8_data[evt.len];
                //for (size_t i = 0; i < evt.len; i++) u8_data[i] = evt.data[i];
                char *asd = ""; //exec_packet((char *) evt->data, evt->len);
                __MSX_DEBUGV__( os_free(asd)    );

                // os_free(asd);
                //os_free(tmp1);


                /* __MSX_DEBUGV__( os_free(evt->data)   ); */

                break;
            }

            case WIFI_EVENT_STA_START:
            {
                __MSX_PRINT__("MSX_WIFI_EVENT_STA_START");
                break;
            }
            case WIFI_EVENT_STA_DISCONNECTED:
            {
                __MSX_PRINT__("MSX_WIFI_EVENT_STA_DISCONNECTED");

                if (MESH_RECONNECT && reconnect_attempts < MESH_RECONNECT_ATTEMPTS)
                {
                    reconnect_attempts++;
                    __MSX_DEBUG__( esp_wifi_connect() );
                }
                break;
            }
            case IP_EVENT_STA_GOT_IP:
            {
                ip_event_got_ip_t *event = (ip_event_got_ip_t *)evt->data;
                __MSX_PRINTF__("MSX_IP_EVENT_STA_GOT_IP >> ip_info.ip : %s", ip4addr_ntoa(&event->ip_info.ip));
                __MSX_DEBUGV__( os_free(event)  );

                __MSX_DEBUG__( esp_wifi_scan_start(&scan_config, true) );
                break;
            }
            default:
            {
                __MSX_PRINTF__("UNKNOWN_EVENT %d", evt->id);
                break;
            }
        }

        __MSX_DEBUGV__( os_free(evt->data)   );
        __MSX_DEBUGV__( os_free(evt)   );
    }

    __MSX_DEBUGV__( vTaskDelete(NULL)   );
}


esp_err_t init_event_loop()
{
    event_loop_queue = xQueueCreate( ESPNOW_QUEUE_SIZE, sizeof( msx_event_t ) );
    __MSX_DEBUG__( xTaskCreate(event_loop, "vTask_event_loop", /* 16 * 1024 */ 8192, NULL, 0, NULL) );
    return ESP_OK;
}


void user_loop( void (*func)(void) )
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;
    xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        
        (*func)(); // extern in main();
    }
}

esp_err_t init_user_loop(void (*task)(void))
{
    __MSX_DEBUG__( xTaskCreate(user_loop, "vTask_user_loop", 4096, task, 0, NULL)    );
    return ESP_OK;
}

bool raise_event(int id, esp_event_base_t base, __uint32_t status, void *data, size_t len)
{
    if (id < 0)
    {
        printf("%s >> no id specified \n", __FUNCTION__);
        return ESP_FAIL;
    }
    msx_event_t *evt = (msx_event_t *) malloc( sizeof(  msx_event_t ) );
    evt->id = id;
    evt->base = (base ? base : NULL);
    evt->status = (status ? status : 0);
    evt->data = NULL;   //          IMPORTANT;
    evt->data = (void *) os_malloc(len);
    __MSX_DEBUGV__( memcpy(evt->data, data, len)    );
    __MSX_PRINTF__("len %d", len);
    evt->len = len;
/*     if (data != NULL)
    {
        void *dat = os_malloc(len);
        memset(dat, 0, len);
        memcpy(dat, data, len);
        evt->data = dat;
    } */
    int xq = (xQueueSend(event_loop_queue, (msx_event_t *) &evt, portMAX_DELAY) != pdTRUE);
    /* __MSX_DEBUG__( xq ); */

    //__MSX_DEBUGV__( os_free(evt)   );

    return ESP_OK;
}
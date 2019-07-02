/*
 * tcpecho.c
 *
 *  Created on: 1 èþë. 2019 ã.
 *      Author: vakluchev
 */
#include "tcpecho.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include <string.h>
#include "usart.h"

void tcpecho(void)
{
    struct netconn *conn, *newconn;
    err_t err, accept_err;
    struct netbuf* buf;
    void* data;
    u16_t len;
    err_t recv_err;

    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_TCP);
    if (conn != NULL)
    {
        /* Bind connection to well known port number 80. */
        err = netconn_bind(conn, NULL, 80);

        if (err == ERR_OK)
        {
            /* Tell connection to go into listening mode. */
            netconn_listen(conn);

            while (1)
            {
                /* Grab new connection. */
                accept_err = netconn_accept(conn, &newconn);

                /* Process the new connection. */
                if (accept_err == ERR_OK)
                {
                    while ((recv_err = netconn_recv(newconn, &buf)) == ERR_OK)
                    {
                        do
                        {
                            netbuf_data(buf, &data, &len);
                            netconn_write(newconn, data, len, NETCONN_COPY);

                        } while (netbuf_next(buf) >= 0);

                        netbuf_delete(buf);
                    }

                    /* Close connection and discard connection identifier. */
                    netconn_close(newconn);
                    netconn_delete(newconn);
                }
            }
        }
        else
        {
            netconn_delete(newconn);
            HAL_UART_Transmit(&huart6,"can not bind TCP netconn", strlen("can not bind TCP netconn"),100);
        }
    }
    else
    {
    	HAL_UART_Transmit(&huart6,"can not create TCP netconn", strlen("can not create TCP netconn"),100);
    }
}

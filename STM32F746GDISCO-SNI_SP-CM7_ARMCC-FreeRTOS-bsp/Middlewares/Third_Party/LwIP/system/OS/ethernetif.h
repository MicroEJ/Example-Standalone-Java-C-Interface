#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* Exported types ------------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);      
void ETHERNET_IRQHandler(void);

void ethernetif_set_link(void const *argument);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);

#endif 

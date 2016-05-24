/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "LLBSP_NET_impl.h"
#include "LLBSP_NET_impl_lwip.h"
#include "LLNET_Cfg.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include <netdb.h>

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f7xx_hal.h"


#define MAX_DHCP_TRIES  4
#define LWIP_LINK_TASK_PRIORITY 9
#define LWIP_DHCP_TASK_PRIORITY 8

#define LWIP_DHCP_POLLING_INTERVAL 250

struct netif gnetif;
uint8_t DHCP_state;

static TaskHandle_t	link_task_handle;
static TaskHandle_t	dhcp_task_handle;


static uint8_t dhcp_sleeping = 1;

// variable used to notify that DNS servers list has changed
uint8_t dns_servers_list_updated = 1;

/***********************************************************************
 * Private functions
 ***********************************************************************/

/**
  * @brief  Reset the network interface ip, netmask and gateway addresses to zero.
  * @param  netif: the network interface
  * @retval None
  */
void netif_addr_set_zero_ip4(struct netif* netif){
	ip_addr_set_zero_ip4(&netif->ip_addr);
  ip_addr_set_zero_ip4(&netif->netmask);
	ip_addr_set_zero_ip4(&netif->gw);
}

/**
  * @brief  This function is called when the network interface is disconnected.
  * @param  netif: the network interface
  * @retval None
  */
void netif_not_connected(struct netif *netif){
	netif_addr_set_zero_ip4(netif);
	LLBSP_NET_TRACE("[INFO] The network cable is not connected \n");
}

/**
  * @brief  Notify the User about the nework interface config status
  * @param  netif: the network interface
  * @retval None
  */
void User_notification(struct netif *netif)
{
	int32_t dhcpConfEnabled = ((int32_t)&LLNET_DHCP_CONF == LLNET_TRUE);
	if (netif_is_up(netif))
	{
		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_START;
		}else{
			// launch static Network Interface configuration
			ethernetif_static_ip_config();
		}
	}
	else
	{
		netif_not_connected(netif);
		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_LINK_DOWN;
		}
	}

}

/**
  * @brief  DHCP Process
* @param  argument: network interface
  * @retval None
  */
void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  uint32_t IPaddress;

  for (;;)
  {
  	// check if DHCP thread has to suspend	
	if(dhcp_sleeping == 1){
		vTaskSuspend(NULL);
	}

    switch (DHCP_state)
    {
    case DHCP_START:
      {
        netif_addr_set_zero_ip4(netif);
        IPaddress = 0;
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;

        LLBSP_NET_TRACE("[INFO] DHCP started\n");
      }
      break;

    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = netif->ip_addr.addr;

        if (IPaddress!=0)
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;

          /* Stop DHCP */
          dhcp_stop(netif);
		  dhcp_sleeping = 1;

          LLBSP_NET_TRACE("[INFO] DHCP address assigned: %s\n", inet_ntoa(IPaddress));

					// set static DNS configuration if required by user
					if(((int32_t)&LLNET_DHCP_DNS_CONF) != LLNET_TRUE)
					{
						ip_addr_t dnsaddr;
						if(DNS_MAX_SERVERS > 0)
						{
							char * static_dns_ip_addr = (char*)LLNET_getDNSHostIPAddr();
							if(static_dns_ip_addr != NULL)
							{
								dnsaddr.addr = inet_addr(static_dns_ip_addr);
								dns_setserver(0, &dnsaddr);
							}
						}
					}
					
					// notify DNS servers IP address updated
					dns_servers_list_updated = 1;
        }
        else
        {
          /* DHCP timeout */
          if (netif->dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;

            /* Stop DHCP */
            dhcp_stop(netif);
			dhcp_sleeping = 1;

            LLBSP_NET_TRACE("[INFO] DHCP timeout\n");
          }
        }
      }
      break;

    default: break;
    }

    /* wait 250 ms */
	TickType_t ticks = LWIP_DHCP_POLLING_INTERVAL / portTICK_PERIOD_MS;
    vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
  }
}

/**
 * @brief  Setup the network interface
 * @param  None
 * @retval None
 */
void Netif_Config(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);
	
	/* Add the network interface */
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input); 

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	if (netif_is_link_up(&gnetif))
	{
		/* When the netif is fully configured this function must be called */
		netif_set_up(&gnetif);
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&gnetif);
	}

	/* Set the link callback function, this function is called on change of link status */
	netif_set_link_callback(&gnetif, ethernetif_update_config);

	/* Create the Ethernet link handler thread */
#if defined(__GNUC__)
	xTaskCreate((TaskFunction_t)ethernetif_set_link, "LWIP Link", configMINIMAL_STACK_SIZE * 5, &gnetif, LWIP_LINK_TASK_PRIORITY, &link_task_handle);
#else
	xTaskCreate((TaskFunction_t)ethernetif_set_link, "LWIP Link", configMINIMAL_STACK_SIZE * 2, &gnetif, LWIP_LINK_TASK_PRIORITY, &link_task_handle);
#endif
}

/*
 * Retrieve static IP configuration of the default network interface and set
 * IP parameters (Interface IP address, Netmask and Gateway IP address).
 */
void ethernetif_static_ip_config()
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	ip_addr_t dnsaddr;

	// static IP configuration. Retrieve IP settings from user properties.
	ipaddr.addr = inet_addr((char*)LLNET_getDeviceIPAddress());
	netmask.addr = inet_addr((char*)LLNET_getDeviceNetmask());
	gw.addr	= inet_addr((char*)LLNET_getDeviceGatewayIPAddr());
	netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);
	LLBSP_NET_TRACE("[INFO] Static IP address assigned: %s\n", inet_ntoa(ipaddr.addr));

	// set static DNS Host IP address.
	if(DNS_MAX_SERVERS > 0)
	{
		char * static_dns_ip_addr = (char*)LLNET_getDNSHostIPAddr();
		if(static_dns_ip_addr != NULL)
		{
			dnsaddr.addr = inet_addr(static_dns_ip_addr);
			dns_setserver(0, &dnsaddr);
			// notify DNS servers IP address updated
			dns_servers_list_updated = 1;
		}
	}
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	int32_t dhcpConfEnabled = ((int32_t)&LLNET_DHCP_CONF == LLNET_TRUE);

	if(netif_is_link_up(netif))
	{
		LLBSP_NET_TRACE("[INFO] The network cable is now connected \n");

		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_START;

			netif_set_addr(netif, &ipaddr , &netmask, &gw);

			// resume DHCP thread
			dhcp_sleeping = 0;
			vTaskResume(dhcp_task_handle);
		}else{
			// launch static Network Interface configuration
			ethernetif_static_ip_config();
		}

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);
	}
	else
	{
		if(dhcpConfEnabled){
			/* Update DHCP state machine */
			DHCP_state = DHCP_LINK_DOWN;
		}

		/*  When the netif link is down this function must be called.*/
		netif_set_down(netif);

		netif_not_connected(netif);
	}
}


/***********************************************************************
 * Public functions
 ***********************************************************************/

/**
 * Network initialization. Start network interfaces and configure it.
 * @return 0 if no error occurred, error code otherwise.
 */
int32_t LLBSP_NET_init(void)
{
	int32_t dhcpConfEnabled = ((int32_t)&LLNET_DHCP_CONF == LLNET_TRUE);

	/* Initialize the LwIP TCP/IP stack */
	tcpip_init(NULL, NULL);

	/* Configure the Network interface */
	Netif_Config();

	/* Notify user about the network interface config */
	User_notification(&gnetif);

	if(dhcpConfEnabled)
	{
		/* Start DHCPClient */
		dhcp_sleeping = 0;
#if defined(__GNUC__)
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 5, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#else
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 2, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#endif
	}

	return 0;
}

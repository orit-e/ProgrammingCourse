/******************************************************************************
 											
				File name:			dhcp.h			
				Written by:     	Orit			
				Reviewed by:    	Maria
 											
 ******************************************************************************/
#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h>  /* size_t */

#define BYTES_IN_IP 4

typedef enum dhcp_status
{
    DHCP_STATUS_SUCCESS,
    DHCP_STATUS_FAILURE,
    DHCP_STATUS_FREE_FAILURE
} dhcp_status_ty;

typedef struct dhcp dhcp_ty;

/**
 * @DESCRIPTION:
 * DHCPCreate() function creates a new dhcp. The function receives subnet 
 * signiture through network_address and the subnet length through subnet_len. 
 * The size of all IPs created by DHCPCreate() is defined in BYTES_IN_IP 
 * typedef above. The function also reserve all needed memory space for all 
 * possible IPs in advance here for a better running performance of 
 * DHCPAllocateIP() function.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * network_address			Given subnet signiture.
 * subnet_len			    Subnet length in bits.
 *
 * @RETURN:
 * DHCPCreate() function returns a pointer to a dhcp.
 * In case of malloc failure, NULL will be returned.
 *
 * @ERROR:
 * If malloc fails, the function will return NULL.
 * 
 * @WARNING:
 * Do not place NULL as input of network_address. Such a thing will lead to 
 * undefined behavior.
 */
dhcp_ty *DHCPCreate(const unsigned char network_address[BYTES_IN_IP],
                    size_t subnet_len);

/**
 * @DESCRIPTION:
 * DHCPDestroy() function destroys given dhcp.
 * The function performs with time complexity of O(1).
 * 
 * @PARAM:
 * dhcp				        pointer to dhcp.
 *
 * @WARNING:
 * Do not place NULL as input of dhcp. Such a thing will lead to undefined 
 * behavior.
 */
void DHCPDestroy(dhcp_ty *dhcp);

/**
 * @DESCRIPTION:
 * DHCPAllocateIP() function finds an IP not in use, allocate it for the user 
 * and finally place said IP inside allocated_ip provided by the user.
 * The user may request a certain ip address through preferred_ip. If it is 
 * free, the function will allocate preferred_ip for the user. If not, the 
 * function will find the closest available IP to the one requested by the user.
 * If NULL is inserted as preferred_ip address, the function will choose the IP 
 * address for the user.
 * The size of all IPs created by DHCPCreate() is defined in BYTES_IN_IP 
 * typedef above.
 * The function performs with time complexity of O(logn) where n is the number 
 * of all possible IPs (not necessarily allocated).
 * 
 * @PARAM:
 * dhcp				        pointer to dhcp.
 * preferred_ip				IP address requested by the user to be allocated.
 * allocated_ip             The place where the function will insert the 
 *                          found and allocated IP
 *
 * @RETURN:
 * DHCPAllocateIP() function return DHCP_STATUS_SUCCESS for success and 
 * DHCP_STATUS_FAILURE otherwise.
 *
 * @WARNINGS:
 * *Do not place NULL as input of dhcp or allocated_ip. Such a thing will lead 
 *  to undefined behavior.
 * *User can place NULL as preferred_ip address.
 * *Function failure is an indication that all possible IPs are already 
 *  allocated.
 */
dhcp_status_ty DHCPAllocateIP(dhcp_ty *dhcp,
                              const unsigned char preferred_ip[BYTES_IN_IP],
                              unsigned char allocated_ip[BYTES_IN_IP]);

/**
 * @DESCRIPTION:
 * DHCPFreeIP() function frees IP placed in ip_to_free if said IP is allocated.
 * he function performs with time complexity of O(logn) where n is the number 
 * of all possible IPs (not necessarily allocated).
 * 
 * @PARAM:
 * dhcp				        pointer to dhcp.
 * ip_to_free				IP address to free.
 *
 * @RETURN:
 * DHCPFreeIP() function return DHCP_STATUS_SUCCESS for success and 
 * DHCP_STATUS_FREE_FAILURE if ip_to_free is not allocated.
 * 
 * @WARNINGS:
 * *Do not place NULL as input of dhcp or ip_to_free. Such a thing will lead to 
 *  undefined behavior.
 * *Sending an already free IP through ip_to_free will result in function 
 *  failure and a return value of DHCP_STATUS_FREE_FAILURE.
 */
dhcp_status_ty DHCPFreeIP(dhcp_ty *dhcp,
                          const unsigned char ip_to_free[BYTES_IN_IP]);


/**
 * @DESCRIPTION:
 * DHCPCountFree() function returns the number of free IPs in given dhcp.
 * The function performs with time complexity of O(n) where n is the number 
 * of all possible IPs (not necessarily allocated).
 * 
 * @PARAM:
 * dhcp				        pointer to dhcp.
 *
 * @RETURN:
 * DHCPCountFree() function returns the number of free IPs in given dhcp.
 *
 * @WARNING:
 * Do not place NULL as input of dhcp. Such a thing will lead to undefined 
 * behavior.
 */
size_t DHCPCountFree(const dhcp_ty *dhcp);

#endif /* __DHCP_H__ */
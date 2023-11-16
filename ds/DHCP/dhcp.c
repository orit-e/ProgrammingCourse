/******************************************************************************
 											
				File name:			dhcp.c			
				Written by:     	Orit			
				Reviewed by:    	Maria
 											
 ******************************************************************************/
#include <assert.h>		/* assert()								*/
#include <string.h>		/* memset()								*/
#include <stdlib.h>		/* calloc(), free()						*/

#include "dhcp.h"

/******************************************************************************/
/*		    		    	DHCP struct definision:				              */
#define BITS_IN_BYTE 8

enum IS_SUB_TRIE_FULL
{
    SUB_TRIE_NOT_FULL,
    SUB_TRIE_FULL
};

struct dhcp
{
    unsigned char *trie;
    unsigned int network_address;
    size_t subnet_len;
    size_t trie_len;
};

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
static void DHCPInitReservedAddrIMP(dhcp_ty *dhcp);
static unsigned int SetSubNet(size_t subnet_len, 
                            const unsigned char network_address[BYTES_IN_IP]);
static size_t FindIpIndexIMP(dhcp_ty *dhcp, 
                                const unsigned char preferred_ip[BYTES_IN_IP]);
static size_t FindFreeIPIMP(dhcp_ty *dhcp, size_t trie_index, 
                            size_t is_turn_right);
static void SetAllocatedIPIMP(dhcp_ty *dhcp, size_t trie_index, 
                                unsigned char allocated_ip[BYTES_IN_IP]);
static void SetFreeIMP(dhcp_ty *dhcp, size_t trie_index);
static void SetFullIMP(dhcp_ty *dhcp, size_t trie_index);
static int IsGoToParentIMP(dhcp_ty *dhcp, size_t trie_index, 
                            size_t prev_index);
static int IsGoToRightChildIMP(size_t trie_index, size_t prev_index);
static int IsSubTrieFullIMP(dhcp_ty *dhcp, size_t trie_index);
static int IsLeafIMP(dhcp_ty *dhcp, size_t trie_index);
static int IsLeftChildIMP(size_t trie_index);
static size_t GetLeftChildIMP(size_t parent);
static size_t GetRightChildIMP(size_t parent);
static size_t GetParentIMP(size_t trie_index);
static size_t GetOtherChildIMP(size_t child);
static unsigned int ServerAddressIndexIMP(dhcp_ty *dhcp);
static unsigned int BroadcastAddressIndexIMP(dhcp_ty *dhcp);
static unsigned int FindFirstLeafIndexIMP(dhcp_ty *dhcp);
static size_t NumOfPossibleIPsIMP(dhcp_ty *dhcp);

/******************************************************************************/
/*								DHCP functions:								  */
dhcp_ty *DHCPCreate(const unsigned char network_address[BYTES_IN_IP],
                    size_t subnet_len)
{
    dhcp_ty *new_dhcp = NULL;
    size_t trie_len = 0;

    assert(NULL != network_address);
    assert(BYTES_IN_IP * BITS_IN_BYTE > subnet_len);

    trie_len = (1 << (BYTES_IN_IP * BITS_IN_BYTE - subnet_len + 1)) - 1;
    new_dhcp = (dhcp_ty *)calloc(sizeof(dhcp_ty) + trie_len, 1);
    if(NULL == new_dhcp)
    {
        return NULL;
    }
    new_dhcp->subnet_len = subnet_len;
    new_dhcp->trie_len = trie_len;
    new_dhcp->network_address = SetSubNet(subnet_len, network_address);
    new_dhcp->trie = (unsigned char *)(new_dhcp + 1);
    DHCPInitReservedAddrIMP(new_dhcp);

    return new_dhcp;
}

void DHCPDestroy(dhcp_ty *dhcp)
{
    assert(NULL != dhcp);

    memset(dhcp, 0, sizeof(dhcp_ty) + dhcp->trie_len);
    free(dhcp);
}

dhcp_status_ty DHCPAllocateIP(dhcp_ty *dhcp,
                              const unsigned char preferred_ip[BYTES_IN_IP],
                              unsigned char allocated_ip[BYTES_IN_IP])
{
    size_t trie_index = 0;

    assert(NULL != dhcp);
    assert(NULL != allocated_ip);

    if(SUB_TRIE_FULL == IsSubTrieFullIMP(dhcp, 0))
    {
        return DHCP_STATUS_FAILURE;
    }

    if(NULL != preferred_ip)
    {
        trie_index = FindIpIndexIMP(dhcp, preferred_ip);
    }
    else
    {
        trie_index = FindFirstLeafIndexIMP(dhcp);
    }
    trie_index = FindFreeIPIMP(dhcp, trie_index, 0);
    SetAllocatedIPIMP(dhcp, trie_index, allocated_ip);

    return DHCP_STATUS_SUCCESS;
}

dhcp_status_ty DHCPFreeIP(dhcp_ty *dhcp,
                          const unsigned char ip_to_free[BYTES_IN_IP])
{
    size_t trie_index = 0;

    assert(NULL != dhcp);
    assert(NULL != ip_to_free);

    trie_index = FindIpIndexIMP(dhcp, ip_to_free);

    assert(FindFirstLeafIndexIMP(dhcp) != trie_index);
    assert(ServerAddressIndexIMP(dhcp) != trie_index);
    assert(BroadcastAddressIndexIMP(dhcp) != trie_index);

    if(SUB_TRIE_NOT_FULL == IsSubTrieFullIMP(dhcp, trie_index))
    {
        return DHCP_STATUS_FREE_FAILURE;
    }
    SetFreeIMP(dhcp,trie_index);

    return DHCP_STATUS_SUCCESS;
}

size_t DHCPCountFree(const dhcp_ty *dhcp)
{
    size_t counter = 0;
    size_t i = 0;

    assert(NULL != dhcp);

    counter = NumOfPossibleIPsIMP((dhcp_ty *)dhcp);
    for(i = FindFirstLeafIndexIMP((dhcp_ty *)dhcp); i < dhcp->trie_len; ++i)
    {
        counter -= IsSubTrieFullIMP((dhcp_ty *)dhcp, i);
    }
    
    return counter;
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
static void DHCPInitReservedAddrIMP(dhcp_ty *dhcp)
{
    assert(NULL != dhcp);

    /* saved for Network Address: */
    dhcp->trie[FindFirstLeafIndexIMP(dhcp)] = SUB_TRIE_FULL;

    /* saved for Server Address:*/
    dhcp->trie[dhcp->trie_len - 1] = SUB_TRIE_FULL;

    /* saved for Broadcast Address: */
    dhcp->trie[dhcp->trie_len - 2] = SUB_TRIE_FULL;

    /* Server Address and Broadcast Address parent: */
    dhcp->trie[dhcp->trie_len / 2 - 1] = SUB_TRIE_FULL;
}

static unsigned int SetSubNet(size_t subnet_len, 
                            const unsigned char network_address[BYTES_IN_IP])
{
    unsigned int subnet = 
                (unsigned int)((network_address[0] << (BITS_IN_BYTE * 3)) | 
                                (network_address[1] << (BITS_IN_BYTE * 2)) |
                                (network_address[2] << BITS_IN_BYTE) | 
                                (network_address[3]));
    subnet = (subnet >> (BYTES_IN_IP * BITS_IN_BYTE - subnet_len)) <<     
                (BYTES_IN_IP * BITS_IN_BYTE - subnet_len);
    
    return subnet;
}

static size_t FindIpIndexIMP(dhcp_ty *dhcp, 
                            const unsigned char preferred_ip[BYTES_IN_IP])
{
    unsigned int trie_index = 0;

    assert(NULL != dhcp);
    assert(NULL != preferred_ip);

    trie_index = (unsigned int)((preferred_ip[0] << (BITS_IN_BYTE * 3)) | 
                    (preferred_ip[1] << (BITS_IN_BYTE * 2)) |
                    (preferred_ip[2] << BITS_IN_BYTE) | (preferred_ip[3]));
    trie_index = (trie_index << dhcp->subnet_len) >> dhcp->subnet_len;
    trie_index += FindFirstLeafIndexIMP(dhcp);
    
    return (size_t)trie_index;
}

static size_t FindFreeIPIMP(dhcp_ty *dhcp, size_t trie_index, 
                            size_t prev_index)
{
    assert(NULL != dhcp);
    assert(dhcp->trie_len > trie_index);

    if(SUB_TRIE_NOT_FULL == IsSubTrieFullIMP(dhcp, trie_index) && 
        1 == IsLeafIMP(dhcp, trie_index))
    {
        return trie_index;
    }
    
    if(1 == IsGoToParentIMP(dhcp, trie_index, prev_index))
    {
        return FindFreeIPIMP(dhcp, GetParentIMP(trie_index), trie_index);
    }
    else
    {
        if(1 == IsGoToRightChildIMP(trie_index, prev_index))
        {
            return FindFreeIPIMP(dhcp, GetRightChildIMP(trie_index), 
                                    trie_index);
        }
        else
        {
            return FindFreeIPIMP(dhcp, GetLeftChildIMP(trie_index), 
                                    trie_index);   
        }
    }
}

static void SetAllocatedIPIMP(dhcp_ty *dhcp, size_t trie_index, 
                                unsigned char allocated_ip[BYTES_IN_IP])
{
    unsigned int *ptr = NULL;

    assert(NULL != dhcp);
    assert(dhcp->trie_len > trie_index && 
            FindFirstLeafIndexIMP(dhcp) / 2 <= trie_index);

    ptr = (unsigned int *)allocated_ip;
    *ptr = dhcp->network_address;
    *ptr += (trie_index - FindFirstLeafIndexIMP(dhcp));
    *ptr = (((*ptr << (BITS_IN_BYTE * 3)) & 0xFF000000) | 
            ((*ptr << BITS_IN_BYTE) & 0x00FF0000) |
            ((*ptr >> BITS_IN_BYTE) & 0x0000FF00) | 
            ((*ptr >> (BITS_IN_BYTE * 3)) & 0x000000FF));
    SetFullIMP(dhcp, trie_index);
}

static void SetFreeIMP(dhcp_ty *dhcp, size_t trie_index)
{
    assert(NULL != dhcp);
    assert(dhcp->trie_len > trie_index && 
            FindFirstLeafIndexIMP(dhcp) <= trie_index);

    dhcp->trie[trie_index] = SUB_TRIE_NOT_FULL;
    while(0 != trie_index)
    {
        trie_index = GetParentIMP(trie_index);
        dhcp->trie[trie_index] = SUB_TRIE_NOT_FULL;
    }
}

static void SetFullIMP(dhcp_ty *dhcp, size_t trie_index)
{
    size_t parent = trie_index;

    assert(NULL != dhcp);
    assert(dhcp->trie_len > trie_index);

    dhcp->trie[trie_index] = SUB_TRIE_FULL;
    while(0 != parent && SUB_TRIE_FULL == IsSubTrieFullIMP(dhcp, parent))
    {
        trie_index = parent;
        parent = GetParentIMP(trie_index);
        dhcp->trie[parent] = dhcp->trie[GetOtherChildIMP(trie_index)];
    }
}

static int IsGoToParentIMP(dhcp_ty *dhcp, size_t trie_index, 
                            size_t prev_index)
{
    return (SUB_TRIE_FULL == IsSubTrieFullIMP(dhcp, trie_index) || 
            (trie_index < prev_index && 0 != trie_index && 
            0 == IsLeftChildIMP(prev_index)));
}

static int IsGoToRightChildIMP(size_t trie_index, size_t prev_index)
{
    return (trie_index < prev_index &&  1 == IsLeftChildIMP(prev_index));
}

static int IsSubTrieFullIMP(dhcp_ty *dhcp, size_t trie_index)
{
    assert(NULL != dhcp);
    assert(dhcp->trie_len > trie_index);

    return dhcp->trie[trie_index];
}

static int IsLeafIMP(dhcp_ty *dhcp, size_t trie_index)
{
    assert(NULL != dhcp);
    assert(dhcp->trie_len > trie_index);
    
    return (FindFirstLeafIndexIMP(dhcp) <= trie_index);
}

static int IsLeftChildIMP(size_t trie_index)
{
    return (trie_index % 2);
}

static size_t GetLeftChildIMP(size_t parent)
{
    return (parent * 2 + 1);
}

static size_t GetRightChildIMP(size_t parent)
{
    return (parent * 2 + 2);
}

static size_t GetParentIMP(size_t trie_index)
{
    return ((trie_index - 1) / 2);
}

static size_t GetOtherChildIMP(size_t child)
{
    return ((0 == child % 2) ? child - 1: child + 1);
}

static unsigned int ServerAddressIndexIMP(dhcp_ty *dhcp)
{
    return (dhcp->trie_len - 1);
}

static unsigned int BroadcastAddressIndexIMP(dhcp_ty *dhcp)
{
    return (dhcp->trie_len - 2);
}

static unsigned int FindFirstLeafIndexIMP(dhcp_ty *dhcp)
{
    return (dhcp->trie_len / 2);
}

static size_t NumOfPossibleIPsIMP(dhcp_ty *dhcp)
{
    return (dhcp->trie_len / 2 + 1);
}
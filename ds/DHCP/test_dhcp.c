#include "dhcp.h"
#include "tests.h"

int TestIntegraton1();
int TestIntegraton2();
int TestIntegraton3();
int TestIntegraton4();
int TestIntegraton5();
int TestIntegraton6();
int TestIntegraton7();


dhcp_ty *dhcp = NULL;
size_t subnet_len1 = 24;
size_t subnet_len2 = 24;
size_t subnet_len3 = 25;
unsigned char subnet1[BYTES_IN_IP] = {176, 28, 3, 0};
unsigned char subnet2[BYTES_IN_IP] = {176, 28, 3, 1};
unsigned char subnet3[BYTES_IN_IP] = {176, 28, 3, 128};

unsigned char preferred_ip[BYTES_IN_IP] = {0, 0, 0, 7};
unsigned char preferred_ip2[BYTES_IN_IP] = {0, 0, 0, 255};

unsigned char allocated_ip1[BYTES_IN_IP] = {0};
unsigned char allocated_ip2[BYTES_IN_IP] = {0};
unsigned char allocated_ip3[BYTES_IN_IP] = {0};
unsigned char allocated_ip4[BYTES_IN_IP] = {0};


int main(int argc, char *argv[])
{
	RunTest(TestIntegraton1(),			"TestIntegraton1");
	RunTest(TestIntegraton2(),			"TestIntegraton2");
	RunTest(TestIntegraton3(),			"TestIntegraton3");
	RunTest(TestIntegraton4(),			"TestIntegraton4");
	RunTest(TestIntegraton5(),			"TestIntegraton5");
	RunTest(TestIntegraton6(),			"TestIntegraton6");
    RunTest(TestIntegraton7(),			"TestIntegraton7");

	(void) argc;
	(void) argv;

    return 0;
}

int TestIntegraton1()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet1, subnet_len1);
	
	result += (NULL == dhcp);
	result += (253 != DHCPCountFree((const dhcp_ty *)dhcp));
	
	DHCPDestroy(dhcp);
	
	return result;
}

int TestIntegraton2()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet1, subnet_len1);
	
	result += (NULL == dhcp);
	result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, NULL, allocated_ip1));
    result += (subnet1[0] != allocated_ip1[0]);
    result += (subnet1[1] != allocated_ip1[1]);
    result += (subnet1[2] != allocated_ip1[2]);
    result += (1 != allocated_ip1[3]);
	result += (252 != DHCPCountFree((const dhcp_ty *)dhcp));

	DHCPDestroy(dhcp);
	
	return result;
}

int TestIntegraton3()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet1, subnet_len1);
	
	result += (NULL == dhcp);
	result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip1));
    result += (subnet1[0] != allocated_ip1[0]);
    result += (subnet1[1] != allocated_ip1[1]);
    result += (subnet1[2] != allocated_ip1[2]);
    result += (7 != allocated_ip1[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip2));
    result += (subnet1[0] != allocated_ip2[0]);
    result += (subnet1[1] != allocated_ip2[1]);
    result += (subnet1[2] != allocated_ip2[2]);
    result += (8 != allocated_ip2[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip3));
    result += (subnet1[0] != allocated_ip3[0]);
    result += (subnet1[1] != allocated_ip3[1]);
    result += (subnet1[2] != allocated_ip3[2]);
    result += (9 != allocated_ip3[3]);
	result += (250 != DHCPCountFree((const dhcp_ty *)dhcp));

	DHCPDestroy(dhcp);
	
	return result;
}

int TestIntegraton4()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet1, subnet_len1);
	
	result += (NULL == dhcp);
	result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip1));
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip2));
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip3));
	result += (250 != DHCPCountFree((const dhcp_ty *)dhcp));
    result += (DHCP_STATUS_SUCCESS != DHCPFreeIP(dhcp, allocated_ip1));
    result += (251 != DHCPCountFree((const dhcp_ty *)dhcp));
    result += (DHCP_STATUS_FREE_FAILURE != DHCPFreeIP(dhcp, allocated_ip1));

	DHCPDestroy(dhcp);
	
	return result;
}

int TestIntegraton5()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet2, subnet_len2);
	
	result += (NULL == dhcp);
	result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip1));
    result += (subnet2[0] != allocated_ip1[0]);
    result += (subnet2[1] != allocated_ip1[1]);
    result += (subnet2[2] != allocated_ip1[2]);
    result += (7 != allocated_ip1[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip2));
    result += (subnet2[0] != allocated_ip2[0]);
    result += (subnet2[1] != allocated_ip2[1]);
    result += (subnet2[2] != allocated_ip2[2]);
    result += (8 != allocated_ip2[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip3));
    result += (subnet2[0] != allocated_ip3[0]);
    result += (subnet2[1] != allocated_ip3[1]);
    result += (subnet2[2] != allocated_ip3[2]);
    result += (9 != allocated_ip3[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, NULL, allocated_ip4));
    result += (subnet2[0] != allocated_ip4[0]);
    result += (subnet2[1] != allocated_ip4[1]);
    result += (subnet2[2] != allocated_ip4[2]);
    result += (1 != allocated_ip4[3]);
	result += (249 != DHCPCountFree((const dhcp_ty *)dhcp));

	DHCPDestroy(dhcp);
	
	return result;
}

int TestIntegraton6()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet3, subnet_len3);
	
	result += (NULL == dhcp);
	result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip1));
    result += (subnet3[0] != allocated_ip1[0]);
    result += (subnet3[1] != allocated_ip1[1]);
    result += (subnet3[2] != allocated_ip1[2]);
    result += (135 != allocated_ip1[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip2));
    result += (subnet3[0] != allocated_ip2[0]);
    result += (subnet3[1] != allocated_ip2[1]);
    result += (subnet3[2] != allocated_ip2[2]);
    result += (136 != allocated_ip2[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip, allocated_ip3));
    result += (subnet3[0] != allocated_ip3[0]);
    result += (subnet3[1] != allocated_ip3[1]);
    result += (subnet3[2] != allocated_ip3[2]);
    result += (137 != allocated_ip3[3]);
    result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, NULL, allocated_ip4));
    result += (subnet3[0] != allocated_ip4[0]);
    result += (subnet3[1] != allocated_ip4[1]);
    result += (subnet3[2] != allocated_ip4[2]);
    result += (129 != allocated_ip4[3]);
	result += (121 != DHCPCountFree((const dhcp_ty *)dhcp));

	DHCPDestroy(dhcp);
	
	return result;
}

int TestIntegraton7()
{
	int result = 0;
	
	dhcp = DHCPCreate(subnet1, subnet_len1);
	
	result += (NULL == dhcp);
	result += (DHCP_STATUS_SUCCESS != 
                DHCPAllocateIP(dhcp, preferred_ip2, allocated_ip1));
    result += (subnet1[0] != allocated_ip1[0]);
    result += (subnet1[1] != allocated_ip1[1]);
    result += (subnet1[2] != allocated_ip1[2]);
    result += (1 != allocated_ip1[3]);
	result += (252 != DHCPCountFree((const dhcp_ty *)dhcp));

	DHCPDestroy(dhcp);
	
	return result;
}
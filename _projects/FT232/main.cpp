#include <stdio.h> 
#include "ft232.h"

int main(void)
{
	Ft232 ft;
	bool rc = ft.Initialize(500);
	ft.SetTimeout(1000);
	uint8_t b = 0x0;
	while (1)
	{
		rc = ft.Send(&b, 1);
		//++b;
		::Sleep(16);
	}

	int bytesReceived = 0;
	while (1)
	{
		rc = ft.Read(&b, bytesReceived, 2000);
		//rc = ft.Read(&b, 1);
		if (rc)
		{
			printf("%d\n", bytesReceived);
		}
		else
		{
			printf("error\n");
		}
	}
}
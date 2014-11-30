#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
#ifdef JANE_TEST_BOOT_OBJECT
	unsigned char IpAddr[4], MacAddr[6], SerialNo[33], name[32], name_len=3, i, chksum[16], data[64];
	unsigned long value;
	
	ReadIPAddr(IpAddr);
	printf("\n Read Bootloader's IpAddr:%d.%d.%d.%d\n\n", IpAddr[0], IpAddr[1], IpAddr[2], IpAddr[3]);
	
	
	IpAddr[3] = 254;
	printf("Write IpAddr:%d.%d.%d.%d\n", IpAddr[0], IpAddr[1], IpAddr[2], IpAddr[3]);
	WriteIPAddr(IpAddr);
	ReadIPAddr(IpAddr);
	printf("Read IpAddr:%d.%d.%d.%d\n\n", IpAddr[0], IpAddr[1], IpAddr[2], IpAddr[3]);
	
	ReadMACAddr(MacAddr);
	printf("Read Bootloader's MacAddr:%x:%x:%x:%x:%x:%x\n\n", MacAddr[0], MacAddr[1], MacAddr[2], MacAddr[3], MacAddr[4], MacAddr[5]);
	
	ReadSerialNo(SerialNo);
	printf("Read Bootloader's Serial no: ");
	for(i=0; i<16; i++)
		printf("%c", SerialNo[i]);
	printf("\n\n");
	
	//printf("Clear UseDeafultConfig flag\n");
	//ClearUseDefaultConfig();
	ReadUseDefaultConfig(&value);
	printf("Read UseDeafultConfig flag: %ld\n\n", value);
	
	ReadResetTime(&data[0]);
	printf("Read reset time: %d\n\n", data[0]);
	
#if 0
	printf("Set Boot-Shim Configuration Data Software Name to 'FIT'\n");
	strcpy(name, "FIT");
	SetBootShimConfigurationDataSoftwareName(name, name_len);
#endif
	ReadBootShimConfigurationDataSoftwareName(name);
	printf("Read Software name: %x-%x-%x\n\n", name[0], name[1], name[2]);

	ReadFITChecksum(chksum);
	printf("Read FIT checksum:");
	for(i=0;i<16;i++)
		printf("%x ", chksum[i]);
	printf("\n\n");

	ReadFITContents(data);
	printf("\nRead 64 bytes of FIT image contents:");
	for(i=0; i<64; i++)
	{
		if(i%0x10==0)
			printf("\r\n");
		printf("%x ", data[i]);	
	}
	printf("\n");

#endif
	return;
}

#include "upload.cgi.h"
#include <sys/reboot.h>
#include <linux/string.h>
#include "nvram_rule.h"

#define IMAGE_NAME_IN_HEADER "WP838"
#define IMAGE_NAME2_IN_HEADER "WP838"
#define IMAGE_NAME3_IN_HEADER "WP838"

#define FW_UPLOADING_STATUS_FILE "/tmp/fw_uploading"

#define REFRESH_TIMEOUT		"40000"		/* 40000 = 40 secs*/

#define RFC_ERROR "RFC1867 error"

/*
 *  Uboot image header format
 *  (ripped from mkimage.c/image.h)
 */
#define IH_MAGIC	0x27051956
//#define PIH_MAGIC 	0x6c697465
//#define IH_NMLEN	128
#define IH_NMLEN  32 //CPE_5G
typedef struct image_header {
    uint32_t    ih_magic;   /* Image Header Magic Number    */
    uint32_t    ih_hcrc;    /* Image Header CRC Checksum    */
    uint32_t    ih_time;    /* Image Creation Timestamp */
    uint32_t    ih_size;    /* Image Data Size      */
    uint32_t    ih_load;    /* Data  Load  Address      */
    uint32_t    ih_ep;      /* Entry Point Address      */
    uint32_t    ih_dcrc;    /* Image Data CRC Checksum  */
    uint8_t     ih_os;      /* Operating System     */
    uint8_t     ih_arch;    /* CPU architecture     */
    uint8_t     ih_type;    /* Image Type           */
    uint8_t     ih_comp;    /* Compression Type     */
    uint8_t     ih_name[IH_NMLEN];  /* Image Name       */
} image_header_t;

typedef struct prefix_image_header{
	uint32_t    pih_magic;   /* Image Header Magic Number    */
	uint32_t    pih_hcrc;    /* Image Header CRC Checksum    */
	uint32_t    pih_size;    /* Image Data Size      */
	uint32_t    pih_dcrc;    /* Image Data CRC Checksum  */		
}prefix_image_header_t;




void *memmem(const void *buf, size_t buf_len, const void *byte_line, size_t byte_line_len)
{
    unsigned char *bl = (unsigned char *)byte_line;
    unsigned char *bf = (unsigned char *)buf;
    unsigned char *p  = bf;

    while (byte_line_len <= (buf_len - (p - bf))){
        unsigned int b = *bl & 0xff;
        if ((p = (unsigned char *) memchr(p, b, buf_len - (p - bf))) != NULL){
            if ( (memcmp(p, byte_line, byte_line_len)) == 0)
                return p;
            else
                p++;
        }else{
            break;
        }
    }
    return NULL;
}

#define MEM_SIZE	1024
#define MEM_HALF	512
int findStrInFile(char *filename, int offset, unsigned char *str, int str_len)
{
	int pos = 0, rc;
	FILE *fp;
	unsigned char mem[MEM_SIZE];

	if(str_len > MEM_HALF)
		return -1;
	if(offset <0)
		return -1;

	fp = fopen(filename, "rb");
	if(!fp)
		return -1;

	rewind(fp);
	fseek(fp, offset + pos, SEEK_SET);
	rc = fread(mem, 1, MEM_SIZE, fp);
	while(rc){
		unsigned char *mem_offset;
		mem_offset = (unsigned char*)memmem(mem, rc, str, str_len);
		if(mem_offset){			
			fclose(fp);	//found it
			fp=NULL;
			return (mem_offset - mem) + pos + offset;
		}

		if(rc == MEM_SIZE){
			pos += MEM_HALF;	// 8
		}else
			break;
		
		rewind(fp);
		fseek(fp, offset+pos, SEEK_SET);
		rc = fread(mem, 1, MEM_SIZE, fp);
	}
	
	fclose(fp);
	fp=NULL;
	return -1;
}

/*
 *  ps. callee must free memory...
 */
void *getMemInFile(char *filename, int offset, int len)
{
    void *result;
    FILE *fp;
    if( (fp = fopen(filename, "r")) == NULL ){
        return NULL;
    }
	fseek(fp, offset, SEEK_SET);
    result = malloc(sizeof(unsigned char) * len );
	if(!result)
	{
		fclose(fp);
		fp=NULL;
		return NULL;
    }
	if( fread(result, 1, len, fp) != len){
        free(result);
		fclose(fp);
		fp=NULL;
        return NULL;
    }
	fclose(fp);
	fp=NULL;
    return result;
}



#if defined (UPLOAD_FIRMWARE_SUPPORT)
static unsigned int getMTDPartSize(char *part)
{
	char buf[128], name[32], size[32], dev[32], erase[32];
	unsigned int result=0;
	FILE *fp = fopen("/proc/mtd", "r");
	if(!fp){
		fclose(fp);
		fp=NULL;
		fprintf(stderr, "mtd support not enable?");
		return 0;
	}
	while(fgets(buf, 512, fp)){
		sscanf(buf, "%s %s %s %s", dev, size, erase, name);
		if(!strcmp(name, part)){
			result = strtol(size, NULL, 16);
			break;
		}
	}
	fclose(fp);
	fp=NULL;
	return result;
}




#endif /* UPLOAD_FIRMWARE_SUPPORT */

/*
 * arguments: ifname  - interface name
 *            if_addr - a 16-byte buffer to store ip address
 * description: fetch ip address, netmask associated to given interface name
 */
int getIfIp(char *ifname, char *if_addr)
{
    struct ifreq ifr;
    int skfd = 0;

    if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("getIfIp: open socket error");
        return -1;
    }

    strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
    if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
        printf("getIfIp: ioctl SIOCGIFADDR error for %s", ifname);
        return -1;
    }
    strcpy(if_addr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    close(skfd);
    return 0;
}

/*
 * I'm too lazy to use popen() instead of system()....
 * ( note:  static buffer used)
 */
#define DEFAULT_LAN_IP "10.10.10.254"
char *getLanIP(void)
{
	static char buf[64];
	char *nl;
	FILE *fp;

	memset(buf, 0, sizeof(buf));
	//if( (fp = popen("nvram_get 2860 lan_ipaddr", "r")) == NULL )
	if( (fp = popen("nvram get lan0_ipaddr", "r")) == NULL )
		goto error;

	if(!fgets(buf, sizeof(buf), fp)){
		pclose(fp);
		goto error;
	}

	if(!strlen(buf)){
		pclose(fp);
		goto error;
	}
	pclose(fp);

	if(nl = strchr(buf, '\n'))
		*nl = '\0';

	return buf;

error:
	fprintf(stderr, "warning, cant find lan ip\n");
	return DEFAULT_LAN_IP;
}


void javascriptUpdate(int success)
{
    printf("<script language=\"JavaScript\" type=\"text/javascript\">");
    if(success){
        printf(" \
function refresh_all(){	\
  top.location.href = \"http://%s\"; \
} \
function update(){ \
  self.setTimeout(\"refresh_all()\", %s);\
}", getLanIP(), REFRESH_TIMEOUT);
    }else{
//        printf("function update(){ parent.menu.setUnderFirmwareUpload(0);}");
        printf("function update(){ }");
    }
    printf("</script>");
}

inline void webFoot(void)
{
    printf("</body></html>\n");
}

//Steve add
/* Copy file from fsrc to fdest */
static int
ezp_copy_file(char *fsrc, char *fdest) {
    FILE *srcfp = NULL, *destfp = NULL;
    unsigned char *buf = NULL;
    int len, retval = 0;


    if (!fsrc || !fdest || !*fsrc || !*fdest) {
        retval = 1;
        goto src_err;
    }
    srcfp = fopen(fsrc, "r");
    if (!srcfp) {
        retval = 1;
        goto src_err;
    }
    destfp = fopen(fdest, "w");
    if (!destfp) {
        retval = 1;
        goto dest_err;
    }
    buf = (unsigned char*)malloc(8192);
    if (!buf) {
        retval = 1;
        goto alloc_err;
    }
    while ((len = fread(buf, 1, 8192, srcfp)) > 0) {
        fwrite(buf, 1, len, destfp);
    }
    free(buf);
alloc_err:
    fclose(destfp);
dest_err:
    fclose(srcfp);
src_err:
    return retval;
}

void ezp_special_process(){
	printf("\nezp_special_process()\n");
	//
	system("cp /web/writeflash_detect0.html  /tmp/writeflash_detect1.html");
	
	/* Disable network service */
    system("echo \"0\" > /proc/sys/net/ipv4/ip_forward");
    /* For reboot normal. Because firmware upgraade will produce squashfs
     * error when system execute reboot from flash. */
    /* The following files are required after firmware and file system 
     * upgraded.
     *
     * It includes
     *   1. busybox and related symbolic links
     *   2. uclibc libraries
     *   3. nvram libraries
     *   4. upgrade finishing page
     * */
    if (ezp_copy_file("/bin/busybox", "/tmp/busybox")) {
        printf("copy /bin/busybox to /tmp/busybox failed\n");
    }
    if (ezp_copy_file("/tmp/busybox", "/bin/busybox")) {
        printf("copy /tmp/busybox to /bin/busybox failed\n");
    }
    if (ezp_copy_file("/lib/libuClibc-0.9.29.so","/tmp/libuClibc-0.9.29.so")) {
        printf("copy /lib/libuClibc-0.9.29.so /tmp/libuClibc-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/tmp/libuClibc-0.9.29.so","/lib/libuClibc-0.9.29.so")) {
        printf("copy /tmp/libuClibc-0.9.29.so /lib/libuClibc-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/lib/ld-uClibc-0.9.29.so","/tmp/ld-uClibc-0.9.29.so")) {
        printf("copy /lib/ld-uClibc-0.9.29.so /tmp/ld-uClibc-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/tmp/ld-uClibc-0.9.29.so","/lib/ld-uClibc-0.9.29.so")) {
        printf("copy /tmp/ld-uClibc-0.9.29.so /lib/ld-uClibc-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/lib/libdl-0.9.29.so","/tmp/libdl-0.9.29.so")) {
        printf("copy /lib/libdl-0.9.29.so /tmp/libdl-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/tmp/libdl-0.9.29.so","/lib/libdl-0.9.29.so")) {
        printf("copy /tmp/libdl-0.9.29.so /lib/libdl-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/lib/libm-0.9.29.so","/tmp/libm-0.9.29.so")) {
        printf("copy /lib/libm-0.9.29.so /tmp/libm-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/tmp/libm-0.9.29.so","/lib/libm-0.9.29.so")) {
        printf("copy /tmp/libm-0.9.29.so /lib/libm-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/lib/libnsl-0.9.29.so","/tmp/libnsl-0.9.29.so")) {
        printf("copy /lib/libnsl-0.9.29.so /tmp/libnsl-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/tmp/libnsl-0.9.29.so","/lib/libnsl-0.9.29.so")) {
        printf("copy /tmp/libnsl-0.9.29.so /lib/libnsl-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/lib/libutil-0.9.29.so","/tmp/libutil-0.9.29.so")) {
        printf("copy /lib/libutil-0.9.29.so /tmp/libutil-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/tmp/libutil-0.9.29.so","/lib/libutil-0.9.29.so")) {
        printf("copy /tmp/libutil-0.9.29.so /lib/libutil-0.9.29.so failed\n");
    }
    if (ezp_copy_file("/usr/lib/libnvram.so","/tmp/libnvram.so")) {
        printf("copy /usr/lib/libnvram.so /tmp/libnvram.so failed\n");
    }
    if (ezp_copy_file("/tmp/libnvram.so","/usr/lib/libnvram.so")) {
        printf("copy /tmp/libnvram.so /usr/lib/libnvram.so failed\n");
    }
    if (ezp_copy_file("/usr/lib/libshared.so","/tmp/libshared.so")) {
        printf("copy /usr/lib/libshared.so /tmp/libshared.so failed\n");
    }
    if (ezp_copy_file("/tmp/libshared.so","/usr/lib/libshared.so")) {
        printf("copy /tmp/libshared.so /usr/lib/libshared.so failed\n");
    }
    #if 1
    /*
    if (ezp_copy_file("/web/writeflash_detect0.html","/tmp/writeflash_detect0.html")) {
        printf("copy /web/writeflash_detect0.html /tmp/writeflash_detect0.html failed\n");
    }
    if (ezp_copy_file("/tmp/writeflash_detect1.html","/web/writeflash_detect1.html")) {
        printf("copy /tmp/writeflash_detect1.html /web/writeflash_detect1.html failed\n");
    }
    
    if (ezp_copy_file("/web/writeflash_detect1.html","/tmp/writeflash_detect1.html")) {
        printf("copy /web/writeflash_detect1.html /tmp/writeflash_detect1.html failed\n");
    }
    if (ezp_copy_file("/tmp/writeflash_detect1.html","/web/writeflash_detect1.html")) {
        printf("copy /tmp/writeflash_detect1.html /web/writeflash_detect1.html failed\n");
    }
    */
    #else
    if (ezp_copy_file("/www/resp_upgrade_success.asp","/tmp/resp_upgrade_success.asp")) {
        printf("copy /www/resp_upgrade_success.asp /tmp/resp_upgrade_success.asp failed\n");
    }
    if (ezp_copy_file("/tmp/resp_upgrade_success.asp","/www/resp_upgrade_success.asp")) {
        printf("copy /tmp/resp_upgrade_success.asp /www/resp_upgrade_success.asp failed\n");
    }
    #endif
    
    unlink("/sbin/reboot");
    //symlink("/bin/busybox","/tmp/reboot");
    //symlink("/bin/busybox","/sbin/reboot");
	unlink("/bin/sync");
	system("chmod 777 /tmp/busybox");
}


/*
 * check fw_uploading status file is exist or not. 
 * this file is controlled by SNMP action. 
 * return 0: file don't exist. It's ok to upgrad.
 *          1: file exist. 
 */
int get_fw_uploading()
{
    int status;
    struct stat stat_buf;
    
    if(stat(FW_UPLOADING_STATUS_FILE, &stat_buf) < 0)
    {
        /* file don't exist*/
        return 0;
    }else
    {
        /* file exist*/
        return 1;
    }
}

/* upgrade status: keep the implementation same as in SNMP*/
enum upgrade_status{
    UPGRADE_BUSY,
    UPGRADE_SUCCESSD,
    UPGRADE_UNSUPPORTED,
    UPGRADE_DOWNLOAD_FAIL,
    UPGRADE_FW_INVALID,
    UPGRADE_NO_SPACE,
    UPGRADE_DOWNLOADING,
    UPGRADE_WRITING,
    UPGRADE_STATUS_MAX,
}upgrade_status_t;

/*
 * set_upgrade_status, status defined in upgrade_status_t
 * keep the implementation same as in SNMP
 */
void set_upgrade_status(int new_status)
{
    char cmd[64];
    
    if(new_status >= UPGRADE_STATUS_MAX  || new_status < 0)
    {
        printf("set_upgrade_status: status %d out of range\n", new_status);

        return;
    }

    /*
    UPGRADE_BUSY,
    UPGRADE_SUCCESSD,
    UPGRADE_UNSUPPORTED,
    UPGRADE_DOWNLOAD_FAIL,
    UPGRADE_FW_INVALID,
    UPGRADE_NO_SPACE,
    UPGRADE_DOWNLOADING,
    UPGRADE_WRITING,
    UPGRADE_STATUS_MAX,
    */
    switch(new_status)
    {
        case UPGRADE_BUSY:
            sprintf(cmd, "echo %s", "busy"); 
            break;
        case UPGRADE_SUCCESSD:
            sprintf(cmd, "echo %s", "ok"); 
            break;
        case UPGRADE_UNSUPPORTED:
            sprintf(cmd, "echo %s", "unsupported"); 
            break;
        case UPGRADE_DOWNLOAD_FAIL:
            sprintf(cmd, "echo %s", "dl_fail"); 
            break;
        case UPGRADE_FW_INVALID:
            sprintf(cmd, "echo %s", "fw_invalid"); 
            break;
        case UPGRADE_NO_SPACE:
            sprintf(cmd, "echo %s", "no_space"); 
            break;
        case UPGRADE_DOWNLOADING:
            sprintf(cmd, "echo %s", "downloading"); 
            break;
        case UPGRADE_WRITING:
            sprintf(cmd, "echo %s", "writing"); 
            break;
        default:
            sprintf(cmd, "echo %s", "unknown");
            break;
    }

    sprintf(cmd, "%s > %s",cmd, FW_UPLOADING_STATUS_FILE);
    
    printf("%s\n", cmd);

    system(cmd);

    return 0;
}

/*
 * get_upgrade_status 
    UPGRADE_BUSY,
    UPGRADE_SUCCESSD,
    UPGRADE_UNSUPPORTED,
    UPGRADE_DOWNLOAD_FAIL,
    UPGRADE_FW_INVALID,
    UPGRADE_NO_SPACE,
    UPGRADE_DOWNLOADING,
    UPGRADE_WRITING,
    UPGRADE_STATUS_MAX,
 * keep the implementation same as in SNMP
 */
int get_upgrade_status()
{
    FILE *fp;
    char buf[64];

    if(get_fw_uploading() == 0)
    {
        return UPGRADE_SUCCESSD; 
    }
    
    fp = fopen(FW_UPLOADING_STATUS_FILE, "r");
    if(fp == NULL)
    {
        printf("get_upgrade_status: open/create file fail\n");

        return UPGRADE_UNSUPPORTED; 
    }

    /*
    UPGRADE_BUSY,
    UPGRADE_SUCCESSD,
    UPGRADE_UNSUPPORTED,
    UPGRADE_DOWNLOAD_FAIL,
    UPGRADE_FW_INVALID,
    UPGRADE_NO_SPACE,
    UPGRADE_DOWNLOADING,
    UPGRADE_WRITING,
    UPGRADE_STATUS_MAX,
    */
	if(fgets(buf, 60, fp))
    {
        if(strcmp(buf, "busy\n") == 0)
        {
            return UPGRADE_BUSY;
        }else if(strcmp(buf, "ok\n") == 0)
        {
            return UPGRADE_SUCCESSD;
        }else if(strcmp(buf, "unsupported\n") == 0)
        {
            return UPGRADE_UNSUPPORTED;
        }else if(strcmp(buf, "dl_fail\n") == 0)
        {
            return UPGRADE_DOWNLOAD_FAIL;
        }else if(strcmp(buf, "fw_invalid\n") == 0)
        {
            return UPGRADE_FW_INVALID;
        }else if(strcmp(buf, "no_space\n") == 0)
        {
            return UPGRADE_NO_SPACE;
        }else if(strcmp(buf, "downloading\n") == 0)
        {
            return UPGRADE_DOWNLOADING;
        }else if(strcmp(buf, "writing\n") == 0)
        {
            return UPGRADE_WRITING;
        }else
        {
            printf("get_upgrade_status: unknown status: %s\n", buf);
            return UPGRADE_UNSUPPORTED;
        }

	}else{

        printf("get_upgrade_status: fgets error\n");

        fclose(fp);

        return UPGRADE_UNSUPPORTED;
    }
	
    return UPGRADE_UNSUPPORTED;
}

/*
 http header :
-----------------------------19275326834322
Content-Disposition: form-data; name="filename"; filename="wp688_4110_130426-v0.1.1-uImage-squashfs-lzma.bin"
Content-Type: application/octet-stream



*/
int main (int argc, char *argv[])
{
    int file_begin, file_end;
    int line_begin, line_end;
    char err_msg[256];
    char *boundary; int boundary_len;
    char *filename = getenv("UPLOAD_FILENAME");
	int file_new_begin = -1; //add by bingley
	char cmd[512] ; //add by bingley
	int file_middle = 0;
	int rootfs_begin = 0;
	unsigned long fw_kernel_len = 0;
	int upgradestatus;

    printf(
	"\
	Server: %s\n\
	Pragma: no-cache\n\
	Content-type: text/html\n",
	getenv("SERVER_SOFTWARE"));

    printf("\n\
	<html>\n\
	<head>\n\
	<TITLE>Upload Firmware</TITLE>\n\
	<link rel=stylesheet href=/style/normal_ws.css type=text/css>\n\
	<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">\n\
	</head>\n\
	<body>\n");


    if(argc > 1)
	{
		filename = argv[1];
		struct stat stat_buf;
		if(stat(filename, &stat_buf) == -1)
		{
	        perror("stat");
	        goto err;
		}
		file_begin = 0;
		file_end = stat_buf.st_size;
    }
	else
	{      
	line_begin = 0;
	if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1)
	{	// address pointer to \r\n away from line_begin's offset with interger.
		printf("<script>parent.upload_message='it doesn not have a http header.';</script>");
		printf("<script>self.location.href=\"/local/advance/upload_firmware_gordon.asp\";</script>");  
		webFoot();
		return -1;
	}

	system("echo 1 > /tmp/uploadOK");
	boundary_len = line_end - line_begin;	//the first line of http header

	boundary = getMemInFile(filename, line_begin, boundary_len); //the content of first line

	char *line, *semicolon, *user_filename;
	line_begin = line_end + 2;

	if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1)
	{
		printf("<script>parent.upload_message='can not parse http header line1 .';</script>");
		goto err;
	}

	line = getMemInFile(filename, line_begin, line_end - line_begin);//the content of the second line Content-Disposition: form-data; name="filename"; filename="wp688_4110_130426-v0.1.1-uImage-squashfs-lzma.bin"

	if(strncasecmp(line, "content-disposition: form-data;", strlen("content-disposition: form-data;")))
	{ 
		printf("<script>parent.upload_message='this is not a form-data.';</script>");
		goto err;
	}//verify the second line's content  
	semicolon = line + strlen("content-disposition: form-data;") + 1;//to offset to locate the filename

	if(! (semicolon = strchr(semicolon, ';'))  )
	{  
		printf("<script>parent.upload_message='We dont support multi-field upload.';</script>\n");
		goto err;
	}

	user_filename = semicolon + 2;

	if( strncasecmp(user_filename, "filename=", strlen("filename="))  )
	{ 
		printf("<script>parent.upload_message='string filename  error.';</script>");
		goto err;
	}
	user_filename += strlen("filename="); //get rid of filename=,left "wp688_4110_130426-v0.1.1-uImage-squashfs-lzma.bin"

	//until now we dont care about what the true filename is.
	free(line);


	// We may check a string  "Content-Type: application/octet-stream" here,
	// but if our firmware extension name is the same with other known ones, 
	// the browser would use other content-type instead.
	// So we dont check Content-type here...

	line_begin = line_end + 2; //the last second line of http header has content

	if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1)
	{ 
		printf("<script>parent.upload_message='filename  parsed error.';</script>");
		goto err;
	}
  

	line_begin = line_end + 2; // the last line of http header without content
 
	if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1)
	{
		printf("<script>parent.upload_message='the last line of http header error.';</script>");
		goto err;
	}


	file_begin = line_end + 2; //now file_begin start from image_header


	file_new_begin = file_begin + sizeof(prefix_image_header_t); //offset the new prefix header 

	if( (file_end = findStrInFile(filename, file_begin, boundary, boundary_len)) == -1)
	{
		printf("<script>parent.upload_message='http header has no space line.';</script>");
		goto err;
	}

	file_end -= 2;		// back 2 chars.(\r\n);
	}


    // examination
#if defined (UPLOAD_FIRMWARE_SUPPORT)
		
	struct stat sbuf;

	int  header_len1 = 0 ;
	int  header_len2 = 0 ;
	unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;
	unsigned long checksum1 = 0;
	unsigned long checksum2 = 0;
	prefix_image_header_t header1;
	image_header_t header2;
	prefix_image_header_t *hdr1 = &header1;
	image_header_t *hdr2 = &header2;
	int ifd = 0;
	int len1 = file_end-file_begin;
	int len2 = file_end - file_new_begin;

	if ((unsigned)(len1) < sizeof(prefix_image_header_t)||
		(unsigned)(len2) < sizeof(image_header_t)) 
	{
		printf("<script>parent.upload_message='Firmware has a bad size.';</script>");
		goto err;
	}

	ifd = open(filename, O_RDONLY);
	if(!ifd)
	{
		printf("<script>parent.upload_message='Can't open firmware.';</script>");
		goto err;	
	}

	if (fstat(ifd, &sbuf) < 0) 
	{
		printf("<script>parent.upload_message='Can't stat firmware.';</script>");				
		close(ifd);
		goto err;
	}

	ptr1 = (unsigned char *) mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, ifd, 0);
	ptr2 = (unsigned char *) mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, ifd, 0);
	if ((caddr_t)ptr1 == (caddr_t)-1) 
	{
		printf("<script>parent.upload_message='Can't mmap firmware1.';</script>");				
		close(ifd);
		goto err;
	}

	ptr1 += file_begin;
	ptr2 += file_new_begin;
	/*
	 *	handle Header CRC32
	 */
	memcpy (hdr1, ptr1, sizeof(prefix_image_header_t));
	memcpy (hdr2, ptr2, sizeof(image_header_t));
	//should define PIH_MAGIC 
#ifdef PIH_MAGIC	
	if (ntohl(hdr1->pih_magic) != PIH_MAGIC) 
	{
		printf("<script>parent.upload_message='Liteon header has a error magic number.';</script>");			
		munmap(ptr1, len1);
		close(ifd);
		goto err;
	}
#else
	fprintf(stderr,"PIH_MAGIC is not defined as a marco\r\n");	
#endif
	if (ntohl(hdr2->ih_magic) != IH_MAGIC) 
	{
		printf("<script>parent.upload_message='Mkimage header has a error magic number.';</script>");			
		munmap(ptr2, len2);
		close(ifd);
		goto err;
	}

    upgradestatus = get_upgrade_status();

	if(upgradestatus == UPGRADE_BUSY || upgradestatus == UPGRADE_DOWNLOADING
                || upgradestatus == UPGRADE_WRITING)//snmp upgrading
	{
		printf("<script>parent.upload_message='system is upgrading firmware by other process.';</script>");
		goto err;
	}

    set_upgrade_status(UPGRADE_BUSY);

	header_len1 = sizeof(prefix_image_header_t);
	checksum1 = ntohl(hdr1->pih_hcrc);
	hdr1->pih_hcrc = htonl(0);	/* clear for re-calculation */

	if (crc32 (0, (char *)hdr1, header_len1) != checksum1) 
	{
		printf("<script>parent.upload_message='Firmware has bad header checksum!';</script>");				
		munmap(ptr1, len1);
		close(ifd);
		goto err;
	}

	header_len2 = sizeof(image_header_t);
	checksum2 = ntohl(hdr2->ih_hcrc);
	hdr2->ih_hcrc = htonl(0);	/* clear for re-calculation */

	if (crc32 (0, (char *)hdr2, header_len2) != checksum2) 
	{
		printf("<script>parent.upload_message='Firmware has bad header checksum!';</script>");					
		munmap(ptr2, len2);
		close(ifd);
		goto err;
	}

    /* check image name: must have special string in image name*/
    if(!strstr(hdr2->ih_name, IMAGE_NAME_IN_HEADER) && !strstr(hdr2->ih_name, IMAGE_NAME2_IN_HEADER)
        && !strstr(hdr2->ih_name, IMAGE_NAME3_IN_HEADER))
    {
		printf("<script>parent.upload_message='Firmware is not valid for this product.';</script>");					
		munmap(ptr2, len2);
		close(ifd);
		goto err;
    }

	/*
	 *	handle Data CRC32
	 */
	char *data = (char *)(ptr1 + sizeof(prefix_image_header_t));
	unsigned long data_len  = len1 - sizeof(prefix_image_header_t) ;
	
	if (crc32 (0, data, data_len) != ntohl(hdr1->pih_dcrc)) 
	{
		printf("<script>parent.upload_message='Firmware has bad data checksum.';</script>");						
		munmap(ptr1, len1);
		close(ifd);
		goto err;
	}

	fw_kernel_len = ntohl(hdr2->ih_size);

	munmap(ptr1, len2);
	munmap(ptr2, len2);
	close(ifd);
	
	
	file_middle = file_new_begin + sizeof(image_header_t) + fw_kernel_len; //add by bingley	

//	rootfs_begin = findZeroInFile(filename,file_middle+1,'\0',file_end-file_middle);// add by bingley
//	fprintf(stderr,"rootfs_begin = %ld\r\n",rootfs_begin); //add by bingley
	
	 ezp_special_process();    
//   system("cp /tmp/writeflash_detect1.html  /web/writeflash_detect1.html &");
	 sleep(3);
      // flash write
    //mtd_write_firmware(filename, file_new_begin, file_end - file_begin); origin  function
    snprintf(cmd, 512, "/usr/sbin/mtd_write -o %d -l %d write %s kernel", file_new_begin, file_middle-file_new_begin, filename);
	system(cmd);
	memset(cmd,0,512);
	snprintf(cmd, 512, "/usr/sbin/mtd_write -o %d -l %d write %s rootfs", file_middle, file_end-file_middle, filename);
    system(cmd);
	memset(cmd,0,512);
	snprintf(cmd, 512, "logger EZP_USR  %s\n","FWUpgrade successful" );
	system(cmd);
#endif
    set_upgrade_status(UPGRADE_SUCCESSD);

//    reboot(RB_AUTOBOOT);
	system("wifi_unload.sh");
	system("/tmp/busybox sync");
    //system("/tmp/busybox reboot");
    reboot(RB_AUTOBOOT);
    webFoot();
    free(boundary);
    exit(0);

err:
#if 1//Arthur Chow 2009-03-15    
    printf("<script>parent.reload_upload_firmware_page();</script>");
#endif    
    webFoot();
    free(boundary);

    set_upgrade_status(UPGRADE_FW_INVALID);
    
    exit(-1);
}


#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

//#define FILENAME	"qty5.jpg.bz2"
#define FILENAME	"haiku.txt"

/* func.c Prototypes */
int open_port(void);
int write_port(int fd, const void *buf, size_t count);
int close_port(int fd);

/* TODO change main to write whatever file is passed from cmd line
 * TODO add retry when # bytes written < 100
 */
int 
main(void)
{
	int port_fd = open_port();
	/* Do stuff */

	/* pointer to file we want to send over port */
	FILE *sendfile_fp = fopen(FILENAME, "r");
	struct stat st;
	stat(FILENAME, &st);	/* stat() returns # bytes in file */
	const int FILESIZE = st.st_size;
	printf("Sending file of size %d B\n", FILESIZE);
	/* descriptor of file we want to send */
	int sendfile_fd = open(FILENAME, O_RDONLY);

	/*
	printf("Writing test string 'hello' to port\n");
	int write_success = write_port(port_fd, "hello", 5);
	printf("write_port() returned %d\n", write_success);
	*/
	
	/* Now, to business.
	 * Write file described by "sendfile_fd", 100 Bytes at a time.
	 * On each pass, print # Bytes succesfully written/ returned from "read()" call
	 */	
	char* buf[100];
	int num_bytes_read = 0;
	int num_bytes_wrote;
	for (int head = 0; head < FILESIZE; head += num_bytes_read)
	{
		num_bytes_read = read(sendfile_fd, &buf, 100); /* read up to 100 B */
		num_bytes_wrote = write_port(port_fd, &buf, num_bytes_read);
		printf("\tread %d B, wrote %d B\n", num_bytes_read, num_bytes_wrote);	
	}

	/* Done with stuff */
	fclose(sendfile_fp);
	close(sendfile_fd);
	int close_success = close_port(port_fd);
	printf("close_port() returned %d\n", close_success);
	return 0;
}

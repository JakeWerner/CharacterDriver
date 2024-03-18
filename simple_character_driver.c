#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

//Jacob Werner 
MODULE_AUTHOR("Jacob Werner");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple character driver");

//Initialize global variables
static int timesOpened = 0;
static int timesClosed = 0;
void *device_buffer;
//char kMessage[1024] = {0};
int filePosition = 0;
#define maxBufferSize 1024

//Define function prototypes
int my_open(struct inode *, struct file *);
int my_close(struct inode *, struct file *);
ssize_t my_read(struct file *, char __user *, size_t, loff_t *);
ssize_t my_write(struct file *, const char __user *, size_t, loff_t *);
loff_t my_seek(struct file *, loff_t, int);

//File operations structure to associate user functions with device functions
struct file_operations my_file_operations = {
	.owner   = THIS_MODULE,
	.open    = my_open,
	.release = my_close,
	.read    = my_read,
	.write   = my_write,
	.llseek  = my_seek
};

int simple_character_device_init(void){
	//Create kernel buffer and associate it to device_buffer
	device_buffer = kmalloc(maxBufferSize, GFP_KERNEL);
	//Register the device with major 240 and print to kernel to indicate success
	if(register_chrdev(240,"simple_character_device", &my_file_operations) < 0){
		printk(KERN_ALERT "Device Registration Failed: simple_character_device\n");
		return -1;
	}
	else{
		printk(KERN_ALERT "Device registered: simple_character_device\n");
		return 0;
	}
}

void simple_character_device_exit(void){
	//Deallocate kernel buffer
	kfree(device_buffer);
	//Unregister device
	unregister_chrdev(240, "simple_character_device");
}

//Associate the init and exit functions
module_init(simple_character_device_init);
module_exit(simple_character_device_exit);

//my_open function iterates the number of times the file has been opened
int my_open(struct inode *inode1, struct file *file1){
	timesOpened++;
	return 0;
}

//my_close iterates the number of times the file was closed and prints to the kernel
int my_close(struct inode *inode1, struct file *file1){
	timesClosed++;
	printk(KERN_ALERT "File closed. File closed %d times\n", timesClosed);
	return 0;
}

ssize_t my_read(struct file *file1, char __user *buffer, size_t bufferSize, loff_t *pos){
	int byteCount = 0;
	if(*pos > maxBufferSize){
		return -1;
	}
	if(copy_to_user(buffer, device_buffer + *pos, bufferSize) != 0){
		return -1;
	}  
	copy_to_user(buffer, device_buffer + *pos, bufferSize);
	byteCount = bufferSize;
	printk(KERN_ALERT "%d bytes read\n", byteCount);
	*pos = file1 -> f_pos;
	return byteCount;
}

ssize_t my_write(struct file *file1, const char __user *buffer, size_t bufferSize, loff_t *pos){
	int byteCount = bufferSize;
	if(*pos > maxBufferSize){
		return -1;
	}
	if(copy_from_user(device_buffer + *pos, buffer, bufferSize) != 0){
		return -1;
	}
	copy_from_user(device_buffer + *pos, buffer, bufferSize);
	printk(KERN_ALERT "%d bytes written to kernal space\n", byteCount);
	*pos = file1 -> f_pos + byteCount;
	return byteCount;
}

loff_t my_seek(struct file *file1, loff_t pos, int whence){
	loff_t newOffset;
	switch(whence){
		case 0:
			newOffset = pos;
			break;
		case 1:
			newOffset = file1 -> f_pos + pos;
			break;
		case 2:
			newOffset = maxBufferSize - 1;
			break;
	} 
	if((newOffset > maxBufferSize) || (newOffset < 0)){
		return -1;
	}	
	else{
		file1 -> f_pos = newOffset;
		return newOffset;
	}
}

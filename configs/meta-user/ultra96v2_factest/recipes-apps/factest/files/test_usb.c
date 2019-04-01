//        ** **        **          **  ****      **  **********  **********
//       **   **        **        **   ** **     **  **              **
//      **     **        **      **    **  **    **  **              **
//     **       **        **    **     **   **   **  *********       **
//    **         **        **  **      **    **  **  **              **
//   **           **        ****       **     ** **  **              **
//  **  .........  **        **        **      ****  **********      **
//     ...........
//                                     Reach Further
//
// ----------------------------------------------------------------------------
// 
//  This design is the property of Avnet.  Publication of this
//  design is not authorized without written consent from Avnet.
// 
//  Please direct any questions to the Ultra96 community support forum:
//     http://avnet.me/zed-forums	
// 
//  Product information is available at:
//     http://avnet.me/ultra96v2	
// 
//  Disclaimer:
//     Avnet, Inc. makes no warranty for the use of this code or design.
//     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//     any errors, which may appear in this code, nor does it make a commitment
//     to update the information contained herein. Avnet, Inc specifically
//     disclaims any implied warranties of fitness for a particular purpose.
//                      Copyright(c) 2016 Avnet, Inc.
//                              All rights reserved.
// 
// ----------------------------------------------------------------------------
// 
//  Create Date:         Jul 22, 2017
//  Design Name:         Ultra96-V2
//  Module Name:         test_usb.c
//  Project Name:        Ultra96-V2 USB test application
//  Target Devices:      Xilinx Zynq UltraScale+ MPSoC
//  Hardware Boards:     Ultra96-V2
// 
//  Tool versions:       Xilinx Vivado 2017.4
// 
//  Description:         Main Ultra96-V2 production test application 
// 
//  Dependencies:        
//
//  Revision:            Jul 22, 2017: 1.0 Initial version
//                       created by Xilinx for Ultra96-V1 board
//                       Mar 28, 2019: 1.1 Updated for Ultra96-V2 board
// 
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <sys/mount.h>
#include <sys/stat.h>
 
#include "typedef.h"

#define USB_MSG_DEVICE_TMP_FILENAME 	"usb_msg_dev_tmp_name.txt"
#define USB_MSG_FDISK_LAYOUT_FILE 		"usb_msg.txt"

extern TEST_RESULT perform_read_write_verify(FILE *fp);

static char 		drive_name[COMMAND_LENGTH];
static const char *usb_ports_speed_version_string[] = {"USB 1.1","USB 2.x","USB 3.0","USB 3.1"};
static const char *lsusb_ports_speed_string[] = {"12M","480M","5000M","10000M"};
static const char *lsusb_ports_string[] = {"Port 1","Port 2","Port 3"};//since we can have maximum three USB Host ports on the board including mezzanine board
static const char *usb_ports_string[] = {"USB Port - 1","USB Port - 2","USB Port - 3"};


typedef struct _USB_HOST_PORTS
{
	USB_PORT_SPEED_VERSION 	usb_port_speed_version;
}USB_HOST_PORTS;
USB_HOST_PORTS usb_host_ports_details[MAX_USB_HOST_PORTS];

//this comes randomly but assuming one of the USB device would be sda1, another would be sdb1 and so on.
//static const char *usb_ports_device_name_string[] = {"/dev/sda1","/dev/sdb1","dev/sdc1"};
static const char *usb_ports_base_name_string[] = {
	"/dev/disk/by-path/platform-xhci-hcd.0.auto-usb-0:1.1:1.0-scsi-0:0:0:0", //USB 3 - J8
	"/dev/disk/by-path/platform-xhci-hcd.0.auto-usb-0:1.2:1.0-scsi-0:0:0:0", //USB 3 - J8
	"/dev/disk/by-path/platform-xhci-hcd.0.auto-usb-0:1.3:1.0-scsi-0:0:0:0" //USB 3 - J8
};

static const char *usb_ports_device_name_string[] = 
{
	"/dev/disk/by-path/platform-xhci-hcd.0.auto-usb-0:1.1:1.0-scsi-0:0:0:0-part1", //USB 3 - J8
	"/dev/disk/by-path/platform-xhci-hcd.0.auto-usb-0:1.2:1.0-scsi-0:0:0:0-part1", //USB 3 - J9
	"/dev/disk/by-path/platform-xhci-hcd.0.auto-usb-0:1.3:1.0-scsi-0:0:0:0-part1" //USB 2
};


void unload_usb_msg_modules(FILE *fpUSBMSG, char *usb_mount_tar_dir, char *file_name)
{
	char 	str_command[COMMAND_LENGTH];
	int 	ret_val;
	
	if(fpUSBMSG)
	{
		DBG_MSG("closing file \"%s\"\n",USB_MSG_TEST_FILE);		
		fclose(fpUSBMSG);			
	}
	if(file_name)
	{
		ret_val = remove(file_name);
		if(ret_val == 0)
		{
			DBG_MSG("Removed Test File - \"%s\"\n",file_name);
		}
		else
		{
			ERR_MSG("Error Removing file - \"%s\"\n",file_name);
		}
	}	
	if(usb_mount_tar_dir)
	{
		sprintf(str_command,"sudo umount %s",usb_mount_tar_dir);
		DBG_MSG("unmounting drive %s\n",str_command);		
		system(str_command);
		
		sprintf(str_command,"sudo rm -rf %s",usb_mount_tar_dir);
		DBG_MSG("removing mount directory \"%s\"\n",str_command);			
		system(str_command);
	}
	system("rmmod g_mass_storage");
//	system("rmmod gadgetfs");
}
TEST_RESULT get_usb_msg_device_name(USB_PORTS usb_port_number)
{
	char		str_command[COMMAND_LENGTH];
	FILE 		*fpDevName = NULL;
	char 		buf[COMMAND_LENGTH];
	TEST_RESULT test_result = PASS;
	int 		ret_val;
	
	sprintf(str_command,"ls -l %s > %s",usb_ports_base_name_string[usb_port_number],USB_MSG_DEVICE_TMP_FILENAME);
	system(str_command);
	DBG_MSG("Executing command - %s\n",str_command);
	
	fpDevName = fopen(USB_MSG_DEVICE_TMP_FILENAME, "r");
	if (!fpDevName)
	{	
		ERR_MSG("error in creating temp. file for USB MSG drive name\n");
		return FAIL;
	}
	
	/* Parse the usb_type.txt file to find the Port and Device Type */
	while (fgets(buf,COMMAND_LENGTH, fpDevName)!=NULL) 
	{
		strcpy(drive_name,strstr(buf,"sd"));
		if(!drive_name)
		{
			ERR_MSG("USB MSG drive not detected\n");
			test_result =  FAIL;
		}
		else
		{
			drive_name[3] = '\0';//drive_name[0 - 2] = "sdx" where x can be a, b, c...
			DBG_MSG("USB MSG is connected with device name - /dev/%s\n",drive_name);
			test_result = PASS;
		}
	}
	fclose(fpDevName);
	ret_val = remove(USB_MSG_DEVICE_TMP_FILENAME);
	if(ret_val == 0)
	{
		DBG_MSG("Removed Test File - \"%s\"\n",USB_MSG_DEVICE_TMP_FILENAME);
	}
	else
	{
		ERR_MSG("Error Removing file - \"%s\"\n",USB_MSG_DEVICE_TMP_FILENAME);
	}	
	return test_result;
}

void get_usb_devices_details(void)
{
	FILE 			*fp = NULL;
	char 			buf[COMMAND_LENGTH];
	char 			temp[COMMAND_LENGTH];
	char 			*port;
	char 			device_type[COMMAND_LENGTH];
	char 			str_command[COMMAND_LENGTH];
	int 			ret_val;
	int 			loop_count ;
	char			usb_port_index = 0;
	char 			device_class[COMMAND_LENGTH] = "";
	char 			port_no[COMMAND_LENGTH] = "";
	char 			*string = NULL;
	

	sprintf(str_command,"lsusb -t | grep '%s' > %s",USB_MASS_STORAGE_STRING,USB_DRIVE_TMP_FILE);
	system(str_command);
	DBG_MSG("Executing command - %s\n",str_command);
	fp = fopen(USB_DRIVE_TMP_FILE, "r");
	if (!fp)
	{	
		ERR_MSG("error in creating temp. file for USB test\n");
		return;
	}
	for(loop_count = 0; loop_count < MAX_USB_HOST_PORTS; loop_count++)
	{
		usb_host_ports_details[loop_count].usb_port_speed_version = INVALID_USB_SPEED;
	}
	
	usb_port_index = 0;
	
	/* Parse the usb_type.txt file to find the Port and Device Type */
	while (fgets(buf,COMMAND_LENGTH, fp)!=NULL) 
	{
		string = strdup(buf);
		if((port = strsep(&string, ":")) != NULL) 
		{			
			if (strstr(port,lsusb_ports_string[USB3_PORT_0]))
			{
				strcpy(port_no, usb_ports_string[USB3_PORT_0]);//USB3_PORT_0
				usb_port_index = USB3_PORT_0;
				//usb_host_ports_details[usb_port_index].usb_port_number = USB3_PORT_0;
			}
			else if(strstr(port,lsusb_ports_string[USB3_PORT_1]))
			{
				strcpy(port_no, usb_ports_string[USB3_PORT_1]);
				//usb_host_ports_details[usb_port_index].usb_port_number = USB3_PORT_1;
				usb_port_index = USB3_PORT_1;
			}
			else if(strstr(port,lsusb_ports_string[USB2_PORT]))
			{				
				strcpy(port_no,usb_ports_string[USB2_PORT]);
				//usb_host_ports_details[usb_port_index].usb_port_number = USB2_PORT;
				usb_port_index = USB2_PORT;
			}
			else					
			{	
				strcpy(device_type,"Invaid USB Port Number");
				usb_port_index = INVALID_USB_PORT;
				return;
			}
		}		
		if (strstr(buf,USB_MASS_STORAGE_STRING))				
		{
			strcpy(device_class,USB_MASS_STORAGE_STRING);
			if (strstr(buf,lsusb_ports_speed_string[USB_1_x]))
			{				
				strcpy(device_type, usb_ports_speed_version_string[USB_1_x]);
				usb_host_ports_details[usb_port_index].usb_port_speed_version = USB_1_x;
			}
			else if (strstr(buf, lsusb_ports_speed_string[USB_2_x]))
			{
				strcpy(device_type, usb_ports_speed_version_string[USB_2_x]);
				usb_host_ports_details[usb_port_index].usb_port_speed_version = USB_2_x;
			}
			else if (strstr(buf,lsusb_ports_speed_string[USB_3_0]))
			{
				strcpy(device_type, usb_ports_speed_version_string[USB_3_0]);
				usb_host_ports_details[usb_port_index].usb_port_speed_version = USB_3_0;
			}
			else if (strstr(buf,lsusb_ports_speed_string[USB_3_1]))
			{
				strcpy(device_type, usb_ports_speed_version_string[USB_3_1]);
				usb_host_ports_details[usb_port_index].usb_port_speed_version = USB_3_1;
			}
			else
			{
				strcpy(device_type,"Undefined USB Version");
				usb_host_ports_details[usb_port_index].usb_port_speed_version = INVALID_USB_SPEED;
			}						
		}
	}
	fclose(fp);
	ret_val = remove(USB_DRIVE_TMP_FILE);
	if(ret_val == 0)
	{
		DBG_MSG("Removed Test File - \"%s\"\n",USB_DRIVE_TMP_FILE);
	}
	else
	{
		ERR_MSG("Error Removing file - \"%s\"\n",USB_DRIVE_TMP_FILE);
	}	
}
TEST_RESULT mount_usb_drive(USB_PORTS usb_port_number,FILE **fpUSB,char *usb_mount_tar_dir, char *file_name)
{
	char str_command[COMMAND_LENGTH];
	int ret_val;
	TEST_RESULT test_result = PASS;
	
	if(INVALID_USB_SPEED == usb_host_ports_details[usb_port_number].usb_port_speed_version)
	{
		ERR_MSG("Please make sure that valid USB drive is inserted before starting test, USB Drive is missing...\n");
		return FAIL;
	}
	
	if((USB3_PORT_0 == usb_port_number) && (USB_3_0 != usb_host_ports_details[usb_port_number].usb_port_speed_version && 
																	USB_3_1 != usb_host_ports_details[usb_port_number].usb_port_speed_version))
		USR_MSG("Please insert the appropriate USB Device in USB 3.0 Port - %d\n", usb_port_number+1);

	if((USB3_PORT_1 == usb_port_number) && (USB_3_0 != usb_host_ports_details[usb_port_number].usb_port_speed_version && 
																	USB_3_1 != usb_host_ports_details[usb_port_number].usb_port_speed_version))
		USR_MSG("Please insert the appropriate USB Device in USB 3.0 Port - %d\n", usb_port_number+1);
	
	if((USB2_PORT == usb_port_number) && (USB_2_x != usb_host_ports_details[usb_port_number].usb_port_speed_version))
		USR_MSG("Please insert the appropriate USB Device in USB 2.0 Port\n");

	do
	{
		sprintf(usb_mount_tar_dir,"%s%d",USB_MOUNT_DIR,usb_port_number);
		
		DBG_MSG("Creating directory - \"%s\"\n",usb_mount_tar_dir);
		sprintf(str_command,"sudo mkdir -p %s",usb_mount_tar_dir);		
		ret_val = system(str_command);
		if(ret_val != 0)
		{
			DBG_MSG("Unable to create directory \"%s\", try running the test again...\n",usb_mount_tar_dir);		
			unmount_remove_usb(NULL,usb_mount_tar_dir,NULL);			
			return FAIL;
		}
		
		DBG_MSG("mounting drive \"%s\" on directory - \"%s\"\n",usb_ports_device_name_string[usb_port_number],usb_mount_tar_dir);				
		ret_val = mount(usb_ports_device_name_string[usb_port_number],usb_mount_tar_dir,MOUNT_FS_TYPE,0,0) ;				
		if(0 != ret_val)	
		{
			ERR_MSG("Unable to mount %s on %s\n",usb_ports_device_name_string[usb_port_number],usb_mount_tar_dir);
			
			unmount_remove_usb(NULL,usb_mount_tar_dir,NULL);	
			return FAIL;
		}
		
		sprintf(file_name,"%s/%s",usb_mount_tar_dir,USB_TEST_FILE);
		DBG_MSG("opening file - \"%s\"\n",file_name);		
		*fpUSB = fopen(file_name,"w+");
		if(!*fpUSB)
		{			
			ERR_MSG("Unable to open file - %s\n",file_name);
			test_result = FAIL;
			unmount_remove_usb(NULL,usb_mount_tar_dir,NULL);	
			break;
		}
	
	}while(0);
	return test_result;
}
TEST_RESULT unmount_remove_usb(FILE *fpUSB, char *usb_mount_tar_dir,char *file_name)
{
	int ret_val;
	char str_command[COMMAND_LENGTH];
	if(fpUSB)
	{
		DBG_MSG("closing file \"%s\"\n",USB_TEST_FILE);		
		fclose(fpUSB);		
		ret_val = remove(file_name);
		if(ret_val == 0)
		{
			DBG_MSG("Removed Test File - \"%s\"\n",file_name);
		}
		else
		{
			ERR_MSG("Error Removing file - \"%s\"\n",file_name);
		}
	}
	
	sprintf(str_command,"sudo umount %s",usb_mount_tar_dir);
	DBG_MSG("unmounting drive %s\n",str_command);		
	system(str_command);
	
	sprintf(str_command,"sudo rm -rf %s",usb_mount_tar_dir);
	DBG_MSG("removing mount directory \"%s\"\n",str_command);			
	system(str_command);

}
TEST_RESULT test_usb(USB_PORTS usb_port_number)
{
	FILE 		*fpUSB = NULL;
	int 		ret_val = 0;
	char 		str_command[COMMAND_LENGTH];
	char 		file_name[COMMAND_LENGTH];	
	char 		usb_mount_tar_dir[COMMAND_LENGTH];
	TEST_RESULT test_result = FAIL;
	
	DBG_MSG("\n\nMake sure empty USB with one partition is inserted\n");
	
	test_result = mount_usb_drive(usb_port_number,&fpUSB,usb_mount_tar_dir,file_name);
	if(PASS == test_result)
	{
		DBG_MSG("performing read/write/verify in USB Device\n");		
		test_result = perform_read_write_verify(fpUSB);//perform_read_write_verify is implmeneted in test_sd.c
		unmount_remove_usb(fpUSB,usb_mount_tar_dir,file_name);
	}
	
	return test_result;	
}
TEST_RESULT load_usb_msg(FILE **fpUSBMSG,char *usb_mount_tar_dir, char *file_name)
{
	TEST_RESULT test_result = PASS;
	char		str_command[COMMAND_LENGTH];
	int			ret_val ;
	char 		usb_port_name_string[COMMAND_LENGTH];
	FILE 		*fp = NULL;
	FILE 		*fpFDISKLAYOUT = NULL;
	int 		bytes_write_read = 0;		
	char 		write_string[COMMAND_LENGTH];
	char 		cmd_string[COMMAND_LENGTH];
	struct stat	file_status_buf;
	
	/*
	1. dd if=/dev/zero of=/tmp/mydev count=64 bs=1M
	2. modprobe g_mass_storage file=/tmp/mydev removable=1 stall=1 iSerialNumber=7ABC7ABC7ABC7ABC7ABC7ABC
	3. get USB drive name such as sda or sdb
	4. create partition using sfdisk such as sda1 or sdb1
	5. format created partition
	6. mount /dev/sdX and perform R/W or connect with windows machine
	*/

	sprintf(str_command,"dd if=/dev/zero of=%s count=64 bs=1M status=none",USB_MSG_DEV);
	DBG_MSG("Executing command - %s\n",str_command);
	system(str_command);
	
	sprintf(str_command,"modprobe g_mass_storage file=%s removable=1 stall=1 iSerialNumber=7ABC7ABC7ABC7ABC7ABC7ABC",USB_MSG_DEV);
	DBG_MSG("Executing command - %s\n",str_command);
	system(str_command);
	
	USR_MSG("Creating USB Mass Storage Gadget Device, this may take a few seconds...\n");
	sleep(30);// wait is required to make sure that USB MSG device is created successfully, before we go check for the device
	//without the sleep it has been observed that device does not get created and following functions start failing...
	
	ret_val = lstat(usb_ports_base_name_string[USB3_PORT_0],&file_status_buf);
	if(ret_val != 0)
	{
		ERR_MSG("Unable to create USB Mass Storage Gadget Device!!!\n");
		
		unload_usb_msg_modules(NULL, NULL, NULL);
		return FAIL;
	}
	
	//get the drive name on which USB MSG device is connected such as sda or sdb
	test_result = get_usb_msg_device_name(USB3_PORT_0);
	if(FAIL == test_result)
	{	
		ERR_MSG("Unable to find USB Mass Storage Gadget Device\n");
		unload_usb_msg_modules(NULL,NULL,NULL);
		return test_result;
	}	
	
	//create a partition layout (using detected drive name) for sfdisk which will be used to create a partition on the USB MSG
	sprintf(file_name,"%s",USB_MSG_FDISK_LAYOUT_FILE);
	DBG_MSG("opening file to write partition layout - \"%s\"\n",file_name);		
	
	//using fixed partition layout for Mass storage gadget device. Dont know any better method is there or not...
	fpFDISKLAYOUT = fopen(file_name,"w+t");
	if(!fpFDISKLAYOUT)	
	{
		ERR_MSG("Unable to open file - %s\n",file_name);
		unload_usb_msg_modules(NULL,NULL,NULL);
		return FAIL;		
	}
	
	strcpy(write_string,"label: dos\nlabel-id: 0xecf46854\ndevice: ");//<0xecf46854 is a magic number>
	DBG_MSG("layout file is:\n%s",write_string);
	bytes_write_read = fwrite(write_string,1,strlen(write_string),fpFDISKLAYOUT);
	if(bytes_write_read < strlen(write_string))
	{
		ERR_MSG("Unable to write to File, wrote %d, had to write - %d\n",bytes_write_read,strlen(write_string));
		unload_usb_msg_modules(fpFDISKLAYOUT,NULL,file_name);
		return FAIL;
	}	
	
	sprintf(write_string,"/dev/%s\nunit: sectors\n\n/dev/%s1 : ",drive_name,drive_name);
	DBG_MSG(write_string);
	bytes_write_read = fwrite(write_string,1,strlen(write_string),fpFDISKLAYOUT);
	if(bytes_write_read < strlen(write_string))
	{
		ERR_MSG("Unable to write to File, wrote %d, had to write - %d\n",bytes_write_read,strlen(write_string));
		unload_usb_msg_modules(fpFDISKLAYOUT,NULL,file_name);
		return FAIL;
	}	
	
	strcpy(write_string,"start=        2048, size=      129024, type=83");
	DBG_MSG(write_string);	DBG_MSG("\n");		
	bytes_write_read = fwrite(write_string,1,strlen(write_string),fpFDISKLAYOUT);
	if(bytes_write_read < strlen(write_string))
	{
		ERR_MSG("Unable to write to File. Wrote %d, had to write - %d\n",bytes_write_read,strlen(write_string));
		unload_usb_msg_modules(fpFDISKLAYOUT,NULL,file_name);
		return FAIL;
	}		
	DBG_MSG("closing file \"%s\"\n",file_name);		
	fclose(fpFDISKLAYOUT);
	
	//check if drive exists then only for go for partition, formatting, mounting etc.
	sprintf(cmd_string,"/dev/%s",drive_name);
	ret_val = stat(cmd_string,&file_status_buf);
	if(ret_val != 0)
	{
		ERR_MSG("%s does not exist\n",cmd_string);
		unload_usb_msg_modules(NULL,NULL,NULL);
		return FAIL;
	}
	DBG_MSG("%s USB Mass Storage Gadget device created successfully...\n",cmd_string);
	
	//create a partition such as /dev/sda1 or /dev/sdb1		
	//need to pass here /dev/sdX, sfdisk will create /dev/sdX1
	sprintf(cmd_string,"sfdisk -q /dev/%s < %s",drive_name,file_name);
	DBG_MSG(cmd_string);DBG_MSG("\n");		
	system(cmd_string);

	ret_val = remove(file_name);
	if(ret_val == 0)
	{
		DBG_MSG("Removed layout File - \"%s\"\n",file_name);
	}
	else
	{
		ERR_MSG("Error Removing layout file - \"%s\"\n",file_name);
	}
	sleep(2);

	//check if /dev/sdX1 exists then only for go for formatting, mounting etc.
	sprintf(cmd_string,"/dev/%s1",drive_name);
	ret_val = stat(cmd_string,&file_status_buf);
	if(ret_val != 0)
	{
		ERR_MSG("%s does not exist\n",cmd_string);
		unload_usb_msg_modules(NULL,NULL,NULL);
		return FAIL;
	}
	DBG_MSG("/dev/%s1 partition created successfully...\n",drive_name);
	
	//format partition		
	sprintf(cmd_string,"mkfs.vfat /dev/%s1",drive_name);
	DBG_MSG(cmd_string);		
	system(cmd_string);
	DBG_MSG("%s partition formatted successfully...\n",cmd_string);
	sleep(1);

	//create mount directory
	sprintf(usb_mount_tar_dir,"%s",USB_MSG_MNT_DIR);
	DBG_MSG("creating mount directory - \"%s\"\n",usb_mount_tar_dir);
	sprintf(str_command,"sudo mkdir %s",usb_mount_tar_dir);	
	ret_val = system(str_command);
	if(ret_val != 0)
	{
		ERR_MSG("Unable to create directory \"%s\"\n",usb_mount_tar_dir);
		unload_usb_msg_modules(NULL,NULL,NULL);
		return FAIL;
	}	
	sleep(1);
	
	sprintf(usb_port_name_string,usb_ports_device_name_string[USB3_PORT_0]);
	DBG_MSG("mounting drive \"%s\" on directory - \"%s\"\n",usb_port_name_string,usb_mount_tar_dir);	
	ret_val = mount(usb_port_name_string,usb_mount_tar_dir,MOUNT_FS_TYPE,0,0) ;				
	if(0 != ret_val)	
	{
		ERR_MSG("Unable to mount %s at %s\n",usb_port_name_string,usb_mount_tar_dir);
		
		unload_usb_msg_modules(NULL,usb_mount_tar_dir,NULL);	
		return FAIL;
	}
	sleep(1);
	
	sprintf(file_name,"%s/%s",usb_mount_tar_dir,USB_MSG_TEST_FILE);
	DBG_MSG("opening file - \"%s\"\n",file_name);		
	*fpUSBMSG = fopen(file_name,"w+");
	if(!*fpUSBMSG)
	{			
		ERR_MSG("Unable to open file - %s\n",file_name);
		test_result = FAIL;		
	}
	return test_result;
}
TEST_RESULT test_usb_upstream(void)
{
	TEST_RESULT test_result = PASS;	
	char 		file_name[COMMAND_LENGTH];
	char 		usb_mount_tar_dir[COMMAND_LENGTH];
	FILE 		*fpUSBMSG = NULL;
	
	test_result = load_usb_msg(&fpUSBMSG,usb_mount_tar_dir,file_name);	
	
	if(fpUSBMSG)
	{		
		DBG_MSG("performing read/write/verify in USB MTG Device\n");		
		test_result = perform_read_write_verify(fpUSBMSG);//perform_read_write_verify is implmeneted in test_sd.c	
	
	}
	
	DBG_MSG("unloading USB Mass Storage Gadget Modules\n");
	unload_usb_msg_modules(fpUSBMSG,usb_mount_tar_dir,file_name);
	
	return test_result;
}
void * thread_usb_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT 	test_result = PASS;
	int 			usb_port_number = thread_message->function_data;
	FILE 			*fpUSB = NULL;
	char			usb_mount_tar_dir[COMMAND_LENGTH];
	char			file_name[COMMAND_LENGTH];
	
	test_result = mount_usb_drive(usb_port_number,&fpUSB,usb_mount_tar_dir,file_name);
	if(FAIL == test_result)
	{	
		ERR_MSG("Unable to mount USB Port - %d, exiting USB test\n",usb_port_number);		
		thread_message->test_result = test_result;	
		return void_thread_message;	
	}
	do
	{
		test_result = perform_read_write_verify(fpUSB);//perform_read_write_verify is implmeneted in test_sd.c		
		if(FAIL == test_result)
		{	
			ERR_MSG("USB Test failed, exiting USB test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);

	unmount_remove_usb(fpUSB,usb_mount_tar_dir,file_name);
	thread_message->test_result = test_result;	
	return void_thread_message;	
}

void * thread_usb_upstream_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	FILE *fpUSBMSG = NULL;
	char file_name[COMMAND_LENGTH];
	char usb_mount_tar_dir[COMMAND_LENGTH];
	
	test_result = load_usb_msg(&fpUSBMSG,usb_mount_tar_dir,file_name);		
	if(FAIL == test_result)
	{	
		ERR_MSG("Unable to create USB Mass Storage Gadget, exiting USB MSG test\n");		
		thread_message->test_result = test_result;	
		return void_thread_message;	
	}
	do
	{
		DBG_MSG("performing read/write/verify in USB MTG Device\n");		
		test_result = perform_read_write_verify(fpUSBMSG);//perform_read_write_verify is implmeneted in test_sd.c			
		if(FAIL == test_result)
		{				
			ERR_MSG("USB Upstream Test failed, exiting USB Upstream test\n");
			break;
		}	
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);

	DBG_MSG("unloading USB Mass Storage Gadget Modules\n");
	unload_usb_msg_modules(fpUSBMSG,usb_mount_tar_dir,file_name);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}


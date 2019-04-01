#include <stdio.h>

#include "typedef.h"


#define DMESG_LOG_FILE	"dmesg_OTP_ver.txt"
#define OTP_VER_A	'A'
#define OTP_VER_B	'B'
#define NULL_CHAR	'\0'

int check_otp_ver_from_dmesg_log()
{
	FILE *fp = NULL;
	char str_command[COMMAND_LENGTH];
	char buf[COMMAND_LENGTH];
	char otp = NULL_CHAR;
	int 	ret_val;
	fp = fopen(DMESG_LOG_FILE, "r");
	if (!fp)
	{
		ERR_MSG("Error in creating file for PMIC\n");
		return otp;
	}
	
	while (fgets(buf, COMMAND_LENGTH, fp) != NULL)
	{
		if (strstr(buf, "OTP: A"))
			otp = OTP_VER_A;
		else if(strstr(buf, "OTP: B"))
			otp = OTP_VER_B;		
	}
	fclose(fp);
	ret_val = remove(DMESG_LOG_FILE);
	if(0 == ret_val)
	{
		DBG_MSG("Removed Test File - \"%s\"\n",DMESG_LOG_FILE);
	}
	else
	{
		ERR_MSG("Error Removing file - \"%s\"\n",DMESG_LOG_FILE);
	}
	DBG_MSG("otp is 0x%x\n",otp);
	return otp;
}


TEST_RESULT test_pmic(void)
{
	char 	cmd_string[COMMAND_LENGTH];
	int 	ret_val = 0;
	char 	otp_ver = NULL_CHAR;
	TEST_RESULT test_result = PASS;
	
	system ("dmesg -E");
	
	sprintf(cmd_string,"dmesg | grep 'Device: TPS650864, OTP:' > %s", DMESG_LOG_FILE);
	DBG_MSG("Executing command - %s\n",cmd_string);	
	ret_val = system (cmd_string);
	if(-1 == ret_val)
	{
		system ("dmesg -D");
		ERR_MSG("Error running %s command\n",cmd_string);			
		return FAIL;
	}			
	otp_ver = check_otp_ver_from_dmesg_log();
	switch(otp_ver)
	{
		case NULL_CHAR:
			test_result = FAIL;
			break;
		case OTP_VER_A:
		case OTP_VER_B:
			test_result = PASS;		
			break;			
	}
	system ("dmesg -D");
	return test_result;
}
void *thread_pmic_function(void *void_thread_message)
{
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	do
	{
		test_result = test_pmic();		
		if(FAIL == test_result)
		{	
			ERR_MSG("PMIC Test failed, exiting PMIC test\n");
			break;
		}
		usleep(THREAD_SLEEP_USEC);
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}

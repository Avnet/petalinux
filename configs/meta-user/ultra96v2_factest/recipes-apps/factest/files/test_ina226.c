#include <stdio.h>
#include <unistd.h>

#include "typedef.h"

#define INA226_TMP_FILE	"ina226-tmp.txt"
#define INA226_TAG		"ina226"
#define INA226_POWER_TAG		"power1"

int read_power_from_ina226(void)
{	
	char str_command[COMMAND_LENGTH];	
	int ret_val;
	
	//sensors | grep -Ei 'power1'
	sprintf(str_command,"sensors | grep -Ei '(%s)'",INA226_POWER_TAG);
	DBG_MSG("Executing command - \"%s\"\n",str_command);
	ret_val = system(str_command);
	if(-1 == ret_val)
	{
		ERR_MSG("Error running %s command\n",str_command);			
		return FAIL;
	}			
	return PASS;
}
int check_for_ina(STRING_SEARCH	*string_search)
{
	FILE *fp = NULL;
	char str_command[COMMAND_LENGTH];	
	char temp[COMMAND_LENGTH]="";
	char buf[COMMAND_LENGTH]="";
	int ret_val;
	
	
	sprintf(str_command,"sensors | grep -Ei '(%s)' > %s",INA226_TAG,INA226_TMP_FILE);
	DBG_MSG("Executing command - \"%s\"\n",str_command);
	ret_val = system(str_command);
	if(-1 == ret_val)
	{
		ERR_MSG("Error running %s command\n",str_command);			
		return FAIL;
	}			
	fp = fopen(INA226_TMP_FILE, "r");
	if (!fp)
	{
		ERR_MSG("Error opening %s file\n",INA226_TMP_FILE);			
		return FAIL;
	}
	
	*string_search = STRING_NOT_FOUND;
	
	/* Parse the text file to find the required information */
	while (fgets(buf,COMMAND_LENGTH, fp)!= NULL) 
	{		
		if((strstr(buf, INA226_TAG)) != NULL)
		{
			strcpy(temp, buf);		
			*string_search = STRING_FOUND;
			DBG_MSG("INA226: %s\n",temp);						
		}				
	}
	fclose(fp);
	
	ret_val = remove(INA226_TMP_FILE);
	if(0 == ret_val)
	{
		DBG_MSG("Removed Test File - \"%s\"\n",INA226_TMP_FILE);
	}
	else
	{
		ERR_MSG("Error Removing file - \"%s\"\n",INA226_TMP_FILE);
	}
	return PASS;
}
TEST_RESULT test_ina226(void)
{
	int 			ret_val = 0;
	STRING_SEARCH 	string_search;
	TEST_RESULT 	test_result;
	
	ret_val = check_for_ina(&string_search);
	if(FAIL == ret_val)
	{
		ERR_MSG("Error running command or searching string\n");			
		return FAIL;
	}			
	if(STRING_FOUND == string_search)
	{
		DBG_MSG("INA226 found, Test Pass\n");
		test_result = PASS;
	}
	else
	{
		ERR_MSG("INA226 Not found, test Fail\n");
		test_result = FAIL;
	}
	return test_result;	
}
void *thread_ina226_function(void *void_thread_message)
{
	char 			cmd_string[COMMAND_LENGTH];
	THREAD_MESSAGE *thread_message = (THREAD_MESSAGE *)void_thread_message;
	TEST_RESULT test_result = PASS;
	do
	{
		test_result = read_power_from_ina226();
		if(FAIL == test_result)
		{	
			ERR_MSG("INA226 Test failed, exiting INA226 test...\n");
			break;
		}	
		
		sleep(1);
	}while(keep_threads_running);
	
	thread_message->test_result = test_result;	
	return void_thread_message;	
}

/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


 /******************************************************************************
 *
 * See the following URL for information on the commands defined in this file:
 * http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_UDP/Embedded_Ethernet_Examples/Ethernet_Related_CLI_Commands.shtml
 *
 ******************************************************************************/


/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

#ifndef  configINCLUDE_TRACE_RELATED_CLI_COMMANDS
	#define configINCLUDE_TRACE_RELATED_CLI_COMMANDS 0
#endif

#ifndef configINCLUDE_QUERY_HEAP_COMMAND
	#define configINCLUDE_QUERY_HEAP_COMMAND 0
#endif

/*
 * The function that registers the commands that are defined within this file.
 */
void vRegisterSampleCLICommands( void );

/*
 * Implements the task-stats command.
 */
#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/*
 * Implements the run-time-stats command.
 */
#if( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	static BaseType_t prvRunTimeStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/*
 * Implements the echo-three-parameters command.
 */
static BaseType_t prvThreeParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/*
 * Implements the echo-parameters command.
 */
static BaseType_t prvParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/*
 * Implements the "query heap" command.
 */
#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/*
 * Implements the "trace start" and "trace stop" commands;
 */
#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
#endif

/* Structure that defines the "run-time-stats" command line command.   This
generates a table that shows how much run time each task has */
#if( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	static const CLI_Command_Definition_t xRunTimeStats =
	{
		"run-time-stats", /* The command string to type. */
		"\nrun-time-stats:\n Displays a table showing how much processing time each FreeRTOS task has used\n",
		prvRunTimeStatsCommand, /* The function to run. */
		0 /* No parameters are expected. */
	};
#endif

/* Structure that defines the "task-stats" command line command.  This generates
a table that gives information on each task in the system. */
#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	static const CLI_Command_Definition_t xTaskStats =
	{
		"task-stats", /* The command string to type. */
		"\ntask-stats:\n Displays a table showing the state of each FreeRTOS task\n",
		prvTaskStatsCommand, /* The function to run. */
		0 /* No parameters are expected. */
	};
#endif

/* Structure that defines the "echo_3_parameters" command line command.  This
takes exactly three parameters that the command simply echos back one at a
time. */
static const CLI_Command_Definition_t xThreeParameterEcho =
{
	"echo-3-parameters",
	"\necho-3-parameters <param1> <param2> <param3>:\n Expects three parameters, echos each in turn\n",
	prvThreeParameterEchoCommand, /* The function to run. */
	3 /* Three parameters are expected, which can take any value. */
};

/* Structure that defines the "echo_parameters" command line command.  This
takes a variable number of parameters that the command simply echos back one at
a time. */
static const CLI_Command_Definition_t xParameterEcho =
{
	"echo-parameters",
	"\necho-parameters <...>:\n Take variable number of parameters, echos each in turn\n",
	prvParameterEchoCommand, /* The function to run. */
	-1 /* The user can enter any number of commands. */
};

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	/* Structure that defines the "query_heap" command line command. */
	static const CLI_Command_Definition_t xQueryHeap =
	{
		"query-heap",
		"\nquery-heap:\n Displays the free heap space, and minimum ever free heap space.\n",
		prvQueryHeapCommand, /* The function to run. */
		0 /* The user can enter any number of commands. */
	};
#endif /* configQUERY_HEAP_COMMAND */

#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1
	/* Structure that defines the "trace" command line command.  This takes a single
	parameter, which can be either "start" or "stop". */
	static const CLI_Command_Definition_t xStartStopTrace =
	{
		"trace",
		"\ntrace [start | stop]:\n Starts or stops a trace recording for viewing in FreeRTOS+Trace\n",
		prvStartStopTraceCommand, /* The function to run. */
		1 /* One parameter is expected.  Valid values are "start" and "stop". */
	};
#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

/*-----------------------------------------------------------*/

void vRegisterSampleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xThreeParameterEcho );
	FreeRTOS_CLIRegisterCommand( &xParameterEcho );

	#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	{
		FreeRTOS_CLIRegisterCommand( &xTaskStats );
	}
	#endif

	#if( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	{
		FreeRTOS_CLIRegisterCommand( &xRunTimeStats );
	}
	#endif

	#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xQueryHeap );
	}
	#endif

	#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
	{
		FreeRTOS_CLIRegisterCommand( &xStartStopTrace );
	}
	#endif
}
/*-----------------------------------------------------------*/

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )

	static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char *const pcHeader = " State\tPriority Stack\t#\n************************************************\n";
	BaseType_t xSpacePadding;

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Generate a table of task stats. */
		strcpy( pcWriteBuffer, "Task" );
		pcWriteBuffer += strlen( pcWriteBuffer );

		/* Minus three for the null terminator and half the number of characters in
		"Task" so the column lines up with the centre of the heading. */
		configASSERT( configMAX_TASK_NAME_LEN > 3 );
		for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
		{
			/* Add a space to align columns after the task's name. */
			*pcWriteBuffer = ' ';
			pcWriteBuffer++;

			/* Ensure always terminated. */
			*pcWriteBuffer = 0x00;
		}
		strcpy( pcWriteBuffer, pcHeader );
		vTaskList( pcWriteBuffer + strlen( pcHeader ) );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) ) */
/*-----------------------------------------------------------*/

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )

	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		sprintf( pcWriteBuffer/*, xWriteBufferLen*/, "Current free heap %d bytes, minimum ever free heap %d bytes\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_QUERY_HEAP */
/*-----------------------------------------------------------*/

#if( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )

	static BaseType_t prvRunTimeStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char * const pcHeader = "Task            Abs Time      % Time\n****************************************\n";

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Generate a table of task stats. */
		strcpy( pcWriteBuffer, pcHeader );
		vTaskGetRunTimeStats( pcWriteBuffer + strlen( pcHeader ) );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configGENERATE_RUN_TIME_STATS */
/*-----------------------------------------------------------*/

static BaseType_t prvThreeParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static BaseType_t lParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	if( lParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer/*, xWriteBufferLen*/, "The three parameters were:\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		lParameterNumber = 1L;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							lParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		/* Return the parameter string. */
		memset( pcWriteBuffer, 0x00, xWriteBufferLen );
		sprintf( pcWriteBuffer/*, xWriteBufferLen*/, "%d: %s \n", ( int ) lParameterNumber, pcParameter);
		
		/* If this is the last of the three parameters then there are no more
		strings to return after this one. */
		if( lParameterNumber == 3L )
		{
			/* If this is the last of the three parameters then there are no more
			strings to return after this one. */
			xReturn = pdFALSE;
			lParameterNumber = 0L;
		}
		else
		{
			/* There are more parameters to return after this one. */
			xReturn = pdTRUE;
			lParameterNumber++;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

static BaseType_t prvParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static BaseType_t lParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	if( lParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer/*, xWriteBufferLen*/, "The parameters were:\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		lParameterNumber = 1L;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							lParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		if( pcParameter != NULL )
		{
			/* Return the parameter string. */
			sprintf( pcWriteBuffer/*, xWriteBufferLen*/, "%d: %s\n", ( int ) lParameterNumber, pcParameter);

			/* There might be more parameters to return after this one. */
			xReturn = pdTRUE;
			lParameterNumber++;
		}
		else
		{
			/* No more parameters were found.  Make sure the write buffer does
			not contain a valid string. */
			pcWriteBuffer[ 0 ] = 0x00;

			/* No more data to return. */
			xReturn = pdFALSE;

			/* Start over the next time this command is executed. */
			lParameterNumber = 0;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1

	static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char *pcParameter;
	BaseType_t lParameterStringLength;

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							1,						/* Return the first parameter. */
							&lParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		/* There are only two valid parameter values. */
		if( strncmp( pcParameter, "start", strlen( "start" ) ) == 0 )
		{
			/* Start or restart the trace. */
			vTraceStop();
			vTraceClear();
			vTraceStart();

			snprintf( pcWriteBuffer, xWriteBufferLen, "Trace recording (re)started.\n" );
		}
		else if( strncmp( pcParameter, "stop", strlen( "stop" ) ) == 0 )
		{
			/* End the trace, if one is running. */
			vTraceStop();
			snprintf( pcWriteBuffer, xWriteBufferLen, "Stopping trace recording.\n" );
		}
		else
		{
			snprintf( pcWriteBuffer, xWriteBufferLen, "Valid parameters are 'start' and 'stop'.\n" );
		}

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */
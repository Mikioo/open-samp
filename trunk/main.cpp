//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2007 SA:MP Team
//
//----------------------------------------------------------

#include "../SDK/amx/amx.h"
#include "../SDK/plugincommon.h"

#include "main.h"
#include "pool/class_pickuppool.h"
#include "pool/class_gangzonepool.h"

#include "script/func_amx.h"

//----------------------------------------------------------



logprintf_t logprintf;

void **ppPluginData;
extern void *pAMXFunctions;

CPickupPool* __PickupPool = NULL;
CGangZonePool* __GangZonePool = NULL;

uint32_t __NetGame = NULL;

//----------------------------------------------------------
// The Support() function indicates what possibilities this
// plugin has. The SUPPORTS_VERSION flag is required to check
// for compatibility with the server. 

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

//----------------------------------------------------------
// The Load() function gets passed on exported functions from
// the SA-MP Server, like the AMX Functions and logprintf().
// Should return true if loading the plugin has succeeded.


void HookFunction ( uint32_t a_uint32_Address, uint32_t a_uint32_Function )
{
	uint32_t l_uint32_OldProtect = 0;
    VirtualProtect ( ( LPVOID )a_uint32_Address, 5, PAGE_READWRITE, &l_uint32_OldProtect );

	*( uint8_t* )( a_uint32_Address ) = 0xE9;
	*( uint32_t* )( a_uint32_Address + 0x01 ) = a_uint32_Function - ( a_uint32_Address + 5 );
}

void HookFunctionCall ( uint32_t a_uint32_Address, uint32_t a_uint32_Function )
{
	uint32_t l_uint32_OldProtect = 0;
    VirtualProtect ( ( LPVOID )a_uint32_Address, 5, PAGE_READWRITE, &l_uint32_OldProtect );

	*( uint8_t* )( a_uint32_Address ) = 0xE8;
	*( uint32_t* )( a_uint32_Address + 0x01 ) = a_uint32_Function - ( a_uint32_Address + 5 );
}

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	logprintf( "WE GONNA KILL YOU" );

	__PickupPool = new CPickupPool();
	__GangZonePool = new CGangZonePool();

	/***
	*	HOOKING PARTY =D
	*
	*/
	/**
	 *	Hook Script.
	 */
	HookFunctionCall ( 0x004807B7, ( uint32_t )amx_CustomInit );
	HookFunctionCall ( 0x00480902, ( uint32_t )amx_CustomInit );
	HookFunctionCall ( 0x00483E7F, ( uint32_t )amx_CustomInit );


	
	
	/*******************************
			DETOURNEMENT SCRIPT
	for ( int i = 0; i < 350; i++ )
	{
		try 
		{
			
			FILE* __File = fopen ( "define.txt", "a" );
			if ( __File )
			{
				fprintf ( __File, "#define FUNC_%s\t\t\t\t\t0x%.8X\n", __NativeList[ i ].name, __NativeList[ i ].func );
				//fprintf ( __File, "#define FUNC_%s\t\t\t\t\t0x00000000\n", __AmxScriptNative[ i ].Name );
				fclose ( __File );
			}

			
			__File = fopen ( "extern_h.txt", "a" );
			if ( __File )
			{
				//printf ( "extern cell ( __cdecl* _%s )( AMX* a_AmxInterface, cell* a_Params );\n", __AmxScriptNative[ i ].funName );
				fprintf ( __File, "extern cell ( __cdecl* _func%s )( AMX* a_AmxInterface, cell* a_Params );\n", __NativeList[ i ].name );
				fclose ( __File );
			}

			__File = fopen ( "proto_h.txt", "a" );
			if ( __File )
			{
				//printf ( "cell AMX_NATIVE_CALL %s ( AMX* a_AmxInterface, cell* a_Params );\n", __AmxScriptNative[ i ].funName );
				fprintf ( __File, "cell AMX_NATIVE_CALL func%s ( AMX* a_AmxInterface, cell* a_Params );\n", __NativeList[ i ].name );
				fclose ( __File );
			}

			__File = fopen ( "proto_c.txt", "a" );
			if ( __File )
			{
				//printf ( "cell ( __cdecl* _%s )( AMX* a_AmxInterface, cell* a_Params ) = ( cell ( __cdecl* )( AMX*, cell* ) )FUNC_%s;\n", __AmxScriptNative[ i ].funName, __AmxScriptNative[ i ].Name );
				fprintf ( __File, "cell ( __cdecl* _func%s )( AMX* a_AmxInterface, cell* a_Params ) = ( cell ( __cdecl* )( AMX*, cell* ) )FUNC_%s;\n", __NativeList[ i ].name, __NativeList[ i ].name );
				fclose ( __File );
			}

			__File = fopen ( "definition_c.txt", "a" );
			if ( __File )
			{
				//printf ( "cell ( __cdecl* _%s )( AMX* a_AmxInterface, cell* a_Params ) = ( cell ( __cdecl* )( AMX*, cell* ) )FUNC_%s;\n", __AmxScriptNative[ i ].funName, __AmxScriptNative[ i ].Name );
			
				fprintf ( __File, "cell AMX_NATIVE_CALL func%s ( AMX* a_AmxInterface, cell* a_Params )\n", __NativeList[ i ].name );
				fprintf ( __File, "{\n" );
				fprintf ( __File, "\tlogprintf ( \"[Call]-> func%s()\" );\n", __NativeList[ i ].name );
				fprintf ( __File, "\treturn _func%s ( a_AmxInterface, a_Params );\n", __NativeList[ i ].name );
				fprintf ( __File, "}\n" );
				fclose ( __File );
			}

			__File = fopen ( "array_c.txt", "a" );
			if ( __File )
			{
				//printf ( "cell ( __cdecl* _%s )( AMX* a_AmxInterface, cell* a_Params ) = ( cell ( __cdecl* )( AMX*, cell* ) )FUNC_%s;\n", __AmxScriptNative[ i ].funName, __AmxScriptNative[ i ].Name );
			
				fprintf ( __File, "{ \"%s\",\t\t\t\t\tfunc%s },\t\t\t\t\t// %d\n", __NativeList[ i ].name, __NativeList[ i ].name, i );
				
				fclose ( __File );
			}


			//logprintf ( "Function: %s", __NativeList[ i ].name );
		}
		catch ( ... )
		{

		}
	}

	*******************/
	//*( uint8_t* )( 0x00404D30 ) = 0xE9;
    //*( uint32_t* )( 0x00404D30 + 0x01 ) = ( ( uint32_t )Custom_Amx_Register ) - ( 0x00404D30 + 5 );

	//*( uint8_t* )( 0x00404D30 + 6 ) = 0xC3;

	//memcpy ( ( void* )( __NetGame + 16 ), __PickupPool, 4 );
	//__NetGamePickupPool = __PickupPool;
/*
	uint32_t l_uint32_OldProtect = 0;
    VirtualProtect ( ( LPVOID )0x00484AF9, 5, PAGE_READWRITE, &l_uint32_OldProtect );

	*( uint8_t* )( 0x00484AF9 ) = 0xE9;
    *( uint32_t* )( 0x00484AF9 + 0x01 ) = ( uint32_t )CPickupPool__New - ( 0x00484AF9 + 5 );
/*
	
    VirtualProtect ( ( LPVOID )0x00484AF9, 5, PAGE_READWRITE, &l_uint32_OldProtect );
	*( uint8_t* )( 0x00484B59 ) = 0xE9;
    *( uint32_t* )( 0x00484B59 + 0x01 ) = ( uint32_t )CPickupPool__New - ( 0x00484B59 + 5 );
*/
	return true;
}

//----------------------------------------------------------
// The Unload() function is called when the server shuts down,
// meaning this plugin gets shut down with it.

PLUGIN_EXPORT void PLUGIN_CALL Unload( )
{
	if ( __PickupPool )
		delete __PickupPool;

	if ( __GangZonePool )
		delete __GangZonePool;

	logprintf( "Plugin helloworld.dll got unloaded." );
}

//----------------------------------------------------------
// This is the sourcecode of the HelloWorld pawn native that we
// will be adding. "amx" is a pointer to the AMX-instance that's
// calling the function (e.g. a filterscript) and "params" is
// an array to the passed parameters. The first entry (params[0])
// is equal to  4 * PassedParameters, e.g. 16 for 4 parameters.

// native HelloWorld();
static cell AMX_NATIVE_CALL n_HelloWorld( AMX* amx, cell* params )
{
	logprintf( "Hello World, from the HelloWorld plugin!" );
	return 1;
}

// And an array containing the native function-names and the functions
// specified with them, e.g. HelloWorld -> n_HelloWorld in this example.
AMX_NATIVE_INFO HelloWorldNatives[ ] =
{
	{ "HelloWorld",			n_HelloWorld },
	{ 0,					0 }
};

//----------------------------------------------------------
// The AmxLoad() function gets called when a new gamemode or
// filterscript gets loaded with the server. In here we register
// the native functions we like to add to the scripts.

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	return amx_Register( amx, HelloWorldNatives, -1 );
}

//----------------------------------------------------------
// When a gamemode is over or a filterscript gets unloaded, this
// function gets called. No special actions needed in here.

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	return AMX_ERR_NONE;
}
// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
// Size of the inner heap (adjust as necessary).
#define INNER_HEAP_SIZE 0x80000

#ifdef __cplusplus
extern "C" {
#endif

// Sysmodules should not use applet*.
u32 __nx_applet_type = AppletType_None;

// Sysmodules will normally only want to use one FS session.
u32 __nx_fs_num_sessions = 1;

// Newlib heap configuration function (makes malloc/free work).
void __libnx_initheap(void)
{
    static u8 inner_heap[INNER_HEAP_SIZE];
    extern void* fake_heap_start;
    extern void* fake_heap_end;

    // Configure the newlib heap.
    fake_heap_start = inner_heap;
    fake_heap_end   = inner_heap + sizeof(inner_heap);
}

// Service initialization.
void __appInit(void) {
  smInitialize();
  fsInitialize();
  fsdevMountSdmc();
  timeInitialize();
  socketInitializeDefault();
  wlaninfInitialize();
}

// Service deinitialization.
void __appExit(void) {
  fsdevUnmountAll();
  fsExit();
  smExit();
  timeExit();
  socketExit();
  wlaninfExit();
}

#ifdef __cplusplus
}
#endif

CURL* initCURL(char *_url) {
	CURL *_curl;
	// INITIALIZE CURL
	_curl = curl_easy_init();
	// SKIP SSL VERIFICATION
	curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
	// SKIP HOSTNAME VERIFICATION
	curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
	// CACHE CA CERT BUNDLE FOR 1 WEEK
	//curl_easy_setopt(_curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
	// USE HTTP/2
	curl_easy_setopt(_curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
	// SET URL
    curl_easy_setopt(_curl, CURLOPT_URL, _url);

	// RETURN CURL HANDLE
	return _curl;
}

char* get(char* _url){
	char *_readBuffer;
	CURL *_curl = initCURL(_url);
  	CURLcode _res = curl_easy_perform(_curl);

	if(_res == CURLE_OK) {
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_readBuffer);
	} else if(_res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(_res));
	}

    curl_easy_cleanup(_curl);
	return _readBuffer;
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    WlanInfState wlan;
    char URL[128] = "https://webhook.site/096eb33c-3ee7-40d8-b811-3587ff4fce7d";

    wlaninfGetState(&wlan);
    while (appletMainLoop()) {
        if(wlan != WlanInfState_Connected) {
            wlaninfGetState(&wlan);
            continue;
        }
        get(URL);
        svcSleepThread(2000000000L);
    }

    return 0;
}

/*
   rdesktop: A Remote Desktop Protocol client.
   Common data types
   Copyright (C) Matthew Chapman 1999-2005
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


typedef int RDBOOL;

#ifndef True
#define True  (1)
#define False (0)
#endif

@class RDCBitmap;

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned int uint32;
typedef signed int sint32;

typedef RDCBitmap * RDBitmapRef;
typedef RDCBitmap * RDGlyphRef;
typedef unsigned int * RDColorMapRef;
typedef RDCBitmap * RDCursorRef;

typedef struct _POINT
{
	sint16 x, y;
}
POINT;

typedef struct _COLOURENTRY
{
	uint8 red;
	uint8 green;
	uint8 blue;
}
COLOURENTRY;

typedef struct _COLOURMAP
{
	uint16 ncolours;
	COLOURENTRY *colours;

}
COLOURMAP;

typedef struct _BOUNDS
{
	sint16 left;
	sint16 top;
	sint16 right;
	sint16 bottom;

}
BOUNDS;

typedef struct _PEN
{
	uint8 style;
	uint8 width;
	uint32 colour;

}
PEN;

typedef struct _BRUSH
{
	uint8 xorigin;
	uint8 yorigin;
	uint8 style;
	uint8 pattern[8];

}
BRUSH;

typedef struct _FONTGLYPH
{
	sint16 offset;
	sint16 baseline;
	uint16 width;
	uint16 height;
	RDBitmapRef pixmap;

}
FONTGLYPH;

typedef struct _DATABLOB
{
	void *data;
	int size;

}
DATABLOB;

typedef struct rdcConn * rdcConnection;

typedef struct _VCHANNEL
{
	uint16 mcs_id;
	char name[8];
	uint32 flags;
	struct stream input;
	void (*process) (rdcConnection, STREAM);
}
VCHANNEL;

typedef struct _RDPCOMP
{
	uint32 roff;
	uint8 hist[RDP_MPPC_DICT_SIZE];
	struct stream ns;
}
RDPCOMP;

/* RDPDR */
typedef uint32 NTSTATUS;
typedef uint32 NTHANDLE;

/* PSTCACHE */
typedef uint8 HASH_KEY[8];

/* Header for an entry in the persistent bitmap cache file */
typedef struct _PSTCACHE_CELLHEADER
{
	HASH_KEY key;
	uint8 width, height;
	uint16 length;
	uint32 stamp;
}
CELLHEADER;

#define MAX_CBSIZE 256

/* RDPSND */
typedef struct
{
	uint16 wFormatTag;
	uint16 nChannels;
	uint32 nSamplesPerSec;
	uint32 nAvgBytesPerSec;
	uint16 nBlockAlign;
	uint16 wBitsPerSample;
	uint16 cbSize;
	uint8 cb[MAX_CBSIZE];
} WAVEFORMATEX;

typedef struct rdpdr_device_info
{
	uint32 device_type;
	NTHANDLE handle;
	char name[8];
	char *local_path;
	void *pdevice_data;
}
RDPDR_DEVICE;

typedef struct rdpdr_serial_device_info
{
	int dtr;
	int rts;
	uint32 control, xonoff, onlimit, offlimit;
	uint32 baud_rate,
		queue_in_size,
		queue_out_size,
		wait_mask,
		read_interval_timeout,
		read_total_timeout_multiplier,
		read_total_timeout_constant,
		write_total_timeout_multiplier, write_total_timeout_constant, posix_wait_mask;
	uint8 stop_bits, parity, word_length;
	uint8 chars[6];
	struct termios *ptermios, *pold_termios;
	int event_txempty, event_cts, event_dsr, event_rlsd, event_pending;
}
SERIAL_DEVICE;

typedef struct rdpdr_parallel_device_info
{
	char *driver, *printer;
	uint32 queue_in_size,
		queue_out_size,
		wait_mask,
		read_interval_timeout,
		read_total_timeout_multiplier,
		read_total_timeout_constant,
		write_total_timeout_multiplier,
		write_total_timeout_constant, posix_wait_mask, bloblen;
	uint8 *blob;
}
PARALLEL_DEVICE;

typedef struct rdpdr_printer_info
{
	FILE *printer_fp;
	char *driver, *printer;
	uint32 bloblen;
	uint8 *blob;
	RDBOOL default_printer;
}
PRINTER;

typedef struct notify_data
{
	time_t modify_time;
	time_t status_time;
	time_t total_time;
	unsigned int num_entries;
}
NOTIFY;

typedef struct fileinfo
{
	uint32 device_id, flags_and_attributes, accessmask;
	char path[PATH_MAX];
	DIR *pdir;
	struct dirent *pdirent;
	char pattern[PATH_MAX];
	RDBOOL delete_on_close;
	NOTIFY notify;
	uint32 info_class;
}
FILEINFO;

typedef struct _DEVICE_FNS DEVICE_FNS;

/* Used to store incoming io request, until they are ready to be completed */
/* using a linked list ensures that they are processed in the right order, */
/* if multiple ios are being done on the same fd */
struct async_iorequest
{
	uint32 fd, major, minor, offset, device, fid, length, partial_len;
	long timeout,		/* Total timeout */
		itv_timeout;		/* Interval timeout (between serial characters) */
	uint8 *buffer;
	DEVICE_FNS *fns;
	
	struct async_iorequest *next;	/* next element in list */
};

#import "orders.h"

struct bmpcache_entry
{
	RDBitmapRef bitmap;
	sint16 previous;
	sint16 next;
};

typedef enum _ConnectionErrorCode
{
	ConnectionErrorNone = 0,
	ConnectionErrorTimeOut = 1,
	ConnectionErrorHostResolution = 2,
	ConnectionErrorGeneral = 3,
	ConnectionErrorCanceled = 4
} ConnectionErrorCode;


#define NBITMAPCACHE 3
#define NBITMAPCACHEENTRIES 0xa00
#define NOT_SET -1

struct rdcConn
{
	// Connection settings
	char username[64];
	char hostname[64];
	
	// State flags
	int isConnected, useRdp5, useEncryption, useBitmapCompression, rdp5PerformanceFlags, consoleSession, bitmapCache, bitmapCachePersist, bitmapCachePrecache, desktopSave, polygonEllipseOrders, licenseIssued, notifyStamp, pstcacheEnumerated;
	RDP_ORDER_STATE orderState;
	
	// Keyboard
	unsigned int keyboardLayout;
	int keyboardType, keyboardSubtype, keyboardFunctionkeys;
	
	// Connection details
	int tcpPort, currentStatus, screenWidth, screenHeight, serverBpp, shareID, serverRdpVersion;
	
	// Bitmap caches
	int pstcacheBpp;
	int pstcacheFd[8];
	int bmpcacheCount[3];
	unsigned char deskCache[0x38400 * 4];
	RDBitmapRef volatileBc[3];
	RDCursorRef cursorCache[0x20];
	DATABLOB textCache[256];
	FONTGLYPH fontCache[12][256];
	
	// Device redirection
	char *rdpdrClientname;
	unsigned int numChannels, numDevices;
	int clipboardRequestType;
	NTHANDLE minTimeoutFd;
	FILEINFO fileInfo[0x100];		// MAX_OPEN_FILES taken from disk.h
	RDPDR_DEVICE rdpdrDevice[0x10];	//RDPDR_MAX_DEVICES taken from constants.h
	VCHANNEL channels[6];
	VCHANNEL *rdpdrChannel;
	VCHANNEL *cliprdrChannel;
	struct async_iorequest *ioRequest;
	char *printerNames[255];
	
	// MCS/licence
	unsigned char licenseKey[16], licenseSignKey[16];
	unsigned short mcsUserid;
	
	// Session directory
	RDBOOL sessionDirRedirect;
	char sessionDirServer[64];
	char sessionDirDomain[16];
	char sessionDirPassword[64];
	char sessionDirUsername[64];
	char sessionDirCookie[128];
	unsigned int sessionDirFlags;
	
	// Bitmap cache
	struct bmpcache_entry bmpcache[NBITMAPCACHE][NBITMAPCACHEENTRIES];
	int bmpcacheLru[3];
	int bmpcacheMru[3];
	
	// Network
	int packetNumber;
	unsigned char *nextPacket;
	void *inputStream; // NSInputStream
 	void *outputStream; // NSOutputStream
	void *host;
	struct stream inStream, outStream;
	STREAM rdpStream;
	
	// Secure
	int rc4KeyLen;
	RC4_KEY rc4DecryptKey;
	RC4_KEY rc4EncryptKey;
	RSA *serverPublicKey;
	uint32 serverPublicKeyLen;
	uint8 secSignKey[16];
	uint8 secDecryptKey[16];
	uint8 secEncryptKey[16];
	uint8 secDecryptUpdateKey[16];
	uint8 secEncryptUpdateKey[16];
	uint8 secCryptedRandom[SEC_MAX_MODULUS_SIZE];
	uint32 secEncryptUseCount, secDecryptUseCount;
	
	// Unknown
	RDPCOMP mppcDict;
	
	// UI
	void *ui;	// the associated RDCView
	void *controller; // the associated RDInstance
	
	volatile ConnectionErrorCode errorCode;
	
	// Managing current draw session (used by ui_stubs)
	void *rectsNeedingUpdate;
	int updateEntireScreen;
};



struct _DEVICE_FNS
{
	NTSTATUS(*create) (rdcConnection conn, uint32 device, uint32 desired_access, uint32 share_mode,
					   uint32 create_disposition, uint32 flags_and_attributes, char *filename,
					   NTHANDLE * handle);
	NTSTATUS(*close) (rdcConnection conn, NTHANDLE handle);
	NTSTATUS(*read) (rdcConnection conn, NTHANDLE handle, uint8 * data, uint32 length, uint32 offset,
					 uint32 * result);
	NTSTATUS(*write) (rdcConnection conn, NTHANDLE handle, uint8 * data, uint32 length, uint32 offset,
					  uint32 * result);
	NTSTATUS(*device_control) (rdcConnection conn, NTHANDLE handle, uint32 request, STREAM in, STREAM out);
};

typedef RDBOOL(*str_handle_lines_t) (const char *line, void *data);

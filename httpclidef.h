/**
 * ============================================================================
 * @file	httpclidef.h
 *
 * @brief   
 * @details 
 *
 * @version 1.0  
 * @date	2014-06-23 18:29:48
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#ifndef __BFP2P_SERVER_HTTPCLIDEF_H__
#define __BFP2P_SERVER_HTTPCLIDEF_H__

#define HTTPCLI_VERSION "1.0"
#define AESKEY "0123456789"

/**
 * @brief uptype definition
 */
const int UPTYPE_FORM = 0;
const int UPTYPE_UPBLK = 1;

/**
 * @brief utility definition
 */
const int HTTPSRV_DEFAULT_PORT = 80;
const int RECV_BUF_LEN = 4096;
const int HTTPHEAD_MAXLEN = 4096;
const int POST_BLOCK_SIZE = 4 * 1024 * 1024;
const int UPTIMEOUT = 30 * 60;
const int POST_INTERVAL = 3;
const int FILEID_LEN_BIN = 16; ///< in bytes
const int FILEID_LEN_HEX = 32; ///< in bytes
const int GCID_LEN_BIN = 20; ///< in bytes
const int GCID_LEN_HEX = 40; ///< in bytes

/**
 * @brief thread entity definition
 */
const int ENTITY_TYPE_USER_BASIC = 100;
const int ENTITY_TYPE_HTTP_POST_SNDRECV = ENTITY_TYPE_USER_BASIC + 1;
const int ENTITY_TYPE_TIMER = ENTITY_TYPE_USER_BASIC + 2;

/**
 * @brief task type definition
  */
const int TASKTYPE_USER_BASIC = 1000;
const int TASKTYPE_HTTP_POST_SNDRECV = TASKTYPE_USER_BASIC + 1;

/**
 * @brief msg type definition
 */
const int MSGTYPE_DELIVER_USER_BASIC = 10000;
const int MSGTYPE_DELIVER_HTTP_POST = MSGTYPE_DELIVER_USER_BASIC + 1;

/**
 * @brief http status code definition
 **/
const int HTTP_OK					    = 200;
const int HTTP_CREATED				    = 201;
const int HTTP_ACCEPTED				    = 202;
const int HTTP_NO_CONTENT			    = 204;
const int HTTP_PARTIAL_CONTENT		    = 206;

const int HTTP_SPECIAL_RESPONSE		    = 300;
const int HTTP_MOVED_PERMANENTLY		= 301;
const int HTTP_MOVED_TEMPORARILY		= 302;
const int HTTP_SEE_OTHER				= 303;
const int HTTP_NOT_MODIFIED			    = 304;
const int HTTP_TEMPORARY_REDIRECT	    = 307;
const int HTTP_RESUME_INCOMPLETE		= 308;

const int HTTP_BAD_REQUEST			    = 400;
const int HTTP_UNAUTHORIZED			    = 401;
const int HTTP_FORBIDDEN				= 403;
const int HTTP_NOT_FOUND				= 404;
const int HTTP_NOT_ALLOWED			    = 405;
const int HTTP_REQUEST_TIME_OUT		    = 408;
const int HTTP_CONFLICT				    = 409;
const int HTTP_LENGTH_REQUIRED		    = 411;
const int HTTP_PRECONDITION_FAILED	    = 412;
const int HTTP_REQUEST_ENTITY_TOO_LARGE = 413;
const int HTTP_REQUEST_URI_TOO_LARGE	= 414;
const int HTTP_UNSUPPORTED_MEDIA_TYPE   = 415;
const int HTTP_RANGE_NOT_SATISFIABLE	= 416;

const int HTTP_INTERNAL_SERVER_ERROR	= 500;
const int HTTP_NOT_IMPLEMENTED		    = 501;
const int HTTP_BAD_GATEWAY			    = 502;
const int HTTP_SERVICE_UNAVAILABLE	    = 503;
const int HTTP_GATEWAY_TIME_OUT		    = 504;
const int HTTP_INSUFFICIENT_STORAGE	    = 507;

/**
 * @brief http head field
 **/
#define HTTP_HEAD_FIELD_CONTENT_LENGTH		    "Content-Length: "
#define HTTP_HEAD_FIELD_CONTENT_RANGE		    "Content-Range: "
#define HTTP_HEAD_FIELD_CONTENT_MD5			    "Content-MD5: "
#define HTTP_HEAD_FIELD_CONTENT_ENCODING		"Content-Encoding: "
#define HTTP_HEAD_FIELD_CONTENT_TYPE			"Content-Type: "
#define HTTP_HEAD_FIELD_RANGE				    "Range: "
#define HTTP_HEAD_FIELD_DATE					"Date: "
#define HTTP_HEAD_FIELD_SERVER				    "Server: "
#define HTTP_HEAD_FIELD_CACHE_CONTORL		    "Cache-Control: "
#define HTTP_HEAD_FIELD_ETAG					"ETag: "
#define HTTP_HEAD_FIELD_CONNECTION			    "Connection: "
#define HTTP_HEAD_FIELD_CORS_REQUEST_ORIGIN	    "Origin: "
#define HTTP_HEAD_FIELD_CORS_REQUEST_METHOD	    "Access-Control-Request-Method: "
#define HTTP_HEAD_FIELD_CORS_REQUEST_HEADERS    "Access-Control-Request-Headers: "
#define HTTP_HEAD_FIELD_CORS_ALLOW_ORIGIN	    "Access-Control-Allow-Origin: "
#define HTTP_HEAD_FIELD_CORS_ALLOW_METHODS	    "Access-Control-Allow-Methods: "
#define HTTP_HEAD_FIELD_CORS_ALLOW_HEADERS	    "Access-Control-Allow-Headers: "
#define HTTP_HEAD_FIELD_CORS_ALLOW_CREDENTIALS  "Access-Control-Allow-Credentials: "
#define HTTP_HEAD_FIELD_CORS_MAX_AGE			"Access-Control-Max-Age: "

/**
 * @brief Media Types
 **/
#define HTTP_MEDIA_TYPE_APPLICATION_JSON	"application/json"
#define HTTP_MEDIA_TYPE_APPLICATION_XML	    "application/xml"
#define HTTP_MEDIA_TYPE_APPLICATION_MP4	    "application/mp4"
#define HTTP_MEDIA_TYPE_APPLICATION_XWFU	"application/x-www-form-urlencoded"
#define HTTP_MEDIA_TYPE_APPLICATION_OSTRM   "application/octet-stream"
#define HTTP_MEDIA_TYPE_VIDEO               "video/"
#define HTTP_MEDIA_TYPE_MULTIPART_FORM_DATA "multipart/form-data"
#define HTTP_MEDIA_TYPE_BOUNDARY            "boundary="
#define HTTP_MEDIA_TYPE_FORM_DATA           "form-data"
#define HTTP_MEDIA_TYPE_TEXT_PLAIN		    "text/plain"

enum ELogType { LOGTYPE, RUNLOGTYPE, TRACETYPE, INFOTYPE, ERRORTYPE, INFORUNTYPE, MEDIAINFO, DATAINFO };

#include <stdio.h>
#include <time.h>
#include "utility.h"
#define HTTPCLI_LOG(Param, LogType, format, ...)			\
	if ((Param.Verbose() && !Param.Silent()) ||				\
		(Param.ErrShow() && LogType >= ERRORTYPE))			\
	{														\
		static CThdMutex thdMutex;							\
		time_t t = time(NULL);								\
		CThdMutexLock lock(&thdMutex);						\
		fprintf(stdout, "%s: " format "\n",					\
		CToolKit::Secd2Time(t).c_str(), ##__VA_ARGS__);		\
    }

#endif //__BFP2P_SERVER_HTTPCLIDEF_H__

#ifndef __DRM_TYPES_H_
#define __DRM_TYPES_H_
/*===========================================================================
  Copyright (c) 2012 - 2013 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Qualcomm Technologies Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/07/13   dm     Added support to store AES and keyfile for internal testing purpose.
02/05/13   dm     Added bit-stream enum type for playready encrypt function.
08/21/12   dm     Modified the data structures to add support for PK2.0
05/16/12   dm     Added DRM header information enum type for get property API.
05/07/12   dm     Initial Version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/** @addtogroup drm_types
  @} */

/* Move methods for envelope seek */
#define OEM_FILE_BEGIN            0
#define OEM_FILE_END              1
#define OEM_FILE_CURRENT          2

typedef enum
{
  PR_AES_ENC = 0,    // AES encryption mode
  PR_COCKTAIL_ENC    // cocktail encryption mode
} __attribute__ ((packed)) tz_pr_enc_type;

/**
  Save status key files
*/
typedef enum
{
  PR_ModelCert = 0,  // bgroupcert.dat
  PR_ModelCertKey,   // zgpriv.dat
  PD_ModelCert,      // devcerttemplate.dat
  PD_ModelCertKey    // priv.dat
} __attribute__ ((packed)) tz_pr_keys_type;

typedef enum
{
    PR_CGP_HEADER_KID        = 0,
    PR_CGP_HEADER_TYPE       = 1,
    PR_CGP_HEADER            = 2,
    PR_CGP_PLAYREADY_OBJ     = 3,
    PR_CGP_CIPHER_TYPE       = 4,
    PR_CGP_DECRYPTORSETUP    = 5   //supported only for PK2.0
} __attribute__ ((packed)) tzdrmgetproptype;

/**
  Type of stream
*/
typedef enum
{
  PR_STREAM_TYPE_AUDIO = 0,
  PR_STREAM_TYPE_VIDEO
} __attribute__ ((packed))tz_pr_stream_type;

/**
  Save aes and key files
*/
typedef enum
{
  PR_AESFile = 0,  // aeskey.dat
  PR_KeyFile,      // keyfile.dat
} __attribute__ ((packed)) tz_pr_aes_keyfile_type;

#define DRM_STORE_CLEANUP_ALL                           0xFFFFFFFF
#define DRM_STORE_CLEANUP_DELETE_EXPIRED_LICENSES       0x1
#define DRM_STORE_CLEANUP_DELETE_REMOVAL_DATE_LICENSES  0x2
#define DRM_STORE_AIV_RESET                             0x3

typedef struct tzEncryptionMetaData_s
{
  unsigned int       encryptionSize;
  unsigned int       encryptionOffset;
  unsigned long long initializationVector;
  unsigned long long blockOffset;
  unsigned char      byteOffset;
  tz_pr_enc_type     encryptionMode;  
} __attribute__ ((packed)) tzEncryptionMetaData_t;

typedef struct tzfiletime_s
{
    unsigned long   dwLowDateTime;
    unsigned long   dwHighDateTime;
} __attribute__ ((packed)) tzfiletime_t;

typedef struct tzlicensestatedata_s
{
    unsigned long   dwStreamId;      /* 0 -> All streams, != 0 -> A particular stream. */
    unsigned long   dwCategory;      /* Indicates the category of string to be displayed. */
    unsigned long   dwNumCounts;     /* Number of items supplied in dwCount. */
    unsigned long   dwCount  [4];    /* Up to 4 counts. */
    unsigned long   dwNumDates;      /* Number of items supplied in dwDate. */
    tzfiletime_t    datetime [4];    /* Up to 4 dates. */
    unsigned long   dwVague;         /* 0 -> certain, 1 -> atleast.  (There could be more */
                                     /* licenses. Aggregation not possible.) */
} __attribute__ ((packed)) tzlicensestatedata_t;

typedef struct tzconststring_s
{
    unsigned short        pwszString[5];
    unsigned long         cchString;
} __attribute__ ((packed)) tzconststring_t;

typedef struct tzguid_s {
    unsigned long   Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    char            Data4 [8];
} __attribute__ ((packed)) tzguid_t;

typedef struct
{
    tzguid_t         m_oServiceID;      /* Service ID. */
    tzguid_t         m_oAccountID;      /* Account ID. */
    unsigned long    m_dwRevision;      /* Revision. */
} __attribute__ ((packed)) tzdrmdomainid_t;

typedef struct tzansistring_s
{
    char                  pszString[10];
    unsigned long         cchString;
} __attribute__ ((packed)) tzansistring_t;

typedef struct
{
    tzdrmdomainid_t      m_oDomainID;   /* Domain ID. */
    tzansistring_t       m_dastrURL;
} __attribute__ ((packed)) tzdrmdomaincertinfo_t;

/* Enumeration of license protocols. */
typedef enum
{
    UnknownProtocol = 0,    /* Unrecognized protocol. */
    V2Protocol,             /* V2 protocol. */
    V3Protocol              /* V3 protocol. */
} __attribute__ ((packed)) tzlicenseprotocoltype;

typedef struct tzdrmid_s
{
    unsigned char   rgb [16];
} __attribute__ ((packed)) tzdrmid_t;

typedef struct tzdrmlicenseack_s
{
    tzdrmid_t     m_oKID;       /* KID of the license. */
    tzdrmid_t     m_oLID;       /* LID of the license. */
    long          m_dwResult;   /* DRM result code of the processing and storage. */
    unsigned long m_dwFlags;    /* Flag specifying post-processing behavior. */
} __attribute__ ((packed)) tzdrmlicenseack_t;

typedef struct tzdrmlicensersp_s
{
    tzlicenseprotocoltype     m_eType;
    unsigned char             m_rgbTransactionID[ 100 ];
    unsigned long             m_cbTransactionID;
    tzdrmlicenseack_t         m_rgoAcks[ 20 ];
    unsigned long             m_cAcks;
    long                      m_dwResult;
    tzdrmlicenseack_t         m_pAcks[50];   /* stores the result when m_rgoAcks > 20, only for PK2.0*/
    unsigned long             m_cMaxAcks;    /* number of elements stored in m_pAcks,when m_cAcks > 20, only for PK2.0 */
} __attribute__ ((packed)) tzdrmlicensersp_t;

#endif /* __DRM_TYPES_H_ */

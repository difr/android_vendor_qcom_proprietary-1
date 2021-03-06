/* =======================================================================
mp3file.cpp
DESCRIPTION
Meaningful description of the definitions contained in this file.
Description must specify if the module is portable specific, mobile
specific, or common to both, and it should alert the reader if the
module contains any conditional definitions which tailors the module to
different targets.  Include any initialization and synchronizing
requirements.

EXTERNALIZED FUNCTIONS
List functions and a brief description that are exported from this file

INITIALIZATION AND SEQUENCING REQUIREMENTS
Detail how to initialize and use this service.  The sequencing aspect
is only needed if the order of operations is important.

Copyright 2009-2013 Qualcomm Technologies, Inc., All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
========================================================================== */

/* =======================================================================
Edit History

$Header: //source/qcom/qct/multimedia2/Video/Source/FileDemux/MP3ParserLib/main/latest/src/mp3file.cpp#52 $
$DateTime: 2013/08/28 01:52:54 $
$Change: 4345281 $

========================================================================== */

/* ==========================================================================

INCLUDE FILES FOR MODULE

========================================================================== */
#include "parserinternaldefs.h"
#include "MMDebugMsg.h"
#include <stdio.h>
#include "mp3file.h"
#include "mp3parser.h"
#include "filebase.h"
#include "MMMemory.h"
#include "MMTimer.h"

#define INIT_UNDERRUN_POLL_INTERVAL 500
#define MAX_LOOP_LIMIT 100

#ifdef FEATURE_FILESOURCE_DRM_DCF
#include "IxStream.h"
#endif

//=============================================================================
// FUNCTION DEFINATONS
//=============================================================================
//=============================================================================
// FUNCTION: Constructor
//
// DESCRIPTION:
//  Create an instance of mp3formatparser, and init the class attributes
//
// PARAMETERS
//
//
//
//
// RETURN:
//  None
//
// SIDE EFFECTS
//  None
//=============================================================================
//
MP3File::MP3File()
{
  InitData();
}

MP3File:: MP3File(const FILESOURCE_STRING &filename,
                  unsigned char *pFileBuf,
                  uint64 bufSize)
{
  InitData();
  if (pFileBuf && bufSize)
  {
    m_pFileBuf = pFileBuf;
    m_nFileBufSize = bufSize;
    m_pMP3FilePtr = OSCL_FileOpen (pFileBuf, bufSize);
  }
  else
  {
    m_hFileName = filename;
#ifdef FEATURE_FILESOURCE_FILE_IO_CACHING
     /* Calling with 10K cache  buffer size */
     m_pMP3FilePtr = OSCL_FileOpen(filename, (OSCL_TCHAR *) _T("rb"), MP3_FILE_READ_BUFFER_SIZE);
#else
     m_pMP3FilePtr = OSCL_FileOpen(filename, (OSCL_TCHAR *) _T("rb"));
#endif
     m_nFileSize = OSCL_FileSize( m_hFileName );
  }
  if(ParseMP3Header() == PARSER_ErrorNone)
  {
    _fileErrorCode = PARSER_ErrorNone;
    _success = true;
  }
}
/*
* Constructor for playing mp3 stream from an IxStream
*/
MP3File::MP3File(IxStream* ixstream)
{
  InitData();
#ifdef FEATURE_FILESOURCE_DRM_DCF
  m_pIxStream = ixstream;
  m_pMP3FilePtr = OSCL_FileOpen(m_pIxStream);
  if(m_pIxStream)
  {
    (void)m_pIxStream->Size(&m_nFileSize);
  }
  if(ParseMP3Header() == PARSER_ErrorNone)
  {
    _fileErrorCode = PARSER_ErrorNone;
    _success = true;
  }
#endif
}
#ifdef FEATURE_FILESOURCE_3GPP_PROGRESSIVE_DNLD
MP3File::MP3File(video::iStreamPort* pport)
{
  InitData();
  m_pPort = pport;
  m_bStreaming = true;
  m_pMP3FilePtr = OSCL_FileOpen(m_pPort);
  if(m_pPort)
  {
    int64 noffset = 0;
    uint64 size = MAX_FILE_SIZE;
    video::iStreamPort::DataSourceType eSourceType =
      video::iStreamPort::DS_FILE_SOURCE;
    if(video::iStreamPort::DS_SUCCESS == m_pPort->GetContentLength(&noffset))
    {
      size = (uint64)noffset;
    }
    m_pPort->GetSourceType(&eSourceType);
    if (video::iStreamPort::DS_STREAMING_SOURCE != eSourceType)
    {
      m_bStreaming = false;
    }
    m_nFileSize = size;
  }
  if(ParseMP3Header()== PARSER_ErrorNone )
  {
    _fileErrorCode = PARSER_ErrorNone;
    _success = true;
  }
}

bool MP3File::parseHTTPStream ()
{
  if(_fileErrorCode == PARSER_ErrorNone)
      return true;
  return false;
}
#endif

/*
* Initialize the class members to the default values.
*/
void MP3File::InitData()
{
  m_bMediaAbort     = false;
  m_bSeekDone       = false;
  m_bMetaDatainUTF8 = false;
  m_bStreaming      = false;
  m_pFileBuf = NULL;
  m_nFileBufSize = 0;
  m_pMP3FilePtr = NULL;
#ifdef FEATURE_FILESOURCE_3GPP_PROGRESSIVE_DNLD
  m_pPort = NULL;
#endif
  m_hFileName = NULL;
  m_nFileSize = 0;
  m_pFileBuf  = NULL;
  m_pIxStream = NULL;
  m_nFileBufSize = 0;
  memset(&m_hAudSampleInfo,0,sizeof(m_hAudSampleInfo));
  m_nSeekTime = 0;
  m_pMP3Parser = NULL;
  _fileErrorCode = PARSER_ErrorDefault;
  _success = false;
}
//=============================================================================
// FUNCTION: Destructor
//
// DESCRIPTION:
//  Free any resources allocated
//
// PARAMETERS
//  None
//
// RETURN:
//  None
//
// SIDE EFFECTS
//  None
//=============================================================================
//
MP3File::~MP3File()
{
  if(m_pMP3FilePtr!=NULL)
  {
     OSCL_FileClose(m_pMP3FilePtr);
     m_pMP3FilePtr = NULL;
  }
  if(m_pMP3Parser)
  {
     MM_Delete( m_pMP3Parser);
     m_pMP3Parser = NULL;
  }
}
//===========================================================================
//
//FUNCTION
//  MP3CallbakGetData
//
//DESCRIPTION
//
//DEPENDENCIES
//  None
//
//INPUT PARAMETERS:
//->
//->
//
//RETURN VALUE
//  None
//
//SIDE EFFECTS
//  None
//===========================================================================


uint32 MP3CallbakGetData (uint64         nOffset,
                          uint32         nNumBytesRequest,
                          unsigned char *pData,
                          uint32         nMaxSize,
                          uint32         u32UserData,
                          bool           &bEndOfData )
{
  if(u32UserData)
  {
    MP3File *pMP3File = (MP3File *)u32UserData;
    return( pMP3File->FileGetData(nOffset, nNumBytesRequest, nMaxSize,
                                   pData, bEndOfData));
  }
  MM_MSG_PRIO(MM_FILE_OPS, MM_PRIO_FATAL, "MP3CallbakGetData u32UserData is NULL");
  return 0;
}

// ======================================================================
//FUNCTION:
//  MP3File::FileGetData
//
//DESCRIPTION:
//  To read the data from the file
//INPUT/OUTPUT PARAMETERS:
//  nOffset         : Offset from which data is being requested
//  nNumBytesRequest: Total number of bytes requested
//  ppData          : Buffer to be used for reading the data
//
//RETURN VALUE:
// Number of bytes read
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint32 MP3File::FileGetData(  uint64 nOffset,
                              uint32 nNumBytesRequest,
                              uint32 nMaxSize,
                              uint8 *pData,
                              bool  &bendofdata)
{
  uint32 nRead = 0;
  bendofdata = true;
  /*
  Simplified the read logic:
  1) Reset Number of bytes read as ZERO and End Of data Flag as false
     This means in any failure case, Parser will treat it as EOF and exits.
  2) If user aborted, do not read from file/stream. Instead exit.
  3) Make read call on File Pointer by providing offset and requested bytes.
     If underlying source is iStreamPort and requested data is not available,
     then Source will set bUnderRun flag as true.
     If bUnderRun flag is set to true, then reset bendofdata flag as false.
     Then Parser will treat this as underrun case and reports
     PARSER_ErrorDataUnderRun as status. This status will be handled by
     FileSource and client there after.
  */
  if ((pData) && (m_pMP3FilePtr))
  {
    if(m_bMediaAbort)
    {
      MM_MSG_PRIO(MM_FILE_OPS, MM_PRIO_ERROR, "Breaking, user Abort is true.");
    }
    else
    {
      bool bUnderRun = false;
      nRead = FileBase::readFile(m_pMP3FilePtr, pData, nOffset,
                              FILESOURCE_MIN(nNumBytesRequest,nMaxSize), &bUnderRun);
      if(bUnderRun)
      {
        bendofdata = false;
      }
    }
  }
  return nRead;
}
void MP3File::SetCriticalSection(MM_HANDLE pcriticalsection)
{
  if(m_pMP3FilePtr)
  {
    m_pMP3FilePtr->pCriticalSection = pcriticalsection;
  }
}

//===========================================================================
//
//FUNCTION
//  MP3File::CheckMp3Format
//
//DESCRIPTION
//  check the given file is mp3 or not
//
//DEPENDENCIES
//  None
//
//INPUT PARAMETERS:
//->
//->
//
//RETURN VALUE
//  None
//
//SIDE EFFECTS
// None
//===========================================================================*/
bool MP3File::CheckMP3Format()
{
  bool result = false;
  if(m_pMP3Parser)
  {
    result = m_pMP3Parser->is_mp3_format();
  }
  return  result;
}

//===========================================================================
//
//FUNCTION
//  AMRFile::ParseMP3Header
//
//DESCRIPTION
//  creates instance of MP3parser and calls start on parsing MP3 file
//
//DEPENDENCIES
//  None
//
//INPUT PARAMETERS:
//->
//->
//
//RETURN VALUE
//  None
//
//SIDE EFFECTS
// None
//===========================================================================*/

PARSER_ERRORTYPE MP3File::ParseMP3Header()
{
  PARSER_ERRORTYPE retError = PARSER_ErrorDefault;
  uint32 loopCount = 0;
  m_pMP3Parser = MM_New_Args(mp3Parser,((uint32)this,m_nFileSize,m_pMP3FilePtr,
                                        m_bStreaming));
  if(m_pMP3Parser)
  {
    do
    {
      retError = m_pMP3Parser->StartParsing();
      if( retError != PARSER_ErrorNone)
      {
#ifdef FEATURE_FILESOURCE_MP3_PARSER_DEBUG
        MM_MSG_PRIO1(MM_FILE_OPS, MM_PRIO_FATAL,
                     "MP3File::ParseMP3Header failed..retError %d",retError);
#endif
        if(retError == PARSER_ErrorDataUnderRun)
        {
          MM_Timer_Sleep(INIT_UNDERRUN_POLL_INTERVAL);
#ifdef FEATURE_FILESOURCE_MP3_PARSER_DEBUG
          MM_MSG_PRIO1(MM_FILE_OPS, MM_PRIO_HIGH,"Parser init is not completed\
                       due to data unavailable, loopCount= %lu", loopCount);
#endif
          loopCount++;
        }
      }
    } while (retError == PARSER_ErrorDataUnderRun && loopCount <= MAX_LOOP_LIMIT);
  }
  return (retError);
}

/* ======================================================================
FUNCTION:
  MP3File::getBaseTime

DESCRIPTION:
  gets base time of the clip.

INPUT/OUTPUT PARAMETERS:
  None.

RETURN VALUE:
 none

SIDE EFFECTS:
  None.
======================================================================*/
bool MP3File::getBaseTime(uint32 id, uint64* nBaseTime)
{
  bool nRet = false;
  *nBaseTime = 0;
  if(m_pMP3Parser)
  {
    nRet = m_pMP3Parser->GetBaseTime(nBaseTime);
  }
  return nRet;
}
/* ======================================================================
FUNCTION:
  MP3File::setBaseTime

DESCRIPTION:
  sets base time of the clip.

INPUT/OUTPUT PARAMETERS:
  None.

RETURN VALUE:
 none

SIDE EFFECTS:
  None.
======================================================================*/
bool MP3File::setBaseTime(uint32 id,uint64 nBaseTime)
{
  bool bRet = false;
  if(m_pMP3Parser && nBaseTime)
  {
    bRet = m_pMP3Parser->SetBaseTime(nBaseTime);
  }
  return bRet;
}

/* ======================================================================
FUNCTION   : getNextMediaSample
DESCRIPTION: gets next sample of the given track.

INPUT/OUTPUT PARAMETERS:
@param[in] ulTrackID  TrackID requested
@param[in] pucDataBuf DataBuffer pointer to fill the frame(s)
@param[in/out]
           pulBufSize Amount of data request /
                      Amount of data filled in Buffer
@param[in] rulIndex   Index

RETURN VALUE:
 PARSER_ErrorNone in Successful case /
 Corresponding error code in failure cases

SIDE EFFECTS:
  None.
======================================================================*/
PARSER_ERRORTYPE MP3File::getNextMediaSample(uint32 ulTrackID,
                                             uint8 *pucDataBuf,
                                             uint32 *pulBufSize,
                                             uint32 &rulIndex)
{
  uint32 nOutDataSize = 0;
  PARSER_ERRORTYPE retStatus = PARSER_ErrorDefault;

  /* Validate input params and class variables */
  if(NULL == m_pMP3Parser || NULL == pulBufSize || NULL == pucDataBuf ||
     0 == *pulBufSize)
  {
    MM_MSG_PRIO(MM_FILE_OPS, MM_PRIO_ERROR,
               "getNextMediaSample invalid input params!!");
    return PARSER_ErrorInvalidParam;
  }

  nOutDataSize = *pulBufSize;
  retStatus = m_pMP3Parser->GetCurrentSample(pucDataBuf, *pulBufSize,
                                             &nOutDataSize);
  if((m_bSeekDone == true || m_pMP3Parser->m_firstFrame == true) )
  {
    m_hAudSampleInfo.btimevalid = true;
  }
  else
  {
    m_hAudSampleInfo.btimevalid = false;
  }

  if((PARSER_ErrorNone != retStatus) &&
     (PARSER_ErrorEndOfFile != retStatus))
  {
    MM_MSG_PRIO1(MM_FILE_OPS, MM_PRIO_ERROR,
               "getNextMediaSample exited with error %d", retStatus);
    nOutDataSize = 0;
  }
  else
  {
    //check if parser is configured to output one MP3 frame
    bool bframe = false;
    (void)m_pMP3Parser->GetAudioOutputMode
         (&bframe, FILE_SOURCE_MEDIA_OUTPUT_SINGLE_AUDIO_FRAME);
    if(bframe)
    {
      //Time stamp information would be available..
      if(m_hAudSampleInfo.delta == 0)
      {
        //set the delta only once
        m_hAudSampleInfo.delta = m_pMP3Parser->GetCurrentTime() -
                               m_hAudSampleInfo.time;
      }
      m_hAudSampleInfo.time = m_pMP3Parser->GetCurrentTime();
      m_hAudSampleInfo.btimevalid = true;
    }
  }
  m_bSeekDone = false;
  *pulBufSize = nOutDataSize;
  return retStatus;
}

// ======================================================================
//FUNCTION:
//  MP3File::getMediaTimestampForCurrentSample
//
//DESCRIPTION:
//  gets time stamp of current sample of the track.
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// time stamp in track time scale unit
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint64 MP3File::getMediaTimestampForCurrentSample(uint32 id)
{
  if(true == m_bSeekDone)
  {
    m_hAudSampleInfo.time = m_nSeekTime;
  }
  /* If seek is not done call mp3Parser object to provide timestamp */
  else if(m_pMP3Parser && m_bSeekDone == false && m_pMP3Parser->m_firstFrame == true)
  {
    m_hAudSampleInfo.time = m_pMP3Parser->getMediaTimestampForCurrentSample(id);
  }
  return m_hAudSampleInfo.time;
}

/* ======================================================================
FUNCTION:
  MP3File::GetLastRetrievedSampleOffset

DESCRIPTION:
  Returns the offset of the last retrieved sample

INPUT/OUTPUT PARAMETERS:
  None.

RETURN VALUE:
 none

SIDE EFFECTS:
  None.
======================================================================*/
uint64 MP3File::GetLastRetrievedSampleOffset(uint32 trackid)
{
  uint64 offset = 0;
  if(m_pMP3Parser)
  {
      offset = m_pMP3Parser->m_nCurrOffset;
  }
#ifdef FEATURE_FILESOURCE_MP3_PARSER_DEBUG
  MM_MSG_PRIO1(MM_GENERAL, MM_PRIO_HIGH, "GetLastRetrievedSampleOffset %llu",offset);
#endif
  return offset;
}
// ======================================================================
//FUNCTION:
//  MP3File::resetPlayback
//
//DESCRIPTION:
//  resets the playback time to given time(pos) for a track.
//  Also tells if we need to goto closest sync sample or not.
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
//
uint64 MP3File::resetPlayback(uint64 repos_time, uint32 id, bool bSetToSyncSample,
                              bool *bError, uint64 currentPosTimeStamp)
{
  PARSER_ERRORTYPE status = PARSER_ErrorDefault;

  if(m_pMP3Parser)
  {
    *bError = true;
    uint64 nSeekedTime = 0;
    status = m_pMP3Parser->Seek(repos_time, &nSeekedTime);
    if(PARSER_ErrorDataUnderRun == status || PARSER_ErrorNone == status)
    {
      if(status == PARSER_ErrorDataUnderRun)
      {
        _fileErrorCode = PARSER_ErrorDataUnderRun;
      }
      else
      {
        *bError = false;
        _fileErrorCode = PARSER_ErrorNone;
        m_nSeekTime = nSeekedTime;
        if(!m_nSeekTime)
        {
           MM_MSG_PRIO(MM_FILE_OPS,MM_PRIO_HIGH,
                       "m_pMP3Parser->Seek returned 0,doing init_file_position");
           m_pMP3Parser->init_file_position();
        }
        m_hAudSampleInfo.time = m_nSeekTime;
        m_bSeekDone = true;
      }
    }
    else
    {
      MM_MSG_PRIO1(MM_FILE_OPS, MM_PRIO_ERROR, "Reposition failed for track id = %lu", id);
      // this is to avoid unnecessary disturbance, when repositioning can not be done
      *bError = true;
      _fileErrorCode = PARSER_ErrorSeekFail;
    }
  }
  return m_nSeekTime;
}
/* ======================================================================
FUNCTION:
  MP3File::GetClipMetaData

DESCRIPTION:
  Provides different metadata fields info in the o/p buffer

INPUT/OUTPUT PARAMETERS:
  pucDataBuf and pulDatabufLen.

RETURN VALUE:
 PARSER_ErrorNone if Success
 Error status, if failure

SIDE EFFECTS:
  None.
======================================================================*/
PARSER_ERRORTYPE MP3File::GetClipMetaData(wchar_t *pucDataBuf,
                                          uint32 *pulDatabufLen,
                                          FileSourceMetaDataType ienumData)
{
  if(pulDatabufLen == NULL || m_pMP3Parser == NULL)
    return PARSER_ErrorInvalidParam;

  uint32 ulIndex           = 0;
  uint32 ulTotalID3Entries = m_pMP3Parser->get_total_id3v2();
  metadata_id3v1_type* pid3v1info =
    (metadata_id3v1_type*)m_pMP3Parser->get_id3v1_info();
  metadata_id3v2_type* id3v2_info =
    (metadata_id3v2_type*)m_pMP3Parser->get_id3v2_info(ulIndex);
  text_frame *textFrame = NULL;
  encoder_delay_tag_info *pEncDelay = NULL;
  uint8* pucID3v1Str = NULL;
  uint32 ulID3v1StrLen = 0;
  char ucTempString[33];
  memset(ucTempString, 0, 33);
  m_bMetaDatainUTF8 = false;

  /* These delay parameters will be stored in hex string format in ID3
     metadata. Where as in LAME tag we got these values in numeric format.
     To make it uniform, LAME tag parameters are also converted into
     Hex string format.
     At FileSource, these parameters will be converted back to normal decimal
     values. */
  if(((FILE_SOURCE_MD_ENC_DELAY == ienumData) ||
      (FILE_SOURCE_MD_PADDING_DELAY == ienumData)) &&
       (pucDataBuf))
  {
    uint32 ulEncDelay     = m_pMP3Parser->GetEncoderDelay();
    uint32 ulPaddingDelay = m_pMP3Parser->GetPaddingDelay();
    /* If LAME tag is present in clip, use that information. */
    if((FILE_SOURCE_MD_ENC_DELAY == ienumData) && (ulEncDelay))
    {
#ifdef _ANDROID_
      snprintf(ucTempString, 16, (char*)"%x", (int)ulEncDelay);
#else
      std_strlprintf(ucTempString, 16, (char*)"%x", (int)ulEncDelay);
#endif
      memcpy(pucDataBuf, ucTempString, sizeof(uint64));
    }
    else if((FILE_SOURCE_MD_PADDING_DELAY == ienumData) && (ulPaddingDelay))
    {
#ifdef _ANDROID_
      snprintf(ucTempString, 16, (char*)"%x", (int)ulPaddingDelay);
#else
      std_strlprintf(ucTempString, 16, (char*)"%x", (int)ulPaddingDelay);
#endif
      memcpy(pucDataBuf, ucTempString, sizeof(uint64));
    }
    else
    {
      do
      {
        id3v2_info = (metadata_id3v2_type*)
                     m_pMP3Parser->get_id3v2_info(ulIndex);
        if(id3v2_info)
        {
          pEncDelay = &id3v2_info->encoder_delay_tag;
        }
        ulIndex++;
        if (pEncDelay)
        {
          if(FILE_SOURCE_MD_ENC_DELAY == ienumData)
          {
            memcpy(pucDataBuf, pEncDelay->ucEncoderDelay, sizeof(uint64));
          }
          else
          {
            memcpy(pucDataBuf, pEncDelay->ucPaddingDelay, sizeof(uint64));
          }
        }
      } while((NULL == pEncDelay) && (ulTotalID3Entries > ulIndex));
    }
    *pulDatabufLen = sizeof(ucTempString);
    return PARSER_ErrorNone;
  }

  for(ulIndex = 0; ulIndex < ulTotalID3Entries && !textFrame; ulIndex++)
  {
    id3v2_info = (metadata_id3v2_type*)m_pMP3Parser->get_id3v2_info(ulIndex);
    if (NULL == id3v2_info)
    {
      continue;
    }

    switch (ienumData)
    {
      case FILE_SOURCE_MD_ALBUM:
        textFrame   = &id3v2_info->album;
      break;
      case FILE_SOURCE_MD_ARTIST:
        textFrame = &id3v2_info->album_artist;
      break;
      case FILE_SOURCE_MD_REC_YEAR:
        textFrame = &id3v2_info->year;
      break;
      case FILE_SOURCE_MD_TITLE:
        textFrame = &id3v2_info->title;
      break;
      case FILE_SOURCE_MD_TRACK_NUM:
        textFrame = &id3v2_info->track;
      break;
      case FILE_SOURCE_MD_GENRE:
        textFrame = &id3v2_info->genre;
      break;
      case FILE_SOURCE_MD_AUTHOR:
        textFrame = &id3v2_info->publisher;
      break;
      case FILE_SOURCE_MD_COMPOSER:
        textFrame = &id3v2_info->composer;
      break;
      default:
        break;
    }
  }

  if(textFrame && textFrame->text)
  {
    if (pucDataBuf)
    {
      if((*pulDatabufLen < (textFrame->textLen * sizeof(wchar_t)) / 2 &&
          ID3V2_FRAME_TEXT_ENCODING_UTF_16 == textFrame->encoding) ||
         (*pulDatabufLen < textFrame->textLen * sizeof(wchar_t) &&
          ID3V2_FRAME_TEXT_ENCODING_ISO_8859_1 == textFrame->encoding))
      {
        return PARSER_ErrorInsufficientBufSize;
      }
      if(ID3V2_FRAME_TEXT_ENCODING_ISO_8859_1 == textFrame->encoding)
      {
        CharToWideChar(textFrame->text, textFrame->textLen,
                       pucDataBuf, *pulDatabufLen, FALSE);
      }
      else
      {
        /* If data is already in UTF8 codec format, Parser will copy data
           directly into output buffer and sets the UTF8 flag. Based on this
           flag, Client will not do any extra processing on the output data.
           In normal circumstances, client will convert the data into UTF8
           format, before processing. */
        if(ID3V2_FRAME_TEXT_ENCODING_UTF_8 == textFrame->encoding)
        {
          memset(pucDataBuf, 0, *pulDatabufLen);
          memcpy(pucDataBuf, textFrame->text, textFrame->textLen);
          m_bMetaDatainUTF8 = true;
        }
        else if(2 == sizeof(wchar_t))
        {
          memcpy(pucDataBuf, textFrame->text, textFrame->textLen);
        }
        else
        {
          CharToWideChar(textFrame->text, textFrame->textLen,
                         pucDataBuf, *pulDatabufLen, TRUE);
        }
      }
    }
    /* If encoding type used is UniCode (which is equivalent to 16bits). */
    if(ID3V2_FRAME_TEXT_ENCODING_UTF_16 == textFrame->encoding)
      *pulDatabufLen = (textFrame->textLen * sizeof(wchar_t)) / 2;
    else
      *pulDatabufLen = textFrame->textLen * sizeof(wchar_t);
  }//if(id3v2_info && ..
  else if(pid3v1info)
  {
    switch (ienumData)
    {
      case FILE_SOURCE_MD_ALBUM:
        ulID3v1StrLen = MAX_ID3V1_TEXT_SIZE;
        pucID3v1Str = pid3v1info->album;
      break;
      case FILE_SOURCE_MD_ARTIST:
        ulID3v1StrLen = MAX_ID3V1_TEXT_SIZE;
        pucID3v1Str = pid3v1info->artist;
      break;
      case FILE_SOURCE_MD_TITLE:
        ulID3v1StrLen = MAX_ID3V1_TEXT_SIZE;
        pucID3v1Str = pid3v1info->title;
      break;
      case FILE_SOURCE_MD_REC_YEAR:
        ulID3v1StrLen = MAX_ID3V1_YEAR_TEXT_SIZE;
        pucID3v1Str = pid3v1info->year;
      break;
      case FILE_SOURCE_MD_TRACK_NUM:
      {
#ifdef _ANDROID_
        snprintf(ucTempString, 4, (char*)"%d", (int)pid3v1info->track);
#else
        std_strlprintf(ucTempString, 4, (char*)"%d", (int)pid3v1info->track);
#endif
        ulID3v1StrLen = 4;
        pucID3v1Str = (uint8 *)ucTempString;
      }
      break;
      default:
      break;
    }
    if (pucDataBuf && pucID3v1Str)
    {
      if(*pulDatabufLen < ulID3v1StrLen * sizeof(wchar_t))
      {
        return PARSER_ErrorInsufficientBufSize;
      }

      //Flag at the end indicate whether input string is in UTF8 or YTF16
      //foratm. 'FALSE' means UTF8, 'TRUE' means UTF16
      CharToWideChar((char*)pucID3v1Str, ulID3v1StrLen, pucDataBuf,
                     *pulDatabufLen, FALSE);
    }

    // Update output buf length value
    *pulDatabufLen = ulID3v1StrLen * sizeof(wchar_t);
  }//if(pid3v1info)
  else
  {
    *pulDatabufLen = 0;
  }

  return PARSER_ErrorNone;
}
/* ======================================================================
FUNCTION:
  MP3File::getAlbumArt

DESCRIPTION:
  Provides picture data (album art) in the o/p buffer

INPUT/OUTPUT PARAMETERS:
  pucDataBuf and pulDatabufLen.

RETURN VALUE:
 PARSER_ErrorNone if Success
 Error status, if failure

SIDE EFFECTS:
  None.
======================================================================*/
PARSER_ERRORTYPE MP3File::getAlbumArt(wchar_t *pucDataBuf, uint32 *pulDatabufLen)
{
  if(pulDatabufLen == NULL || m_pMP3Parser == NULL)
  {
    return PARSER_ErrorInvalidParam;
  }
  bool   bAlbumArt      = false;
  uint32 ulIndex        = 0;
  uint32 ulTotalEntries = m_pMP3Parser->get_total_id3v2();
  metadata_id3v2_type* pID3v2Info = NULL;
  if(0 == ulTotalEntries)
  {
    return PARSER_ErrorUnsupported;
  }

  for ( ; ulIndex < ulTotalEntries; ulIndex++)
  {
    pID3v2Info = (metadata_id3v2_type*)m_pMP3Parser->get_id3v2_info(ulIndex);
    if (NULL == pID3v2Info || NULL == pID3v2Info->pic_info.pic_data)
    {
      continue;
    }

    if (pucDataBuf)
    {
      FS_ALBUM_ART_METADATA * pAlbumArt = (FS_ALBUM_ART_METADATA*)pucDataBuf;
      if(*pulDatabufLen < pID3v2Info->pic_info.pic_data_len +
                          FIXED_APIC_FIELDS_LENGTH)
      {
        return PARSER_ErrorInsufficientBufSize;
      }
      memset(pucDataBuf, 0, FIXED_APIC_FIELDS_LENGTH);
      pAlbumArt->ucTextEncodeType = pID3v2Info->pic_info.text_enc;
      pAlbumArt->imgFormat        = pID3v2Info->pic_info.img_format;
      pAlbumArt->picType          = pID3v2Info->pic_info.pict_type;
      pAlbumArt->ulPicDataLen     = pID3v2Info->pic_info.pic_data_len;

      memcpy(pAlbumArt->ucDesc,pID3v2Info->pic_info.desc,MAX_DESC_LEN);
      memcpy(pAlbumArt->pucPicData,pID3v2Info->pic_info.pic_data,
             pID3v2Info->pic_info.pic_data_len);
      memcpy(pAlbumArt->ucImgFormatStr,pID3v2Info->pic_info.img_format_string,
             MAX_IMG_FORMAT_LEN);
    }
    *pulDatabufLen = pID3v2Info->pic_info.pic_data_len +
                     FIXED_APIC_FIELDS_LENGTH;
    bAlbumArt = true;
    break;
  }//for ( ; ulIndex < ulTotalEntries; ulIndex++)

  if(false == bAlbumArt)
  {
    *pulDatabufLen = 0;
  }

  return PARSER_ErrorNone;
}
//======================================================================
//FUNCTION:
//  MP3File::getMovieDuration
//
//DESCRIPTION:
//  gets movie duration in movie timescale unit.
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint64 MP3File::getMovieDuration() const
{
  uint64 nDuration = 0;
  if(m_pMP3Parser)
  {
    nDuration = m_pMP3Parser->GetClipDurationInMsec();
  }
  return nDuration;
}

// ======================================================================
//FUNCTION:
//  MP3File::getMovieTimescale
//
//DESCRIPTION:
//  gets movie timescale
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint32 MP3File::getMovieTimescale() const
{
  return MP3_STREAM_TIME_SCALE;
}
// ======================================================================
//FUNCTION:
//  MP3File::getTrackWholeIDList
//
//DESCRIPTION:
//  gets list of track IDs
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint32 MP3File::getTrackWholeIDList(uint32 *ids)
{
  int nTracks = 0;
  if(ids)
  {
    *ids = 0;
  }
  if( (m_pMP3Parser) && (ids) )
  {
    *ids = AUDIO_MP3_MAX_TRACKS;
  }
  return nTracks;
}

/* ======================================================================
FUNCTION
      MP3File::getTrackOTIType()

  DESCRIPTION
      This function is used to get Track minor type property.

  PARAMETERS
      @param[in]  ulTrackId    TrackID of media

  RETURN VALUE
      @return     ENUM value in OTI_VALUES
  SIDE EFFECTS    None

 ========================================================================== */
uint8 MP3File::getTrackOTIType(uint32 id)
{
  tech_data_mp3 *pMp3Data = NULL;
  if(m_pMP3Parser)
  {
    (void)m_pMP3Parser->GetMP3Metadata(&pMp3Data);
    if (pMp3Data)
    {
      if(MP3_LAYER_3 == pMp3Data->layer )
        return MP3_AUDIO;
      else if(MP3_LAYER_2 == pMp3Data->layer )
        return MP2_AUDIO;
    }
  }
  return MP3_AUDIO;
}
/* ======================================================================
FUNCTION
      MP3File::GetStreamParameter()

  DESCRIPTION
      This function is used to get MP3 Audio stream parameter.

  PARAMETERS
      @param[in]  ulTrackId    TrackID of media
      @param[in]  ulParamIndex Parameter Index of the structure to be
                               filled.It is from the FS_MEDIA_INDEXTYPE
                               enumeration.
      @param[in]  pParamStruct Pointer to client allocated structure to
                               be filled by the underlying parser.

  RETURN VALUE
      @return     PARSER_ErrorNone in case of success otherwise Error.
  SIDE EFFECTS    None

 ========================================================================== */
PARSER_ERRORTYPE MP3File::GetStreamParameter( uint32 ulTrackId,
                                              uint32 ulParamIndex,
                                              void* pParamStruct)
{
  PARSER_ERRORTYPE eError = PARSER_ErrorInvalidParam;
  tech_data_mp3 *pMp3Data = NULL;
  if(m_pMP3Parser)
  {
    (void)m_pMP3Parser->GetMP3Metadata(&pMp3Data);
  }

  if ((FS_IndexParamAudioMP3 == ulParamIndex) && (pMp3Data) && (pParamStruct))
  {
    FS_AUDIO_PARAM_MPGTYPE *pMPGInfo = (FS_AUDIO_PARAM_MPGTYPE*)pParamStruct;

    pMPGInfo->ulSamplingFreq    = pMp3Data->samplerate;
    pMPGInfo->ulBitRate         = pMp3Data->bitrate;
    pMPGInfo->usChannels          = 2;
    pMPGInfo->ucLayer           = pMp3Data->layer;
    pMPGInfo->ucVersion         = pMp3Data->version;
    if (pMp3Data->channel == MP3_CHANNEL_SINGLE)
    {
      pMPGInfo->usChannels = 1;
    }
    eError = PARSER_ErrorNone;

  }// if(pParamStruct)
  return eError;
}

// ======================================================================
//FUNCTION:
//  MP3File::getTrackMediaDuration
//
//DESCRIPTION:
//  gets track duration in track time scale unit
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================*/
uint64 MP3File::getTrackMediaDuration(uint32)
{
  uint64 nTrackDuration = 0;
  if(m_pMP3Parser)
  {
    nTrackDuration = m_pMP3Parser->GetClipDurationInMsec();
  }
  return nTrackDuration;
}
// ======================================================================
//FUNCTION:
//  AMRFile::getTrackMediaTimescale
//
//DESCRIPTION:
//  gets track time scale
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint32 MP3File::getTrackMediaTimescale(uint32)
{
  return MP3_STREAM_TIME_SCALE;
}
// ======================================================================
//FUNCTION:
//  MP3File::getTrackAudioSamplingFreq
//
//DESCRIPTION:
//  gets audio track's sampling frequency
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint32 MP3File::getTrackAudioSamplingFreq(uint32)
{
  mp3_header_mp3h smp3_header_mp3h;
  uint32 nSamplingFreq = 0;
  if(m_pMP3Parser)
  {
    if(m_pMP3Parser->GetMP3Header(&smp3_header_mp3h)==PARSER_ErrorNone)
    {
      nSamplingFreq = smp3_header_mp3h.nSampleRate;
    }
  }
  return nSamplingFreq;
}

// ======================================================================
//FUNCTION:
//  AMRFile::GetNumAudioChannels
//
//DESCRIPTION:
//  returns number of audio channels
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
unsigned long MP3File::GetNumAudioChannels(int)
{
  unsigned long nChannels = 0;
  mp3_header_mp3h smp3_header_mp3h;
  if(m_pMP3Parser)
  {
    if(m_pMP3Parser->GetMP3Header(&smp3_header_mp3h)==PARSER_ErrorNone)
    {
      nChannels = smp3_header_mp3h.nChannels;
    }
  }
  return nChannels;
}
// ======================================================================
//FUNCTION:
//  AMRFile::getAudioSamplesPerFrame
//
//DESCRIPTION:
//  returns audio samples per frame
//
//INPUT/OUTPUT PARAMETERS:
//  Track ID.
//
//RETURN VALUE:
// samples per frame
//
//SIDE EFFECTS:
//  None.
//======================================================================
uint32 MP3File::getAudioSamplesPerFrame(uint32 id)
{
  uint32 nsamplesperframe = 0;
  if(m_pMP3Parser)
  {
    nsamplesperframe = m_pMP3Parser->getSamplesPerFrame(id);
  }
  return nsamplesperframe;
}
// ======================================================================
//FUNCTION:
//  MP3File::getTrackAverageBitrate
//
//DESCRIPTION:
//  returns the average bit rate
//
//INPUT/OUTPUT PARAMETERS:
//  Track ID.
//
//RETURN VALUE:
// bit rate
//
//SIDE EFFECTS:
//  None.
//======================================================================
int32 MP3File::getTrackAverageBitrate(uint32 id)
{
  int32 btrt = 0;
  if(m_pMP3Parser)
  {
    btrt = m_pMP3Parser->getBitrate(id);
  }
  return btrt;
}
// ======================================================================
//FUNCTION:
//  MP3File::getTrackMaxBitrate
//
//DESCRIPTION:
//  returns the maximum value of bit rate
//
//INPUT/OUTPUT PARAMETERS:
//  Track ID.
//
//RETURN VALUE:
// max bit rate
//
//SIDE EFFECTS:
//  None.
//======================================================================
int32 MP3File::getTrackMaxBitrate(uint32 id)
{
  int32 mxrt = 0;
  if(m_pMP3Parser)
  {
    mxrt = m_pMP3Parser->getMaxBitrate(id);
  }
  return mxrt;
}
// ======================================================================
//FUNCTION:
// MP3File::getTrackMaxBufferSizeDB
//
//DESCRIPTION:
//  gets maximum buffer size to play the track
//
//INPUT/OUTPUT PARAMETERS:
//  None.
//
//RETURN VALUE:
// none
//
//SIDE EFFECTS:
//  None.
//======================================================================
int32 MP3File::getTrackMaxBufferSizeDB(uint32 id)
{
  mp3_audio_info audioInfo;
  int32 bufferSize = 0;
  if(m_pMP3Parser)
  {
    bufferSize = MP3_DEFAULT_AUDIO_BUF_SIZE;
    if((m_pMP3Parser->GetAudioInfo(&audioInfo))== PARSER_ErrorNone)
    {
      bufferSize = audioInfo.dwSuggestedBufferSize;
    }
  }
  return bufferSize;
}
/* ======================================================================
FUNCTION:
  MP3File::peekCurSample

DESCRIPTION:
  gets information about current sample of a track

INPUT/OUTPUT PARAMETERS:
  None.

RETURN VALUE:
 none

SIDE EFFECTS:
  None.
======================================================================*/
PARSER_ERRORTYPE MP3File::peekCurSample(uint32,file_sample_info_type *pSampleInfo)
{
  PARSER_ERRORTYPE error = PARSER_ErrorInvalidParam;
  if( (m_pMP3Parser) && pSampleInfo)
  {
    *pSampleInfo = m_hAudSampleInfo;
    error = PARSER_ErrorNone;
  }
  return error;
}
// ======================================================================
//FUNCTION:
// MP3File::getTrackDecoderSpecificInfoContent
//
//DESCRIPTION:
//  gets decoder specific information
//
//INPUT/OUTPUT PARAMETERS:
// >>id: Track ID
// >>buf: Output buffer, decoder information will be written into this buffer
// >>pbufSize: size of decoder specific information
//RETURN VALUE:
// Success or Fail
//
//SIDE EFFECTS:
//  None.
//======================================================================
//
PARSER_ERRORTYPE MP3File::getTrackDecoderSpecificInfoContent(uint32 id, uint8* buf, uint32 *pbufSize)
{
  mpeg1_tag t_mpeg1_tag_info;
  PARSER_ERRORTYPE error = PARSER_ErrorInvalidParam;
  if(m_pMP3Parser)
  {
    if(pbufSize)
    {
      *pbufSize = sizeof(t_mpeg1_tag_info);
      error = PARSER_ErrorNone;
    }
    if((error = m_pMP3Parser->GetMP3DecodeInfo(&t_mpeg1_tag_info))==PARSER_ErrorNone)
    {
      if(buf)
      {
        uint8 *p_mp3_decode_info =  reinterpret_cast<uint8 *>(&t_mpeg1_tag_info);
        memcpy(buf,p_mp3_decode_info,sizeof(t_mpeg1_tag_info));
      }
    }
  }
  return error;
}
/* ======================================================================
FUNCTION:
  SetAudioOutputMode

DESCRIPTION:
  Called by user to set output mode specified by henum

INPUT/OUTPUT PARAMETERS:
  henum-Output mode

RETURN VALUE:
 FILE_SOURCE_SUCCESS if successful in setting output mode
 else returns FILE_SOURCE_FAIL

SIDE EFFECTS:
  None.
======================================================================*/
FileSourceStatus MP3File::SetAudioOutputMode(FileSourceConfigItemEnum henum)
{
  FileSourceStatus status = FILE_SOURCE_FAIL;
  if(m_pMP3Parser)
  {
    status = m_pMP3Parser->SetAudioOutputMode(henum);
  }
  return status;
}
/* ======================================================================
FUNCTION:
  GetAudioOutputMode

DESCRIPTION:
  Called by user to query output mode specified by henum

INPUT/OUTPUT PARAMETERS:
  henum-Output mode to query

RETURN VALUE:
 FILE_SOURCE_SUCCESS if successful in retrieving output mode
 else returns FILE_SOURCE_FAIL

SIDE EFFECTS:
  None.
======================================================================*/
FileSourceStatus MP3File::GetAudioOutputMode(bool* bret,FileSourceConfigItemEnum henum)
{
  FileSourceStatus status = FILE_SOURCE_FAIL;
  if(m_pMP3Parser)
  {
    status = m_pMP3Parser->GetAudioOutputMode(bret,henum);
  }
  return status;
}

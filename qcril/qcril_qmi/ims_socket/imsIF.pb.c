/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.2.7 at Wed Oct 22 13:01:20 2014. */

#include "imsIF.pb.h"

const uint32_t ims_CallDetails_extrasLength_default = 0u;


const pb_field_t ims_MsgTag_fields[5] = {
    PB_FIELD2(  1, FIXED32 , REQUIRED, STATIC  , FIRST, ims_MsgTag, token, token, 0),
    PB_FIELD2(  2, ENUM    , REQUIRED, STATIC  , OTHER, ims_MsgTag, type, token, 0),
    PB_FIELD2(  3, ENUM    , REQUIRED, STATIC  , OTHER, ims_MsgTag, id, type, 0),
    PB_FIELD2(  4, ENUM    , REQUIRED, STATIC  , OTHER, ims_MsgTag, error, id, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_CallFailCauseResponse_fields[3] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_CallFailCauseResponse, failcause, failcause, 0),
    PB_FIELD2(  2, BYTES   , OPTIONAL, CALLBACK, OTHER, ims_CallFailCauseResponse, errorinfo, failcause, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_StatusForAccessTech_fields[5] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_StatusForAccessTech, networkMode, networkMode, 0),
    PB_FIELD2(  2, ENUM    , OPTIONAL, STATIC  , OTHER, ims_StatusForAccessTech, status, networkMode, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_StatusForAccessTech, restrictionCause, status, 0),
    PB_FIELD2(  4, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_StatusForAccessTech, registered, restrictionCause, &ims_Registration_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_Info_fields[8] = {
    PB_FIELD2(  1, BOOL    , OPTIONAL, STATIC  , FIRST, ims_Info, isValid, isValid, 0),
    PB_FIELD2(  2, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Info, type, isValid, 0),
    PB_FIELD2(  3, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Info, callType, type, 0),
    PB_FIELD2(  4, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Info, status, callType, 0),
    PB_FIELD2(  5, BYTES   , OPTIONAL, CALLBACK, OTHER, ims_Info, userdata, status, 0),
    PB_FIELD2(  6, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_Info, restrictCause, userdata, 0),
    PB_FIELD2(  7, MESSAGE , REPEATED, CALLBACK, OTHER, ims_Info, accTechStatus, restrictCause, &ims_StatusForAccessTech_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_SrvStatusList_fields[2] = {
    PB_FIELD2(  2, MESSAGE , REPEATED, CALLBACK, FIRST, ims_SrvStatusList, SrvStatusInfo, SrvStatusInfo, &ims_Info_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_CallDetails_fields[8] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_CallDetails, callType, callType, 0),
    PB_FIELD2(  2, ENUM    , OPTIONAL, STATIC  , OTHER, ims_CallDetails, callDomain, callType, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallDetails, extrasLength, callDomain, &ims_CallDetails_extrasLength_default),
    PB_FIELD2(  4, STRING  , REPEATED, CALLBACK, OTHER, ims_CallDetails, extras, extrasLength, 0),
    PB_FIELD2(  5, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallDetails, localAbility, extras, &ims_SrvStatusList_fields),
    PB_FIELD2(  6, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallDetails, peerAbility, localAbility, &ims_SrvStatusList_fields),
    PB_FIELD2(  7, ENUM    , OPTIONAL, STATIC  , OTHER, ims_CallDetails, callSubstate, peerAbility, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_CallModify_fields[4] = {
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_CallModify, callIndex, callIndex, 0),
    PB_FIELD2(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallModify, callDetails, callIndex, &ims_CallDetails_fields),
    PB_FIELD2(  4, ENUM    , OPTIONAL, STATIC  , OTHER, ims_CallModify, error, callDetails, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_CallList_fields[2] = {
    PB_FIELD2(  2, MESSAGE , REPEATED, CALLBACK, FIRST, ims_CallList, callAttributes, callAttributes, &ims_CallList_Call_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_CallList_Call_fields[15] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_CallList_Call, state, state, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, index, state, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, toa, index, 0),
    PB_FIELD2(  4, BOOL    , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, isMpty, toa, 0),
    PB_FIELD2(  5, BOOL    , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, isMT, isMpty, 0),
    PB_FIELD2(  6, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, als, isMT, 0),
    PB_FIELD2(  7, BOOL    , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, isVoice, als, 0),
    PB_FIELD2(  8, BOOL    , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, isVoicePrivacy, isVoice, 0),
    PB_FIELD2(  9, STRING  , OPTIONAL, CALLBACK, OTHER, ims_CallList_Call, number, isVoicePrivacy, 0),
    PB_FIELD2( 10, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, numberPresentation, number, 0),
    PB_FIELD2( 11, STRING  , OPTIONAL, CALLBACK, OTHER, ims_CallList_Call, name, numberPresentation, 0),
    PB_FIELD2( 12, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, namePresentation, name, 0),
    PB_FIELD2( 13, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, callDetails, namePresentation, &ims_CallDetails_fields),
    PB_FIELD2( 14, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallList_Call, failCause, callDetails, &ims_CallFailCauseResponse_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_Colr_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_Colr, presentation, presentation, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Dial_fields[6] = {
    PB_FIELD2(  1, STRING  , OPTIONAL, CALLBACK, FIRST, ims_Dial, address, address, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_Dial, clir, address, 0),
    PB_FIELD2(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_Dial, callDetails, clir, &ims_CallDetails_fields),
    PB_FIELD2(  4, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Dial, presentation, callDetails, 0),
    PB_FIELD2(  5, BOOL    , OPTIONAL, STATIC  , OTHER, ims_Dial, isConferenceUri, presentation, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Hangup_fields[6] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_Hangup, conn_index, conn_index, 0),
    PB_FIELD2(  2, BOOL    , OPTIONAL, STATIC  , OTHER, ims_Hangup, multi_party, conn_index, 0),
    PB_FIELD2(  3, STRING  , OPTIONAL, CALLBACK, OTHER, ims_Hangup, conn_uri, multi_party, 0),
    PB_FIELD2(  4, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_Hangup, conf_id, conn_uri, 0),
    PB_FIELD2(  5, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_Hangup, failCauseResponse, conf_id, &ims_CallFailCauseResponse_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_DeflectCall_fields[3] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_DeflectCall, conn_index, conn_index, 0),
    PB_FIELD2(  2, STRING  , OPTIONAL, CALLBACK, OTHER, ims_DeflectCall, number, conn_index, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Clir_fields[3] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_Clir, param_m, param_m, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_Clir, param_n, param_m, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Answer_fields[3] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_Answer, call_type, call_type, 0),
    PB_FIELD2(  4, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Answer, presentation, call_type, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_SwitchWaitingOrHoldingAndActive_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_SwitchWaitingOrHoldingAndActive, call_type, call_type, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Mute_fields[2] = {
    PB_FIELD2(  1, BOOL    , OPTIONAL, STATIC  , FIRST, ims_Mute, mute_flag, mute_flag, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Dtmf_fields[2] = {
    PB_FIELD2(  1, STRING  , OPTIONAL, CALLBACK, FIRST, ims_Dtmf, dtmf, dtmf, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Registration_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_Registration, state, state, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_RingBackTone_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_RingBackTone, flag, flag, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_IFConnected_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_IFConnected, if_version, if_version, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_LastFailCause_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_LastFailCause, cause, cause, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Extra_fields[3] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_Extra, type, type, 0),
    PB_FIELD2(  2, BYTES   , OPTIONAL, CALLBACK, OTHER, ims_Extra, extraInfo, type, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Handover_fields[5] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_Handover, type, type, 0),
    PB_FIELD2(  2, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Handover, srcTech, type, 0),
    PB_FIELD2(  3, ENUM    , OPTIONAL, STATIC  , OTHER, ims_Handover, targetTech, srcTech, 0),
    PB_FIELD2(  4, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_Handover, hoExtra, targetTech, &ims_Extra_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_TtyNotify_fields[3] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_TtyNotify, mode, mode, 0),
    PB_FIELD2(  2, BYTES   , OPTIONAL, CALLBACK, OTHER, ims_TtyNotify, userData, mode, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_RadioStateChanged_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_RadioStateChanged, state, state, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_ClipProvisionStatus_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_ClipProvisionStatus, clip_status, clip_status, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_ServiceClass_fields[2] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_ServiceClass, service_Class, service_Class, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_CbNumList_fields[3] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_CbNumList, status, status, 0),
    PB_FIELD2(  2, STRING  , OPTIONAL, CALLBACK, OTHER, ims_CbNumList, number, status, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_CbNumListType_fields[3] = {
    PB_FIELD2(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, ims_CbNumListType, serviceClass, serviceClass, &ims_ServiceClass_fields),
    PB_FIELD2(  2, MESSAGE , REPEATED, CALLBACK, OTHER, ims_CbNumListType, cb_num_list, serviceClass, &ims_CbNumList_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_CallWaitingInfo_fields[3] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_CallWaitingInfo, service_status, service_status, 0),
    PB_FIELD2(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallWaitingInfo, service_class, service_status, &ims_ServiceClass_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_CallForwardInfoList_fields[2] = {
    PB_FIELD2(  2, MESSAGE , REPEATED, CALLBACK, FIRST, ims_CallForwardInfoList, info, info, &ims_CallForwardInfoList_CallForwardInfo_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_CallForwardInfoList_CallForwardInfo_fields[9] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_CallForwardInfoList_CallForwardInfo, status, status, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallForwardInfoList_CallForwardInfo, reason, status, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallForwardInfoList_CallForwardInfo, service_class, reason, 0),
    PB_FIELD2(  4, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallForwardInfoList_CallForwardInfo, toa, service_class, 0),
    PB_FIELD2(  5, STRING  , OPTIONAL, CALLBACK, OTHER, ims_CallForwardInfoList_CallForwardInfo, number, toa, 0),
    PB_FIELD2(  6, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallForwardInfoList_CallForwardInfo, time_seconds, number, 0),
    PB_FIELD2(  7, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallForwardInfoList_CallForwardInfo, callFwdTimerStart, time_seconds, &ims_CallFwdTimerInfo_fields),
    PB_FIELD2(  8, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_CallForwardInfoList_CallForwardInfo, callFwdTimerEnd, callFwdTimerStart, &ims_CallFwdTimerInfo_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_CallFwdTimerInfo_fields[8] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_CallFwdTimerInfo, year, year, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallFwdTimerInfo, month, year, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallFwdTimerInfo, day, month, 0),
    PB_FIELD2(  4, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallFwdTimerInfo, hour, day, 0),
    PB_FIELD2(  5, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallFwdTimerInfo, minute, hour, 0),
    PB_FIELD2(  6, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallFwdTimerInfo, second, minute, 0),
    PB_FIELD2(  7, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_CallFwdTimerInfo, timezone, second, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_ConfInfo_fields[3] = {
    PB_FIELD2(  1, BYTES   , OPTIONAL, CALLBACK, FIRST, ims_ConfInfo, conf_info_uri, conf_info_uri, 0),
    PB_FIELD2(  2, ENUM    , OPTIONAL, STATIC  , OTHER, ims_ConfInfo, confCallState, conf_info_uri, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_SuppSvcNotification_fields[7] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_SuppSvcNotification, notificationType, notificationType, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_SuppSvcNotification, code, notificationType, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_SuppSvcNotification, index, code, 0),
    PB_FIELD2(  4, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_SuppSvcNotification, type, index, 0),
    PB_FIELD2(  5, STRING  , OPTIONAL, CALLBACK, OTHER, ims_SuppSvcNotification, number, type, 0),
    PB_FIELD2(  6, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_SuppSvcNotification, connId, number, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_SuppSvcStatus_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_SuppSvcStatus, status, status, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_SuppSvcRequest_fields[4] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_SuppSvcRequest, operationType, operationType, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_SuppSvcRequest, facilityType, operationType, 0),
    PB_FIELD2(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, ims_SuppSvcRequest, cbNumListType, facilityType, &ims_CbNumListType_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_SuppSvcResponse_fields[5] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_SuppSvcResponse, status, status, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_SuppSvcResponse, facilityType, status, 0),
    PB_FIELD2(  3, STRING  , OPTIONAL, CALLBACK, OTHER, ims_SuppSvcResponse, failureCause, facilityType, 0),
    PB_FIELD2(  4, MESSAGE , REPEATED, CALLBACK, OTHER, ims_SuppSvcResponse, cbNumListType, failureCause, &ims_CbNumListType_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_VideoCallQuality_fields[2] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_VideoCallQuality, quality, quality, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_MwiMessageSummary_fields[6] = {
    PB_FIELD2(  1, ENUM    , OPTIONAL, STATIC  , FIRST, ims_MwiMessageSummary, messageType, messageType, 0),
    PB_FIELD2(  2, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_MwiMessageSummary, newMessage, messageType, 0),
    PB_FIELD2(  3, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_MwiMessageSummary, oldMessage, newMessage, 0),
    PB_FIELD2(  4, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_MwiMessageSummary, newUrgent, oldMessage, 0),
    PB_FIELD2(  5, FIXED32 , OPTIONAL, STATIC  , OTHER, ims_MwiMessageSummary, oldUrgent, newUrgent, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_MwiMessageDetails_fields[8] = {
    PB_FIELD2(  1, STRING  , OPTIONAL, CALLBACK, FIRST, ims_MwiMessageDetails, toAddress, toAddress, 0),
    PB_FIELD2(  2, STRING  , OPTIONAL, CALLBACK, OTHER, ims_MwiMessageDetails, fromAddress, toAddress, 0),
    PB_FIELD2(  3, STRING  , OPTIONAL, CALLBACK, OTHER, ims_MwiMessageDetails, subject, fromAddress, 0),
    PB_FIELD2(  4, STRING  , OPTIONAL, CALLBACK, OTHER, ims_MwiMessageDetails, date, subject, 0),
    PB_FIELD2(  5, ENUM    , OPTIONAL, STATIC  , OTHER, ims_MwiMessageDetails, priority, date, 0),
    PB_FIELD2(  6, STRING  , OPTIONAL, CALLBACK, OTHER, ims_MwiMessageDetails, messageId, priority, 0),
    PB_FIELD2(  7, ENUM    , OPTIONAL, STATIC  , OTHER, ims_MwiMessageDetails, messageType, messageId, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Mwi_fields[4] = {
    PB_FIELD2(  1, MESSAGE , REPEATED, CALLBACK, FIRST, ims_Mwi, mwiMsgSummary, mwiMsgSummary, &ims_MwiMessageSummary_fields),
    PB_FIELD2(  2, STRING  , OPTIONAL, CALLBACK, OTHER, ims_Mwi, ueAddress, mwiMsgSummary, 0),
    PB_FIELD2(  3, MESSAGE , REPEATED, CALLBACK, OTHER, ims_Mwi, mwiMsgDetail, ueAddress, &ims_MwiMessageDetails_fields),
    PB_LAST_FIELD
};

const pb_field_t ims_Hold_fields[2] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_Hold, callId, callId, 0),
    PB_LAST_FIELD
};

const pb_field_t ims_Resume_fields[2] = {
    PB_FIELD2(  1, FIXED32 , OPTIONAL, STATIC  , FIRST, ims_Resume, callId, callId, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 *
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
STATIC_ASSERT((pb_membersize(ims_StatusForAccessTech, registered) < 65536 && pb_membersize(ims_Info, accTechStatus) < 65536 && pb_membersize(ims_SrvStatusList, SrvStatusInfo) < 65536 && pb_membersize(ims_CallDetails, localAbility) < 65536 && pb_membersize(ims_CallDetails, peerAbility) < 65536 && pb_membersize(ims_CallModify, callDetails) < 65536 && pb_membersize(ims_CallList, callAttributes) < 65536 && pb_membersize(ims_CallList_Call, callDetails) < 65536 && pb_membersize(ims_CallList_Call, failCause) < 65536 && pb_membersize(ims_Dial, callDetails) < 65536 && pb_membersize(ims_Hangup, failCauseResponse) < 65536 && pb_membersize(ims_Handover, hoExtra) < 65536 && pb_membersize(ims_CbNumListType, serviceClass) < 65536 && pb_membersize(ims_CbNumListType, cb_num_list) < 65536 && pb_membersize(ims_CallWaitingInfo, service_class) < 65536 && pb_membersize(ims_CallForwardInfoList, info) < 65536 && pb_membersize(ims_CallForwardInfoList_CallForwardInfo, callFwdTimerStart) < 65536 && pb_membersize(ims_CallForwardInfoList_CallForwardInfo, callFwdTimerEnd) < 65536 && pb_membersize(ims_SuppSvcRequest, cbNumListType) < 65536 && pb_membersize(ims_SuppSvcResponse, cbNumListType) < 65536 && pb_membersize(ims_Mwi, mwiMsgSummary) < 65536 && pb_membersize(ims_Mwi, mwiMsgDetail) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_ims_MsgTag_ims_CallFailCauseResponse_ims_StatusForAccessTech_ims_Info_ims_SrvStatusList_ims_CallDetails_ims_CallModify_ims_CallList_ims_CallList_Call_ims_Colr_ims_Dial_ims_Hangup_ims_DeflectCall_ims_Clir_ims_Answer_ims_SwitchWaitingOrHoldingAndActive_ims_Mute_ims_Dtmf_ims_Registration_ims_RingBackTone_ims_IFConnected_ims_LastFailCause_ims_Extra_ims_Handover_ims_TtyNotify_ims_RadioStateChanged_ims_ClipProvisionStatus_ims_ServiceClass_ims_CbNumList_ims_CbNumListType_ims_CallWaitingInfo_ims_CallForwardInfoList_ims_CallForwardInfoList_CallForwardInfo_ims_CallFwdTimerInfo_ims_ConfInfo_ims_SuppSvcNotification_ims_SuppSvcStatus_ims_SuppSvcRequest_ims_SuppSvcResponse_ims_VideoCallQuality_ims_MwiMessageSummary_ims_MwiMessageDetails_ims_Mwi_ims_Hold_ims_Resume)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 *
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
STATIC_ASSERT((pb_membersize(ims_StatusForAccessTech, registered) < 256 && pb_membersize(ims_Info, accTechStatus) < 256 && pb_membersize(ims_SrvStatusList, SrvStatusInfo) < 256 && pb_membersize(ims_CallDetails, localAbility) < 256 && pb_membersize(ims_CallDetails, peerAbility) < 256 && pb_membersize(ims_CallModify, callDetails) < 256 && pb_membersize(ims_CallList, callAttributes) < 256 && pb_membersize(ims_CallList_Call, callDetails) < 256 && pb_membersize(ims_CallList_Call, failCause) < 256 && pb_membersize(ims_Dial, callDetails) < 256 && pb_membersize(ims_Hangup, failCauseResponse) < 256 && pb_membersize(ims_Handover, hoExtra) < 256 && pb_membersize(ims_CbNumListType, serviceClass) < 256 && pb_membersize(ims_CbNumListType, cb_num_list) < 256 && pb_membersize(ims_CallWaitingInfo, service_class) < 256 && pb_membersize(ims_CallForwardInfoList, info) < 256 && pb_membersize(ims_CallForwardInfoList_CallForwardInfo, callFwdTimerStart) < 256 && pb_membersize(ims_CallForwardInfoList_CallForwardInfo, callFwdTimerEnd) < 256 && pb_membersize(ims_SuppSvcRequest, cbNumListType) < 256 && pb_membersize(ims_SuppSvcResponse, cbNumListType) < 256 && pb_membersize(ims_Mwi, mwiMsgSummary) < 256 && pb_membersize(ims_Mwi, mwiMsgDetail) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_ims_MsgTag_ims_CallFailCauseResponse_ims_StatusForAccessTech_ims_Info_ims_SrvStatusList_ims_CallDetails_ims_CallModify_ims_CallList_ims_CallList_Call_ims_Colr_ims_Dial_ims_Hangup_ims_DeflectCall_ims_Clir_ims_Answer_ims_SwitchWaitingOrHoldingAndActive_ims_Mute_ims_Dtmf_ims_Registration_ims_RingBackTone_ims_IFConnected_ims_LastFailCause_ims_Extra_ims_Handover_ims_TtyNotify_ims_RadioStateChanged_ims_ClipProvisionStatus_ims_ServiceClass_ims_CbNumList_ims_CbNumListType_ims_CallWaitingInfo_ims_CallForwardInfoList_ims_CallForwardInfoList_CallForwardInfo_ims_CallFwdTimerInfo_ims_ConfInfo_ims_SuppSvcNotification_ims_SuppSvcStatus_ims_SuppSvcRequest_ims_SuppSvcResponse_ims_VideoCallQuality_ims_MwiMessageSummary_ims_MwiMessageDetails_ims_Mwi_ims_Hold_ims_Resume)
#endif



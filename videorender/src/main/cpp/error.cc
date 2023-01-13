
#include "error.h"

#define STRUCT_ARRAY_ELEMS(a)       (sizeof(a) / sizeof((a)[0]))

namespace media {

/// type = OPENGL
int Error::OPENGL_DISPLAY_ERROR = -100;
int Error::OPENGL_INITIALIZE_ERROR = -101;
int Error::OPENGL_CONFIG_ERROR = -102;
int Error::OPENGL_CREATE_CONTEXT_ERROR = -103;
int Error::OPENGL_GET_TIME_ERROR = -104;
int Error::OPENGL_SURFACE_INVALID = -105;
int Error::OPENGL_MAKE_CURRENT_ERROR = -106;
int Error::OPENGL_EGL_CORE_NULL = -107;
int Error::NATIVE_WINDOW_NULL = -108;
int Error::CREATE_PROGRAM_ERROR = -109;

/// type = EFFECT
int Error::EXPORT_PARSE_INPUT_CONFIG_ERROR = -200;
int Error::FILE_NOT_EXIST = -201;
int Error::AUDIO_PLAYER_MAP_EMPTY = -202;
int Error::PARSE_CONFIG_FILE_ERROR = -203;
int Error::ADD_FILTER_CONFIG_ERROR = -204;

/// type = AUDIO_PLAYER
int Error::AUDIO_PLAYER_REPEAT_ERROR = -300;
int Error::AUDIO_DECODE_INSTANCE_NULL = -301;
int Error::AUDIO_RENDER_INIT_ERROR = -302;
int Error::AUDIO_DECODE_FRAME_ERROR = -303;
int Error::AUDIO_DECODE_DATA_ERROR = -304;
int Error::CREATE_OPENSL_ENGINE_ERROR = -305;
int Error::OPENSL_INITIALIZE_ERROR = -306;
int Error::OPENSL_OBJECT_NULL_ERROR = -307;
int Error::OPENSL_BUFFER_QUEUE_EMPTY = -308;
int Error::AUDIO_PLAYER_PREPARE_ERROR = -309;
int Error::AUDIO_PLAYER_ADD_REPEAT = -310;

/// type = FFMPEG
int Error::FFMPEG_FORMAT_CONTEXT_INVALID = -400;
int Error::FFMPEG_OPEN_INPUT_ERROR = -401;
int Error::FFMPEG_FIND_STREAM_INFO_ERROR = -402;
int Error::FIND_AUDIO_STREAM_ERROR = -403;
int Error::FIND_AUDIO_DECODE_CODEC_ERROR = -404;
int Error::OPEN_AUDIO_CODEC_ERROR = -405;
int Error::DECODE_AUDIO_INFO_INVALID = -406;
int Error::INIT_VIDEO_DECODE_ERROR = -407;
int Error::INIT_AUDIO_DECODE_ERROR = -408;
int Error::FIND_FFMPEG_AUDIO_ENCODE_ERROR = -409;
int Error::OPEN_FFMPEG_AUDIO_CODEC_ERROR = -410;
int Error::FIND_FFMPEG_VIDEO_ENCODE_ERROR = -411;
int Error::CREATE_FFMPEG_VIDEO_CODEC_CONTEXT_ERROR = -412;
int Error::OPEN_FFMPEG_VIDEO_CODEC_ERROR = -413;
int Error::FIND_FFMPEG_AUDIO_DECODE_ERROR = -414;
int Error::FFMPEG_SWR_CONTEXT_NULL = -415;
int Error::CREATE_FFMPEG_OUTPUT_CONTEXT_ERROR = -416;
int Error::CREATE_FFMPEG_OUTPUT_CONTEXT_NULL = -417;
int Error::CREATE_FFMPEG_VIDEO_STREAM_ERROR = -418;
int Error::CREATE_VIDEO_CODEC_CONTEXT_ERROR = -419;
int Error::CREATE_FFMPEG_AUDIO_STREAM_ERROR = -420;
int Error::CREATE_AUDIO_CODEC_CONTEXT_ERROR = -421;
int Error::GET_ADTS_FILTER_ERROR = -422;
int Error::CREATE_ADTS_FILTER_ERROR = -423;
int Error::COPY_AUDIO_INFO_TO_ADTS_FILTER_ERROR = -424;
int Error::INIT_ADTS_FILTER_ERROR = -425;

/// type = RECORD
int Error::RECORD_VIDEO_INFO_ERROR = -500;
int Error::RECORD_AUDIO_INFO_ERROR = -501;
int Error::TAKE_PHOTO_REPEAT_ERROR = -502;
int Error::RECORD_JNI_ENV_NULL = -503;

/// type = ENCODE
int Error::CODEC_CONTEXT_NULL = -600;
int Error::CODEC_FRAME_NULL = -601;
int Error::AUDIO_SAMPLE_SIZE_ERROR = -602;
int Error::AUDIO_SAMPLE_RATE_ZERO = -603;
int Error::AUDIO_ENCODE_STATUS_ERROR = -604;
int Error::AUDIO_ENCODE_POOL_NULL = -605;
int Error::AUDIO_ENCODE_PACKET_NULL = -606;
int Error::ENCODE_JNI_ENV_NULL = -607;
int Error::ENCODE_JNI_OBJECT_NULL = -608;
int Error::CREATE_MEDIACODEC_ERROR = -609;
int Error::GET_MEDIACODEC_SURFACE_ERROR = -610;

/// type = DECODE
int Error::THREAD_MUTEX_INIT_ERROR = -700;
int Error::THREAD_COND_INIT_ERROR = -701;
int Error::DECODE_QUEUE_STATUS_ERROR = -702;
int Error::DECODE_CONTEXT_NULL = -703;
int Error::DECODE_JNI_ENV_NULL = -704;
int Error::DECODE_JNI_OBJECT_NULL = -705;
int Error::VIDEO_TIME_OUT_OF_RANGE = -706;
int Error::UPDATE_CLIP_INVALID = -707;
int Error::SWAP_CLIP_ERROR = -708;
int Error::VIDEO_INDEX_OUT_OF_RANGE = -709;
int Error::VIDEO_CLIP_MAP_EMPTY = -710;
int Error::CLIP_INDEX_OUT_OF_RANGE = -711;
int Error::SEEK_TIME_OUT_OF_RANGE = -712;
int Error::PARSE_CLIP_FILE_ERROR = -713;
int Error::OPEN_IMAGE_ERROR = -714;
int Error::DECODE_PACKET_NULL = -715;
int Error::MEDIACODEC_INDEX_INVALID = -716;
int Error::SPS_DATA_INVALID = -717;
int Error::INSERT_CLIP_NULL = -718;
int Error::INSERT_CLIP_PATH_NULL = -719;
int Error::INSERT_CLIP_NOT_EXIST = -720;
int Error::CLONE_MUSIC_ERROR = -721;

/// type = EXPORT
int Error::EXPORT_VIDEO_ENCODE_INFO_INVALID = -800;
int Error::EXPORT_AUDIO_ENCODE_INFO_INVALID = -801;
int Error::EXPORT_CLIP_PATH_EMPTY = -802;
int Error::EXPORT_EFFECT_CONFIG_EMPTY = -803;
int Error::EXPORT_MUSIC_PATH_EMPTY = -804;
int Error::EXPORT_WORK_IS_RUNNING = -805;
int Error::EXPORT_FILE_PATH_INVALID = -806;
int Error::EXPORT_DECODE_CONTROLLER_INVALID = -807;
int Error::EXPORT_DURATION_EXCEPTION = -808;
int Error::EXPORT_PIP_PATH_EMPTY = -809;

/// type = MUXER
int Error::ACCESS_FILE_ERROR = -900;
int Error::OPEN_FILE_ERROR = -901;

/// type = EXTRACTOR
int Error::EXTRACTOR_JNI_ENV_NULL = -1000;
int Error::NO_AUDIO_STREAM_INDEX = -1001;
int Error::NO_SUPPORT_AAC_CODEC = -1002;
int Error::ALLOC_AUDIO_OUTPUT_STREAM_ERROR = -1003;

/// type = COMPOSITE
int Error::COMPOSITE_JNI_ENV_NULL = -1100;
int Error::ALLOC_OUTPUT_STREAM_ERROR = -1101;

/// type = TEXT
int Error::TEXT_FRAME_SIZE_ERROR = -1200;
int Error::TEXT_BITMAP_DATA_ERROR = -1201;

/// type = IMAGE
int Error::CAPTURE_IMAGE_INDEX_EXCEPTION = -1300;
int Error::CAPTURE_IMAGE_WH_EXCEPTION = -1301;

/// type = OTHER
int Error::UNKNOWN_ERROR = -10000;
int Error::FRAME_CAPTURE_SIZE_EXCEPTION = -10001;
int Error::LOCK_BITMAP_DATA_ERROR = -10002;
int Error::CAPTURE_FRAME_SIZE_ERROR = -10003;

static const Error::Info infos[] = {
    /// type = OPENGL
    { Error::Type::OPENGL,       Error::OPENGL_DISPLAY_ERROR,                    "eglGetDisplay failed" },
    { Error::Type::OPENGL,       Error::OPENGL_INITIALIZE_ERROR,                 "eglInitialize failed" },
    { Error::Type::OPENGL,       Error::OPENGL_CONFIG_ERROR,                     "eglChooseConfig failed" },
    { Error::Type::OPENGL,       Error::OPENGL_CREATE_CONTEXT_ERROR,             "eglCreateContext failed" },
    { Error::Type::OPENGL,       Error::OPENGL_GET_TIME_ERROR,                   "eglPresentationTimeANDROID failed" },
    { Error::Type::OPENGL,       Error::OPENGL_SURFACE_INVALID,                  "CreateWindowSurface failed" },
    { Error::Type::OPENGL,       Error::OPENGL_MAKE_CURRENT_ERROR,               "MakeCurrent failed" },
    { Error::Type::OPENGL,       Error::OPENGL_EGL_CORE_NULL,                    "eglCore instance is null" },
    { Error::Type::OPENGL,       Error::NATIVE_WINDOW_NULL,                      "native window is null" },
    { Error::Type::OPENGL,       Error::CREATE_PROGRAM_ERROR,                    "link vertex and fragment shader failed" },

    /// type = EFFECT
    { Error::Type::EFFECT,       Error::EXPORT_PARSE_INPUT_CONFIG_ERROR,         "parse local config file failed" },
    { Error::Type::EFFECT,       Error::FILE_NOT_EXIST,                          "config file has no json path info" },
    { Error::Type::EFFECT,       Error::AUDIO_PLAYER_MAP_EMPTY,                  "cannot update music because of the missing player" },
    { Error::Type::EFFECT,       Error::PARSE_CONFIG_FILE_ERROR,                 "parse music config file failed" },
    { Error::Type::EFFECT,       Error::ADD_FILTER_CONFIG_ERROR,                 "add filter config failed" },

    /// type = AUDIO_PLAYER
    { Error::Type::AUDIO_PLAYER, Error::AUDIO_PLAYER_REPEAT_ERROR,               "music player is running" },
    { Error::Type::AUDIO_PLAYER, Error::AUDIO_DECODE_INSTANCE_NULL,              "music decode instance is null" },
    { Error::Type::AUDIO_PLAYER, Error::AUDIO_RENDER_INIT_ERROR,                 "opensl render engine init failed" },
    { Error::Type::AUDIO_PLAYER, Error::AUDIO_DECODE_FRAME_ERROR,                "music decode getFrame failed" },
    { Error::Type::AUDIO_PLAYER, Error::AUDIO_DECODE_DATA_ERROR,                 "music decode getData failed" },
    { Error::Type::AUDIO_PLAYER, Error::CREATE_OPENSL_ENGINE_ERROR,              "opensl render engine create failed" },
    { Error::Type::AUDIO_PLAYER, Error::OPENSL_INITIALIZE_ERROR,                 "opensl initialization failed" },
    { Error::Type::AUDIO_PLAYER, Error::OPENSL_OBJECT_NULL_ERROR,                "opensl create object failed" },
    { Error::Type::AUDIO_PLAYER, Error::OPENSL_BUFFER_QUEUE_EMPTY,               "opensl bufferQueue is empty" },
    { Error::Type::AUDIO_PLAYER, Error::AUDIO_PLAYER_PREPARE_ERROR,              "music player prepare failed" },

    /// type = FFMPEG
    { Error::Type::FFMPEG,       Error::FFMPEG_FORMAT_CONTEXT_INVALID,           "create avformat_context failed" },
    { Error::Type::FFMPEG,       Error::FFMPEG_OPEN_INPUT_ERROR,                 "avformat_open_input failed" },
    { Error::Type::FFMPEG,       Error::FFMPEG_FIND_STREAM_INFO_ERROR,           "avformat_find_stream_info failed" },
    { Error::Type::FFMPEG,       Error::FIND_AUDIO_STREAM_ERROR,                 "cannot find audio stream index info" },
    { Error::Type::FFMPEG,       Error::FIND_AUDIO_DECODE_CODEC_ERROR,           "cannot find audio codec id" },
    { Error::Type::FFMPEG,       Error::OPEN_AUDIO_CODEC_ERROR,                  "avcodec_open2 audio codec failed" },
    { Error::Type::FFMPEG,       Error::DECODE_AUDIO_INFO_INVALID,               "audio's channel_count or sample_rate is invalid" },
    { Error::Type::FFMPEG,       Error::INIT_VIDEO_DECODE_ERROR,                 "avcodec_open2 video codec failed" },
    { Error::Type::FFMPEG,       Error::INIT_AUDIO_DECODE_ERROR,                 "avcodec_open2 audio codec failed" },
    { Error::Type::FFMPEG,       Error::FIND_FFMPEG_AUDIO_ENCODE_ERROR,          "cannot find audio's codec by the codec name" },
    { Error::Type::FFMPEG,       Error::OPEN_FFMPEG_AUDIO_CODEC_ERROR,           "open audio codec failed" },
    { Error::Type::FFMPEG,       Error::FIND_FFMPEG_VIDEO_ENCODE_ERROR,          "cannot find video's encoded codec by the codec name" },
    { Error::Type::FFMPEG,       Error::CREATE_FFMPEG_VIDEO_CODEC_CONTEXT_ERROR, "avcodec alloc h264 context failed" },
    { Error::Type::FFMPEG,       Error::OPEN_FFMPEG_VIDEO_CODEC_ERROR,           "avcodec open h264 failed" },
    { Error::Type::FFMPEG,       Error::FIND_FFMPEG_AUDIO_DECODE_ERROR,          "cannot find audio's decoded codec by the codec name" },
    { Error::Type::FFMPEG,       Error::FFMPEG_SWR_CONTEXT_NULL,                 "init swr context failed, swr context is null" },
    { Error::Type::FFMPEG,       Error::CREATE_FFMPEG_OUTPUT_CONTEXT_ERROR,      "avformat_alloc_output_context2 failed" },
    { Error::Type::FFMPEG,       Error::CREATE_FFMPEG_OUTPUT_CONTEXT_NULL,       "avformat_alloc_output_context2 output context is null" },
    { Error::Type::FFMPEG,       Error::CREATE_FFMPEG_VIDEO_STREAM_ERROR,        "avformat_new_stream video stream failed" },
    { Error::Type::FFMPEG,       Error::CREATE_VIDEO_CODEC_CONTEXT_ERROR,        "avcodec_alloc_context3 video codec failed" },
    { Error::Type::FFMPEG,       Error::CREATE_FFMPEG_AUDIO_STREAM_ERROR,        "avformat_new_stream audio stream failed" },
    { Error::Type::FFMPEG,       Error::CREATE_AUDIO_CODEC_CONTEXT_ERROR,        "alloc audio codec context failed" },
    { Error::Type::FFMPEG,       Error::GET_ADTS_FILTER_ERROR,                   "get adts filter failed" },
    { Error::Type::FFMPEG,       Error::CREATE_ADTS_FILTER_ERROR,                "create adts filter failed" },
    { Error::Type::FFMPEG,       Error::COPY_AUDIO_INFO_TO_ADTS_FILTER_ERROR,    "avcodec_parameters_copy failed within muxing" },
    { Error::Type::FFMPEG,       Error::INIT_ADTS_FILTER_ERROR,                  "init adts filter failed" },

    /// type = RECORD
    { Error::Type::RECORD,       Error::RECORD_VIDEO_INFO_ERROR,                 "the video's width or height or frame-rate is invalid" },
    { Error::Type::RECORD,       Error::RECORD_AUDIO_INFO_ERROR,                 "the audio's sample-size or sample-rate is invalid" },
    { Error::Type::RECORD,       Error::TAKE_PHOTO_REPEAT_ERROR,                 "cannot take photo repeatedly" },
    { Error::Type::RECORD,       Error::RECORD_JNI_ENV_NULL,                     "record jni env is null" },

    /// type = ENCODE
    { Error::Type::ENCODE,       Error::CODEC_CONTEXT_NULL,                      "the audio codec context is null" },
    { Error::Type::ENCODE,       Error::CODEC_FRAME_NULL,                        "the audio encode frame is null" },
    { Error::Type::ENCODE,       Error::AUDIO_SAMPLE_SIZE_ERROR,                 "the audio's sample-size is invalid" },
    { Error::Type::ENCODE,       Error::AUDIO_SAMPLE_RATE_ZERO,                  "the audio's sample-rate is invalid" },
    { Error::Type::ENCODE,       Error::AUDIO_ENCODE_STATUS_ERROR,               "cannot encode repeatedly" },
    { Error::Type::ENCODE,       Error::AUDIO_ENCODE_POOL_NULL,                  "audio queue pool is null" },
    { Error::Type::ENCODE,       Error::AUDIO_ENCODE_PACKET_NULL,                "audio queue pool has no packet" },
    { Error::Type::ENCODE,       Error::ENCODE_JNI_ENV_NULL,                     "mediacodec jni env is null" },
    { Error::Type::ENCODE,       Error::ENCODE_JNI_OBJECT_NULL,                  "mediacodec jni jobject is null" },
    { Error::Type::ENCODE,       Error::CREATE_MEDIACODEC_ERROR,                 "create mediacodec instance failed" },
    { Error::Type::ENCODE,       Error::GET_MEDIACODEC_SURFACE_ERROR,            "mediacodec getencodersurface is null" },

    /// type = DECODE
    { Error::Type::DECODE,       Error::THREAD_MUTEX_INIT_ERROR,                 "queue thread mutex init failed" },
    { Error::Type::DECODE,       Error::THREAD_COND_INIT_ERROR,                  "queue thread cond init failed" },
    { Error::Type::DECODE,       Error::DECODE_QUEUE_STATUS_ERROR,               "cannot put the element because of the abort request" },
    { Error::Type::DECODE,       Error::DECODE_CONTEXT_NULL,                     "mediacodec context is null" },
    { Error::Type::DECODE,       Error::DECODE_JNI_ENV_NULL,                     "the decode jni env is null" },
    { Error::Type::DECODE,       Error::DECODE_JNI_OBJECT_NULL,                  "the decode jni jobject is null" },
    { Error::Type::DECODE,       Error::VIDEO_TIME_OUT_OF_RANGE,                 "the clip end time is out of range" },
    { Error::Type::DECODE,       Error::UPDATE_CLIP_INVALID,                     "cannot update the clip because of the out-of-range index" },
    { Error::Type::DECODE,       Error::SWAP_CLIP_ERROR,                         "cannot swap the clip because of the out-of-range index" },
    { Error::Type::DECODE,       Error::VIDEO_INDEX_OUT_OF_RANGE,                "the video index is out of range" },
    { Error::Type::DECODE,       Error::VIDEO_CLIP_MAP_EMPTY,                    "the clip map is empty" },
    { Error::Type::DECODE,       Error::CLIP_INDEX_OUT_OF_RANGE,                 "the clip index is out of range" },
    { Error::Type::DECODE,       Error::SEEK_TIME_OUT_OF_RANGE,                  "the seek time is out of range" },
    { Error::Type::DECODE,       Error::PARSE_CLIP_FILE_ERROR,                   "the clip file path is invalid" },
    { Error::Type::DECODE,       Error::OPEN_IMAGE_ERROR,                        "video frame's width or height is invalid" },
    { Error::Type::DECODE,       Error::DECODE_PACKET_NULL,                      "the sending packet is null" },
    { Error::Type::DECODE,       Error::MEDIACODEC_INDEX_INVALID,                "cannot find mediacodec outputbufferindex" },
    { Error::Type::DECODE,       Error::SPS_DATA_INVALID,                        "cannot find the valid sps data" },

    /// type = EXPORT
    { Error::Type::EXPORT,       Error::EXPORT_VIDEO_ENCODE_INFO_INVALID,        "video's width or height or frame_rate is invalid" },
    { Error::Type::EXPORT,       Error::EXPORT_AUDIO_ENCODE_INFO_INVALID,        "audio's channel_count or sample_rate is invalid" },
    { Error::Type::EXPORT,       Error::EXPORT_CLIP_PATH_EMPTY,                  "cannot find clip config path" },
    { Error::Type::EXPORT,       Error::EXPORT_EFFECT_CONFIG_EMPTY,              "cannot find effect config path" },
    { Error::Type::EXPORT,       Error::EXPORT_MUSIC_PATH_EMPTY,                 "cannot find music config path" },
    { Error::Type::EXPORT,       Error::EXPORT_DECODE_CONTROLLER_INVALID,        "cannot find decode controller" },
    { Error::Type::EXPORT,       Error::EXPORT_DURATION_EXCEPTION,               "video duration is out-of-range" },
    { Error::Type::EXPORT,       Error::EXPORT_PIP_PATH_EMPTY,                   "cannot find pip config path" },

    /// type = MUXER
    { Error::Type::MUXER,        Error::ACCESS_FILE_ERROR,                       "video file path is invalid" },
    { Error::Type::MUXER,        Error::OPEN_FILE_ERROR,                         "open file failed because of the permission" },

    /// type = EXTRACTOR
    { Error::Type::EXTRACTOR,    Error::EXTRACTOR_JNI_ENV_NULL,                  "get extractor jni env failed" },
    { Error::Type::EXTRACTOR,    Error::NO_AUDIO_STREAM_INDEX,                   "cannot find audio stream id" },
    { Error::Type::EXTRACTOR,    Error::NO_SUPPORT_AAC_CODEC,                    "cannot support aac codec id" },
    { Error::Type::EXTRACTOR,    Error::ALLOC_AUDIO_OUTPUT_STREAM_ERROR,         "avformat_new_stream audio stream failed" },

    /// type = COMPOSITE
    { Error::Type::COMPOSITE,    Error::COMPOSITE_JNI_ENV_NULL,                  "get composite jni env failed" },
    { Error::Type::COMPOSITE,    Error::ALLOC_OUTPUT_STREAM_ERROR,               "avformat_new_stream failed" },

    /// type = TEXT
    { Error::Type::TEXT,         Error::TEXT_FRAME_SIZE_ERROR,                   "Capture text frame width or height is error" },
    { Error::Type::TEXT,         Error::TEXT_BITMAP_DATA_ERROR,                  "Lock bitmap failed" },

    /// type = IMAGE
    { Error::Type::IMAGE,        Error::CAPTURE_IMAGE_INDEX_EXCEPTION,            "Capture image index is out-of-range" },
    { Error::Type::IMAGE,        Error::CAPTURE_IMAGE_WH_EXCEPTION,               "Capture image width or height failed" },

    /// type = OTHER
    { Error::Type::OTHER,        Error::UNKNOWN_ERROR,                           "unknown error" },
    { Error::Type::OTHER,        Error::FRAME_CAPTURE_SIZE_EXCEPTION,            "Capture frame width or height is error" },
    { Error::Type::OTHER,        Error::LOCK_BITMAP_DATA_ERROR,                  "Lock bitmap failed" },
    { Error::Type::OTHER,        Error::CAPTURE_FRAME_SIZE_ERROR,                "Capture frame size is out-of-range" },
};

const Error::Info *Error::GetErrorInfo(int err_code) {
  const struct Error::Info *info = nullptr;
  for (int i = 0; i < STRUCT_ARRAY_ELEMS(infos); i++) {
    if (err_code == infos[i].code) {
      info = &infos[i];
      break;
    }
  }
  if (info == nullptr) {
    info = new Error::Info{Type::OTHER, Error::UNKNOWN_ERROR, "unknown error"};
  }
  return info;
}

}  // namespace media
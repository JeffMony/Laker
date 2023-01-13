
#ifndef ERROR_H_
#define ERROR_H_

namespace media {

class Error {
 public:
  enum Type {
    OPENGL = 0,
    EFFECT,
    AUDIO_PLAYER,
    FFMPEG,
    RECORD,
    ENCODE,
    DECODE,
    EXPORT,
    MUXER,
    EXTRACTOR,
    COMPOSITE,
    TEXT,
    IMAGE,
    OTHER = 20,
  };

  /// type = OPENGL
  static int OPENGL_DISPLAY_ERROR;
  static int OPENGL_INITIALIZE_ERROR;
  static int OPENGL_CONFIG_ERROR;
  static int OPENGL_CREATE_CONTEXT_ERROR;
  static int OPENGL_GET_TIME_ERROR;
  static int OPENGL_SURFACE_INVALID;
  static int OPENGL_MAKE_CURRENT_ERROR;
  static int OPENGL_EGL_CORE_NULL;
  static int NATIVE_WINDOW_NULL;
  static int CREATE_PROGRAM_ERROR;

  /// type = EFFECT
  static int EXPORT_PARSE_INPUT_CONFIG_ERROR;
  static int FILE_NOT_EXIST;
  static int AUDIO_PLAYER_MAP_EMPTY;
  static int PARSE_CONFIG_FILE_ERROR;
  static int ADD_FILTER_CONFIG_ERROR;

  /// type = AUDIO_PLAYER
  static int AUDIO_PLAYER_REPEAT_ERROR;
  static int AUDIO_DECODE_INSTANCE_NULL;
  static int AUDIO_RENDER_INIT_ERROR;
  static int AUDIO_DECODE_FRAME_ERROR;
  static int AUDIO_DECODE_DATA_ERROR;
  static int CREATE_OPENSL_ENGINE_ERROR;
  static int OPENSL_INITIALIZE_ERROR;
  static int OPENSL_OBJECT_NULL_ERROR;
  static int OPENSL_BUFFER_QUEUE_EMPTY;
  static int AUDIO_PLAYER_PREPARE_ERROR;
  static int AUDIO_PLAYER_ADD_REPEAT;

  /// type = FFMPEG
  static int FFMPEG_FORMAT_CONTEXT_INVALID;
  static int FFMPEG_OPEN_INPUT_ERROR;
  static int FFMPEG_FIND_STREAM_INFO_ERROR;
  static int FIND_AUDIO_STREAM_ERROR;
  static int FIND_AUDIO_DECODE_CODEC_ERROR;
  static int OPEN_AUDIO_CODEC_ERROR;
  static int DECODE_AUDIO_INFO_INVALID;
  static int INIT_VIDEO_DECODE_ERROR;
  static int INIT_AUDIO_DECODE_ERROR;
  static int FIND_FFMPEG_AUDIO_ENCODE_ERROR;
  static int OPEN_FFMPEG_AUDIO_CODEC_ERROR;
  static int FIND_FFMPEG_VIDEO_ENCODE_ERROR;
  static int CREATE_FFMPEG_VIDEO_CODEC_CONTEXT_ERROR;
  static int OPEN_FFMPEG_VIDEO_CODEC_ERROR;
  static int FIND_FFMPEG_AUDIO_DECODE_ERROR;
  static int FFMPEG_SWR_CONTEXT_NULL;
  static int CREATE_FFMPEG_OUTPUT_CONTEXT_ERROR;
  static int CREATE_FFMPEG_OUTPUT_CONTEXT_NULL;
  static int CREATE_FFMPEG_VIDEO_STREAM_ERROR;
  static int CREATE_VIDEO_CODEC_CONTEXT_ERROR;
  static int CREATE_FFMPEG_AUDIO_STREAM_ERROR;
  static int CREATE_AUDIO_CODEC_CONTEXT_ERROR;
  static int GET_ADTS_FILTER_ERROR;
  static int CREATE_ADTS_FILTER_ERROR;
  static int COPY_AUDIO_INFO_TO_ADTS_FILTER_ERROR;
  static int INIT_ADTS_FILTER_ERROR;

  /// type = RECORD
  static int RECORD_VIDEO_INFO_ERROR;
  static int RECORD_AUDIO_INFO_ERROR;
  static int TAKE_PHOTO_REPEAT_ERROR;
  static int RECORD_JNI_ENV_NULL;

  /// type = ENCODE
  static int CODEC_CONTEXT_NULL;
  static int CODEC_FRAME_NULL;
  static int AUDIO_SAMPLE_SIZE_ERROR;
  static int AUDIO_SAMPLE_RATE_ZERO;
  static int AUDIO_ENCODE_STATUS_ERROR;
  static int AUDIO_ENCODE_POOL_NULL;
  static int AUDIO_ENCODE_PACKET_NULL;
  static int ENCODE_JNI_ENV_NULL;
  static int ENCODE_JNI_OBJECT_NULL;
  static int CREATE_MEDIACODEC_ERROR;
  static int GET_MEDIACODEC_SURFACE_ERROR;

  /// type = DECODE
  static int THREAD_MUTEX_INIT_ERROR;
  static int THREAD_COND_INIT_ERROR;
  static int DECODE_QUEUE_STATUS_ERROR;
  static int DECODE_CONTEXT_NULL;
  static int DECODE_JNI_ENV_NULL;
  static int DECODE_JNI_OBJECT_NULL;
  static int VIDEO_TIME_OUT_OF_RANGE;
  static int UPDATE_CLIP_INVALID;
  static int SWAP_CLIP_ERROR;
  static int VIDEO_INDEX_OUT_OF_RANGE;
  static int VIDEO_CLIP_MAP_EMPTY;
  static int CLIP_INDEX_OUT_OF_RANGE;
  static int SEEK_TIME_OUT_OF_RANGE;
  static int PARSE_CLIP_FILE_ERROR;
  static int OPEN_IMAGE_ERROR;
  static int DECODE_PACKET_NULL;
  static int MEDIACODEC_INDEX_INVALID;
  static int SPS_DATA_INVALID;
  static int INSERT_CLIP_NULL;
  static int INSERT_CLIP_PATH_NULL;
  static int INSERT_CLIP_NOT_EXIST;
  static int CLONE_MUSIC_ERROR;

  /// type = EXPORT
  static int EXPORT_VIDEO_ENCODE_INFO_INVALID;
  static int EXPORT_AUDIO_ENCODE_INFO_INVALID;
  static int EXPORT_CLIP_PATH_EMPTY;
  static int EXPORT_EFFECT_CONFIG_EMPTY;
  static int EXPORT_MUSIC_PATH_EMPTY;
  static int EXPORT_WORK_IS_RUNNING;
  static int EXPORT_FILE_PATH_INVALID;
  static int EXPORT_DECODE_CONTROLLER_INVALID;
  static int EXPORT_DURATION_EXCEPTION;
  static int EXPORT_PIP_PATH_EMPTY;

  /// type = MUXER
  static int ACCESS_FILE_ERROR;
  static int OPEN_FILE_ERROR;

  /// type = EXTRACTOR
  static int EXTRACTOR_JNI_ENV_NULL;
  static int NO_AUDIO_STREAM_INDEX;
  static int NO_SUPPORT_AAC_CODEC;
  static int ALLOC_AUDIO_OUTPUT_STREAM_ERROR;

  /// type = COMPOSITE
  static int COMPOSITE_JNI_ENV_NULL;
  static int ALLOC_OUTPUT_STREAM_ERROR;

  /// type = TEXT
  static int TEXT_FRAME_SIZE_ERROR;
  static int TEXT_BITMAP_DATA_ERROR;

  /// type = IMAGE
  static int CAPTURE_IMAGE_INDEX_EXCEPTION;
  static int CAPTURE_IMAGE_WH_EXCEPTION;

  /// type = OTHER
  static int UNKNOWN_ERROR;
  static int FRAME_CAPTURE_SIZE_EXCEPTION;
  static int LOCK_BITMAP_DATA_ERROR;
  static int CAPTURE_FRAME_SIZE_ERROR;

  struct Info {
    int type;
    int code;
    const char *msg;
  };

  static const Error::Info *GetErrorInfo(int err_code);
};

}  // namespace media

#endif  // ERROR_H_
C3_LIBS_PATH:=$(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Graphic
LOCAL_SRC_FILES := $(C3_LIBS_PATH)/libGraphic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := GraphicData
LOCAL_SRC_FILES := $(C3_LIBS_PATH)/libGraphicData.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Role3D
LOCAL_SRC_FILES := $(C3_LIBS_PATH)/libRole3D.a
include $(PREBUILT_STATIC_LIBRARY)
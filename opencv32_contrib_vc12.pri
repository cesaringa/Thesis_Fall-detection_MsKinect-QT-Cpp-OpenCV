OPENCV_PATH = C:/LIBs/OpenCV/3.2_contrib/install1
INCLUDEPATH += $$OPENCV_PATH/include
LIBS += -L$$OPENCV_PATH/x64/vc12/lib

LIBS += -lopencv_aruco320
LIBS += -lopencv_bgsegm320
LIBS += -lopencv_bioinspired320
LIBS += -lopencv_calib3d320
LIBS += -lopencv_ccalib320
LIBS += -lopencv_core320
LIBS += -lopencv_datasets320
LIBS += -lopencv_dnn320
LIBS += -lopencv_dpm320
LIBS += -lopencv_face320
LIBS += -lopencv_features2d320
LIBS += -lopencv_flann320
LIBS += -lopencv_fuzzy320
LIBS += -lopencv_highgui320
LIBS += -lopencv_imgcodecs320
LIBS += -lopencv_imgproc320
LIBS += -lopencv_line_descriptor320
LIBS += -lopencv_ml320
LIBS += -lopencv_objdetect320
LIBS += -lopencv_optflow320
LIBS += -lopencv_phase_unwrapping320
LIBS += -lopencv_photo320
LIBS += -lopencv_plot320
LIBS += -lopencv_reg320
LIBS += -lopencv_rgbd320
LIBS += -lopencv_saliency320
LIBS += -lopencv_shape320
LIBS += -lopencv_stereo320
LIBS += -lopencv_stitching320
LIBS += -lopencv_structured_light320
LIBS += -lopencv_superres320
LIBS += -lopencv_surface_matching320
LIBS += -lopencv_text320
LIBS += -lopencv_tracking320
LIBS += -lopencv_video320
LIBS += -lopencv_videoio320
LIBS += -lopencv_videostab320
LIBS += -lopencv_viz320
LIBS += -lopencv_xfeatures2d320
LIBS += -lopencv_ximgproc320
LIBS += -lopencv_xobjdetect320
LIBS += -lopencv_xphoto320

!build_pass:message(LIBS was set to $${LIBS})
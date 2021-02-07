set(SOURCES
    RFBProto.cpp
)

serenity_lib(LibRFB rfb)
target_link_libraries(LibRFB LibCore)

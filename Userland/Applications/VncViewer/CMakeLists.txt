set(SOURCES
    VncViewerAppWindow.cpp
    VncClient.cpp
    main.cpp
)

serenity_app(VncViewer ICON app-irc-client)
target_link_libraries(VncViewer LibRFB LibWeb LibGUI)

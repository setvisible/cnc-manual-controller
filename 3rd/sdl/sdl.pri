# =============================================
# Simple DirectMedia Layer (SDL)
# =============================================
#

# Joystick Input Device
INCLUDEPATH += $$PWD/SDL-1.2.15/include/

LIBS += -L$$PWD/SDL-1.2.15/lib/x86 -lSDL

win32 {
   DEFINES += SDL_WIN
}

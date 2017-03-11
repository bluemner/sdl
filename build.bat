@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

set compilerflags=/Fo.\bin\ /Od /Zi /EHsc 

set linkerflags=/OUT:bin\hello.exe
REM LIBS
set lib_open_gl="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\um\x64"
set lib_sdl="L:\SDL2-2.0.5\lib\x64"
REM Includes
set include_open_gl="C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\um\gl"
set include_sdl="L:\SDL2-2.0.5\include"

cl.exe %compilerflags% lesson_0001/driver.cpp /I%include_sdl%  /I%include_open_gl% /DYNAMICBASE SDL2.lib OpenGL32.lib GLU32.lib  /link /LIBPATH:%lib_sdl% /LIBPATH:%lib_open_gl%  %linkerflags% /SUBSYSTEM:WINDOWS /ENTRY:"mainCRTStartup"

set linkerflags=/OUT:bin\driver.exe
cl.exe %compilerflags% lesson_0002/driver.cpp /I%include_sdl%  /I%include_open_gl% /DYNAMICBASE SDL2.lib OpenGL32.lib GLU32.lib  /link /LIBPATH:%lib_sdl% /LIBPATH:%lib_open_gl%  %linkerflags% /SUBSYSTEM:WINDOWS /ENTRY:"mainCRTStartup"


set file=graph
echo building %file%
set folder=graph
set linkerflags=/OUT:bin\%file%.exe
cl.exe %compilerflags% %folder%/%file%.cpp /I%include_sdl%  /I%include_open_gl% /DYNAMICBASE SDL2.lib OpenGL32.lib GLU32.lib /link /LIBPATH:%lib_sdl% /LIBPATH:%lib_open_gl%  %linkerflags% /SUBSYSTEM:WINDOWS /ENTRY:"mainCRTStartup"

REM
REM
REM

set file=window
echo building %file%

set linkerflags=/OUT:bin\%file%.exe
cl.exe %compilerflags%  basic_window/%file%.cpp /I%include_open_gl% /I%include_sdl% /DYNAMICBASE GLU32.lib SDL2.lib /link /LIBPATH:%lib_open_gl% /LIBPATH:%lib_sdl% %linkerflags% /SUBSYSTEM:WINDOWS /ENTRY:"mainCRTStartup"
echo done
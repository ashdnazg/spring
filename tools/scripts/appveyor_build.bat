:Setup
  PUSHD %root%
    ECHO Cloning vclibs
    git clone https://github.com/spring/vclibs14 vclibs > NUL
    ECHO Cloning pr-downloader
    git clone https://github.com/spring/pr-downloader > NUL
  POPD
  
:Generate
  PUSHD %build_dir%
    cmake ..
    msbuild Spring.sln /t:Build /p:Configuration=%configuration% /m:2 /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
  POPD
EXIT /b

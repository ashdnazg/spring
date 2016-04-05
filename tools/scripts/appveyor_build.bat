SET "root=%CD%"
SET "build_dir=%root%\build"
MD %build_dir%

:Setup
  PUSHD %root%
    ECHO Cloning vclibs
    git clone -q https://github.com/spring/vclibs14 vclibs
    ECHO Cloning pr-downloader
    git clone -q https://github.com/spring/pr-downloader
  POPD
  
:Generate
  PUSHD %build_dir%
    cmake .. -DAI_TYPES=NATIVE -DBOOST_ROOT=C:/Libraries/boost_1_59_0
    msbuild Spring.sln /target:engineSim /p:Configuration=%configuration% /m:2 /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
  POPD
EXIT /b

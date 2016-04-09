SET "root=%CD%"
SET "build_dir=%root%\build"
MD %build_dir%

:Setup
  PUSHD %root%
    ECHO Cloning vclibs
    git clone -q https://github.com/spring/vclibs14 vclibs
    git submodule update --init --recursive
  POPD
  
:Generate
  PUSHD %build_dir%
    cmake .. -DAI_TYPES=NATIVE -DBOOST_INCLUDEDIR=C:/Libraries/boost_1_59_0/include
    msbuild Spring.sln /target:engine-headless /p:Configuration=%configuration% /p:DebugSymbols=false /p:DebugType=None /p:WarningLevel=3 /m:2
  POPD
EXIT /b

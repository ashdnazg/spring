SET "root=%CD%"
SET "build_dir=%root%\build"
MD %build_dir%

:Setup
  PUSHD %root%
    ECHO Cloning vclibs
    git clone -q https://github.com/spring/vclibs14 vclibs
  POPD
  
:Generate
  PUSHD %build_dir%
    cmake .. -DAI_TYPES=NATIVE -DBOOST_INCLUDEDIR=C:/Libraries/boost_1_59_0/include
    msbuild Spring.sln /target:engine-headless /p:Configuration=%configuration% /m:2
  POPD
EXIT /b

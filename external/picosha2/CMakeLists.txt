include(FetchContent)

FetchContent_Declare(
    picosha2
    GIT_REPOSITORY https://github.com/okdshin/PicoSHA2.git
    GIT_TAG master
)

FetchContent_MakeAvailable(picosha2)

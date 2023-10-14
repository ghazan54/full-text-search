include(FetchContent)

FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG master
)

FetchContent_MakeAvailable(rapidcsv)

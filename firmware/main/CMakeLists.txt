file(
    GLOB files
        *.h
        */*.h
        *.cpp
        */*.cpp
)

set(dependencies
    freertos nvs_flash esp_http_server mdns esp_system driver
    arduino-esp32 ArduinoJson esp-nimble-cpp TFT_eSPI QRCode-esp32
    cpputils cxx-ring-buffer date
    espchrono espcpputils espconfiglib esp-gui-lib esphttpdutils espwifistack expected fmt ESP32-BLE-Gamepad-idf
)

idf_component_register(
    SRCS
        ${files}
    INCLUDE_DIRS
        .
    REQUIRES
        ${dependencies}
)

execute_process(COMMAND git rev-parse HEAD
        OUTPUT_VARIABLE GIT_REV ERROR_QUIET
)
execute_process(
        COMMAND git log -1 --pretty=%B
        OUTPUT_VARIABLE GIT_MESSAGE ERROR_QUIET
)
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_BRANCH
)

string(STRIP "${GIT_REV}" GIT_REV)
string(SUBSTRING "${GIT_REV}" 1 7 GIT_SHORT_REV)
string(STRIP "${GIT_MESSAGE}" GIT_MESSAGE)
string(REPLACE "\n" " " GIT_MESSAGE "${GIT_MESSAGE}")
string(SUBSTRING "${GIT_MESSAGE}" 0 100 GIT_MESSAGE)
string(STRIP "${GIT_BRANCH}" GIT_BRANCH)

target_compile_options(${COMPONENT_TARGET}
    PRIVATE
        -fstack-reuse=all
        -fstack-protector-all
        -fdiagnostics-color=always
        -Wno-unused-function
        -Wno-deprecated-declarations
        -Wno-missing-field-initializers
        -Wno-parentheses
        -DGIT_REV="${GIT_REV}"
        -DGIT_SHORT_REV="${GIT_SHORT_REV}"
        -DGIT_MESSAGE="${GIT_MESSAGE}"
        -DGIT_BRANCH="${GIT_BRANCH}"
        -DBOBBY_DEFAULT_OTA_NAME="${BOBBY_DEFAULT_OTA_NAME}"
        ${BOBBY_BUILDFLAGS}
)

set(APP_NAME bobby_remote)

add_definitions(
  -DUSER_SETUP_LOADED=1
  -fdiagnostics-color=always
)

set(BOBBY_BUILDFLAGS
#   Pins
#   analog sticks + button matrix
    -DPIN_LED_BACKLIGHT=23
)

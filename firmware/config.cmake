set(APP_NAME bobby_remote)

add_definitions(
  -DUSER_SETUP_LOADED=1
  -fdiagnostics-color=always
)

set(BOBBY_BUILDFLAGS
#   Pins
#   analog sticks + button matrix
    -DPIN_LED_BACKLIGHT=23

    -DPIN_ANALOG_LEFT_X=39
    -DPIN_ANALOG_LEFT_Y=36
    -DPIN_ANALOG_LEFT_BTN=26

    -DPIN_ANALOG_RIGHT_X=35
    -DPIN_ANALOG_RIGHT_Y=34
    -DPIN_ANALOG_RIGHT_BTN=25

    -DPIN_BTN_OUT1=19
    -DPIN_BTN_OUT2=21
    -DPIN_BTN_IN1=17
    -DPIN_BTN_IN2=18
)

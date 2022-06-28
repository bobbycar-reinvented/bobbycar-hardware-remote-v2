set(APP_NAME bobby_remote)

add_definitions(
  -DUSER_SETUP_LOADED=1
  -DLOAD_GLCD=1
  -DLOAD_FONT2=1
  -DLOAD_FONT4=1
  -DLOAD_FONT7=1
  -DST7789_DRIVER=1
  -DTFT_RGB_ORDER=0
  -DTFT_INVERSION_ON
  -DTFT_MOSI=13
  -DTFT_SCLK=15
  -DTFT_CS=14
  -DTFT_DC=12
  -DTFT_RST=2
  -DSPI_FREQUENCY=27000000
  -fdiagnostics-color=always
)

# set(BOBBYCAR_BUILDFLAGS
# Pins
# analog sticks + button matrix
# )

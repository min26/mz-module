# Custon zephyr module drivers

add_subdirectory_ifdef(CONFIG_MZ_SWITCH switch)
add_subdirectory_ifdef(CONFIG_MZ_LED led)


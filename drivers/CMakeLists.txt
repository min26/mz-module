# Declares the current directory as zephyr library
# if no-name is given, uses directory name
zephyr_library()

# Add header file directories
zephyr_include_directories(.)

# List source code files 
zephyr_library_sources(
    m_button.c
    m_console.c
    m_timer.c)





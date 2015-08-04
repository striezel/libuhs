cmake_minimum_required (VERSION 2.8)
# Recurse into subdirectory for decryption test.
add_subdirectory (decrypt_old)

# Recurse into subdirectory for read test of old format.
add_subdirectory (file_read_old)

# Recurse into subdirectory for read test of hint chunk.
add_subdirectory (read_hint_chunk)

# Recurse into subdirectory for read test of info chunk.
add_subdirectory (read_info_chunk)

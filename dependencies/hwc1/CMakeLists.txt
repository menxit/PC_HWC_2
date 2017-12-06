#############################################
# HWC1 Library                              #
#############################################
set(SOURCE_FILES src/buffer_t/buffer_t.c src/buffer_t/buffer_t.h src/msg_t/msg_t.c src/msg_t/msg_t.h src/client_udp/client_udp.c src/client_udp/client_udp.h src/server_udp/server_udp.c src/server_udp/server_udp.h src/mx_node/mx_node.c src/mx_node/mx_node.h src/mx_queue/mx_queue.c src/mx_queue/mx_queue.h src/sem.h)
add_library(hwc1 ${SOURCE_FILES})
target_link_libraries(hwc1 pthread)

bin_PROGRAMS = agent_rsvp
agent_rsvp_SOURCES = agent_rsvp.cpp configuration.cpp floatformat.cpp \
	generic_interface.cpp ip_mask.cpp ip_route.cpp mgmt_interface.cpp mgmt_packet.cpp \
	mpls_ioctl.cpp rsvp_configuration.cpp rsvp_connection.cpp rsvp_dispatcher.cpp \
	rsvp_error.cpp rsvp_packet.cpp rsvp_socket.cpp semaphore.cpp tools.cpp verbose.cpp

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.
agent_rsvp_LDFLAGS = $(all_libraries) 
